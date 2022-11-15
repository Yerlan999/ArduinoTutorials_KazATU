#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
ThreeWire myWire(4,5,2); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

#define Echo 9
#define Trig 8
#define countof(a) (sizeof(a) / sizeof(a[0]))
 
unsigned int impulseTime=0; 
unsigned int distance_sm=0; 
char datestring[20];


unsigned int get_distance(){
  digitalWrite(Trig, HIGH); 
  /* Подаем импульс на вход trig дальномера */
  delayMicroseconds(10); // равный 10 микросекундам 
  digitalWrite(Trig, LOW); // Отключаем 
  impulseTime=pulseIn(Echo, HIGH); // Замеряем длину              импульса 
  distance_sm=impulseTime/58; // Пересчитываем в сантиметры 
  delay(100);
  return distance_sm;    
}

void printDateTime(const RtcDateTime& dt){
    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print("Current time: ");
    Serial.println(datestring);
}

void setup (){
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  Serial.print("compiled: ");
  Serial.print(__DATE__);
  Serial.println(__TIME__);

  Rtc.Begin();

  RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
  printDateTime(compiled);
  Serial.println();

  if (!Rtc.IsDateTimeValid()){
      Serial.println("RTC lost confidence in the DateTime!");
      Rtc.SetDateTime(compiled);}
  if (Rtc.GetIsWriteProtected()){
      Serial.println("RTC was write protected, enabling writing now");
      Rtc.SetIsWriteProtected(false);}
  if (!Rtc.GetIsRunning()){
      Serial.println("RTC was not actively running, starting now");
      Rtc.SetIsRunning(true);}
  
  RtcDateTime now = Rtc.GetDateTime();
  
  if (now < compiled) {
      Serial.println("RTC is older than compile time!  (Updating DateTime)");
      Rtc.SetDateTime(compiled);}
  else if (now > compiled){
      Serial.println("RTC is newer than compile time. (this is expected)");}
  else if (now == compiled){
      Serial.println("RTC is the same as compile time! (not expected but all is fine)");}
}

void loop (){
  RtcDateTime now = Rtc.GetDateTime();
  printDateTime(now);
  Serial.println();

  if (!now.IsValid()){
    Serial.println("RTC lost confidence in the DateTime!");}
    delay(3000);
  Serial.print("Distance: ");
  Serial.println();
  unsigned int distance = get_distance();
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print(distance);
  lcd.setCursor(0,1);
  lcd.print(datestring);
}
