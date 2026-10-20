#include "DHT.h"

// Tentukan pin data dan jenis sensor mengikut sambungan
#define DHTPIN1 13       // Pin data DHT22 disambungkan ke GPIO 13
#define DHTPIN2 14       // Pin data DHT22 disambungkan ke GPIO 14
#define DHTTYPE DHT22   // Jenis sensor ialah DHT22 (AM2302)

#define LED1_PIN 18      // Pin LED 1
#define LED2_PIN 19      // Pin LED 2

// Batas ambang suhu
const float TEMP_THRESHOLD = 60.0;

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setup() {
  // Mulai komunikasi serial pada baud rate 115200
  Serial.begin(115200);
  Serial.println(F("Mulai membaca sensor DHT22..."));

  // start sensor DHT
  dht1.begin();
  dht2.begin();

  // Konfigurasi pin LED sebagai OUTPUT
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Kondisi awal LED mati
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
}

void loop() {
  // Tunggu sekurang-kurangnya 2 saat antara setiap bacaan
  delay(2000);

  // Baca kelembapan udara (peratusan)
  float humidity1 = dht1.readHumidity();
  float humidity2 = dht2.readHumidity();
  // Baca suhu dalam Celsius (°C)
  float temperature1 = dht1.readTemperature();
  float temperature2 = dht2.readTemperature();

  // Semak sama ada bacaan berjaya diperoleh
  if (isnan(humidity1) || isnan(temperature1) 
     || isnan(humidity1) || isnan(temperature1)) {
    Serial.println(F("Ralat: Gagal membaca data dari sensor DHT22!"));
    return;
  }

  // Paparkan hasil bacaan ke Serial Monitor
  Serial.print(F("Suhu 1: "));
  Serial.print(temperature1);
  Serial.print(F(" °C | Kelembapan 1: "));
  Serial.print(humidity1);
  Serial.println(F(" %"));

  // Logika kontrol LED berdasarkan suhu > 60 °C
  if (temperature1 > TEMP_THRESHOLD) {
    digitalWrite(LED1_PIN, HIGH);
  } else {
    digitalWrite(LED1_PIN, LOW);
  }

  Serial.print(F("Suhu 2: "));
  Serial.print(temperature2);
  Serial.print(F(" °C | Kelembapan 2: "));
  Serial.print(humidity2);
  Serial.println(F(" %"));

  // Logika kontrol LED berdasarkan suhu > 60 °C
  if (temperature2 > TEMP_THRESHOLD) {
    digitalWrite(LED2_PIN, HIGH);
  } else {
    digitalWrite(LED2_PIN, LOW);
  }

  Serial.println();
}

