const int buzzer = 2; 

void setup(){
  pinMode(buzzer, OUTPUT); 
}

void loop(){
  tone(buzzer, 500); delay(1000);        
  noTone(buzzer); delay(1000);

  tone(buzzer, 1000); delay(1000);        
  noTone(buzzer); delay(1000);

  tone(buzzer, 1500); delay(1000);        
  noTone(buzzer); delay(1000);
}
