/*
 * Cooja Validation Script for Data Sync (Convergence) - Matrix Based
 * 
 * Style: User-Friendly Reporting & Exact Match Verification
 * Goal: Validation of "Convergence Time" and "Network State Matrix"
 */

TIMEOUT(3600000); // 60 Minutes (1 Hour)
var REPORT_INTERVAL = 30000; // Report every 30 seconds (ms)

// Data Structures
var networkMatrix = {}; // Key: ViewerID -> Value: { TargetID -> SeqNum }
var lastReportTime = 0;
var totalNodes = 0;

function printReport() {
    var timeSec = (mote.getSimulation().getSimulationTimeMillis() / 1000.0).toFixed(2);

    log.log("\n");
    log.log("================================================================\n");
    log.log("=== CONVERGENCE REPORT (Time: " + timeSec + " sec) ===\n");
    log.log("================================================================\n");

    // 1. Final Network Knowledge Matrix (RAM Dump)
    log.log("\nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    log.log("xxx CURRENT NETWORK STATE MATRIX (From RAM) xxx\n");
    log.log("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");

    // Get all known nodes (Viewers and Targets)
    var allIds = [];
    for (var v in networkMatrix) {
        if (allIds.indexOf(parseInt(v)) === -1) allIds.push(parseInt(v));
        for (var t in networkMatrix[v]) {
            if (allIds.indexOf(parseInt(t)) === -1) allIds.push(parseInt(t));
        }
    }
    allIds.sort(function (a, b) { return a - b });

    // Header Construction
    var header = "   |";
    for (var i = 0; i < allIds.length; i++) {
        var id = allIds[i];
        header += " S" + (id < 10 ? "0" + id : id) + " |";
    }
    log.log(header + "\n");

    var sep = "---|";
    for (var i = 0; i < allIds.length; i++) sep += "----|";
    log.log(sep + "\n");

    // Rows
    for (var i = 0; i < allIds.length; i++) {
        var viewer = allIds[i];
        var rowStr = "N" + (viewer < 10 ? "0" + viewer : viewer) + "|";

        for (var j = 0; j < allIds.length; j++) {
            var target = allIds[j];
            var valLine = "  - |"; // Unknown

            if (networkMatrix[viewer] && networkMatrix[viewer][target] !== undefined) {
                var val = networkMatrix[viewer][target];
                var sVal = (val < 10) ? "0" + val : "" + val;
                valLine = " " + sVal + " |";
            }
            // Highlight self-knowledge
            if (viewer == target) {
                // valLine = "[" + valLine.trim() + "]|"; 
            }
            rowStr += valLine;
        }
        log.log(rowStr + "\n");
    }
    log.log("================================================================\n\n");
}

while (true) {
    try {
        YIELD();
    } catch (e) {
        break;
    }

    var time = mote.getSimulation().getSimulationTimeMillis();
    var msgTrim = msg.trim();
    var id = mote.getID();

    // Parse MATRIX_ROW
    // Format: MATRIX_ROW: <LocalID> <Seq1> <Seq2> ...
    if (msgTrim.startsWith("MATRIX_ROW:")) {
        var parts = msgTrim.split(" ");
        // parts[0] = MATRIX_ROW:
        // parts[1] = LocalID
        // parts[2...] = Sequence numbers starting from Node 1

        var viewerId = parseInt(parts[1]);
        if (!networkMatrix[viewerId]) networkMatrix[viewerId] = {};

        // Data starts at index 2, corresponding to Target Node ID 1
        for (var i = 2; i < parts.length; i++) {
            var targetSeq = parseInt(parts[i]);
            var targetId = i - 1; // Index 2 -> Node 1

            // Log only positive sequence numbers (known data)
            if (targetSeq > 0) {
                networkMatrix[viewerId][targetId] = targetSeq;
            }
        }
    }

    // Periodic Reporting
    if (time - lastReportTime >= REPORT_INTERVAL) {
        printReport();
        lastReportTime = time;
    }
}
