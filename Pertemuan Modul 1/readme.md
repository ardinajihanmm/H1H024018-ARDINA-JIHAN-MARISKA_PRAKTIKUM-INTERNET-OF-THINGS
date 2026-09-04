# Praktikum Internet of Things (TK245005)
## Modul 1 – Sensor dan Aktuator

| | |
|---|---|
| Nama | Ardina Jihan Mariska |
| NIM | H1H024018 |
| Shift | B |
| Modul | 1 – Sensor dan Aktuator |

---

Modul 1 ini membahas dua konsep dasar sistem IoT, yaitu akuisisi data sensor dan kendali aktuato, menggunakan mikrokontroler ESP32.

- Percobaan 1A membaca data suhu dan kelembaban dari sensor DHT22 dan menampilkannya ke Serial Monitor. Percobaan ini dilakukan langsung menggunakan hardware di lab (ESP32 + DHT22 pada breadboard).
- Percobaan 2A mengendalikan relay (disimulasikan dengan LED) secara otomatis berdasarkan nilai suhu yang dibaca dari DHT22, menggunakan logika ambang batas (threshold). Percobaan ini dilakukan menggunakan simulasi Wokwi, karena keterbatasan waktu dan jumlah komponen DHT22/relay yang harus dipakai bergantian dengan praktikan lain.

Alur kerja umum sistem: sensor membaca kondisi lingkungan → ESP32 memproses data dan mengambil keputusan → aktuator memberikan respons sesuai kondisi yang telah ditentukan (threshold).

---

## Library / Dependencies

| Library | Fungsi | Sumber |
|---|---|---|
| DHT.h (DHT sensor library) | Membaca data suhu & kelembaban dari sensor DHT22/DHT11 | Arduino Library Manager (Adafruit DHT sensor library) |
| Board Manager ESP32 | Agar Arduino IDE dapat meng-compile & upload program ke board ESP32 | esp32 by Espressif Systems (Board Manager URL) |

Fungsi-fungsi utama dari DHT.h yang digunakan pada modul ini:
- dht.begin() — menginisialisasi komunikasi dengan sensor DHT.
- dht.readTemperature() — membaca nilai suhu dalam satuan derajat Celsius.
- dht.readHumidity() — membaca nilai kelembaban relatif dalam satuan persen (%).
- isnan(nilai) — memeriksa apakah hasil pembacaan sensor valid (bukan Not a Number).

Instalasi:
1. Buka Arduino IDE → Tools → Board → Boards Manager → cari esp32, install.
2. Sketch → Include Library → Manage Libraries → cari DHT sensor library (Adafruit), install (beserta dependensinya, Adafruit Unified Sensor).
3. Pilih board ESP32 Dev Module dan port USB yang sesuai sebelum compile/upload.

---

## Percobaan 1A – Akuisisi Data Sensor DHT22

### Kode Program

```cpp
#include <DHT.h>

#define DHTPIN 4        // pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT22    // tipe sensor yang digunakan
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin(); // inisialisasi sensor DHT22
  Serial.println("Memulai akuisisi data sensor DHT22...");
}

void loop() {
  // Membaca data kelembaban dan suhu
  float kelembaban = dht.readHumidity();
  float suhu = dht.readTemperature();

  // Periksa apakah pembacaan berhasil
  if (isnan(kelembaban) || isnan(suhu)) {
    Serial.println("Gagal membaca data dari sensor DHT22!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C, Kelembaban: ");
    Serial.print(kelembaban);
    Serial.println(" %");
  }
  delay(2000); // jeda pembacaan setiap 2 detik
}
```

### Penjelasan Kode

| Baris / Bagian | Penjelasan |
|---|---|
| #include <DHT.h> | Memanggil pustaka DHT sensor library agar ESP32 dapat berkomunikasi dengan sensor DHT22. |
| #define DHTPIN 4 | Mendefinisikan pin data sensor pada GPIO 4. |
| #define DHTTYPE DHT22 | Mendefinisikan tipe sensor yang dipakai (DHT22, bukan DHT11). |
| DHT dht(DHTPIN, DHTTYPE); | Membuat objek dht dari kelas DHT berdasarkan pin dan tipe sensor yang telah didefinisikan. |
| Serial.begin(115200); | Mengaktifkan komunikasi serial dengan baud rate 115200 agar data dapat ditampilkan di Serial Monitor. |
| dht.begin(); | Menginisialisasi komunikasi antara ESP32 dan sensor DHT22. |
| dht.readHumidity() / dht.readTemperature() | Membaca nilai kelembaban dan suhu terkini dari sensor. |
| isnan(kelembaban) \|\| isnan(suhu) | Memeriksa apakah salah satu hasil pembacaan tidak valid (NaN). |
| delay(2000); | Memberi jeda 2 detik sebelum pembacaan berikutnya, sesuai batas waktu sampling minimum sensor DHT22. |

