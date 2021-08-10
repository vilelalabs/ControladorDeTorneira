/*
  Controlador de Torneira 
  Autor: Henrique Vilela
  Data: 10/08/2021
  Versão: 1.0

*/
#include <Arduino.h>
#include <Servo.h>
#include <avr/sleep.h>

//definições de parâmetros

#define BTN_UP 4
#define BTN_CENTER 5
#define BTN_DOWN 6

#define SERVO 12

#define POS_MAX 180
#define POS_CENTER 90
#define POS_MIN 0

#define INCREMENT 2
#define VELOCIDADE 30

#define WAKE_PIN 2

Servo myservo; // cria objeto para controlar o servo
int pos = 90;  // variável pra armazenar posição do servo já com a posição inicial central

// protótipos das funções
void centraliza();
void sobe();
void desce();
void sleepMode();

void setup()
{
  myservo.attach(SERVO); // attaches the servo on pin 9 to the servo object

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_CENTER, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);

  pinMode(WAKE_PIN, INPUT_PULLUP);

  // set the servo to a neutral position
  centraliza();
}

void loop()
{
  sleepMode();

  while (!digitalRead(BTN_UP) || !digitalRead(BTN_CENTER) || !digitalRead(BTN_DOWN))
  {
    // botao central
    if (digitalRead(BTN_CENTER) == LOW && pos != POS_CENTER)
      centraliza();
    //botão cima
    else if (digitalRead(BTN_UP) == LOW && pos <= POS_MAX)
      sobe();
    //botão baixo
    else if (digitalRead(BTN_DOWN) == LOW && pos >= POS_MIN)
      desce();
  }
}

// funções ----------------------------------------------
void centraliza()
{
  while (pos != POS_CENTER)
  {

    if (pos > POS_CENTER)
    {
      pos -= INCREMENT;
    }
    else if (pos < POS_CENTER)
    {
      pos += INCREMENT;
    }
    delay(VELOCIDADE);
    myservo.write(pos);
  }
}
//-------------------------------------------------------

void sobe()
{
  pos += INCREMENT;
  delay(VELOCIDADE);
  myservo.write(pos);
}

//-------------------------------------------------------
void desce()
{
  pos -= INCREMENT;
  delay(VELOCIDADE);
  myservo.write(pos);
}

//-------------------------------------------------------
void sleepMode()
{
  attachInterrupt(
      digitalPinToInterrupt(WAKE_PIN),
      []()
      {
        //função lâmbda para acordar o Arduino
        sleep_disable();
        detachInterrupt(digitalPinToInterrupt(WAKE_PIN));
      },
      LOW);

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  delay(1000);
  sleep_cpu();
}