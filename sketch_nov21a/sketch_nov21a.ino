int led = 3;
const int analog_pin = A3;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  // конвертация от диапазона (0-1023) в диапазон (0-255)
  analogWrite(led, map(analogRead(analog_pin), 0, 1023, 0, 255));
}