### Penjelasan Fungsi

- setup() — dijalankan satu kali saat ESP32 dinyalakan/reset; digunakan untuk inisialisasi komunikasi serial dan sensor.
- loop() — dijalankan berulang secara terus-menerus; berisi logika pembacaan sensor, pengecekan validitas data, dan penampilan hasil ke Serial Monitor setiap 2 detik.
- dht.begin(), dht.readHumidity(), dht.readTemperature() — fungsi bawaan dari pustaka DHT.h yang menangani komunikasi protokol 1-Wire dengan sensor DHT22.
- isnan() — fungsi bawaan C/C++ untuk memeriksa apakah suatu nilai float adalah Not a Number.

### Penjelasan Percabangan (Conditional)

```cpp
if (isnan(kelembaban) || isnan(suhu)) {
  // kondisi gagal: salah satu atau kedua nilai tidak valid
} else {
  // kondisi berhasil: kedua nilai valid, tampilkan hasil
}
```
Percabangan ini menggunakan operator logika OR (||) sehingga program masuk ke cabang "gagal" apabila salah satu saja dari nilai suhu atau kelembaban bernilai NaN — ini penting karena sensor bisa saja berhasil membaca satu besaran tapi gagal membaca besaran lainnya pada siklus yang sama.

### Hasil Pengamatan

| No. | Waktu | Kondisi Sensor | Suhu (°C) | Kelembaban (%) | Status Pembacaan |
|---|---|---|---|---|---|
| 1 | 00:00 | Kondisi normal | 28.10 | 52 | Valid |
| 2 | 00:02 | Sensor didekatkan ke api/sumber panas | 49.80 | 16 | Valid |
| 3 | 00:04 | Sensor didekatkan ke AC/sumber udara dingin | 26.70 | 49 | Valid |

Seluruh pembacaan berstatus Valid, tidak ada NaN. Suhu naik signifikan saat didekatkan ke sumber panas dan kelembaban ikut turun (udara panas menampung lebih banyak uap air relatif terhadap jumlah uap air yang tetap, sehingga RH turun); sebaliknya saat didekatkan ke AC, suhu turun dan kelembaban naik mendekati kondisi normal.

### Jawaban Pertanyaan Praktikum 1A 
1) Flowchart proses akuisisi data sensor DHT22

![Flowchart](Skematik%20Rangkaian/Flowchart.png)

3) Apa fungsi dari perintah isnan() pada program tersebut?
isnan() memeriksa apakah nilai hasil pembacaan sensor (suhu atau kelembaban) berupa NaN (Not a Number), yaitu kondisi ketika sensor gagal memberikan data valid, misalnya akibat gangguan komunikasi antara ESP32 dan DHT22. Dengan pengecekan ini, program dapat menampilkan pesan kesalahan alih-alih menampilkan data yang tidak valid.

4) Mengapa diperlukan jeda (delay) minimal ±2 detik antar pembacaan sensor DHT22?
Sensor DHT22 memiliki sampling rate maksimum sekitar 0,5 Hz (satu kali pembacaan tiap 2 detik) sesuai datasheet-nya. Jika dibaca lebih cepat dari itu, sensor belum menyelesaikan siklus pengukuran internalnya sehingga data yang dikembalikan bisa tidak akurat atau bahkan NaN.

5) Modifikasi: rata-rata 5 kali pembacaan sebelum ditampilkan

```cpp
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const int JUMLAH_SAMPEL = 5; // banyaknya pembacaan yang dirata-ratakan

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("Memulai akuisisi data sensor DHT22 (rata-rata 5 sampel)...");
}

void loop() {
  float totalSuhu = 0;
  float totalKelembaban = 0;
  int sampelValid = 0;

  for (int i = 0; i < JUMLAH_SAMPEL; i++) {
    float k = dht.readHumidity();
    float s = dht.readTemperature();
    if (!isnan(k) && !isnan(s)) {
      totalSuhu += s;
      totalKelembaban += k;
      sampelValid++;
    }
    delay(2000); // jeda wajib antar pembacaan sensor
  }

  if (sampelValid > 0) {
    float rataSuhu = totalSuhu / sampelValid;
    float rataKelembaban = totalKelembaban / sampelValid;
    Serial.print("Rata-rata Suhu: ");
    Serial.print(rataSuhu);
    Serial.print(" °C, Rata-rata Kelembaban: ");
    Serial.print(rataKelembaban);
    Serial.println(" %");
  } else {
    Serial.println("Seluruh pembacaan pada siklus ini gagal!");
  }
}
```

