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
 * This file is part of the Contiki operating system.
 *
 */

/**
 * \file
 *         Path Computation Element (PCE) Communication Protocol (PCEP)
 *
 *         https://tools.ietf.org/html/rfc5440
 *
 * \author
 *         Ahmet Faruk Yavuz <ahmfrk61@gmail.com>
 *
 *         Sedat Gormus <sedatgormus@gmail.com>
 */

#include "contiki.h"
#include "net/nbr-table.h"
#include "net/ipv6/uip.h"
#include "lib/memb.h"
#include "lib/list.h"

#if TSCH_TIME_SYNCH
#include "net/mac/4emac/4emac-private.h"
#include "net/mac/4emac/4emac-buf.h"
#include "nbr-cell-table.h"

#include <string.h>

#include "sys/log.h"
#define LOG_MODULE "4EMAC-NC-TBL"
#ifndef LOG_CONF_LEVEL_4EMAC
#define LOG_LEVEL LOG_LEVEL_INFO
#else
#define LOG_LEVEL LOG_CONF_LEVEL_4EMAC
#endif


#ifdef SIXP_CONF_NEIGHBOUR_NUM
#define SIXP_NEIGHBOUR_NUM SIXP_CONF_NEIGHBOUR_NUM
#else
#define SIXP_NEIGHBOUR_NUM (NBR_TABLE_MAX_NEIGHBORS)
#endif

#ifdef SIXP_CONF_MAX_CELL_NUM
#define SIXP_MAX_CELL_NUM SIXP_CONF_MAX_CELL_NUM
#else
#define SIXP_MAX_CELL_NUM 11
#endif
/*------------------------------------------------------------------------------*/
#define NBR_HEAD()    list_head(nbr_cell_table_list)
#define NBR_TAIL()    list_tail(nbr_cell_table_list)
#define NBR_COUNT()   list_length(nbr_cell_table_list)
#define NBR_NEXT(nbr) nbr = list_item_next(nbr)

MEMB(nbr_cell_table_mem, struct nbr_cell_table, SIXP_NEIGHBOUR_NUM);
LIST(nbr_cell_table_list);
/*------------------------------------------------------------------------------*/
#define CELL_HEAD(nbr)     list_head(nbr->slot_frame_list)
#define CELL_TAIL(nbr)     list_tail(nbr->slot_frame_list)
#define CELL_COUNT(nbr)    list_length(nbr->slot_frame_list)
#define CELL_NEXT(cell)    cell = list_item_next(cell)

MEMB(slot_frame_mem, struct cell, SIXP_NEIGHBOUR_NUM * SIXP_MAX_CELL_NUM);
/*------------------------------------------------------------------------------*/
#define LOLLIPOP_COUNTER_CLR(x) (x) = 0x0
#define LOLLIPOP_COUNTER_INC(x) (x) = ((x) < 0xff) ? (x) + 1 : 1

/*
 *       https://tools.ietf.org/html/rfc8480
 *
 *                  Value       Meaning
 *                +-----------+-----------------------------+
 *                |      0x00 | Clear or after device reset |
 *                +-----------+-----------------------------+
 *                | 0x01-0xFF | Lollipop counter values     |
 *                +-----------+-----------------------------+
 *
 *       Possible values of the sequence number.
 */

