#include <IRremote.h>

int RECV_PIN = 7;
const int red_pin = 9;
const int green_pin = 10;
const int blue_pin = 11;

int red_brightness = 0;
int green_brightness = 0;
int blue_brightness = 0;

int current_color = 0;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  pinMode(red_pin,OUTPUT);
  pinMode(green_pin,OUTPUT);
  pinMode(blue_pin,OUTPUT);
    
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}


void rbg_led(int red, int green, int blue){
  red_brightness = red;
  green_brightness = green;
  blue_brightness = blue;
  
  if (red_brightness == 255){
    current_color = 1;
  }
  else if (green_brightness == 255){
    current_color = 2;
  }
  else if  (blue_brightness == 255){
    current_color = 3;
  }
  else{
    current_color = 0;
  }
  analogWrite(red_pin, red_brightness);
  analogWrite(green_pin, green_brightness);
  analogWrite(blue_pin, blue_brightness);
}

void decreaseORincrease_brightness(int increment){
  if (current_color == 1){
    red_brightness += increment;
    analogWrite(red_pin, red_brightness);
  }
  else if (current_color == 2){
    green_brightness += increment;
    analogWrite(red_pin, green_brightness);    
  }
  else if (current_color == 3){
    blue_brightness += increment;
    analogWrite(red_pin, blue_brightness);    
  }
  else{
    red_brightness += increment; green_brightness += increment; blue_brightness += increment;
    analogWrite(red_pin, red_brightness); 
    analogWrite(red_pin, green_brightness); 
    analogWrite(red_pin, blue_brightness);    
  }
}

void loop() {
  if (irrecv.decode(&results)) {
    
    switch (results.value){
    
    case 0xFF30CF:
      Serial.println("1 is pressed");
      rbg_led(255, 0, 0);
      break;

    case 0xFF18E7:
      Serial.println("2 is pressed");
      rbg_led(0, 255, 0);
      break;
    
    case 0xFF7A85:
      Serial.println("3 is pressed");
      rbg_led(0, 0, 255);
      break;

    case 0xFFA857:
      Serial.println("+ is pressed");
      decreaseORincrease_brightness(100);
      break;    

    case 0xFFE01F:
      Serial.println("- is pressed");
      decreaseORincrease_brightness(-100);
      break;    

    case 0xFF906F:
      Serial.println("EQ is pressed");
      rbg_led(0, 0, 0);
      break; 
                
    default:
      Serial.println(results.value, HEX);
      break;
    }
  
    irrecv.resume(); // Receive the next value
    
  }
  delay(100);
}
