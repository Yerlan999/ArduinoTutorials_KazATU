#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define Echo 3
#define Trig 2
 
unsigned int impulseTime=0; 
unsigned int distance_sm=0; 
 
void setup(){  
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  Serial.begin(9600);

  lcd.begin();
  lcd.backlight();
  lcd.clear();
} 


unsigned int get_distance(){
  digitalWrite(Trig, HIGH); 
  /* Подаем импульс на вход trig дальномера */
  delayMicroseconds(10); // равный 10 микросекундам 
  digitalWrite(Trig, LOW); // Отключаем 
  impulseTime=pulseIn(Echo, HIGH); // Замеряем длину              импульса 
  distance_sm=impulseTime/58; // Пересчитываем в сантиметры 
  Serial.println(distance_sm);
  delay(100);
  return distance_sm;
     
}

void loop(){  
  

  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(get_distance());
}
