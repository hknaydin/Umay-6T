/*
 * 6TiSCH ANALYZER: 6P + RPL + PDR + LATENCY
 * Server Reply: KAPALI (Log analizi ile hesaplanir)
 * Rapor Araligi: 2 DAKIKA
 */

TIMEOUT(4000000); 
var STOP_TIME = 3600000000; /* 60 Dakika */

/* Rapor Araligi: 2 Dakika (120.000.000 us) */
var PRINT_INTERVAL = 120000000;
var next_print_time = PRINT_INTERVAL;

var nodes = {};

/* Global Network Stats */
var global_hello_tx = 0;
var global_hello_rx = 0;
var global_latency_sum = 0;
var global_latency_count = 0;

/* IP'den Node ID bulma */
function getSenderIdFromIp(ipString) {
  if (!ipString) return -1;
  var parts = ipString.trim().split(":");
  var lastPart = parts[parts.length - 1];
  return parseInt(lastPart, 16); 
}

function initNode(id) {
    if (!nodes[id]) {
      nodes[id] = { 
        add: 0, list: 0, del_cnt: 0, succ: 0, fail: 0, total: 0, // 6P
        dio_tx: 0, dio_rx: 0, dao_tx: 0, dao_rx: 0, dis_tx: 0, dis_rx: 0, // RPL
        hello_tx: 0, hello_rx: 0 // PDR
      };
    }
}

function printStats() {
  log.log("\n========== ISTATISTIK RAPORU (" + (time/1000000.0).toFixed(2) + " sn) ==========\n");
  
  var keys = [];
  var k;
  for (k in nodes) {
    keys.push(parseInt(k, 10));
  }
  keys.sort(function(a, b) { return a - b; });

  /* --- 6P ISTATISTIKLERI --- */
  log.log("[6P MESAJLARI]\n");
  log.log("Node | ADD | LIST | DEL | SUCC | FAIL | TOTAL\n");
  log.log("-----|-----|------|-----|------|------|------\n");

  var s_add=0, s_list=0, s_del=0, s_succ=0, s_fail=0, s_total=0;
  var rpl_dio_tx=0, rpl_dio_rx=0, rpl_dao_tx=0, rpl_dao_rx=0, rpl_dis_tx=0, rpl_dis_rx=0, rpl_total=0;

  for (var i = 0; i < keys.length; i++) {
    var id = keys[i];
    var n = nodes[id];

    log.log("  " + id + "  |  " + n.add + "  |  " + n.list + "  |  " + n.del_cnt + "  |  " + n.succ + "  |  " + n.fail + "  |  " + n.total + "\n");
    s_add += n.add; s_list += n.list; s_del += n.del_cnt; s_succ += n.succ; s_fail += n.fail; s_total += n.total;
    
    rpl_dio_tx += n.dio_tx; rpl_dio_rx += n.dio_rx; rpl_dao_tx += n.dao_tx; rpl_dao_rx += n.dao_rx; rpl_dis_tx += n.dis_tx; rpl_dis_rx += n.dis_rx;
    rpl_total += (n.dio_tx + n.dio_rx + n.dao_tx + n.dao_rx + n.dis_tx + n.dis_rx);
  }
  log.log("-----|-----|------|-----|------|------|------\n");
  log.log("TOTAL|  " + s_add + "  |  " + s_list + "  |  " + s_del + "  |  " + s_succ + "  |  " + s_fail + "  |  " + s_total + "\n\n");

  /* --- RPL OZET --- */
  log.log("[RPL KONTROL MESAJLARI]\n");
  log.log("DIO (Tx/Rx): " + rpl_dio_tx + " / " + rpl_dio_rx + "\n");
  log.log("DAO (Tx/Rx): " + rpl_dao_tx + " / " + rpl_dao_rx + "\n");
  log.log("DIS (Tx/Rx): " + rpl_dis_tx + " / " + rpl_dis_rx + "\n");
  log.log("TOPLAM RPL : " + rpl_total + "\n\n");

  /* --- GLOBAL PDR & LATENCY --- */
  var global_pdr = 0;
  var avg_latency = 0;

  if (global_hello_tx > 0) {
      global_pdr = (global_hello_rx * 100.0) / global_hello_tx;
  }
  if (global_latency_count > 0) {
      avg_latency = global_latency_sum / global_latency_count;
  }

  log.log("##################################################\n");
  log.log("# AG PERFORMANS ANALIZI (NETWORK PERFORMANCE)    #\n");
  log.log("##################################################\n");
  log.log("# TOPLAM GONDERILEN (HELLO Tx) : " + global_hello_tx + "\n");
  log.log("# TOPLAM ALINAN     (HELLO Rx) : " + global_hello_rx + "\n");
  log.log("# ORTALAMA GECIKME  (LATENCY)  : " + avg_latency.toFixed(2) + " ms\n");
  log.log("# ---------------------------------------------- #\n");
  log.log("# GLOBAL PDR                   : % " + global_pdr.toFixed(2) + "\n");
  log.log("##################################################\n");
  log.log("==============================================================\n");
}


