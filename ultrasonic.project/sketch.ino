#define TRIG_PIN 5
#define ECHO_PIN 18
#define LED_PIN 14

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT); // TRIG memancarkan suara
  pinMode(ECHO_PIN, INPUT);  // ECHO mendengarkan pantulan
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(LED_PIN, LOW);
}

void loop() {
  // 1. Memancarkan gelombang suara selama 10 mikrodetik
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // 2. Membaca waktu pantulan (dalam mikrodetik)
  long duration = pulseIn(ECHO_PIN, HIGH);

  // 3. Menghitung jarak ke centimeter
  float distance = duration * 0.034 / 2;

  if (distance < 20.0) {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }

  Serial.print("Jarak: ");
  Serial.print(distance);
  Serial.println(" cm");

  delay(1000); // Jeda 1 detik antar pembacaan
}
