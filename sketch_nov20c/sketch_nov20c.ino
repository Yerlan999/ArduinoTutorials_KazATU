#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 10

const int red_led = 2;
const int green_led = 3;

byte readCard[4];
String MasterTag = "1BFD2E21";  // Сюда вводите данные Card UID !без пробела!
String tagID = "";

MFRC522 mfrc522(SS_PIN, RST_PIN);

void access_granted(){
  digitalWrite(green_led, HIGH); delay(3000);
  digitalWrite(green_led, LOW);
}


void access_denied(){
  digitalWrite(red_led, HIGH); delay(500);
  digitalWrite(red_led, LOW); delay(500);
  digitalWrite(red_led, HIGH); delay(500);
  digitalWrite(red_led, LOW); delay(500);
  digitalWrite(red_led, HIGH); delay(500);
  digitalWrite(red_led, LOW); delay(500);   
}

void setup() {
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
    
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
}

void loop() {
  // Ожидание до приложения новой карточки
  while (getID()){    
    if (tagID == MasterTag){
      access_granted();}
    else{
      access_denied();} 
    delay(1000);
  }
}

boolean getID(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return false;}
  if ( ! mfrc522.PICC_ReadCardSerial()) { 
    return false;}
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) {
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX));}
  tagID.toUpperCase();
  mfrc522.PICC_HaltA();
  return true;
}