while (true) {
  /* 1. BITIS */
  if (time >= STOP_TIME) {
      log.log("\n>>> SIMULASYON BITTI (60 DK) <<<\n");
      printStats();
      log.testOK();
      break; 
  }

  /* 2. ARA RAPOR (2 Dakikada Bir) */
  if (time >= next_print_time) {
      log.log("\n>>> ARA RAPOR (" + (next_print_time/60000000) + ". Dakika) <<<\n");
      printStats();
      next_print_time += PRINT_INTERVAL;
  }

  /* 3. LOG ANALIZI */
  if (msg) {
    initNode(id);
    var s = nodes[id];

    /* --- 6P --- */
    if (msg.indexOf("Send ADD") >= 0 || msg.indexOf("Add request reached") >= 0) { s.add++; s.total++; }
    else if (msg.indexOf("Send LIST") >= 0 || msg.indexOf("List request reached") >= 0) { s.list++; s.total++; }
    else if (msg.indexOf("Send DELETE") >= 0 || msg.indexOf("Delete request reached") >= 0) { s.del_cnt++; s.total++; }
    else if (msg.indexOf("Send SUCCESS") >= 0 || msg.indexOf("cmd ADD rc SUCCESS") >= 0) { s.succ++; s.total++; }
    else if (msg.indexOf("FAILED") >= 0 || msg.indexOf("RSC: FAILED") >= 0) { s.fail++; s.total++; }
  
    /* --- RPL --- */
    if (msg.indexOf("RPL") >= 0) {
        if (msg.indexOf("Received a DIO") >= 0) { s.dio_rx++; }
        if (msg.indexOf("Sending a DIO") >= 0 || msg.indexOf("Sending a multicast-DIO") >= 0) { s.dio_tx++; }
        if (msg.indexOf("Received a DAO") >= 0) { s.dao_rx++; }
        if (msg.indexOf("Sending a DAO") >= 0) { s.dao_tx++; }
        if (msg.indexOf("Received a DIS") >= 0) { s.dis_rx++; }
        if (msg.indexOf("Sending a DIS") >= 0) { s.dis_tx++; }
    }
    
    /* --- PDR (HELLO Tx) --- */
    if (msg.indexOf("Sending HELLO") >= 0) {
        s.hello_tx++;
        global_hello_tx++;
    }

    /* --- PDR (HELLO Rx) & LATENCY --- */
    if (msg.indexOf("Received HELLO") >= 0 && msg.indexOf("latency") >= 0) {
        try {
            // 1. PDR icin kaynak ID'yi bul
            var splitFrom = msg.split("from");
            if (splitFrom.length > 1) {
                var rest = splitFrom[1]; 
                var ipAndLat = rest.trim().split(" ");
                var senderIp = ipAndLat[0]; 
                
                var senderId = getSenderIdFromIp(senderIp);
                if (senderId > 0) {
                    initNode(senderId);
                    nodes[senderId].hello_rx++;
                    global_hello_rx++;
                }

                // 2. Latency degerini bul
                var splitLat = msg.split("latency");
                if (splitLat.length > 1) {
                    var latStr = splitLat[1].trim().split(" ")[0]; 
                    var latVal = parseInt(latStr, 10);
                    if (!isNaN(latVal)) {
                        global_latency_sum += latVal;
                        global_latency_count++;
                    }
                }
            }
        } catch(e) { }
    }
  }

  YIELD();
}
