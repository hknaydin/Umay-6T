#test code: python3 /home/mavialp/Downloads/Umay/run_parallel_cooja.py "/home/mavialp/Downloads/Umay/6tisch-ng-sra-v17/6tisch-ng-sra/umay-6t h8l2/30noderandommobiliteenabled/mobil_network_random_30node_mobilite_enabled.csc" -n 5
import os
import re
import random
import subprocess
import argparse
import time

def parse_results(log_content):
    """
    Parses the results from the log file.
    Tailored specifically to the Script Runner output format in your .csc.
    """
    results = {
        "pdr": None,
        "latency": None,
        "dao_pdr": None,
        "p6_add": None,
        "p6_list": None,
        "p6_del": None,
        "p6_succ": None,
        "p6_fail": None,
        "p6_total": None,
        "dio_tx": None,
        "dio_rx": None,
        "power_on": None
    }
    
    # DATA PDR             : % 95.5
    match_pdr = re.search(r"# DATA PDR\s+:\s+%\s+([0-9.]+)", log_content)
    if match_pdr:
        results["pdr"] = float(match_pdr.group(1))
        
    # ORTALAMA GECIKME     : 145.2 ms
    match_latency = re.search(r"# ORTALAMA GECIKME\s+:\s+([0-9.]+)\s+ms", log_content)
    if match_latency:
        results["latency"] = float(match_latency.group(1))
        
    # RPL DAO PDR (Unicast): % 98.2 (Tx:100 / Rx:98)
    match_dao = re.search(r"# RPL DAO PDR.*?:\s+%\s+([0-9.]+)", log_content)
    if match_dao:
        results["dao_pdr"] = float(match_dao.group(1))

    # 6P Statistics: TOTAL|  add  |  list  |  del  |  succ  |  fail  |  total
    match_6p = re.search(r"TOTAL\|\s+(\d+)\s+\|\s+(\d+)\s+\|\s+(\d+)\s+\|\s+(\d+)\s+\|\s+(\d+)\s+\|\s+(\d+)", log_content)
    if match_6p:
        results["p6_add"] = int(match_6p.group(1))
        results["p6_list"] = int(match_6p.group(2))
        results["p6_del"] = int(match_6p.group(3))
        results["p6_succ"] = int(match_6p.group(4))
        results["p6_fail"] = int(match_6p.group(5))
        results["p6_total"] = int(match_6p.group(6))

    # DIO Stats: # DIO Tx / Rx          : 45 / 80
    match_dio = re.search(r"# DIO Tx / Rx\s+:\s+(\d+)\s+/\s+(\d+)", log_content)
    if match_dio:
        results["dio_tx"] = int(match_dio.group(1))
        results["dio_rx"] = int(match_dio.group(2))

    # PowerTracker Stats
    # Genelde simulasyon kapanirken cikan "PowerTracker:  Radio ON: 1.25 %" benzeri bir log
    match_pwr = re.search(r"PowerTracker.*?radio [oO][nN].*?([0-9.]+)\s*%", log_content)
    if match_pwr:
        results["power_on"] = float(match_pwr.group(1))

    return results

