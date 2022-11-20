#include <Keypad.h>

const byte ROWS = 4; // 4 строки
const byte COLS = 4; // 4 столбца

const int red_led = 2;
const int green_led = 3;

const String correct_password = "1957"; 

// Обозначение наименовании кнопок
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; // пины к строкам
byte colPins[COLS] = {10, 11, 12, 13}; // пины к столбцам

char password[4];  
int counter = 0;


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
void check_password(){
  if (String(password[counter]) == String(correct_password[counter])){
  }
  else{
    counter = -1;
    memset(password, 0, sizeof(password));
    access_denied();
  }  
}

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

void setup(){
  Serial.begin(9600);
  pinMode(red_led, OUTPUT);
  pinMode(green_led, OUTPUT);
}

void loop(){
  char customKey = customKeypad.getKey();
  if (customKey){
    password[counter] = customKey;
    check_password();
    counter++;
    if (counter > 3){
      counter = 0;
      memset(password, 0, sizeof(password));
      access_granted();
    }
  }
}
