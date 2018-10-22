// А0-якрость красного А1-Яркость зеленого А2-Яркость синего А3-Яркость беголо
// А4-вкл RGB А5-вкл белый А7-Пуск мотора А6-Реверс мотора 7 и 8-энкодер пины
//
//  Настройки драйвера двигателя:
// 2-Enable
// 3-Step
// 5-Dir
//
// Настройки светодиодов
// Шим порты!
// 6-Белый выход
// 9-Красный выход
// 10-Зеленый выход
// 11-Синий выход
//

//todo Подключаем библиотеки
#include <Arduino.h>
#include <StepDirDriver.h>
#include <TimerOne.h>
#include <Encod_er.h>

//todo Объявляем глобальные переменные
//* Для RGB ленты
int Red = 0;              //Яркость красного
int Green = 0;            //Яркость зеленого
int Blue = 0;             //Яркость Синего
bool rgb_btn = false;     //Нажатие кнопки
bool rgb_btn_old = false; //Предыдущее состояние кнопки
bool rgb_stat = false;

//* Для белой ленты
int white = 0;              //Яркость белого
bool white_btn = false;     //Нажатие кнопки
bool white_btn_old = false; //Предыдущее состояние кнопки
bool white_stat = false;

//* Для мотора
StepDirDriver motor(3, 5, 2); // Step, Dir, Enable, pin
int speed = 50;              //Скорость вращениея
bool drive_stat = false;
bool drive_run = false;     //Пуск стоп мотора
bool drive_run_old = false; //Предыдущее состояние кнопки
bool revers = false;
bool drive_revers = false;     //Направление вращения
bool drive_revers_old = false; //Предыдущее состояние кнопки

Encod_er enc(7, 8, 4);

bool enc1 = true;
bool enc2 = true;

void setup()
{
    Serial.begin(9600);
    // TCCR2A = TCCR2A & 0xe0 | 1;
    // TCCR2B = TCCR2B & 0xe0 | 0x01;

    // TCCR1A = TCCR1A & 0xe0 | 1;
    // TCCR1B = TCCR1B & 0xe0 | 0x01;

    // TCCR0A = TCCR0A & 0xe0 | 1;
    // TCCR0B = TCCR0B & 0xe0 | 0x01;

    void timerinterrupt();
    Timer1.initialize(250);
    Timer1.attachInterrupt(timerinterrupt, 250);

    motor.setMode(0, false);

    pinMode(7, INPUT);
    pinMode(8, INPUT);

    tone(13, 1000, 200);
    delay(220);
    tone(13, 1200, 150);
    delay(170);
    tone(13, 1400, 100);
    delay(120);
    tone(13, 1600, 50);
    
    
}

void loop()
{
    // Обработка мотора------------------------------------------------------
    // Даем комманду крутиться и куда
    if (drive_stat == true)
    {
        if (revers == true) //Направление вращения
        {
            motor.step(-1000);
        }
        else
        {
            motor.step(1000);
        }
    }
    else //Стоп мотор
    {
        motor.step(0);
    }

    // Обработка LED лент------------------------------------------------------------
    // Вкл\Выкл RGB лента
    if (rgb_stat == true)
    {
        analogWrite(9, map(analogRead(A0), 0, 1024, 0, 255));
        analogWrite(10, map(analogRead(A1), 0, 1024, 0, 255));
        analogWrite(11, map(analogRead(A2), 0, 1024, 0, 160));
    }
    else
    {
        analogWrite(9, 0);
        analogWrite(10, 0);
        analogWrite(11, 0);
    }

    // Вкл\Выкл белой ленты
    if (white_stat == true)
    {
        analogWrite(6, map(analogRead(A3), 0, 1024, 0, 255));
    }
    else
    {
        analogWrite(6, 0);
    }
    //-----------------------------------------------------------------------------------------
    //Проверяем нажание кнопок
    // Управление RGB лентой
    if (analogRead(A4) > 600)
    {
        if (rgb_btn_old == false)
        {
            rgb_stat = !rgb_stat;
            rgb_btn_old = true;
            tone(13, 1500, 50);
            delay(50);
        }
    }
    else
    {
        rgb_btn_old = false;
    }

    // Управление белой лентой
    if (analogRead(A5) > 600)
    {
        if (white_btn_old == false)
        {
            white_stat = !white_stat;
            white_btn_old = true;
            tone(13, 1500, 50);
        }
    }
    else
    {
        white_btn_old = false;
    }

    // Управление старт стоп мотор
    if (analogRead(A7) > 600)
    {
        if (drive_run_old == false)
        {
            drive_stat = !drive_stat;
            drive_run_old = true;
            tone(13, 1500, 50);
        }
    }
    else
    {
        drive_run_old = false;
    }

    // Управление направлением вращения мотора
    if (analogRead(A6) > 600)
    {
        if (drive_revers_old == false)
        {
            revers = !revers;
            drive_revers_old = true;
            tone(13, 1500, 50);
        }
    }
    else
    {
        drive_revers_old = false;
    }


// Обрабатываем энкодер-----------------------------------------------------------
    enc.scanState();

    if (enc.timeRight != 0)
    {
        if (speed > 8)
        {
            speed = speed - 2;
            
        }
        else
        {
            tone(13, 500, 50);
            delay(70);
            tone(13, 500, 50);
        }
        enc.timeRight = 0;
    }

    if (enc.timeLeft != 0)
    {
        if (speed < 70)
        {
            speed = speed + 2;
           
        }
        else
        {
            tone(13, 500, 50);
            delay(70);
            tone(13, 500, 50);
        }
        enc.timeLeft = 0;
    }
}
void timerinterrupt()
{ //todo Вызов функций по таймеру каждые 250милиСек.

    //Обрабатываем мотор
    motor.setDivider(speed);
    motor.control();

    enc.scanState();
}
