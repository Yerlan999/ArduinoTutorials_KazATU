// Подключение необходимых библиотек
#include <ThreeWire.h>  
#include <RtcDS1302.h>

// Создание объекта Rtc посредством объекта myWire
// для дальнейшего взаимодействия
ThreeWire myWire(4,5,2); // пины: IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);

void setup (){
    Serial.begin(9600); // старт цифрового послед. порта (монитора)
    // Вывод на экран текущего времени во время компиляции(загрузки) скетча
    // с Операционной Системы устройства (ПК)
    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);

    Rtc.Begin(); // начало работы с модулем реального времени.
    // Создание объекта текущего времени с ОС
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled); // вывод полученных значении
    Serial.println();

    if (!Rtc.IsDateTimeValid()){ // Если данные не корректные...
        Serial.println("RTC выдает некорректные данные о времени");
        Rtc.SetDateTime(compiled);}
    if (Rtc.GetIsWriteProtected()){ // Если модуль защищен от записи...
        Serial.println("RTC защищено от записи. Получение разрешения.");
        Rtc.SetIsWriteProtected(false);}
    if (!Rtc.GetIsRunning()){ // Если модуль не активирован...
        Serial.println("RTC не активирован. Активация.");
        Rtc.SetIsRunning(true);}

    // Получение данных о времени с модуля
    RtcDateTime now = Rtc.GetDateTime();
    
    if (now < compiled) { // Если время с модуля меньше времени компиляции...
        Serial.println("RTC время менльше времени компиляции (Перезапись)");
        Rtc.SetDateTime(compiled);}
    else if (now > compiled){ // Если время с модуля больше времени компиляци...
        Serial.println("RTC время больше времени компиляции (Ожидаемо)");}
    else if (now == compiled){
        Serial.println("RTC время равно времени компиляции (Не ожидаемо, но норм)");}
}

void loop () 
{
    // Опрос модуля о времени
    RtcDateTime now = Rtc.GetDateTime();
    printDateTime(now); // Вывод на экран
    Serial.println();

    if (!now.IsValid()){ // Проверка корректности...
      Serial.println("RTC выдает некорректные данные");}
      delay(5000);}

#define countof(a) (sizeof(a) / sizeof(a[0]))

// Функция для форматированя получаемых данных от модуля в удобный для вывода формат
void printDateTime(const RtcDateTime& dt){
    char datestring[20];
    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}
