# Umay-6T Source Code Repository

This repository contains the core implementation of **Umay-6T: Mobility-Resilient Cell Caching for Uninterrupted 6TiSCH Networks**, specifically targeting the Contiki-NG operating system.

## Overview
Umay-6T introduces a **Soft State** cell retention and fast reconnection mechanism natively at the 6top Operation Sublayer (6P). By avoiding immediate cell deletions during transient link losses, Umay-6T minimizes 6P/RPL re-negotiation overhead and safely reactivates cached time-frequency resources under mobility constraints.

## Repository Contents
To facilitate reproducibility and simplify experimental integration for the academic community, this folder provides the specific `.c`, `.h`, and `.h` configurations that override the traditional baseline Contiki-NG OS behaviors:

- **`project-conf.h`**: Defines the activation macros (`UMAY_6T_ENABLED`), the consistency switches, and the adaptive cache model parameters ($\alpha, \beta, T_{min}$).
- **`nbr-cell-table.h`**: Extends the default `cell_t` schedule object with expiration timestamps and `state` transition enumerators (`CELL_STATE_ACTIVE`, `CELL_STATE_CACHED`, `CELL_STATE_VALIDATING`).
- **`nbr-cell-table.c`**: Implements the Cell Retention Mechanism (CRM) garbage collection, soft-deletion handling, and RPL cross-layer resynchronization logic.
- **`6top-pce.c`**: Contains the protocol consistency callback responsible for verifying the `6P COUNT` of the returning neighbor to prevent "Zombie Cell" scheduling conflicts.
- **`4emac-6top-scheduler-minimal.c`**: Demonstrates how the core TSCH queue iteration process gracefully skips TX procedures for cells marked as `CACHED` or `VALIDATING` untilFRM reauthorizes them.
- **`Makefile`**: Compilation integration example for the `cooja` target.

## Usage
1. Replace or integrate the logic in `nbr-cell-table.c` and `nbr-cell-table.h` into your `os/net/mac/tsch` or equivalent MAC driver folder.
2. Enable `UMAY_6T_ENABLED 1` inside your local `project-conf.h`.
3. Compile normally via `make TARGET=cooja` and launch your dynamic mobility scenarios using the RWP mobility extensions.

For further evaluation details, please refer to the main Umay-6T Manuscript evaluation section.
