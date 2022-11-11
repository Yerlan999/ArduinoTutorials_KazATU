#include <IRremote.h>

int RECV_PIN = 7;

const int red_pin = 9;
const int green_pin = 10;
const int blue_pin = 11;

int brightness = 0;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  pinMode(red_pin,OUTPUT);
  pinMode(green_pin,OUTPUT);
  pinMode(blue_pin,OUTPUT);
    
  Serial.begin(9600);
  irrecv.enableIRIn();
}


void setRGBLed(int red_value, int green_value, int blue_value){
  analogWrite(red_pin, red_value);
  analogWrite(green_pin, green_value);
  analogWrite(blue_pin, blue_value);
}


void changeBrightness(int increment){
  
}

void loop() {
  if (irrecv.decode(&results)) {
    
    switch (results.value){
    
    case 0xFD08F7:
      Serial.println("1 is pressed");
      setRGBLed(255, 0, 0);
      break;

    case 0xFD8877:
      Serial.println("2 is pressed");
      setRGBLed(0, 255, 0);
      break;
    
    case 0xFD48B7:
      Serial.println("3 is pressed");
      setRGBLed(0, 0, 255);
      break;

    case 0xFD807F:
      Serial.println("+ is pressed");
      changeBrightness(100);
      break;    

    case 0xFD906F:
      Serial.println("- is pressed");
      changeBrightness(-100);
      break;    

    case 0xFDB04F:
      Serial.println("EQ is pressed");
      setRGBLed(0, 0, 0);
      break; 
                
    default:
      Serial.println(results.value, HEX);
      break;
    }
  
    irrecv.resume();
    
  }
  delay(100);
}
