# Mobility Trace Files (30-node & 40-node Scenarios)

This repository provides the mobility trace (`.dat`) files used in the experimental evaluation of our paper.

The goal of sharing these traces is to ensure reproducibility: the same node movements can be replayed by other researchers under identical simulation settings.

---

## 1. Mobility Model and Trace Generation

- Trace generator: **BonnMotion**
- Mobility model: **Random Waypoint (RWP)**

The traces were generated offline and exported as position files. Each file contains time-indexed (x,y) coordinates for each mobile node.

### Visualization

The following visualization illustrates the random waypoint mobility pattern. Nodes (represented by indices) move towards random destinations within the 100x100m simulation area. The lines represent the trajectories of individual nodes over the simulation duration.

<img width="1448" height="957" alt="image" src="https://github.com/user-attachments/assets/5b93ece3-76a0-4b19-a955-01d99e1db403" />

*Figure 1: Example of node trajectories generated using the Random Waypoint model in BonnMotion.*

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

```text
<node_id> <time_seconds> <x_meters> <y_meters>
```

Example:
```text
0 0.0 0.00 10.00
1 0.0 12.34 41.60
1 2.0 23.34 25.50
```

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

`positions_h_<pause>_l_<speed>.dat`

If you need, you can rename files into a more explicit format such as:

`rwp_nodes30_speed0.5_pause2.dat`
`rwp_nodes40_speed2.0_pause8.dat`

---

## 6. Custom Methodology: Trace Generation with ID Remapping

Unlike standard simulations where nodes are generically indexed (0 to N-1), our experimental setup required **specific Cooja Node IDs** to be mobile while others remained static. To achieve this, we developed a custom generation workflow.

### Tools Used
- `bin/generate_cooja_trace.sh`: A wrapper shell script that automates the generation process.
- `bin/wml2dat`: A custom Python script that converts WiseML output to Cooja's `.dat` format and handles ID remapping.
- `test_config.txt`: A configuration file defining the specific nodes to include.

### Workflow Description
1. **Configuration**: We define the target nodes and their initial static positions in a config file (e.g., `test_config.txt`):
   ```text
   # NodeID X Y
   1 10 10
   3 50 50
   5 90 90
   ```
   *This example specifies that only Node 1, Node 3, and Node 5 are mobile.*

2. **Automated Generation**: The `generate_cooja_trace.sh` script parses this config to determine the required node count ($N=3$). It then invokes **BonnMotion** to generate random movements for $N$ abstract nodes.

3. **ID Mapping**: The output is processed by `wml2dat`, which maps the abstract BonnMotion node indices (0, 1, 2) to our specific Cooja IDs (1, 3, 5).

4. **Result**: The final `positions.dat` file contains trajectories strictly for the IDs defined in our configuration, ensuring seamless integration with our Cooja simulation experiments.

**Command Example:**
```bash
./bin/generate_cooja_trace.sh -o positions.dat --config test_config.txt -- -d 100 -x 100 -y 100
```
Figure 2: Execution of the custom trace generation workflow. The screenshot demonstrates the validation of the configuration file (test_config.txt), which defines the specific target nodes and their static coordinates. Subsequently, the wrapper script (generate_cooja_trace.sh) is executed to invoke BonnMotion. Unlike the standard utilization, this script parses the configuration to automatically determine the node count and performs an ID remapping process, assigning the generated Random Waypoint trajectories to the designated Cooja node IDs (e.g., Node 1, 3, 5) ensuring precise mobility control within the simulation.

<img width="1448" height="957" alt="image" src="https://github.com/user-attachments/assets/5b93ece3-76a0-4b19-a955-01d99e1db403" />
Figure 2. Execution of the custom trace generation workflow.
