int led = 3;
const int analog_pin = A0;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  // readings = от 0 до 1023
  int readings = analogRead(analog_pin);
  // 1023/256 ~ 4
  analogWrite(led, readings/4);
}
