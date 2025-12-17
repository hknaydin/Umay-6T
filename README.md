# Mobility Trace Files (30-node & 40-node Scenarios)

This repository provides the mobility trace (`.dat`) files used in the experimental evaluation of our paper.

The goal of sharing these traces is to ensure reproducibility: the same node movements can be replayed by other researchers under identical simulation settings.

---

## 1. Mobility Model and Trace Generation

- Trace generator: **BonnMotion**
- Mobility model: **Random Waypoint (RWP)**

The traces were generated offline and exported as position files. Each file contains time-indexed (x,y) coordinates for each mobile node.

---

## 2. Scenarios Included

Two network sizes are provided:

### A) 30-node scenario
All traces under:
`mobility-traces/30-nodes/`

### B) 40-node scenario
All traces under:
`mobility-traces/40-nodes/`

> Each scenario folder contains multiple `.dat` files corresponding to different mobility parameter sets (e.g., different speed/pause configurations).

---

## 3. File Format (`.dat`)

Each `.dat` file is a plain text file where each line encodes the position of a node at a given time:

<node_id> <time_seconds> <x_meters> <y_meters>

Example:
0 0.0 0.00 10.00
1 0.0 12.34 41.60
1 2.0 23.34 25.50

- `node_id`: Integer identifier of the node
- `time_seconds`: Simulation time (seconds)
- `x_meters`, `y_meters`: Node coordinates in meters

---

## 4. How to Use

1) Select the scenario folder (30-nodes or 40-nodes).
2) Choose the desired `.dat` trace file.
3) Load the trace file into your simulator / mobility replay tool (e.g., by mapping node IDs to trace node IDs).

---

## 5. Notes on Naming

Some files may follow a compact naming scheme such as:

positions_h_<pause>_l_<speed>.dat

If you need, you can rename files into a more explicit format such as:

rwp_nodes30_speed0.5_pause2.dat
rwp_nodes40_speed2.0_pause8.dat
