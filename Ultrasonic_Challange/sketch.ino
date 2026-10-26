#define TRIG_PIN 5
#define ECHO_PIN 18

// Definisi pin komponen baru
#define LED_RED 14
#define LED_YELLOW 27  // Sambungkan ke pin D27 di Wokwi
#define LED_GREEN 26   // Sambungkan ke pin D26 di Wokwi
#define BUZZER_PIN 25  // Sambungkan ke pin D25 di Wokwi

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;

  if (distance > 60.0) {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, LOW);
    noTone(BUZZER_PIN); 
  } else if (distance >= 30.0 && distance <= 60.0) {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, HIGH);
    digitalWrite(LED_RED, LOW);
    tone(BUZZER_PIN, 1000); delay(200); noTone(BUZZER_PIN); delay(200);
  } else {
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_RED, HIGH);
    tone(BUZZER_PIN, 1500); delay(50); noTone(BUZZER_PIN); delay(50);
  }
  delay(100);
}