Penjelasan tiap baris tambahan:
- const int JUMLAH_SAMPEL = 5; — jumlah pembacaan yang akan dirata-ratakan dalam satu siklus.
- totalSuhu, totalKelembaban, sampelValid — variabel akumulator untuk menjumlahkan hasil pembacaan valid dan menghitung banyaknya sampel yang berhasil dibaca.
- for (int i = 0; i < JUMLAH_SAMPEL; i++) — perulangan 5 kali untuk mengambil 5 sampel suhu dan kelembaban.
- if (!isnan(k) && !isnan(s)) — hanya sampel valid yang dijumlahkan ke total, agar sampel gagal tidak merusak hasil rata-rata.
- delay(2000) di dalam perulangan — tetap mempertahankan jeda minimum antar pembacaan sensor pada setiap iterasi.
- Setelah perulangan selesai, rata-rata dihitung dengan membagi total nilai dengan sampelValid, lalu ditampilkan ke Serial Monitor.
- Jika seluruh sampel dalam satu siklus gagal (sampelValid == 0), program menampilkan pesan kegagalan untuk menghindari pembagian dengan nol.

---

## Percobaan 2A – Kendali Aktuator Relay Berdasarkan Data Sensor
### Kode Program

```cpp
#include <DHT.h>

#define DHTPIN 4       // pin data DHT22 terhubung ke GPIO 4
#define DHTTYPE DHT22
#define RELAYPIN 26    // pin kendali relay/LED indikator
DHT dht(DHTPIN, DHTTYPE);

const float suhuThreshold = 30.0; // ambang batas suhu (°C)

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW); // pastikan aktuator mati di awal
}

void loop() {
  float suhu = dht.readTemperature();

  if (isnan(suhu)) {
    Serial.println("Gagal membaca data sensor!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    // Kendali aktuator berdasarkan hasil akuisisi data sensor
    if (suhu > suhuThreshold) {
      digitalWrite(RELAYPIN, HIGH); // aktifkan relay/LED
      Serial.println("Aktuator: ON");
    } else {
      digitalWrite(RELAYPIN, LOW); // matikan relay/LED
      Serial.println("Aktuator: OFF");
    }
  }
  delay(2000);
}
```

### Penjelasan Kode

| Baris / Bagian | Penjelasan |
|---|---|
| #define RELAYPIN 26 | Pin GPIO 26 digunakan sebagai keluaran digital untuk mengendalikan relay/LED. |
| const float suhuThreshold = 30.0; | Nilai ambang batas suhu (°C) sebagai acuan keputusan kendali aktuator. |
| pinMode(RELAYPIN, OUTPUT); | Mengatur pin 26 sebagai pin keluaran (output). |
| digitalWrite(RELAYPIN, LOW); (di setup()) | Memastikan aktuator dalam kondisi mati saat program pertama kali dijalankan. |
| dht.readTemperature() | Membaca suhu terkini dari sensor DHT22. |
| if (isnan(suhu)) | Mengecek validitas data suhu sebelum dipakai untuk pengambilan keputusan. |
| if (suhu > suhuThreshold) { ... } else { ... } | Logika utama kendali aktuator: relay ON jika suhu melebihi threshold, OFF jika tidak. |
| digitalWrite(RELAYPIN, HIGH/LOW) | Mengubah status keluaran fisik pin relay sesuai keputusan program. |

### Penjelasan Fungsi

- setup() — menginisialisasi komunikasi serial, sensor, serta pin relay sebagai output dan memastikan kondisi awal aktuator OFF.
- loop() — membaca suhu setiap 2 detik, memvalidasi data, lalu memutuskan status relay berdasarkan perbandingan terhadap suhuThreshold.
- pinMode() / digitalWrite() — fungsi bawaan Arduino core untuk mengatur mode pin dan menuliskan sinyal digital HIGH/LOW ke pin tersebut, yang secara fisik menyalakan/mematikan relay atau LED.

