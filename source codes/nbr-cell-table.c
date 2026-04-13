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

  if(n != NULL){
    n->seqnum = seqnum;
    return seqnum;
  }

  return 0xFF;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_inc_nbr_seqnum_management(linkaddr_t *linkaddr)
{
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  if(n != NULL){
    LOLLIPOP_COUNTER_INC(n->seqnum);
    return n->seqnum;
  }

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

  if(linkaddr != NULL){
    for(n = NBR_HEAD(); n != NULL; NBR_NEXT(n)){
      if(memcmp(&n->linkaddr, linkaddr, sizeof(linkaddr_t)) == 0)
        return  n;
    }
  }
  return NULL;
}
/*------------------------------------------------------------------------------*/
nbr_cell_table_t *
nbr_cell_table_get_nbr_from_ipaddr(uip_ipaddr_t *ipaddr)
{
  nbr_cell_table_t *n;
  linkaddr_t linkaddr;

  if(ipaddr != NULL){
    nbr_set_lladdr_from_ipaddr(&linkaddr, ipaddr);

    for(n = NBR_HEAD(); n != NULL; NBR_NEXT(n)){
      if(memcmp(&n->linkaddr, &linkaddr, sizeof(linkaddr_t)) == 0)
        return  n;
    }
  }
  return NULL;
}
/*------------------------------------------------------------------------------*/
linkaddr_t *
nbr_cell_table_get_linkaddr_from_ipaddr(uip_ipaddr_t *ipaddr)
{
  nbr_cell_table_t *n;
  linkaddr_t linkaddr;

  if(ipaddr != NULL){
    nbr_set_lladdr_from_ipaddr(&linkaddr, ipaddr);

    for(n = NBR_HEAD(); n != NULL; NBR_NEXT(n)){
      if(memcmp(&n->linkaddr, &linkaddr, sizeof(linkaddr_t)) == 0)
        return  &n->linkaddr;
    }
  }
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
  if(linkaddr != NULL && memcmp(linkaddr, &linkaddr_null, sizeof(linkaddr_t)) != 0){
    nbr_cell_table_add_nbr(linkaddr);
  }
}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_delete_nbr(linkaddr_t *linkaddr)
{
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  if(n != NULL){
    LOG_INFO("0x%02x%02x was deleted\n", n->linkaddr.u8[6], n->linkaddr.u8[7]);
#if UMAY_6T_ENABLED
    /* Soft Delete: Cache Instead of Hard Delete */
    cell_t *c;
    for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)) {
        c->state = CELL_STATE_CACHED;
        c->expiry_time = clock_seconds() + UMAY_DEFAULT_CACHE_DURATION; 
    }
    LOG_INFO("UMAY-6T: Cached %u cells for fast reconnection.\n", CELL_COUNT(n));
#else
    nbr_cell_table_del_all_nbr_cells(linkaddr, (CELL_TYPE_NEIGHBOR | CELL_TYPE_TRACK_ALL | CELL_TYPE_ALL));
    list_remove(nbr_cell_table_list, n);
    memb_free(&nbr_cell_table_mem, n);
#endif
  }
}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_delete_nbr_callback(linkaddr_t *linkaddr)
{
  if(linkaddr != NULL && memcmp(linkaddr, &linkaddr_null, sizeof(linkaddr_t)) != 0){
    nbr_cell_table_delete_nbr(linkaddr);
  }
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
  if(n != NULL){
    /* we put it at the start of the nbr_cell_table_list list. */
    list_remove(nbr_cell_table_list, n);
    list_push(nbr_cell_table_list, n);
  }
}

