#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
 
// Выбор типа дисплея
U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
 
void setup(void) {
  u8g2.begin();
  u8g2.enableUTF8Print();    // включение поддержки кодировки UTF8
}
 
void loop(void) {
  u8g2.setFont(u8g2_font_unifont_t_cyrillic);  // выбор шрифта
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 15);
    u8g2.print(452);
    u8g2.setCursor(0, 40);
    u8g2.print("Привет, мир!");
  } while ( u8g2.nextPage() );
  delay(1000);
}