### Penjelasan Percabangan (Conditional)

```cpp
if (isnan(suhu)) {
  // data tidak valid: jangan ambil keputusan kendali
} else {
  if (suhu > suhuThreshold) {
    // suhu melebihi threshold -> aktuator ON
  } else {
    // suhu di bawah/sama dengan threshold -> aktuator OFF
  }
}
```
Struktur ini bersifat nested if-else: pengecekan validitas data (isnan) dilakukan lebih dulu sebagai guard clause, baru kemudian logika kendali aktuator dijalankan hanya jika data valid — sehingga aktuator tidak diubah statusnya berdasarkan data yang keliru.

### Hasil Pengamatan

| No. | Waktu | Suhu (°C) | Threshold (°C) | Kondisi Suhu | Status Aktuator | Kondisi LED/Relay |
|---|---|---|---|---|---|---|
| 1 | 00:00 | 28.0 | 30 | ≤ threshold | OFF | Mati |
| 2 | 00:02 | 30.4 | 30 | > threshold | ON | Menyala |
| 3 | 00:04 | 29.5 | 30 | ≤ threshold | OFF | Mati |

Aktuator berubah status secara konsisten mengikuti kondisi if (suhu > suhuThreshold), tanpa error selama simulasi.

### Jawaban Pertanyaan Praktikum 2A 
1) Mengapa diperlukan nilai ambang batas (threshold) dalam sistem kendali aktuator berbasis sensor?
Threshold menjadi acuan batas keputusan agar mikrokontroler dapat menentukan kapan aktuator harus diaktifkan/dimatikan berdasarkan kondisi lingkungan yang terukur. Tanpa threshold, sistem tidak punya patokan jelas untuk membedakan kondisi normal dan kondisi yang memerlukan respons aktuator.

2) Apa yang terjadi jika suhuThreshold diturunkan menjadi 20.0?
Aktuator akan ON hampir sepanjang waktu karena suhu ruangan normal umumnya sudah di atas 20 °C. Sistem kehilangan fungsinya sebagai kendali otomatis yang responsif terhadap kondisi ekstrem, menjadi boros energi dan mempercepat keausan komponen aktuator.

3) Perbedaan kendali kondisi tunggal vs histerisis (dua ambang batas)?
Kondisi tunggal memakai satu nilai threshold untuk menyalakan dan mematikan aktuator, sehingga saat suhu berada tepat di sekitar nilai tersebut, aktuator dapat menyala-mati berulang kali dalam waktu singkat (chattering). Histerisis memakai dua nilai batas berbeda (batas atas untuk menyalakan, batas bawah untuk mematikan) sehingga ada rentang aman (dead band) yang mencegah perubahan status terlalu sering akibat fluktuasi suhu kecil.

4) Modifikasi: histerisis (ON di atas 30 °C, OFF di bawah 28 °C)

```cpp
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT22
#define RELAYPIN 26
DHT dht(DHTPIN, DHTTYPE);

const float batasAtas = 30.0;  // suhu untuk menyalakan aktuator
const float batasBawah = 28.0; // suhu untuk mematikan aktuator
bool statusAktuator = false;   // status aktuator saat ini (false = OFF)

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);
}

void loop() {
  float suhu = dht.readTemperature();

  if (isnan(suhu)) {
    Serial.println("Gagal membaca data sensor!");
  } else {
    Serial.print("Suhu: ");
    Serial.print(suhu);
    Serial.print(" °C -> ");

    // Logika histerisis dua ambang batas
    if (!statusAktuator && suhu > batasAtas) {
      statusAktuator = true; // nyalakan jika suhu melewati batas atas
    } else if (statusAktuator && suhu < batasBawah) {
      statusAktuator = false; // matikan jika suhu turun di bawah batas bawah
    }

    digitalWrite(RELAYPIN, statusAktuator ? HIGH : LOW);
    Serial.println(statusAktuator ? "Aktuator: ON" : "Aktuator: OFF");
  }
  delay(2000);
}
```

