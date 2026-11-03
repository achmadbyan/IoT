#include <WiFi.h>
#include <time.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi bawaan simulator Wokwi
const char* WIFI_SSID = "Wokwi-GUEST";
const char* WIFI_PASS = "";

// Pengaturan NTP / zona waktu
const char* NTP_SERVER = "pool.ntp.org";
const long  GMT_OFFSET_SEC = 7 * 3600;  // WIB = UTC+7
const int   DST_OFFSET_SEC = 0;

// LCD 16x2 dengan alamat I2C 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

const char* HARI[] = {"Min", "Sen", "Sel", "Rab", "Kam", "Jum", "Sab"};

int lastSecond = -1;

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);  // SDA = GPIO21, SCL = GPIO22
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Menghubungkan...");

  WiFi.begin(WIFI_SSID, WIFI_PASS, 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung");

  configTime(GMT_OFFSET_SEC, DST_OFFSET_SEC, NTP_SERVER);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sinkron waktu...");

  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    delay(500);
  }
  lcd.clear();
}

void loop() {
  struct tm t;
  if (!getLocalTime(&t)) {
    lcd.setCursor(0, 0);
    lcd.print("Gagal ambil NTP ");
    delay(1000);
    return;
  }

  // Update layar hanya saat detik berganti
  if (t.tm_sec == lastSecond) {
    delay(50);
    return;
  }
  lastSecond = t.tm_sec;

  char baris1[17];
  char baris2[17];

  // Baris 1: Hari, tanggal  -> "Sel 03/11/2026"
  snprintf(baris1, sizeof(baris1), "%s %02d/%02d/%04d  ",
           HARI[t.tm_wday], t.tm_mday, t.tm_mon + 1, t.tm_year + 1900);

  // Baris 2: Jam digital    -> "  14:23:45 WIB "
  snprintf(baris2, sizeof(baris2), "  %02d:%02d:%02d WIB ",
           t.tm_hour, t.tm_min, t.tm_sec);

  lcd.setCursor(0, 0);
  lcd.print(baris1);
  lcd.setCursor(0, 1);
  lcd.print(baris2);
}
