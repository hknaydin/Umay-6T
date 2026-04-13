/*
 * Copyright (c) 2016, Mavialp Research Limited.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 */

/**
 * \file
 *         A simple Distributed PCE scheduler.
 * \author
 *         Ahmet Faruk Yavuz <ahmfrk61@gmail.com>
 *
 *         Sedat Gormus <sedatgormus@gmail.com>
 */

#include "contiki.h"
#include "net/packetbuf.h"
#include "net/routing/rpl-classic/rpl-private.h"
#include "net/link-stats.h"
#include "lib/random.h"

#if TSCH_TIME_SYNCH
#include "net/mac/4emac/4emac-private.h"
#include "net/mac/4emac/ieee802154e.h"
#include "net/mac/4emac/4emac.h"
#include "6top-pce.h"
#include "nbr-cell-table.h"
#include "6p-sf.h"
#include "6p-conn.h"

/* [Mavi Alp Research Proprietary Code Redacted: Macros and Prototypes] */

static void ds_error_handler(void *ptr) {
    /* [Mavi Alp Research Proprietary Code Redacted] */
}

static PT_THREAD(list_thread(process_event_t ev, struct etimer *t)) {
    /* [Mavi Alp Research Proprietary Code Redacted] */
    PT_END(&list_pt);
}

static PT_THREAD(cell_allocation_thread(process_event_t ev, process_data_t data)) {
    /* [Mavi Alp Research Proprietary Code Redacted] */
    PT_END(&cell_allocation_pt);
}

static void ds_process_confirm(linkaddr_t *linkaddr, struct ieee802154_sixp *p) {
    /* [Mavi Alp Research Proprietary Code Redacted] */
}

static void ds_process_response(linkaddr_t *linkaddr, struct ieee802154_sixp *p) {
    /* [Mavi Alp Research Proprietary Code Redacted: Basic validation] */

    sixp_queuebuf_t *q; // Placeholder queue

    switch(p->opcode){
      case RC_SUCCESS:
        if(q->p.opcode == CMD_ADD){
            /* [Mavi Alp Research Proprietary Code Redacted] */
        }else if(q->p.opcode == CMD_DELETE){
            /* [Mavi Alp Research Proprietary Code Redacted] */
        }else if(q->p.opcode == CMD_RELOCATE){
            /* [Mavi Alp Research Proprietary Code Redacted] */
        }else if(q->p.opcode == CMD_SIXP_COUNT){
            LOG_INFO("Count response reached from 0x%02x%02x cell num %u\n", linkaddr->u8[6], linkaddr->u8[7], p->numcell);
            /* UMAY-6T: Invoke Consistency Check Callback */
#if UMAY_6T_ENABLED
            extern void umay_consistency_callback(const linkaddr_t *peer, uint8_t status, uint16_t peer_cell_count);
            umay_consistency_callback(linkaddr, p->opcode, p->numcell);
#endif
        }else if(q->p.opcode == CMD_LIST){
            /* [Mavi Alp Research Proprietary Code Redacted] */
        }
        break;
      /* [Mavi Alp Research Proprietary Code Redacted: EOL, ERR, RESET] */
    }
}

static void ds_process_request(linkaddr_t *linkaddr, struct ieee802154_sixp *p) {
    /* [Mavi Alp Research Proprietary Code Redacted] */
}

/*------------------------------------------------------------------------------*/
#if UMAY_6T_ENABLED

extern void reactivate_cached_cells(void *n);
extern void umay_sync_rpl(const linkaddr_t *peer);
#define CELL_TYPE_ALL SLOT_TYPE_ALL

/* Implementation Consistency Callback */
void umay_consistency_callback(const linkaddr_t *peer, uint8_t status, uint16_t peer_cell_count) {
    void *n = nbr_cell_table_get_nbr((linkaddr_t*)peer);
    uint16_t local_count = nbr_cell_table_get_all_cell_count((linkaddr_t*)peer);
    
    if(status == RC_SUCCESS && peer_cell_count == local_count) {
        LOG_INFO("UMAY-6T: Consistency Verified! Reactivating cells for neighbor.\n");
        reactivate_cached_cells(n); 
        umay_sync_rpl(peer);
    } else {
        LOG_WARN("UMAY-6T: Consistency Failed (Local:%u Peer:%u). Hard Resetting state.\n", 
                 local_count, peer_cell_count);
        nbr_cell_table_del_all_nbr_cells((linkaddr_t*)peer, CELL_TYPE_ALL);
    }
}

#endif /* UMAY_6T_ENABLED */
/*------------------------------------------------------------------------------*/

#endif /* TSCH_TIME_SYNCH */
