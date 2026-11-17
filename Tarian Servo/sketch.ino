#include <ESP32Servo.h>

const int NUM_SERVOS = 14;
const int servoPins[NUM_SERVOS] = {
  13, 12, 14, 27, 26,   // kiri   (0-4)
  25, 33,               // atas   (5-6)
  32, 15, 2, 4, 16,     // kanan  (7-11)
  17, 5                 // bawah  (12-13)
};

const int NUM_PATTERNS = 4;
const unsigned long PATTERN_DURATION = 6000;  // ms per pola

const char* patternNames[NUM_PATTERNS] = {
  "Gelombang", "Simetris", "Berurutan", "Acak"
};

Servo servos[NUM_SERVOS];
int lastAngle[NUM_SERVOS];   // sudut terakhir yang dikirim ke servo
int target[NUM_SERVOS];      // target untuk pola acak

int pattern = 0;
unsigned long patternStart = 0;
unsigned long lastChange = 0;

void setAngle(int i, int a) {
  a = constrain(a, 0, 180);
  lastAngle[i] = a;
  servos[i].write(a);
}

void startPattern(int p) {
  pattern = p;
  patternStart = millis();
  lastChange = 0;
  for (int i = 0; i < NUM_SERVOS; i++) target[i] = lastAngle[i];
  Serial.print("Pola: ");
  Serial.println(patternNames[pattern]);
}

// Pola 0: gelombang
void patternWave(float t) {
  for (int i = 0; i < NUM_SERVOS; i++) {
    float phase = i * TWO_PI / NUM_SERVOS;
    setAngle(i, 90 + 80 * sin(TWO_PI * 0.5 * t + phase));
  }
}

// Pola 1: simetris kiri-kanan dan atas-bawah
void patternMirror(float t) {
  for (int i = 0; i < 5; i++) {
    int a = 90 + 70 * sin(TWO_PI * 0.6 * t + i * 0.6);
    setAngle(i, a);
    setAngle(i + 7, 180 - a);
  }
  for (int i = 0; i < 2; i++) {
    int a = 90 + 70 * sin(TWO_PI * 0.6 * t + i * 0.8);
    setAngle(5 + i, a);
    setAngle(12 + i, 180 - a);
  }
}

// Pola 2: berurutan (efek domino), satu servo bergerak per giliran
void patternChase(unsigned long elapsed) {
  unsigned long slot = 400;
  int active = (elapsed / slot) % NUM_SERVOS;
  float p = (float)(elapsed % slot) / slot;
  for (int i = 0; i < NUM_SERVOS; i++) {
    if (i == active) setAngle(i, 90 + 70 * sin(TWO_PI * p));
    else setAngle(i, 90);
  }
}

// Pola 3: acak, bergerak halus menuju target baru tiap 0,8 detik
void patternRandom() {
  if (millis() - lastChange > 800) {
    lastChange = millis();
    for (int i = 0; i < NUM_SERVOS; i++) target[i] = random(20, 161);
  }
  for (int i = 0; i < NUM_SERVOS; i++) {
    int a = lastAngle[i];
    if (a < target[i]) a += 2;
    else if (a > target[i]) a -= 2;
    setAngle(i, a);
  }
}

void setup() {
  Serial.begin(115200);
  randomSeed(esp_random());

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].setPeriodHertz(50);
    servos[i].attach(servoPins[i], 500, 2400);
    setAngle(i, 90);
  }

  startPattern(0);
}

void loop() {
  unsigned long elapsed = millis() - patternStart;

  if (elapsed >= PATTERN_DURATION) {
    startPattern((pattern + 1) % NUM_PATTERNS);
    elapsed = 0;
  }

  float t = elapsed / 1000.0;

  switch (pattern) {
    case 0: patternWave(t);        break;
    case 1: patternMirror(t);      break;
    case 2: patternChase(elapsed); break;
    case 3: patternRandom();       break;
  }

  delay(15);
}
