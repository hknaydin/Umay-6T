# 6TiSCH Hareketlilik Değerlendirme Projesi (Detaylı Teknik Dokümantasyon)

Bu proje, **Nesnelerin İnterneti (IoT)** alanında, özellikle **6TiSCH** (IPv6 over the TSCH mode of IEEE 802.15.4e) protokol yığınının hareketli (mobil) ağlardaki performansını değerlendirmek için oluşturulmuş kapsamlı bir simülasyon ortamıdır.

Bu doküman, projeyi tüm teknik detaylarıyla, kod analiziyle ve mimari yapısıyla bir ders kitabı niteliğinde anlatmak için hazırlanmıştır.

---

## 📚 İçindekiler

1.  [Proje Mimarisi](#1-proje-mimarisi)
2.  [Simülasyon Yapısı (.csc Analizi)](#2-simülasyon-yapısı-csc-analizi)
3.  [Kod Analizi (cooja_analysis.js)](#3-kod-analizi-cooja_analysisjs)
4.  [Veri Formatları (.dat)](#4-veri-formatları-dat)
5.  [Nasıl Çalıştırılır?](#5-nasıl-çalıştırılır)

---

## 1. Proje Mimarisi

Bu proje üç ana bileşenin birleşiminden oluşur:

1.  **Contiki-NG İşletim Sistemi**: IoT cihazları için geliştirilmiş, açık kaynaklı, düşük güç tüketimli bir işletim sistemidir. Bu projede ağ protokol yığını (IPv6, RPL, 6LoWPAN, TSCH) Contiki-NG tarafından sağlanır.
2.  **Cooja Simülatörü**: Contiki ağlarını emüle eden güçlü bir simülatördür. Gerçek donanım kodunu (C kodu) derleyip sanal cihazlarda (Motes) çalıştırır.
3.  **BonnMotion**: Hareketlilik izlerini (mobility traces) oluşturmak için kullanılan bir araçtır. Düğümlerin zaman içindeki (x, y) koordinatlarını belirler.

### Çalışma Mantığı
Simülasyon başladığında:
1.  Cooja, `experiments/` klasöründeki `.csc` dosyasını okur.
2.  Sanal düğümleri (Motes) oluşturur ve onlara Contiki-NG firmware'ini yükler.
3.  `mobility-traces/` klasöründeki `.dat` dosyasını okuyarak düğümleri simülasyon alanında hareket ettirir.
4.  Düğümler arasındaki radyo iletişimi **UDGM (Unit Disk Graph Medium)** modeli ile simüle edilir.
5.  `scripts/cooja_analysis.js` dosyası, simülasyon çıktısını (logları) anlık olarak dinler ve performans metriklerini (PDR, Gecikme vb.) hesaplar.

---

## 2. Simülasyon Yapısı (.csc Analizi)

Simülasyon dosyaları (`.csc`), XML formatındadır ve deneyin tüm parametrelerini tanımlar.

### 2.1. Radyo Ortamı (Radio Medium)
Simülasyonda **Unit Disk Graph Medium (UDGM)** kullanılmıştır.
- **Transmitting Range (50m)**: Bir düğümün sinyalinin kesin olarak ulaşabileceği mesafe.
- **Interference Range (100m)**: Sinyalin gürültü yaratıp diğer iletişimleri bozabileceği mesafe.
- **Tx/Rx Success Ratio (1.0)**: Paket kaybı oranı. 1.0 olması, menzil içindeki iletimlerin %100 başarılı olacağını (ideal ortam) varsayar.

### 2.2. Düğüm Tipleri (Mote Types)
Kullanılan donanım modeli **MSP430 (Exp5438)** platformudur. Bu, gerçekçi bir donanım emülasyonu sağlar.
- **Firmware**: `border-router-server` (Kök düğüm) ve `border-router-client` (Uç düğümler) olmak üzere iki tip yazılım yüklenmiştir. RPL (Routing Protocol for Low-Power and Lossy Networks) bu yazılımların içinde çalışır.

### 2.3. Eklentiler (Plugins)
Simülasyonun yönetimi için kritik eklentiler aktiftir:
- **Visualizer**: Ağı görsel olarak (ızgara, trafik, LED'ler) gösterir.
- **ScriptRunner**: `cooja_analysis.js` dosyasını çalıştırarak verileri toplar.
- **Mobility**: (Radyo ortamı içinde gizli olabilir veya harici bir plugin olarak) `.dat` dosyasındaki koordinatları periyodik olarak düğümlere uygular.

---

## 3. Kod Analizi (cooja_analysis.js)

Bu JavaScript dosyası, Cooja'nın "Script Editor" eklentisi içinde çalışır. Simülasyondaki her log satırını yakalar (`msg` değişkeni) ve işler.

### 3.1. Temel Yapı ve Sabitler
```javascript
TIMEOUT(4000000);             // Scriptin zaman aşımı (güvenlik için)
var STOP_TIME = 3600000000;   // 60 Dakika (Mikrosaniye cinsinden)
var PRINT_INTERVAL = 120000000; // 2 Dakikada bir rapor yazdır
```
Simülasyonun ne kadar süreceği ve ne sıklıkla rapor vereceği burada tanımlanır.

### 3.2. Düğüm Veri Yapısı (`nodes` nesnesi)
Her düğüm için istatistikler `nodes` objesinde tutulur:
```javascript
nodes[id] = { 
  add: 0, list: 0, del_cnt: 0, ... // 6P (6top Protocol) Sayaçları
  dio_tx: 0, dio_rx: 0, ...        // RPL Kontrol Mesajları
  hello_tx: 0, hello_rx: 0         // PDR (Packet Delivery Ratio) için Hello mesajları
};
```

### 3.3. Log Analiz Döngüsü
Script sonsuz bir döngüde (`while(true)`) çalışır ve `YIELD()` komutu ile bir sonraki log satırını bekler.

#### A. 6P (6TiSCH Operation Sublayer) Analizi
6TiSCH ağlarında hücre (cell) rezervasyonu için kullanılan 6P protokolünün mesajlarını takip eder:
```javascript
if (msg.indexOf("Send ADD") >= 0) { s.add++; ... }
else if (msg.indexOf("Send DELETE") >= 0) { s.del_cnt++; ... }
```
Bu sayede ağın ne kadar dinamik olduğu ve kaynak yönetiminin başarısı ölçülür.

#### B. RPL (Routing Protocol) Analizi
Yönlendirme protokolünün sağlığını ölçmek için kontrol mesajlarını sayar:
- **DIO (DODAG Information Object)**: Ağ topolojisini kurmak için.
- **DAO (Destination Advertisement Object)**: Yukarı yönlü rota bilgisi.
- **DIS (DODAG Information Solicitation)**: Ağa katılmak isteyen düğümlerin çağrısı.

#### C. Performans: PDR ve Gecikme (Latency)
Ağın en önemli performans göstergeleridir.
1.  **PDR (Packet Delivery Ratio)**: Gönderilen paketlerin ne kadarının hedefe ulaştığı.
    - `Sending HELLO`: Düğüm paket gönderdiğinde artar (`hello_tx`).
    - `Received HELLO`: Hedef düğüm paketi aldığında artar (`hello_rx`).
    - **Formül**: `(Alınan Paket / Gönderilen Paket) * 100`

2.  **Gecikme (Latency)**: Paketin kaynaktan hedefe varması için geçen süre.
    - Log mesajında `latency=123 ms` gibi bir ifade aranır ve bu değerler toplanarak ortalaması alınır.

### 3.4. Raporlama (`printStats` fonksiyonu)
Belirlenen aralıklarda (`PRINT_INTERVAL`) konsola düzenli bir tablo yazdırır. Bu tablo şunları içerir:
- Her düğüm için ayrı ayrı 6P işlem sayıları.
- Toplam RPL kontrol mesaj trafiği.
- Ağ genelindeki PDR yüzdesi ve ortalama gecikme süresi.

---

## 4. Veri Formatları (.dat)

`mobility-traces/` klasöründeki dosyalar **BonnMotion** formatındadır. Her satır bir "hareket olayını" temsil eder.

**Format:**
```text
<Düğüm ID> <Zaman> <X Koordinatı> <Y Koordinatı>
```

**Örnek:**
```text
0 0.0 10.5 20.3  -> 0 nolu düğüm, 0. saniyede (10.5, 20.3) konumundadır.
1 2.5 15.0 22.1  -> 1 nolu düğüm, 2.5. saniyede (15.0, 22.1) konumuna varır.
```
Simülatör bu dosyayı okur ve iki zaman damgası arasındaki hareketi "enterpolasyon" ile tamamlar. Yani düğüm 0. saniyeden 2.5. saniyeye kadar o iki nokta arasında düz bir çizgide kayarak ilerler.

---

## 5. Nasıl Çalıştırılır?

1.  **Cooja'yı Başlatın**: Terminalden `ant cooja` komutu ile (Contiki dizininde).
2.  **Senaryoyu Yükleyin**: `File > Open simulation > experiments/mobil_network30node.csc`.
3.  **İz Dosyasını Kontrol Edin**: Cooja'da "Mobility" eklentisine gidin (genellikle eklentiler menüsünde veya mote özelliklerinde bulunur). `mobility-traces/` altındaki `.dat` dosyasının seçili olduğundan emin olun.
4.  **Script'i Aktif Edin**: "Simulation Script Editor" penceresinin açık olduğundan ve kodun yüklü olduğundan emin olun.
5.  **Başlat (Start)**: Simülasyonu başlatın. Script penceresinde "Log Output" kısmında istatistiklerin aktığını göreceksiniz.

---
*Bu dokümantasyon, projenin şeffaflığını ve bilimsel tekrarlanabilirliğini sağlamak amacıyla detaylandırılmıştır.*
