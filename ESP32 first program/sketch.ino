const int ledPin = 15;
const int buttonPin = 21;

int buttonState = 0;// variabel untuk menyimpan status tombol

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  buttonState = digitalRead(buttonPin);// baca status tombol
  
  if (buttonState == HIGH) {
    digitalWrite(ledPin, HIGH);// hidupkan LED
    } else {
    digitalWrite(ledPin, LOW);// matikan LED
  }
}