/*------------------------------------------------------------------------------*/
void
nbr_cell_table_clr_nbr_seqnum_management(linkaddr_t *linkaddr)
{
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  if(n != NULL){
    LOLLIPOP_COUNTER_CLR(n->seqnum);
  }
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_nbr_seqnum_management(linkaddr_t *linkaddr)
{
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  if(n != NULL){
    return n->seqnum;
  }

  return 0xFF;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_set_nbr_seqnum_management(linkaddr_t *linkaddr, uint8_t seqnum)
{
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  /* [Mavi Alp Research Proprietary Code Redacted] */
  return 0xFF;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_inc_nbr_seqnum_management(linkaddr_t *linkaddr)
{
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  /* [Mavi Alp Research Proprietary Code Redacted] */
  return 0xFF;
}

/*------------------------------------------------------------- NEIGHBOUR TABLE -------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
nbr_cell_table_t *
nbr_cell_table_get_head_nbr()
{
  return NBR_HEAD();
}
/*------------------------------------------------------------------------------*/
nbr_cell_table_t *
nbr_cell_table_get_nbr(linkaddr_t *linkaddr)
{
  nbr_cell_table_t *n;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return NULL;
}
/*------------------------------------------------------------------------------*/
nbr_cell_table_t *
nbr_cell_table_get_nbr_from_ipaddr(uip_ipaddr_t *ipaddr)
{
  nbr_cell_table_t *n;
  linkaddr_t linkaddr;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return NULL;
}
/*------------------------------------------------------------------------------*/
linkaddr_t *
nbr_cell_table_get_linkaddr_from_ipaddr(uip_ipaddr_t *ipaddr)
{
  nbr_cell_table_t *n;
  linkaddr_t linkaddr;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return NULL;
}
/*------------------------------------------------------------------------------*/
static nbr_cell_table_t *
nbr_cell_table_add_nbr(linkaddr_t *linkaddr)
{
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  if(n != NULL) {
#if UMAY_6T_ENABLED
    cell_t *c;
    uint8_t has_cached = 0;
    for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)) {
        if(c->state == CELL_STATE_CACHED) has_cached = 1;
    }
    if(has_cached) {
        LOG_INFO("UMAY-6T: Neighbor detected. Starting Validation.\n");
#if UMAY_CONSISTENCY_CHECK_ENABLED
        for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)) {
            if(c->state == CELL_STATE_CACHED) {
               c->state = CELL_STATE_VALIDATING;
            }
        }
        sixtop_send_count_request(linkaddr); 
#else
        reactivate_cached_cells(n);
#endif
    }
#endif
    return n;
  }

  n = memb_alloc(&nbr_cell_table_mem);
  if(n == NULL){ /* no more space left in the buffer */
    LOG_ERR("List full!\n");
    FOURE_LOG(LOG_LEVEL_2, CORE_NET_MAC, EC_4EMAC_NBR_LIST_FULL);
    return NULL;
  }

  memcpy(&n->linkaddr, linkaddr, sizeof(linkaddr_t));
  nbr_cell_table_clr_nbr_seqnum_management(linkaddr);
  n->list_info_time = clock_seconds() - (2 * NBR_REACHABLE_INTERVAL);
  LIST_STRUCT_INIT(n, slot_frame_list);

  list_add(nbr_cell_table_list, n);

  LOG_INFO("0x%02x%02x was added\n", n->linkaddr.u8[6], n->linkaddr.u8[7]);

  return n;
}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_add_nbr_callback(linkaddr_t *linkaddr)
{
  /* [Mavi Alp Research Proprietary Code Redacted] */
}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_delete_nbr(linkaddr_t *linkaddr)
{
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  /* [Mavi Alp Research Proprietary Code Redacted] */

}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_delete_nbr_callback(linkaddr_t *linkaddr)
{
  /* [Mavi Alp Research Proprietary Code Redacted] */

}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_nbr_num()
{
  return NBR_COUNT();
}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_set_preferred_parent(nbr_cell_table_t *n)
{
  /* [Mavi Alp Research Proprietary Code Redacted] */

}

