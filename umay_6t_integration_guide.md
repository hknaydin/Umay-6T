# Umay-6T Integration Guide & User Manual (Publication-Ready / Advanced)
<p class="justify">
This document is the **definitive guide** for integrating the Umay-6T protocol extensions into the <a href="https://www.mavialp.com/tr/">Mavi Alp</a> "4emac" Contiki-NG stack. Unlike basic implementations, this version includes critical **Robustness Features** demanded by academic reviewers: **Consistency Checks** (handling node reboots) and **Cross-Layer Support** (RPL synchronization).
</p>

## 1. Overview & Key Concepts

Umay-6T introduces "Soft State" management to 6TiSCH.

- **Cell Retention (CRM):** Caches cells ($T_{cache}$) upon disconnection.
- **Fast Reconnection (FRM):** Reactivates cells upon reappearance.
- **Consistency Verification:** Verifies the neighbor's state (via `6P COUNT`) to prevent "Zombie Cells" if the neighbor rebooted.
- **RPL Synchronization:** Triggers the IP routing layer to recognize the restored link immediately.

## 2. Configuration Parameters

Add these to `project-conf.h`:

```c

/* Scheduling Function (SF) Selection Baselines */
#define SF_MSF      0
#define SF_DSF      1
#define SF_DeTAS    2
#define SF_UMAY     3
#ifndef SCHEDULING_FUNCTION
#define SCHEDULING_FUNCTION SF_UMAY /* Set to SF_MSF, SF_DSF, SF_DeTAS, or SF_UMAY to select SF */
#endif
/* Umay-6T Protocol Switch (Automatically enabled if UMAY is selected) */
#define UMAY_6T_ENABLED (SCHEDULING_FUNCTION == SF_UMAY)


/* Cache window. All cache timings are in SECONDS (clock_seconds() domain). */
#define UMAY_DEFAULT_CACHE_DURATION 60



/* T_cache = max(T_min, alpha * RTT_est + beta * mobility_factor), see Eq. (2).
 * T_min is expressed in slotframes and converted to seconds in
 * umay_compute_cache_duration() using TSCH slotframe period.
 */
#define UMAY_TMIN_SLOTFRAMES 3         
#define UMAY_ALPHA 1.2
#define UMAY_BETA 0.5

/* Advanced Robustness Switches */
#define UMAY_CONSISTENCY_CHECK_ENABLED 1 /* Prevents "Zombie" cells */
#define UMAY_RPL_SYNC_ENABLED 1          /* Cross-layer optimization */

/* Validation phase deadline. If 6P COUNT response does not arrive in time,
 * cached cells are hard-deleted. */
#define UMAY_VALIDATION_TIMEOUT 4   /* seconds */

/* Cell States */
#define CELL_STATE_ACTIVE 0
#define CELL_STATE_CACHED 1
#define CELL_STATE_VALIDATING 2 /* Wait for 6P COUNT Response */
```

## 3. Detailed Implementation Steps

Step 1: Update Data Structures (`nbr-cell-table.h`)

We need to track the validation state.

```c
typedef struct cell {
  struct cell *next;
  uint8_t slot_offset;
  uint8_t channel_offset;
  uint8_t type;
  uint8_t lock;

  /* UMAY-6T EXTENSIONS */
  uint8_t state;             /* shared with TSCH ISR -> volatile */
  unsigned long expiry_time; /* cache expiry, seconds */
} cell_t;
```

Step 2: Implement Robust Core Logic (`nbr-cell-table.c`)

#### A. Soft Delete (Standard)

Modify nbr_cell_table_delete_nbr. Instead of freeing memory, we cache it.
```c
/*
 * Called when TSCH/6P observes that a neighbor has become unreachable.
 * Instead of freeing the cell memory, mark every active cell as CACHED
 * and schedule a hard-delete deadline at now + UMAY_DEFAULT_CACHE_DURATION.
 */
void umay_soft_delete_nbr(nbr_cell_table_t *n) {
    cell_t *c;
    unsigned long now = clock_seconds();

    for(c = CELL_HEAD(n); c != NULL; c = CELL_NEXT(c)) {     /* fixed loop */
        if(c->state == CELL_STATE_ACTIVE) {
            c->state       = CELL_STATE_CACHED;
            c->expiry_time = now + UMAY_DEFAULT_CACHE_DURATION;
        }
    }
    LOG_INFO("UMAY-6T: Neighbor soft-deleted, %u cells cached.\n",
             nbr_cell_table_get_all_cell_count(&n->linkaddr));
}
```
#### B. Robust Fast Reconnection (Validation Logic)
Replace the simple add logic with this robust state machine.

