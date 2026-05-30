import os
import re
import matplotlib.pyplot as plt
import argparse

def parse_log(filepath):
    results = {
        "pdr": None,
        "latency": None,
        "overhead": None
    }
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
            
            # Extract PDR
            match_pdr = re.search(r"# DATA PDR\s+:\s+%\s+([0-9.]+)", content)
            if match_pdr:
                results["pdr"] = float(match_pdr.group(1))
                
            # Extract Latency
            match_lat = re.search(r"# ORTALAMA GECIKME\s+:\s+([0-9.]+)\s+ms", content)
            if match_lat:
                results["latency"] = float(match_lat.group(1))
                
            # Extract 6P Total Overhead
            match_6p = re.search(r"TOTAL\|\s+(\d+)\s+\|\s+(\d+)\s+\|\s+(\d+)\s+\|\s+(\d+)\s+\|\s+(\d+)\s+\|\s+(\d+)", content)
            if match_6p:
                results["overhead"] = int(match_6p.group(6))
                
    except Exception as e:
        print(f"Error reading {filepath}: {e}")
        
    return results

def main():
    parser = argparse.ArgumentParser(description="Parse Cooja logs and plot results for reproducibility.")
    parser.add_argument("--logs_dir", default="../raw_logs", help="Directory containing the raw .txt/.log files")
    parser.add_argument("--output_dir", default=".", help="Directory to save the generated PDF plots")
    args = parser.parse_args()

    print(f"Scanning directory: {args.logs_dir} for log files...")
    log_files = [f for f in os.listdir(args.logs_dir) if "log" in f]
    
    if not log_files:
        print("No log files found. Please ensure the --logs_dir is correct.")
        return

    pdrs = []
    latencies = []
    overheads = []

    print("\n--- Extracted Data ---")
    for lf in log_files:
        path = os.path.join(args.logs_dir, lf)
        res = parse_log(path)
        if res["pdr"] is not None:
            pdrs.append(res["pdr"])
        if res["latency"] is not None:
            latencies.append(res["latency"])
        if res["overhead"] is not None:
            overheads.append(res["overhead"])
        print(f"File: {lf[:30]}... | PDR: {res['pdr']} | Latency: {res['latency']} | Overhead: {res['overhead']}")

    print("\n--- Summary ---")
    avg_pdr = sum(pdrs)/len(pdrs) if pdrs else 0
    avg_lat = sum(latencies)/len(latencies) if latencies else 0
    avg_ovh = sum(overheads)/len(overheads) if overheads else 0
    
    print(f"Average PDR: {avg_pdr:.2f}%")
    print(f"Average Latency: {avg_lat:.2f} ms")
    print(f"Average 6P Overhead: {avg_ovh:.1f} packets")

    # Plotting
    if not pdrs and not latencies:
        print("Not enough data to plot.")
        return

    print("\nGenerating reproducibility plots...")
    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(15, 5))
    
    # PDR Plot
    ax1.bar(["Umay-6T (Aggregated)"], [avg_pdr], color='skyblue')
    ax1.set_ylabel("Packet Delivery Ratio (%)")
    ax1.set_ylim(0, 100)
    ax1.set_title("Average PDR")

    # Latency Plot
    ax2.bar(["Umay-6T (Aggregated)"], [avg_lat], color='lightcoral')
    ax2.set_ylabel("End-to-End Latency (ms)")
    ax2.set_title("Average Latency")
    
    # Overhead Plot
    ax3.bar(["Umay-6T (Aggregated)"], [avg_ovh], color='lightgreen')
    ax3.set_ylabel("6P Control Packets")
    ax3.set_title("Total 6P Overhead")

    plt.tight_layout()
    plot_path = os.path.join(args.output_dir, "reproducibility_plots.pdf")
    plt.savefig(plot_path)
    print(f"Successfully generated plots: {plot_path}")

if __name__ == '__main__':
    main()