/*------------------------------------------------------------- NEIGHBOUR CELLS -------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
static cell_t *
nbr_cell_table_get_nbr_cell(nbr_cell_table_t *n, sixp_cell_t *cell_list)
{
  cell_t *c;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return NULL;
}
/*------------------------------------------------------------------------------*/
cell_t *
nbr_cell_table_get_nbr_slot(linkaddr_t *linkaddr, uint16_t slot_offset)
{
  cell_t *c;
  nbr_cell_table_t *n;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return NULL;
}
/*------------------------------------------------------------------------------*/
cell_t *
nbr_cell_table_get_nbr_min_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type)
{
  nbr_cell_table_t *n;
  cell_t *c ,*cell_min = NULL;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cell_min;
}
/*------------------------------------------------------------------------------*/
cell_t *
nbr_cell_table_get_nbr_max_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type)
{
  nbr_cell_table_t *n;
  cell_t *c ,*cell_max = NULL;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cell_max;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_nbr_cells(linkaddr_t *linkaddr, uint8_t cell_type, sixp_cell_t *cell_list)
{
  uint8_t cell_num = 0;
  cell_t *c;
  nbr_cell_table_t *n;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cell_num;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_nbr_cell_num(linkaddr_t *linkaddr, uint8_t cell_type)
{
  uint8_t cell_num = 0;
  cell_t *c;
  nbr_cell_table_t *n;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cell_num;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_add_nbr_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num, uint8_t cell_type)
{
  uint8_t i, cellnum = 0;
  cell_t *c;
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cellnum;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_del_nbr_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num)
{
  uint8_t i, cellnum = 0;
  cell_t *c;
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cellnum;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_del_all_nbr_cells(linkaddr_t *linkaddr, uint8_t cell_type)
{
  uint8_t cellnum = 0;
  cell_t *c, *temp_c;
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cellnum;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_is_cell_scheduled_for_any_nbr(uint16_t slot_offset, uint16_t channel_offset, linkaddr_t *ignored_linkaddr)
{
  cell_t *c;
  nbr_cell_table_t *n;
  sixp_cell_t cell = {slot_offset, channel_offset};

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return 0;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_is_slot_scheduled_for_any_nbr(uint16_t slot_offset, linkaddr_t *ignored_linkaddr)
{
  cell_t *c;
  nbr_cell_table_t *n;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return 0;
}
/*------------------------------------------------------------- NODE CELLS -------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
slot_t *
nbr_cell_table_get_slot(linkaddr_t *linkaddr, uint16_t slot_offset)
{
  destination_t *dn;
  slot_t *sl;
  /* [Mavi Alp Research Proprietary Code Redacted] */

  return NULL;
}
/*------------------------------------------------------------------------------*/
slot_t *
nbr_cell_table_get_min_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type)
{
  destination_t *dn;
  slot_t *sl, *slot_min = NULL;

  /* [Mavi Alp Research Proprietary Code Redacted] */
  return slot_min;
}
/*------------------------------------------------------------------------------*/
slot_t *
nbr_cell_table_get_max_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type)
{
  destination_t *dn;
  slot_t *sl, *slot_max = NULL;

  /* [Mavi Alp Research Proprietary Code Redacted] */
  return slot_max;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_cells(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock, uint8_t cell_offset, sixp_cell_t *cell_list, uint8_t cell_list_len)
{
  destination_t *dn;
  slot_t *sl;
  uint8_t cell_num = 0;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cell_num;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_cell_num(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock, uint8_t cell_offset)
{
  destination_t *dn;
  slot_t *sl;
  uint8_t cell_num = 0;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cell_num;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_schedule_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num, uint8_t cell_type, uint8_t cell_lock, unsigned long life_time)
{
  uint8_t i, ret, cellnum = 0;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cellnum;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_unschedule_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num)
{
  uint8_t i, cellnum = 0;

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return cellnum;
}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_lock_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num, uint8_t cell_lock)
{
  slot_t *sl;
  uint8_t i;

  /* [Mavi Alp Research Proprietary Code Redacted] */

}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_lock_all(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock)
{
  destination_t *dn;
  slot_t *sl;
  uint8_t cell_num = 0;

  /* [Mavi Alp Research Proprietary Code Redacted] */
  return cell_num;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_cell_lifetime_expired(linkaddr_t *linkaddr, uint8_t slot_offset)
{
  slot_t *sl;

  /* [Mavi Alp Research Proprietary Code Redacted] */
  return 0;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_diff_cells(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock, sixp_cell_t *cell_list, uint8_t cell_num)
{
  uint8_t i, j, num_cell = 0;
  uint8_t cellnum;
  sixp_cell_t celllist[FOURE_MAX_SLOTS_PER_DESTINATION];

  /* [Mavi Alp Research Proprietary Code Redacted] */

  return num_cell;
}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_sort_cells(sixp_cell_t *cell_list, uint8_t cell_num)
{
  sixp_cell_t cell;
  uint8_t i, j;

  /* [Mavi Alp Research Proprietary Code Redacted] */

}
/*------------------------------------------------------------------------------------------------------------------------------------------------------------
 *  set ipaddr from given linkaddr and prefix
 */
void
nbr_set_ip_from_prefix(uip_ipaddr_t *ipaddr, uip_ipaddr_t *prefix, linkaddr_t *linkaddr)
{
  memset(ipaddr, 0, sizeof(uip_ipaddr_t));
  memcpy(ipaddr, prefix, 64 >> 3);
  uip_ds6_set_addr_iid(ipaddr, (uip_lladdr_t *)linkaddr);
}
/*------------------------------------------------------------------------------
 *  set linkaddr from given ipaddr
 */
void
nbr_set_lladdr_from_ipaddr(linkaddr_t *linkaddr, uip_ipaddr_t *ipaddr)
{
  /* We consider only links with IEEE EUI-64 identifier or
   * IEEE 48-bit MAC addresses */
  /* [Mavi Alp Research Proprietary Code Redacted] */

}
/*------------------------------------------------------------------------------*/
const char *
nbr_cell_type_to_str(uint8_t cell_type)
{
  uint8_t i, type;

  for(type = cell_type, i = 0; type; type >>= 1, i++);

  return cell_type_str[i];
}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_init()
{
  memb_init(&nbr_cell_table_mem);
  list_init(nbr_cell_table_list);

  memb_init(&slot_frame_mem);
}
/*------------------------------------------------------------------------------*/
const char *const cell_type_str[] = {
  "Null",
  "Transmit",       // 0b0000 0001
  "Receive",        // 0b0000 0010
  "Shared",         // 0b0000 0100
  "Neighbor",       // 0b0000 1000
  "Track Transmit", // 0b0001 0000
  "Track Receive",  // 0b0010 0000
  "Track Shared"    // 0b0100 0000
};
/*------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------*/
#if UMAY_6T_ENABLED

void reactivate_cached_cells(nbr_cell_table_t *nbr) {
    cell_t *c;
    for(c = CELL_HEAD(nbr); c != NULL; CELL_NEXT(c)) {
        if(c->state == CELL_STATE_VALIDATING || c->state == CELL_STATE_CACHED) {
            /* Check Conflicts natively with nbr_cell_table_is_slot_scheduled_for_any_nbr */
            if(!nbr_cell_table_is_slot_scheduled_for_any_nbr(c->slot_offset, &nbr->linkaddr)) {
                c->state = CELL_STATE_ACTIVE;
                c->expiry_time = 0;
            } else {
                /* Remove conflicted cell */
                list_remove(nbr->slot_frame_list, c);
                memb_free(&slot_frame_mem, c);
            }
        }
    }
}

void umay_cache_maintenance_routine(void) {
    nbr_cell_table_t *n;
    cell_t *c, *next_c;
    unsigned long current_time = clock_seconds();

    for(n = NBR_HEAD(); n != NULL; NBR_NEXT(n)) {
        c = CELL_HEAD(n);
        uint8_t has_active_or_cached_cells = 0;
        
        while(c != NULL) {
            next_c = list_item_next(c);
            
            if(c->state == CELL_STATE_CACHED && current_time >= c->expiry_time) {
                LOG_WARN("UMAY-6T: Cache expired for cell offset %u. Hard deleting.\n", c->slot_offset);
                list_remove(n->slot_frame_list, c);
                memb_free(&slot_frame_mem, c);
            } else {
                has_active_or_cached_cells = 1;
            }
            c = next_c;
        }

        /* If no cells remain for this neighbor, fallback to baseline 6TiSCH deletion */
        if(!has_active_or_cached_cells) {
            LOG_INFO("UMAY-6T: All cached cells expired. Removing neighbor state completely.\n");
            list_remove(nbr_cell_table_list, n);
            memb_free(&nbr_cell_table_mem, n);
        }
    }
}

#endif /* UMAY_6T_ENABLED */
/*------------------------------------------------------------------------------*/

#endif /* TSCH_TIME_SYNCH */
