int val;
int led = 13;
 
void setup()
{
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
}
 
void loop()
{
  if (Serial.available())
  {
    val = Serial.read();
    Serial.println(val);
    // При символе "1" включаем светодиод
    if (val == '1')
    {
      digitalWrite(led, HIGH);
    }
    // При символе "0" выключаем светодиод
    if ( val == '0')
    {
      digitalWrite(led, LOW);
    }
  }
}
