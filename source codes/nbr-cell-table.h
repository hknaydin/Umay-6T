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

#ifndef NBR_CELL_TABLE_H_
#define NBR_CELL_TABLE_H_

#include "6p-protocol.h"

#define NULL_CHANNEL_OFFSET 0xFF
#define NULL_SLOT_OFFSET    0xFF

#define NBR_REACHABLE_INTERVAL (5 * 60)

#define CELL_RESERVED  SLOT_RESERVED
#define CELL_LOCK      SLOT_LOCK
#define CELL_UNLOCK    SLOT_UNLOCK
#define CELL_LOCK_ALL  SLOT_LOCK_ALL

#define CELL_TYPE_TRANSMIT       SLOT_TYPE_TRANSMIT // 0b0000 0001
#define CELL_TYPE_RECEIVE        SLOT_TYPE_RECEIVE  // 0b0000 0010
#define CELL_TYPE_SHARED         SLOT_TYPE_SHARED   // 0b0000 0100
#define CELL_TYPE_TIMEKEEP       SLOT_TYPE_TIMEKEEP // 0b0000 1000
#define CELL_TYPE_NEIGHBOR       0x10               // 0b0001 0000 /* only neighbours use to define their own cells */
#define CELL_TYPE_TRACK_TRANSMIT 0x20               // 0b0010 0000
#define CELL_TYPE_TRACK_RECEIVE  0x40               // 0b0100 0000
#define CELL_TYPE_TRACK_SHARED   0x80               // 0b1000 0000
#define CELL_TYPE_ALL            SLOT_TYPE_ALL      // 0b0000 1111

#define CELL_TYPE_TRACK_ALL   (CELL_TYPE_TRACK_TRANSMIT | CELL_TYPE_TRACK_RECEIVE | CELL_TYPE_TRACK_SHARED)
#define CELL_TYPE_TRACK_SHIFT 5

extern const char *const cell_type_str[];

/* Umay-6T Internal Cell States */
#if UMAY_6T_ENABLED
#define CELL_STATE_ACTIVE 0
#define CELL_STATE_CACHED 1
#define CELL_STATE_VALIDATING 2 /* Wait for 6P COUNT Response from returning neighbor */
#endif

typedef struct cell{
  struct cell *next;
  uint8_t slot_offset;
  uint8_t channel_offset;
  uint8_t type;
  uint8_t lock;
#if UMAY_6T_ENABLED
  /* UMAY-6T EXTENSIONS: Cell Retention Mechanism (CRM) */
  uint8_t state;             
  unsigned long expiry_time; 
#endif
}cell_t;

typedef struct nbr_cell_table{
  struct nbr_cell_table *next;
  linkaddr_t linkaddr;
  uint8_t seqnum;
  unsigned long list_info_time;
  LIST_STRUCT(slot_frame_list);
} nbr_cell_table_t;

/*--------------------------------------- NEIGHBOUR TABLE --------------------------------------*/

void nbr_cell_table_init();

nbr_cell_table_t * nbr_cell_table_get_head_nbr();

nbr_cell_table_t * nbr_cell_table_get_nbr(linkaddr_t *linkaddr);

nbr_cell_table_t * nbr_cell_table_get_nbr_from_ipaddr(uip_ipaddr_t *ipaddr);

linkaddr_t * nbr_cell_table_get_linkaddr_from_ipaddr(uip_ipaddr_t *ipaddr);

void nbr_cell_table_delete_nbr(linkaddr_t *linkaddr);

uint8_t nbr_cell_table_nbr_num();

void nbr_cell_table_set_preferred_parent(nbr_cell_table_t *n);

/*-------------------------------- NEIGHBOUR SEQ-NUM MANAGEMENT -----------------------------*/

void nbr_cell_table_clr_nbr_seqnum_management(linkaddr_t *linkaddr);

uint8_t nbr_cell_table_get_nbr_seqnum_management(linkaddr_t *linkaddr);

uint8_t nbr_cell_table_set_nbr_seqnum_management(linkaddr_t *linkaddr, uint8_t seqnum);

uint8_t nbr_cell_table_inc_nbr_seqnum_management(linkaddr_t *linkaddr);

/*------------------------------------- NEIGHBOUR CELLS ----------------------------------------*/

cell_t * nbr_cell_table_get_nbr_slot(linkaddr_t *linkaddr, uint16_t slot_offset);

cell_t * nbr_cell_table_get_nbr_min_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type);

cell_t * nbr_cell_table_get_nbr_max_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type);

uint8_t nbr_cell_table_get_nbr_cells(linkaddr_t *linkaddr, uint8_t cell_type, sixp_cell_t *cell_list);

uint8_t nbr_cell_table_get_nbr_cell_num(linkaddr_t *linkaddr, uint8_t cell_type);

uint8_t nbr_cell_table_add_nbr_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num, uint8_t cell_type);

uint8_t nbr_cell_table_del_nbr_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num);

uint8_t nbr_cell_table_del_all_nbr_cells(linkaddr_t *linkaddr, uint8_t cell_type);

uint8_t nbr_cell_table_is_slot_scheduled_for_any_nbr(uint16_t slot_offset, linkaddr_t *ignored_linkaddr);

uint8_t nbr_cell_table_is_cell_scheduled_for_any_nbr(uint16_t slot_offset, uint16_t channel_offset, linkaddr_t *ignored_linkaddr);

/*------------------------------------- NODE CELLS ---------------------------------------------*/

slot_t * nbr_cell_table_get_slot(linkaddr_t *linkaddr, uint16_t slot_offset);

slot_t * nbr_cell_table_get_min_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type);

slot_t * nbr_cell_table_get_max_offset_slot(linkaddr_t *linkaddr, uint8_t cell_type);

uint8_t nbr_cell_table_get_cells(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock, uint8_t cell_offset, sixp_cell_t *cell_list, uint8_t cell_list_len);

uint8_t nbr_cell_table_get_cell_num(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock, uint8_t cell_offset);

uint8_t nbr_cell_table_schedule_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num, uint8_t cell_type, uint8_t cell_lock, unsigned long life_time);

uint8_t nbr_cell_table_unschedule_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num);

void nbr_cell_table_lock_cells(linkaddr_t *linkaddr, sixp_cell_t *cell_list, uint8_t cell_num, uint8_t cell_lock);

uint8_t nbr_cell_table_lock_all(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock);

uint8_t nbr_cell_table_cell_lifetime_expired(linkaddr_t *linkaddr, uint8_t slot_offset);

uint8_t nbr_cell_table_get_diff_cells(linkaddr_t *linkaddr, uint8_t cell_type, uint8_t cell_lock, sixp_cell_t *cell_list, uint8_t cell_num);

void nbr_cell_table_sort_cells(sixp_cell_t *cell_list, uint8_t cell_num);
/*----------------------------------------------------------------------------------------------*/
 
void nbr_set_ip_from_prefix(uip_ipaddr_t *ipaddr, uip_ipaddr_t *prefix, linkaddr_t *linkaddr);

void nbr_set_lladdr_from_ipaddr(linkaddr_t *linkaddr, uip_ipaddr_t *ipaddr);

const char * nbr_cell_type_to_str(uint8_t cell_type);

#endif /* NBR_CELL_TABLE_H_ */
