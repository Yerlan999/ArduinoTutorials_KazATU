// Зеленая кнопка на 8 пин (Игрок 2)
// Красная кнопка на 9 пин (Игрок 1)

// Зеленый светодиод на 2 пин
// Желтые светодиоды на 3-6 пины
// Красный светодиод на 7 пин


int timer = 100;           
int ledPins[] = {2, 3, 4, 5, 6, 7};
int pinCount = 6;

const int green_button = 8;
const int red_button = 9;

int player_one_score_Red = 0;
int player_two_score_Green = 0;


void register_button_push(int thisPin){
  if (digitalRead(red_button) == HIGH && ledPins[thisPin] != 7){
    player_one_score_Red--;
    Serial.println("Player RED: " + String(player_one_score_Red) + " || Player GREEN: " + String(player_two_score_Green));
  }
  else if (digitalRead(red_button) == HIGH && ledPins[thisPin] == 7){
    player_one_score_Red++;
    Serial.println("Player RED: " + String(player_one_score_Red) + " || Player GREEN: " + String(player_two_score_Green));    
  }
  
  if (digitalRead(green_button) == HIGH && ledPins[thisPin] != 2){
    player_two_score_Green--;
    Serial.println("Player RED: " + String(player_one_score_Red) + " || Player GREEN: " + String(player_two_score_Green));
  }
  else if (digitalRead(green_button) == HIGH && ledPins[thisPin] == 2){
    player_two_score_Green++;
    Serial.println("Player RED: " + String(player_one_score_Red) + " || Player GREEN: " + String(player_two_score_Green));
  }
}


void turn_on_all_leds(){
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    digitalWrite(ledPins[thisPin], HIGH);
  }
}


void turn_off_all_leds(){
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    digitalWrite(ledPins[thisPin], LOW);
  }
}


void check_who_won(){
  if (player_one_score_Red > 5){
    Serial.println("Player Red Has Won The Game!!!");
    turn_on_all_leds();
    delay(10000);
    player_one_score_Red = 0;
    player_two_score_Green = 0;
    turn_off_all_leds();
  }
  if (player_two_score_Green > 5){
    Serial.println("Player Green Has Won The Game!!!");
    turn_on_all_leds();
    delay(10000);
    player_one_score_Red = 0;
    player_two_score_Green = 0;
    turn_off_all_leds();
  }
}


void setup() {
  Serial.begin(9600);
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
  }
  pinMode(green_button, INPUT);
  pinMode(red_button, INPUT);
}


void loop() {
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    register_button_push(thisPin);
    digitalWrite(ledPins[thisPin], HIGH);
    delay(timer);
    digitalWrite(ledPins[thisPin], LOW);
  }
  for (int thisPin = pinCount - 2; thisPin >= 1; thisPin--) {
    register_button_push(thisPin);
    digitalWrite(ledPins[thisPin], HIGH);
    delay(timer);
    digitalWrite(ledPins[thisPin], LOW);
  }
  check_who_won();
}
