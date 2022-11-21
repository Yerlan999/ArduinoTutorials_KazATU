#include <Stepper.h>
#include <Servo.h>

const int servoPin = 3;
 
const int X_pin = A0; 
const int Y_pin = A1; 

const int stepsPerRevolution = 2038;

Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
Servo servo;

void setup() {
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  servo.attach(servoPin);
}

void loop() {
  
  servo.write(map(analogRead(X_pin), 0, 1023, 0, 255));
  
  myStepper.setSpeed(50);
  myStepper.step(int((map(analogRead(Y_pin), 0, 1023, -2038, 2038)/360)*30));
  
}
