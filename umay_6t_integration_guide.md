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
#define UMAY_6T_ENABLED 1
#define UMAY_DEFAULT_CACHE_DURATION 60 

/* Advanced Features */
#define UMAY_CONSISTENCY_CHECK_ENABLED 1
#define UMAY_RPL_SYNC_ENABLED 1

/* Cell States */
#define CELL_STATE_ACTIVE 0
#define CELL_STATE_CACHED 1
#define CELL_STATE_VALIDATING 2 /* Temporary state during Consistency Check */
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
  uint8_t state;             
  unsigned long expiry_time; 
} cell_t;
```

Step 2: Implement Robust Core Logic (`nbr-cell-table.c`)

#### A. Soft Delete (Standard)
(See previous guide - marks cells as `CACHED`).

#### B. Robust Fast Reconnection (Validation Logic)
Replace the simple add logic with this robust state machine.

```c
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
void umay_sync_rpl(linkaddr_t *lladdr) {
#if UMAY_RPL_SYNC_ENABLED
    uip_ipaddr_t ipaddr;
    uip_ds6_nbr_t *nbr;

    /* Construct Global IPv6 Address */
    uip_ip6addr(&ipaddr, 0xfe80, 0, 0, 0, 0, 0, 0, 0);
    uip_ds6_set_addr_iid(&ipaddr, (uip_lladdr_t *)lladdr);

    /* Update Neighbor Table */
    nbr = uip_ds6_nbr_lookup(&ipaddr);
    if(nbr == NULL) {
        nbr = uip_ds6_nbr_add(&ipaddr, (uip_lladdr_t *)lladdr, 0, NBR_REACHABLE);
    } else {
        nbr->state = NBR_REACHABLE;
    }
    
    /* Trigger RPL Neighbor Update */
    rpl_neighbor_set_reachable(nbr);
    LOG_INFO("UMAY-6T: Synced RPL reachability for neighbor.\n");
#endif
}
```

Step 3: Implement Consistency Callback (`6top-pce.c`)

When the `6P COUNT` response arrives, we decide the fate of the cached cells.

**Action:** Update `ds_process_response` (or create a specific callback).

```c
void umay_consistency_callback(linkaddr_t *peer, uint8_t status, uint16_t peer_cell_count) {
    nbr_cell_table_t *n = nbr_cell_table_get_nbr(peer);
    uint16_t local_count = nbr_cell_table_get_all_cell_count(peer);
    
    if(status == SIXP_RC_SUCCESS && peer_cell_count == local_count) {
        /* SUCCESS: Cells match! Reactivate. */
        LOG_INFO("UMAY-6T: Consistency Verified! Reactivating cells.\n");
        reactivate_cached_cells(n); /* Sets state to ACTIVE */
        
        /* Sync RPL now that link is stable */
        umay_sync_rpl(peer);
        
    } else {
        /* FAILURE: Mismatch (Node Rebooted?). Hard Delete. */
        LOG_WARN("UMAY-6T: Consistency Failed (Local:%u Peer:%u). Hard Resetting.\n", 
                 local_count, peer_cell_count);
        nbr_cell_table_del_all_nbr_cells(peer, CELL_TYPE_ALL); // Hard Delete
        // Trigger standard negotiation...
    }
}
```

Step 4: The `reactivate_cached_cells` Helper

Consolidate the activation logic.

```c
void reactivate_cached_cells(nbr_cell_table_t *n) {
    cell_t *c;
    for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)) {
        if(c->state == CELL_STATE_VALIDATING || c->state == CELL_STATE_CACHED) {
             /* Check Conflicts */
             if(!is_cell_conflicting(c->slot_offset)) {
                 c->state = CELL_STATE_ACTIVE;
                 c->expiry_time = 0;
             } else {
                 /* Remove conflicted cell */
                 // remove logic...
             }
        }
    }
}
```