def main():
    parser = argparse.ArgumentParser(description="Run Cooja instances in parallel with random seeds.")
    parser.add_argument("csc_file", help="Path to the base .csc file")
    parser.add_argument("-n", "--runs", type=int, default=5, help="Number of simulations to run (Default: 5)")
    
    parser.add_argument("--cmd", type=str, 
                        default="cd ~/Downloads/Umay/6tisch-ng-sra-v17/6tisch-ng-sra/tools/cooja && java -mx512m -jar dist/cooja.jar -nogui={csc_file_path} -contiki=../../",
                        help="Command to run Cooja in headless mode")

    args = parser.parse_args()

    if not os.path.exists(args.csc_file):
        print(f"Error: File {args.csc_file} not found.")
        return

    with open(args.csc_file, "r", encoding="utf-8") as f:
        csc_content = f.read()

    print(f"--- Starting {args.runs} parallel simulations... ---")
    
    processes = []
    
    for i in range(args.runs):
        seed = random.randint(100000, 999999)
        
        # Replace the <randomseed> tag in the XML
        new_content = re.sub(
            r"<randomseed>.*?</randomseed>", 
            f"<randomseed>{seed}</randomseed>", 
            csc_content
        )
        
        # If <randomseed> doesn't exist, insert it into <simulation>
        if "<randomseed>" not in csc_content:
            new_content = new_content.replace("<simulation>", f"<simulation>\n    <randomseed>{seed}</randomseed>")

        # Create a temporary .csc file
        base_name = os.path.basename(args.csc_file)
        name, ext = os.path.splitext(base_name)
        new_csc_path = os.path.abspath(f"{name}_seed_{seed}{ext}")
        log_file_path = os.path.abspath(f"{name}_seed_{seed}_log.txt")
        
        with open(new_csc_path, "w", encoding="utf-8") as f:
            f.write(new_content)
        
        # Prepare the execution command
        run_cmd = args.cmd.format(csc_file_path=new_csc_path)
        
        print(f"[{i+1}/{args.runs}] Starting... Seed: {seed} | Log: {log_file_path}")
        
        # Open log file and start the background process
        log_f = open(log_file_path, "w")
        p = subprocess.Popen(run_cmd, shell=True, stdout=log_f, stderr=subprocess.STDOUT)
        
        processes.append((p, log_f, new_csc_path, log_file_path, seed))

        # Add a 5 second delay so identical motes do not lock up while compiling simultaneously
        time.sleep(5)

    print("All tasks are running in the background. Please wait for them to finish...\n")

    # Wait for all processes to finish
    completed_results = []
    for p, log_f, new_csc_path, log_file_path, seed in processes:
        p.wait()
        log_f.close()
        
        # Parse the results
        with open(log_file_path, "r", encoding="utf-8") as f:
            log_content = f.read()
        res = parse_results(log_content)
        res["seed"] = seed
        res["log"] = log_file_path
        completed_results.append(res)
        
        print(f"Finished: Seed {seed} -> PDR: {res['pdr']} | Latency: {res['latency']} ms")

        # Optionally delete the temporary .csc file
        try:
            os.remove(new_csc_path)
        except OSError:
            pass

    # Calculate average values
    valid_pdr = [r["pdr"] for r in completed_results if r["pdr"] is not None]
    valid_latency = [r["latency"] for r in completed_results if r["latency"] is not None]
    valid_dao_pdr = [r["dao_pdr"] for r in completed_results if r["dao_pdr"] is not None]
    valid_p6_total = [r["p6_total"] for r in completed_results if r["p6_total"] is not None]

    print("\n" + "="*50)
    print("                 OVERALL RESULTS")
    print("="*50)
    
    if valid_pdr:
        avg_pdr = sum(valid_pdr) / len(valid_pdr)
        print(f"Average DATA PDR           : % {avg_pdr:.2f}")
    else:
        print("!! Could not find DATA PDR in logs.")
        
    if valid_latency:
        avg_lat = sum(valid_latency) / len(valid_latency)
        print(f"Average LATENCY            : {avg_lat:.2f} ms")
    else:
        print("!! Could not find LATENCY in logs.")
        
    if valid_dao_pdr:
        avg_dao = sum(valid_dao_pdr) / len(valid_dao_pdr)
        print(f"Average DAO PDR            : % {avg_dao:.2f}")
    else:
        print("!! Could not find DAO PDR in logs.")
        
    if valid_p6_total:
        avg_p6_add = sum(r["p6_add"] for r in completed_results if r["p6_add"] is not None) / len(valid_p6_total)
        avg_p6_del = sum(r["p6_del"] for r in completed_results if r["p6_del"] is not None) / len(valid_p6_total)
        avg_p6_succ = sum(r["p6_succ"] for r in completed_results if r["p6_succ"] is not None) / len(valid_p6_total)
        avg_p6_total = sum(valid_p6_total) / len(valid_p6_total)
        print(f"Average 6P ADD             : {avg_p6_add:.1f}")
        print(f"Average 6P DELETE          : {avg_p6_del:.1f}")
        print(f"Average 6P SUCCESS         : {avg_p6_succ:.1f}")
        print(f"Average 6P TOTAL           : {avg_p6_total:.1f}")
        
    valid_dio = [r["dio_tx"] for r in completed_results if r.get("dio_tx") is not None]
    if valid_dio:
        avg_dio_tx = sum(r["dio_tx"] for r in completed_results if r.get("dio_tx") is not None) / len(valid_dio)
        avg_dio_rx = sum(r["dio_rx"] for r in completed_results if r.get("dio_rx") is not None) / len(valid_dio)
        print(f"Average DIO Tx             : {avg_dio_tx:.1f}")
        print(f"Average DIO Rx             : {avg_dio_rx:.1f}")
        
    valid_pwr = [r["power_on"] for r in completed_results if r.get("power_on") is not None]
    if valid_pwr:
        avg_pwr = sum(r["power_on"] for r in completed_results if r.get("power_on") is not None) / len(valid_pwr)
        print(f"Average Radio ON (Energy)  : % {avg_pwr:.2f}")
    
    print("="*50)
    print("Log files and specific run results were saved to the current directory.")

if __name__ == "__main__":
    main()