```c
#include "net/mac/tsch/sixtop/sixp.h"
#include "net/mac/tsch/sixtop/sixp-pkt.h"
#include "net/routing/rpl-classic/rpl.h"   /* public RPL header */
#include "net/ipv6/uip-ds6-nbr.h"

/* Required Headers for RPL Sync */
#include "net/routing/rpl-classic/rpl-private.h"
#include "net/ipv6/uip-ds6-nbr.h"

static nbr_cell_table_t * nbr_cell_table_add_nbr(linkaddr_t *linkaddr) {
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);
  cell_t *c;

  /* CASE 1: Reconnecting Neighbor found in Cache */
  if(n != NULL && has_cached_cells(n)) {
      LOG_INFO("UMAY-6T: Neighbor detected. Starting Validation.\n");

#if UMAY_CONSISTENCY_CHECK_ENABLED
      /* PHASE 1: Validation State */
      /* Do NOT activate cells yet. Mark as VALIDATING. */
      for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)) {
          if(c->state == CELL_STATE_CACHED) {
             c->state = CELL_STATE_VALIDATING;
          }
      }
      
      /* Trigger 6P COUNT Request */
      /* This must be implemented in 6top-pce.c to send a request */
      sixtop_send_count_request(linkaddr); 
      return n;

#else
      /* NAIVE APPROACH (If Consistency Check Disabled) */
      reactivate_cached_cells(n);
      return n;
#endif
  }

  /* ... Standard New Neighbor Allocation ... */
}
```

#### C. Cross-Layer RPL Synchronization
Add this helper function to sync the IP layer.

```c

/* Reactivate cached/validating cells after a successful consistency check
 * (or fast reconnect when consistency check is disabled). Also informs RPL.
 */
void umay_reactivate_cells(nbr_cell_table_t *n) {
    cell_t *c;
    for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)) {
        if((c->state == CELL_STATE_CACHED || c->state == CELL_STATE_VALIDATING) &&
           !is_cell_conflicting(c->slot_offset)) {
            
            c->state = CELL_STATE_ACTIVE;
            c->expiry_time = 0;
            LOG_INFO("UMAY-6T: Cell %u reactivated.\n", c->slot_offset);
        }
    }
    /* Cross-Layer: Inform RPL */
    #if UMAY_RPL_SYNC_ENABLED
    umay_sync_rpl(&n->linkaddr);
    #endif
}

#if UMAY_RPL_SYNC_ENABLED
/* Marks the peer reachable in the IPv6 neighbor cache so that RPL
 * stops penalizing the link as soon as TSCH cells are restored.
 *
 * NOTE: we construct the LINK-LOCAL (fe80::) address only. Global
 *       reachability is restored by RPL itself once the link is up.
 */

void umay_sync_rpl(linkaddr_t *lladdr) {
    uip_ipaddr_t ipaddr;
    uip_ds6_nbr_t *nbr;

    uip_ip6addr(&ipaddr, 0xfe80, 0, 0, 0, 0, 0, 0, 0);
    uip_ds6_set_addr_iid(&ipaddr, (uip_lladdr_t *)lladdr);

    nbr = uip_ds6_nbr_lookup(&ipaddr);
    if(nbr == NULL) {
        nbr = uip_ds6_nbr_add(&ipaddr,
                              (uip_lladdr_t *)lladdr,
                              0,                       /* isrouter */
                              NBR_REACHABLE,
                              NBR_TABLE_REASON_IPV6_ND, /* fixed: was missing */
                              NULL);                    /* fixed: was missing */
    } else {
        nbr->state = NBR_REACHABLE;
    }

    LOG_INFO("UMAY-6T: IPv6 neighbor state set to REACHABLE.\n");
}
#endif

```

Step 3: Implement Consistency Callback (`6top-pce.c`)

When the `6P COUNT` response arrives, we decide the fate of the cached cells.

**Action:** Update `ds_process_response` (or create a specific callback).

