/*
 * Copyright (c) 2015, Mavi Alp Research Limited.
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
 *         A simple scheduler.
 * \author
 *         Sedat Gormus <sedatgormus@gmail.com>
 *         Ahmet Faruk Yavuz <ahmfrk61@gmail.com>
 */

#include "contiki.h"

#if TSCH_TIME_SYNCH
#include "net/mac/4emac/4emac-private.h"
#include "net/mac/4emac/ieee802154e.h"
#include "net/mac/4emac/4emac-6tisch-minimal-security.h"
#include "net/mac/4emac/6top-pce/nbr-cell-table.h"

/* Truncate received drift correction information to maximum half
 * of the guard time (one fourth of FOURE_DEFAULT_TS_RX_WAIT). */
#define TRUNCATE_SYNC_IE 1

/*---------------------------------------------------------------------------
 * Return the current channel depending on the channel offset and ASN
 */
uint8_t current_channel(uint8_t channel_offset) {
    /* [Mavi Alp Research Proprietary Code Redacted] */
    return 0;
}

/*---------------------------------------------------------------------------
 * Return the current slot using the ASN value and slot frame size.
 */
uint8_t current_slot() {
    /* [Mavi Alp Research Proprietary Code Redacted] */
    return 0;
}

/*---------------------------------------------------------------------------
 * The callback function for the slots. Function is scheduled recursively for
 * each scheduled slot. If a number of slots are not scheduled, the function
 * skips over them to let the cpu sleep longer durations to save energy.
 */
PT_THREAD(sixtop_slot_callback(struct rtimer *t, void *ptr))
{
    static struct foure_buf_item buf_item;
    static uint8_t ret_val;

    PT_BEGIN(&sixtop_slot_callback_pt);

    /* [Mavi Alp Research Proprietary Code Redacted: Synchronization Checks] */

    //INFO : Go over all the slots to find if there is any packet scheduled for the current slot.
    // ret_val = foure_mac_buf_content_get_scheduled(&buf_item.dn, &buf_item.cn, &buf_item.sl, current_slot());

#if UMAY_6T_ENABLED
    /* UMAY-6T: Skip transmission if the cell is softly deleted (Cached/Validating) */
    if(ret_val != FOURE_SLOT_UNSCHEDULED && buf_item.sl != NULL) {
        cell_t *u_cell = NULL;
        nbr_cell_table_t *u_nbr;
        for(u_nbr = nbr_cell_table_get_head_nbr(); u_nbr != NULL; u_nbr = u_nbr->next) {
            cell_t *u_c = nbr_cell_table_get_nbr_slot(&u_nbr->linkaddr, current_slot());
            if(u_c != NULL && (u_c->state == CELL_STATE_CACHED || u_c->state == CELL_STATE_VALIDATING)) {
                ret_val = FOURE_SLOT_UNSCHEDULED; // Bypass TSCH driver TX phase
                break;
            }
        }
    }
#endif

    /* [Mavi Alp Research Proprietary Code Redacted: TX/RX Logic and Erase Page Trigger] */

    PT_END(&sixtop_slot_callback_pt);
}

/*---------------------------------------------------------------------------
 * Initialise the sixtop process with the default values.
 */
void sixtop_init() {
    /* [Mavi Alp Research Proprietary Code Redacted] */
}

/*---------------------------------------------------------------------------*/
void foure_mac_black_channel_inc(uint8_t channel) {
    /* [Mavi Alp Research Proprietary Code Redacted] */
}

#endif /* TSCH_TIME_SYNCH */
