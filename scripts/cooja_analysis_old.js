/*
 * 6TiSCH / 6top (6P) Per-Node & Network Total Message Counter
 * 30 Dakikalik Test + 5 Dakikada Bir Ara Rapor
 */

/* 
 * GUI/Watchdog Timeout: 
 * Asil kontrolu biz yaptigimiz icin buna uzun bir sure veriyoruz (60 dk).
 */
TIMEOUT(4000000); 

/* Simülasyon Bitiş: 30 Dakika (1.8 Milyar us) */
var STOP_TIME = 3600000000;

/* Ara Rapor Aralığı: 5 Dakika (300 Milyon us) */
var PRINT_INTERVAL = 300000000;
var next_print_time = PRINT_INTERVAL;

var nodes = {};

function printStats() {
  log.log("\n========== 6P STATISTICS (" + (time/1000000.0).toFixed(2) + " sec) ==========\n");
  log.log("Node | ADD | LIST | DEL | SUCC | FAIL | TOTAL\n");
  log.log("-----|-----|------|-----|------|------|------\n");

  var keys = [];
  var k;
  for (k in nodes) {
    keys.push(parseInt(k, 10));
  }

  keys.sort(function(a, b) { return a - b; });

  var sum_add = 0;
  var sum_list = 0;
  var sum_del = 0;
  var sum_succ = 0;
  var sum_fail = 0;
  var sum_total = 0;

  var i;
  for (i = 0; i < keys.length; i++) {
    var id = keys[i];
    var n = nodes[id];

    log.log(
      "  " + id +
      "  |  " + n.add +
      "  |  " + n.list +
      "  |  " + n.del_cnt +
      "  |  " + n.succ +
      "  |  " + n.fail +
      "  |  " + n.total + "\n"
    );

    sum_add += n.add;
    sum_list += n.list;
    sum_del += n.del_cnt;
    sum_succ += n.succ;
    sum_fail += n.fail;
    sum_total += n.total;
  }

  log.log("-----|-----|------|-----|------|------|------\n");
  log.log(
    "TOTAL|  " + sum_add +
    "  |  " + sum_list +
    "  |  " + sum_del +
    "  |  " + sum_succ +
    "  |  " + sum_fail +
    "  |  " + sum_total + "\n"
  );
  log.log("===================================================\n");
}

while (true) {

  /* 1. BITIS KONTROLU (60. Dakika) */
  if (time >= STOP_TIME) {
      log.log("\n>>> SIMULASYON BITTI (60 DK) <<<\n");
      printStats();   // Son rapor
      log.testOK();   // Cooja'ya basarili bitti sinyali
      break; 
  }

  /* 2. PERIYODIK RAPOR KONTROLU (5 Dakikada Bir) */
  if (time >= next_print_time) {
      log.log("\n>>> PERIYODIK RAPOR (" + (next_print_time/60000000) + ". Dakika) <<<\n");
      printStats();
      next_print_time += PRINT_INTERVAL; // Bir sonraki hedefi 5dk otele
  }

  /* 3. LOG ANALIZI */
  if (msg) {
    if (!nodes[id]) {
      nodes[id] = { add: 0, list: 0, del_cnt: 0, succ: 0, fail: 0, total: 0 };
    }
    var s = nodes[id];

    if (msg.indexOf("Send ADD") >= 0 || msg.indexOf("Add request reached") >= 0) {
      s.add++; s.total++;
    }
    if (msg.indexOf("Send LIST") >= 0 || msg.indexOf("List request reached") >= 0) {
      s.list++; s.total++;
    }
    if (msg.indexOf("Send DELETE") >= 0 || msg.indexOf("Delete request reached") >= 0) {
      s.del_cnt++; s.total++;
    }
    if (msg.indexOf("Send SUCCESS") >= 0 || msg.indexOf("cmd ADD rc SUCCESS") >= 0) {
      s.succ++; s.total++;
    }
    if (msg.indexOf("FAILED") >= 0 || msg.indexOf("RSC: FAILED") >= 0) {
      s.fail++; s.total++;
    }
  }

  YIELD();
}
