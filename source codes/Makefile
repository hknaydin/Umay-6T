# Makefile for Umay-6T Source Integration Example
# This Makefile demonstrates how to include the Umay-6T specific MAC/6top files in a Contiki-NG build.

CONTIKI_PROJECT = umay-6t-demo
all: $(CONTIKI_PROJECT)

# Add Umay-6T MAC layer specific source files to the build
PROJECT_SOURCEFILES += nbr-cell-table.c 6top-pce.c 4emac-6top-scheduler-minimal.c

# Specify network stack routines
MAKE_MAC = MAKE_MAC_TSCH
MAKE_NET = MAKE_NET_IPV6
MAKE_ROUTING = MAKE_ROUTING_RPL_CLASSIC

# Point to the root of the Contiki-NG or customized stack directory
CONTIKI = ../../contiki-ng

# Standard Contiki-NG compilation included
include $(CONTIKI)/Makefile.include