```c
void umay_consistency_callback(const linkaddr_t *peer,
                               uint8_t status,
                               uint16_t peer_cell_count) {
    nbr_cell_table_t *n     = nbr_cell_table_get_nbr(peer);
    uint16_t local_count    = nbr_cell_table_get_all_cell_count(peer);

    if(n == NULL) {
        LOG_WARN("UMAY-6T: COUNT response for unknown peer, ignoring.\n");
        return;
    }

    if(status == SIXP_RC_SUCCESS && peer_cell_count == local_count) {
        LOG_INFO("UMAY-6T: Consistency verified (%u cells). Reactivating.\n",
                 local_count);
        umay_reactivate_cells(n);          /* RPL sync happens inside */
    } else {
        LOG_WARN("UMAY-6T: Consistency failed (local=%u peer=%u). Hard reset.\n",
                 local_count, peer_cell_count);
        nbr_cell_table_del_all_nbr_cells(peer, CELL_TYPE_ALL);
        /* Standard 6P ADD negotiation will be triggered by the SF
         * on the next packet enqueue, no explicit call needed. */
    }
}
```

Step 4: Scheduler Logic (4emac-6top-scheduler-minimal.c)

Ensure the scheduler respects the cell states.

```c
/* In sixtop_slot_callback */
ret_val = foure_mac_buf_content_get_scheduled(...);
/* 1. If CACHED or VALIDATING, Treat as Unscheduled */
if(buf_item.sl != NULL && buf_item.sl->state != CELL_STATE_ACTIVE) {
    /* Do not transmit on cached/validating cells */
    ret_val = FOURE_SLOT_UNSCHEDULED;
}
/* 2. If ACTIVE (Reactivated by FRM), Proceed Normally */
if(ret_val != FOURE_SLOT_UNSCHEDULED) {
    /* Standard Transmission Logic */
}
```

Step 5: Cache Maintenance & Fallback (Garbage Collection)

To comply with the fail-safe mechanism detailed in Algorithm 3 of the Umay-6T manuscript, we must implement a periodic routine to permanently delete soft-state cells if a neighbor fails to return before $T_{cache}$ expires.

**Action:** Create a `ctimer` that ticks every slotframe or every few seconds to execute this cleanup routine.

```c
void umay_cache_maintenance_routine(void) {
    nbr_cell_table_t *n;
    cell_t *c, *next_c;
    unsigned long current_time = clock_seconds();

    for(n = NBR_HEAD(); n != NULL; NBR_NEXT(n)) {
        if(!has_cached_cells(n)) continue;

        c = CELL_HEAD(n);
        while(c != NULL) {
            next_c = list_item_next(c);
            
            /* If cell is cached and expired */
            if(c->state == CELL_STATE_CACHED && current_time >= c->expiry_time) {
                LOG_WARN("UMAY-6T: Cache expired for cell offset %u. Hard deleting.\n", c->slot_offset);
                /* Remove from slotframe schedule */
                list_remove(n->slot_frame_list, c);
                memb_free(&slot_frame_mem, c);
            }
            c = next_c;
        }

        /* If no cells remain for this neighbor, fallback to baseline 6TiSCH deletion */
        if(!has_active_or_cached_cells(n)) {
            LOG_INFO("UMAY-6T: All cached cells expired. Removing neighbor state.\n");
            list_remove(nbr_cell_table_list, n);
            memb_free(&nbr_cell_table_mem, n);
        }
    }
}
```

## 4. Reproducibility & Open Science

To address reviewer concerns regarding reproduction and empirical validation, Umay-6T provides full transparency on its build parameters.

### Code Availability
The full source code of the modified Contiki-NG 4emac MAC layer, custom 6P scheduler, and the simulation configuration files (.csc) are publicly available on GitHub to ensure complete reproducibility of the manuscript's findings.
* **Repository Link:** <https://github.com/hknaydin/Umay-6T>

### Cooja Simulation Setup
To reproduce the numerical results presented in the paper(approximately 20% average and up to ~40% peak reduction in combined 6P + RPL control overhead under high mobility), simulators must use the following configuration parameters:
- **Mobility Model:** BonnMotion Random Waypoint implementation loaded dynamically into Cooja.
- **Node Type:** Exp5438 Mote
- **Compile Flags:** Ensure `UMAY_6T_ENABLED=1`, `UMAY_CONSISTENCY_CHECK_ENABLED=1`, and `UMAY_RPL_SYNC_ENABLED=1` are defined in your `project-conf.h`.
- **Make Target:** Compile using the standard `make TARGET=exp5438` within your simulation directory.

### Testbed Deployment (Future Work)
When transitioning from Cooja to industrial testbeds (e.g., Umote / Mahmote) as discussed in the paper, ensure that hardware timers appropriately reflect the `clock_seconds()` precision required by the Cache Maintenance routine.