Penjelasan:
- batasAtas = 30.0 dan batasBawah = 28.0 — dua ambang batas suhu untuk logika histerisis, menggantikan satu variabel suhuThreshold.
- bool statusAktuator = false; — menyimpan status aktuator saat ini, agar program "mengingat" kondisi aktuator antar-siklus loop().
- if (!statusAktuator && suhu > batasAtas) — aktuator hanya dinyalakan jika sebelumnya mati dan suhu telah melewati batas atas (30 °C).
- else if (statusAktuator && suhu < batasBawah) — aktuator hanya dimatikan jika sebelumnya menyala dan suhu telah turun di bawah batas bawah (28 °C).
- Jika suhu berada di antara 28–30 °C, tidak ada kondisi terpenuhi sehingga statusAktuator tetap sama seperti sebelumnya (tidak terjadi chattering).
- digitalWrite(RELAYPIN, statusAktuator ? HIGH : LOW); — menerapkan status aktuator ke pin relay berdasarkan nilai boolean statusAktuator.

---

## Pertanyaan Analisis

1) Uraikan hasil tugas pada praktikum yang telah dilakukan pada setiap percobaan!
Pada Percobaan 1A, program berhasil mengakuisisi data suhu dan kelembaban dari sensor DHT22 secara langsung menggunakan hardware, dengan seluruh pembacaan berstatus valid (tidak ada NaN) dan responsif terhadap perubahan kondisi lingkungan (naik saat didekatkan sumber panas, turun saat didekatkan AC). Pada Percobaan 2A, melalui simulasi Wokwi, program berhasil mengendalikan aktuator (LED sebagai representasi relay) secara otomatis berdasarkan nilai suhu terhadap threshold 30 °C — aktuator ON saat suhu melebihi threshold dan OFF saat suhu berada di bawahnya, sesuai spesifikasi yang diharapkan pada kedua percobaan.

2) Bagaimana pengaruh akurasi dan waktu tanggap (response time) sensor terhadap kecepatan reaksi aktuator pada sistem IoT?
Akurasi sensor menentukan seberapa tepat keputusan yang diambil mikrokontroler; jika data yang dibaca meleset dari kondisi sebenarnya, aktuator dapat memberikan respons yang salah (misalnya menyala padahal suhu sebenarnya belum melewati threshold). Waktu tanggap (response time) sensor — pada DHT22 dibatasi oleh sampling rate sekitar 0,5 Hz — menentukan seberapa cepat perubahan kondisi lingkungan dapat terdeteksi dan diteruskan menjadi aksi aktuator. Semakin lambat waktu tanggap sensor, semakin besar jeda antara terjadinya perubahan kondisi nyata dengan reaksi aktuator, sehingga untuk aplikasi yang membutuhkan respons cepat (misalnya sistem keselamatan) diperlukan sensor dengan waktu tanggap yang lebih singkat dibanding DHT22.

3) Bagaimana cara kerja sistem dalam mengubah data sensor menjadi keputusan kendali aktuator (proses akuisisi hingga aktuasi)?
Prosesnya dimulai dari akuisisi: sensor DHT22 mengukur suhu (dan kelembaban) lalu mengirimkan data digital ke ESP32 melalui pin data. Data tersebut divalidasi menggunakan isnan() untuk memastikan pembacaan berhasil. Selanjutnya pada tahap pengambilan keputusan, ESP32 membandingkan nilai suhu terhadap nilai ambang batas (threshold) menggunakan struktur percabangan (if-else). Hasil perbandingan tersebut menentukan aktuasi: ESP32 mengirim sinyal digital (digitalWrite) ke pin yang terhubung ke relay, sehingga relay berubah status (ON/OFF) sesuai keputusan yang diambil — mengubah data mentah sensor menjadi aksi fisik pada aktuator.

4) Bagaimana kombinasi antara akuisisi data sensor dan kendali aktuator dapat digunakan untuk membangun sistem IoT yang responsif terhadap perubahan kondisi lingkungan, misalnya pada smart farming atau smart home?
Kombinasi akuisisi data dan kendali aktuator memungkinkan sistem bereaksi secara otomatis tanpa campur tangan manusia secara terus-menerus. Pada smart farming, misalnya, sensor kelembaban tanah/udara dapat memicu aktuator berupa pompa air atau kipas untuk menyalakan irigasi/ventilasi otomatis ketika kondisi berada di luar rentang ideal tanaman. Pada smart home, sensor suhu seperti DHT22 dapat mengendalikan relay yang terhubung ke AC atau kipas agar menyala otomatis saat suhu ruangan melebihi threshold kenyamanan, dan mati kembali saat suhu sudah turun. Dengan menambahkan logika seperti histerisis (dua ambang batas) sebagaimana dibahas pada Percobaan 2A, sistem semacam ini dapat bekerja lebih stabil dan efisien energi karena tidak berganti status secara berlebihan akibat fluktuasi kecil pada data sensor.
