int pin =5;
unsigned long duration;
void setup() {
   Serial.begin(9600);  
   pinMode(pin, INPUT);
   delay(500); 
}

void loop() {

  uint16_t distance=0;
  duration = pulseIn(pin, HIGH);
  distance=duration/10;
  
   Serial.print("distance: ");
   Serial.print( distance);
   Serial.print(" mm,time: ");
   Serial.print( duration); 
   Serial.println("us");
   delay(100);   
}