/*------------------------------------------------------------- NEIGHBOUR CELLS -------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
static cell_t *
nbr_cell_table_get_nbr_cell(nbr_cell_table_t *n, sixp_cell_t *cell_list)
{
  cell_t *c;

  if(n != NULL){
    for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)){
      if(c->slot_offset == cell_list->slot_offset &&
         c->channel_offset == cell_list->channel_offset){
        return c;
      }
    }
  }
  return NULL;
}
/*------------------------------------------------------------------------------*/
cell_t *
nbr_cell_table_get_nbr_slot(linkaddr_t *linkaddr, uint16_t slot_offset)
{
  cell_t *c;
  nbr_cell_table_t *n;

  n = nbr_cell_table_get_nbr(linkaddr);
  if(n != NULL){
    for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)){
      if(c->slot_offset == slot_offset){
        return c;
      }
    }
  }
  return NULL;
}
/*------------------------------------------------------------------------------*/
cell_t *
nbr_cell_table_get_nbr_min_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type)
{
  nbr_cell_table_t *n;
  cell_t *c ,*cell_min = NULL;

  n = nbr_cell_table_get_nbr(linkaddr);
  if(n != NULL){
    for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)){
      if((c->type & cell_type) && (cell_min == NULL || cell_min->slot_offset > c->slot_offset)){
        cell_min = c;
      }
    }
  }
  return cell_min;
}
/*------------------------------------------------------------------------------*/
cell_t *
nbr_cell_table_get_nbr_max_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type)
{
  nbr_cell_table_t *n;
  cell_t *c ,*cell_max = NULL;

  n = nbr_cell_table_get_nbr(linkaddr);
  if(n != NULL){
    for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)){
      if((c->type & cell_type) && (cell_max == NULL || cell_max->slot_offset < c->slot_offset)){
        cell_max = c;
      }
    }
  }
  return cell_max;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_nbr_cells(linkaddr_t *linkaddr, uint8_t cell_type, sixp_cell_t *cell_list)
{
  uint8_t cell_num = 0;
  cell_t *c;
  nbr_cell_table_t *n;

  n = nbr_cell_table_get_nbr(linkaddr);
  if(n != NULL){
    for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)){
      if((c->type & cell_type)/* && (c->lock & cell_lock)*/){
        if(cell_num < SIXP_MAX_CELLS){
          cell_list[cell_num].slot_offset = c->slot_offset;
          cell_list[cell_num].channel_offset = c->channel_offset;
          cell_num++;
        }else{
          LOG_DBG("Cell %u-%u Overflow\n", c->slot_offset, c->channel_offset);
        }
      }
    }
  }
  return cell_num;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_nbr_cell_num(linkaddr_t *linkaddr, uint8_t cell_type)
{
  uint8_t cell_num = 0;
  cell_t *c;
  nbr_cell_table_t *n;

  n = nbr_cell_table_get_nbr(linkaddr);
  if(n != NULL){
    for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)){
      if((c->type & cell_type)/* && (c->lock & cell_lock)*/){
        cell_num++;
      }
    }
  }
  return cell_num;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_add_nbr_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num, uint8_t cell_type)
{
  uint8_t i, cellnum = 0;
  cell_t *c;
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  if(n != NULL){
    for(i = 0; i < cell_num; i++){
      c = nbr_cell_table_get_nbr_slot(linkaddr, cell_list[i].slot_offset);
      if(c == NULL){
        c = memb_alloc(&slot_frame_mem);
        if(c == NULL){
          LOG_WARN("Cell memory allocation failed!\n");
          FOURE_LOG(LOG_LEVEL_2, CORE_NET_MAC, EC_4EMAC_CELL_MEMORY_ALLOC_FAILED);
          return cellnum;
        }
        c->slot_offset = cell_list[i].slot_offset;
        c->channel_offset = cell_list[i].channel_offset;
        c->type = cell_type;
        //c->lock = cell_lock;
        list_add(n->slot_frame_list, c);
      }else{
        LOG_DBG("Existing cell %u-%u t%u ", c->slot_offset, c->channel_offset, c->type);
        c->channel_offset = cell_list[i].channel_offset;
        c->type = cell_type;
        //c->lock = cell_lock;
      }
      LOG_DBG("Cell %u-%u added for 0x%02x%02x\n", c->slot_offset, c->channel_offset, linkaddr->u8[6], linkaddr->u8[7]);
      cellnum++;
    }
  }
  return cellnum;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_del_nbr_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num)
{
  uint8_t i, cellnum = 0;
  cell_t *c;
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  if(n != NULL){
    for(i = 0; i < cell_num; i++){
      c = nbr_cell_table_get_nbr_cell(n, &cell_list[i]);
      if(c != NULL){
        LOG_DBG("Cell %u-%u deleted for 0x%02x%02x\n", c->slot_offset, c->channel_offset, linkaddr->u8[6], linkaddr->u8[7]);
        list_remove(n->slot_frame_list, c);
        memb_free(&slot_frame_mem, c);
        cellnum++;
      }
    }
  }
  return cellnum;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_del_all_nbr_cells(linkaddr_t *linkaddr, uint8_t cell_type)
{
  uint8_t cellnum = 0;
  cell_t *c, *temp_c;
  nbr_cell_table_t *n = nbr_cell_table_get_nbr(linkaddr);

  if(n != NULL){
    c = CELL_HEAD(n);
    while(c != NULL){
      if((c->type & cell_type)/* && (c->lock & cell_lock)*/){
        LOG_DBG("Cell %u-%u deleted for 0x%02x%02x\n", c->slot_offset, c->channel_offset, linkaddr->u8[6], linkaddr->u8[7]);
        temp_c = c;
        CELL_NEXT(c);
        list_remove(n->slot_frame_list, temp_c);
        memb_free(&slot_frame_mem, temp_c);
        cellnum++;
      }else
        CELL_NEXT(c);
    }
  }
  return cellnum;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_is_cell_scheduled_for_any_nbr(uint16_t slot_offset, uint16_t channel_offset, linkaddr_t *ignored_linkaddr)
{
  cell_t *c;
  nbr_cell_table_t *n;
  sixp_cell_t cell = {slot_offset, channel_offset};

  for(n = NBR_HEAD(); n != NULL; NBR_NEXT(n)){
    if(ignored_linkaddr == NULL || memcmp(&n->linkaddr, ignored_linkaddr, sizeof(linkaddr_t)) != 0){
      c = nbr_cell_table_get_nbr_cell(n, &cell);
      if(c != NULL){
        LOG_INFO("Cell %u-%u is used by 0x%02x%02x\n", c->slot_offset, c->channel_offset, n->linkaddr.u8[6], n->linkaddr.u8[7]);
        return 1;
      }
    }
  }
  return 0;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_is_slot_scheduled_for_any_nbr(uint16_t slot_offset, linkaddr_t *ignored_linkaddr)
{
  cell_t *c;
  nbr_cell_table_t *n;

  for(n = NBR_HEAD(); n != NULL; NBR_NEXT(n)){
    if(ignored_linkaddr == NULL || memcmp(&n->linkaddr, ignored_linkaddr, sizeof(linkaddr_t)) != 0){
      for(c = CELL_HEAD(n); c != NULL; CELL_NEXT(c)){
        if(c->slot_offset == slot_offset){
          LOG_INFO("Cell %u-%u is used by 0x%02x%02x\n", c->slot_offset, c->channel_offset, n->linkaddr.u8[6], n->linkaddr.u8[7]);
          return 1;
        }
      }
    }
  }
  return 0;
}
/*------------------------------------------------------------- NODE CELLS -------------------------------------------------------------*/

/*------------------------------------------------------------------------------*/
slot_t *
nbr_cell_table_get_slot(linkaddr_t *linkaddr, uint16_t slot_offset)
{
  destination_t *dn;
  slot_t *sl;

  if(linkaddr != NULL){
    dn = foure_mac_buf_destination_get(linkaddr);
    for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
      if(sl->slot_offset == slot_offset){
        return sl;
      }
    }
  }else{ /* cells for all neighbor */
    for(dn = foure_mac_buf_destination_get_head(); dn != NULL; dn = list_item_next(dn)){
      for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
        if(sl->slot_offset == slot_offset){
          return sl;
        }
      }
    }
  }

  return NULL;
}
/*------------------------------------------------------------------------------*/
slot_t *
nbr_cell_table_get_min_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type)
{
  destination_t *dn;
  slot_t *sl, *slot_min = NULL;

  if(linkaddr != NULL){
    dn = foure_mac_buf_destination_get(linkaddr);
    for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
      if((sl->type & cell_type) && (slot_min == NULL || slot_min->slot_offset > sl->slot_offset)){
        slot_min = sl;
      }
    }
  }else{ /* cells for all neighbor */
    for(dn = foure_mac_buf_destination_get_head(); dn != NULL; dn = list_item_next(dn)){
      for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
        if((sl->type & cell_type) && (slot_min == NULL || slot_min->slot_offset > sl->slot_offset)){
          slot_min = sl;
        }
      }
    }
  }

  return slot_min;
}
/*------------------------------------------------------------------------------*/
slot_t *
nbr_cell_table_get_max_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type)
{
  destination_t *dn;
  slot_t *sl, *slot_max = NULL;

  if(linkaddr != NULL){
    dn = foure_mac_buf_destination_get(linkaddr);
    for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
      if((sl->type & cell_type) && (slot_max == NULL || slot_max->slot_offset < sl->slot_offset)){
        slot_max = sl;
      }
    }
  }else{ /* cells for all neighbor */
    for(dn = foure_mac_buf_destination_get_head(); dn != NULL; dn = list_item_next(dn)){
      for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
        if((sl->type & cell_type) && (slot_max == NULL || slot_max->slot_offset < sl->slot_offset)){
          slot_max = sl;
        }
      }
    }
  }

  return slot_max;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_cells(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock, uint8_t cell_offset, sixp_cell_t *cell_list, uint8_t cell_list_len)
{
  destination_t *dn;
  slot_t *sl;
  uint8_t cell_num = 0;

  if(linkaddr != NULL){
    dn = foure_mac_buf_destination_get(linkaddr);
    for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
      if((sl->type & cell_type) && (sl->lock & cell_lock) && sl->slot_offset >= cell_offset){
#if TRACKING_ENABLED
        if(sl->t == NULL)
#endif
        {
          if(cell_num < cell_list_len){
            cell_list[cell_num].slot_offset = sl->slot_offset;
            cell_list[cell_num].channel_offset = sl->channel_offset;
            cell_num++;
          }else{
            LOG_DBG("Cell %u-%u Overflow\n", sl->slot_offset, sl->channel_offset);
          }
        }
      }
    }
  }else{ /* cells for all neighbor */
    for(dn = foure_mac_buf_destination_get_head(); dn != NULL; dn = list_item_next(dn)){
      for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
        if((sl->type & cell_type) && (sl->lock & cell_lock) && sl->slot_offset >= cell_offset){
#if TRACKING_ENABLED
          if(sl->t == NULL)
#endif
          {
            if(cell_num < cell_list_len){
              cell_list[cell_num].slot_offset = sl->slot_offset;
              cell_list[cell_num].channel_offset = sl->channel_offset;
              cell_num++;
            }else{
              LOG_DBG("Cell %u-%u Overflow\n", sl->slot_offset, sl->channel_offset);
            }
          }
        }
      }
    }
  }

  return cell_num;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_cell_num(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock, uint8_t cell_offset)
{
  destination_t *dn;
  slot_t *sl;
  uint8_t cell_num = 0;

  if(linkaddr != NULL){
    dn = foure_mac_buf_destination_get(linkaddr);
    for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
      if((sl->type & cell_type) && (sl->lock & cell_lock) && sl->slot_offset >= cell_offset){
#if TRACKING_ENABLED
        if(sl->t == NULL)
#endif
        {
          cell_num++;
        }
      }
    }
  }else{ /* cells for all neighbor */
    for(dn = foure_mac_buf_destination_get_head(); dn != NULL; dn = list_item_next(dn)){
      for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
        if((sl->type & cell_type) && (sl->lock & cell_lock) && sl->slot_offset >= cell_offset){
#if TRACKING_ENABLED
          if(sl->t == NULL)
#endif
          {
            cell_num++;
          }
        }
      }
    }
  }

  return cell_num;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_schedule_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num, uint8_t cell_type, uint8_t cell_lock, unsigned long life_time)
{
  uint8_t i, ret, cellnum = 0;

  for(i = 0; i < cell_num; i++){
    ret = foure_mac_buf_slot_schedule(linkaddr, cell_type, cell_lock, cell_list[i].slot_offset, cell_list[i].channel_offset, life_time);
    if(ret & (FOURE_RET_OK | FOURE_RET_UPDATE)){
      LOG_DBG("Cell %u-%u scheduled for 0x%02x%02x\n", cell_list[i].slot_offset, cell_list[i].channel_offset, linkaddr->u8[6], linkaddr->u8[7]);
      cellnum++;
    }
  }

  return cellnum;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_unschedule_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num)
{
  uint8_t i, cellnum = 0;

  for(i = 0; i < cell_num; i++){
    foure_mac_buf_slot_unschedule(linkaddr, cell_list[i].slot_offset);
    LOG_DBG("Cell %u-%u unscheduled for 0x%02x%02x\n", cell_list[i].slot_offset, cell_list[i].channel_offset, linkaddr->u8[6], linkaddr->u8[7]);
    cellnum++;
  }

  return cellnum;
}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_lock_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num, uint8_t cell_lock)
{
  slot_t *sl;
  uint8_t i;

  for(i = 0; i < cell_num; i++){
    sl = nbr_cell_table_get_slot(linkaddr, cell_list[i].slot_offset);
    if(sl != NULL){
      sl->lock = cell_lock;
      LOG_DBG("Cell %u-%u was %s\n", cell_list[i].slot_offset, cell_list[i].channel_offset,
                                           (cell_lock == CELL_LOCK) ? "locked" : ((cell_lock == CELL_UNLOCK) ? "unlocked" : "reserved"));
    }
  }
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_lock_all(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock)
{
  destination_t *dn;
  slot_t *sl;
  uint8_t cell_num = 0;

  dn = foure_mac_buf_destination_get(linkaddr);
  for(sl = foure_mac_buf_slot_head(dn); sl != NULL; sl = list_item_next(sl)){
    if((sl->type & cell_type)){
#if TRACKING_ENABLED
      if(sl->t == NULL)
#endif
      {
        sl->lock = cell_lock;
        LOG_DBG("Cell %u-%u was %s\n", sl->slot_offset, sl->channel_offset,
                                             (cell_lock == CELL_LOCK) ? "locked" : ((cell_lock == CELL_UNLOCK) ? "unlocked" : "reserved"));
        cell_num++;
      }
    }
  }
  return cell_num;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_cell_lifetime_expired(linkaddr_t *linkaddr, uint8_t slot_offset)
{
  slot_t *sl;

  sl = nbr_cell_table_get_slot(linkaddr, slot_offset);
  if(sl != NULL){
    if(sl->lifetime.interval != FOURE_SLOT_INFINITE_LIFETIME &&
       stimer_expired(&sl->lifetime)){
      return 1;
    }
  }
  return 0;
}
/*------------------------------------------------------------------------------*/
uint8_t
nbr_cell_table_get_diff_cells(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock, sixp_cell_t *cell_list, uint8_t cell_num)
{
  uint8_t i, j, num_cell = 0;
  uint8_t cellnum;
  sixp_cell_t celllist[FOURE_MAX_SLOTS_PER_DESTINATION];

  cellnum = nbr_cell_table_get_cells(linkaddr, cell_type, cell_lock, 0, celllist, FOURE_MAX_SLOTS_PER_DESTINATION);
  if(cell_num >= cellnum){
    for(i = 0; i < cell_num; i++){
      for(j = 0; j < cellnum; j++){
        if(celllist[j].slot_offset == cell_list[i].slot_offset &&
           celllist[j].channel_offset == cell_list[i].channel_offset){
          break;
        }
      }
      if(j == cellnum){
        cell_list[num_cell].slot_offset = cell_list[i].slot_offset;
        cell_list[num_cell].channel_offset = cell_list[i].channel_offset;
        num_cell++;
      }
    }
  }else{
    for(i = 0; i < cellnum; i++){
      for(j = 0; j < cell_num; j++){
        if(cell_list[j].slot_offset == celllist[i].slot_offset &&
           cell_list[j].channel_offset == celllist[i].channel_offset){
          break;
        }
      }
      if(j == cell_num){
        celllist[num_cell].slot_offset = celllist[i].slot_offset;
        celllist[num_cell].channel_offset = celllist[i].channel_offset;
        num_cell++;
      }
    }
    memcpy(cell_list, celllist, num_cell * sizeof(sixp_cell_t));
  }
  return num_cell;
}
/*------------------------------------------------------------------------------*/
void
nbr_cell_table_sort_cells(sixp_cell_t *cell_list, uint8_t cell_num)
{
  sixp_cell_t cell;
  uint8_t i, j;

  for(i = 0; i < cell_num - 1; i++){
    for(j = 0; j < cell_num - i - 1; j++){
      if(cell_list[j].slot_offset > cell_list[j + 1].slot_offset){
        memcpy(&cell, &cell_list[j], sizeof(sixp_cell_t));
        memcpy(&cell_list[j], &cell_list[j + 1], sizeof(sixp_cell_t));
        memcpy(&cell_list[j + 1], &cell, sizeof(sixp_cell_t));
      }
    }
  }
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
#if (UIP_LLADDR_LEN == 8)
  memcpy(linkaddr, ipaddr->u8 + 8, UIP_LLADDR_LEN);
  linkaddr->u8[0] ^= 0x02;
#elif (UIP_LLADDR_LEN == 6)
  memcpy(linkaddr, ipaddr->u8 + 8, 3);
  memcpy((uint8_t *)linkaddr + 3, ipaddr->u8 + 13, 3);
  linkaddr->u8[0] ^= 0x02;
#else
#error uip-ds6.c cannot build interface address when UIP_LLADDR_LEN is not 6 or 8
#endif
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
