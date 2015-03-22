#include "Arduino.h"
#include "Math.h"
#include <kasadas-core.h>
#ifndef Arduino_h
#define Arduino_h
#endif
#define RED_LED_NUM 4
#define BLUE_LED_NUM 2
#define YELLOW_LED_NUM 2
#define LED_PIN_NUM 8

const int ledpin[LED_PIN_NUM]= {46,47,48,49,50,51,52,53};
const int blueledpin[BLUE_LED_NUM]={ledpin[0], ledpin[4]};
const int redledpin[RED_LED_NUM]={ledpin[1],ledpin[3],ledpin[5],ledpin[7]};
const int touchpin = 3;
const int lightpin = 4;
const int buzzerpin = 7;
const int fingerpin = 8;

bool led_pinstate[LED_PIN_NUM] = {true, false,true,false,true,false,true,false};

void BuzzerOn(int pin){
  digitalWrite(pin,HIGH);
}

void BuzzerOff(int pin){
  digitalWrite(pin,LOW);
}

void ToggleLED(int pin){
  if(led_pinstate[pin] == true)
    led_pinstate[pin] = false;
  else led_pinstate[pin] = true;
  digitalWrite(pin, led_pinstate[pin]);
}

bool isTouched(int pin){
  if(digitalRead(pin))
    return true;
  else return false;
}

bool isLight(int pin){
    if(digitalRead(pin))
      return true;
    else return false;
}

void setLight(int pin){
  bool state[LED_PIN_NUM];
  for(int i =0; i < LED_PIN_NUM; i++){
    state[i] = false;
  }
  state[pin] = true;
  for(int i=0; i < LED_PIN_NUM; i++){
    digitalWrite(ledpin[i],state[i]);
  }
}

void fadeLight(){
   bool state[LED_PIN_NUM];
   for(int i=0; i < LED_PIN_NUM; i++){
        state[i] = true;
   }
   for(int j =0; j < LED_PIN_NUM;j++){
      digitalWrite(ledpin[j],state[j]);
   }
   if(isTouched(touchpin)) return;
   delay(200);
   for(int i =0; i < LED_PIN_NUM;i++){
      state[i] = false;
      for(int j =0; j < LED_PIN_NUM;j++){
        digitalWrite(ledpin[j],state[j]);
      }
      if(isTouched(touchpin)) break;
      delay(200);
   }
}

void spinLight(){
   bool state[LED_PIN_NUM];
  for(int i =0; i < LED_PIN_NUM; i++){
    for(int j =0; j < LED_PIN_NUM; j++){
      state[j] = false;
    }
    state[i] = true;
    for(int j = 0; j < LED_PIN_NUM;j++){
    digitalWrite(ledpin[j],state[j]);
    }
    if(isTouched(touchpin)) break;
    delay(200);
  }
}
double i = 0;
void Emergency(){
  i += 0.07;
  bool state[LED_PIN_NUM];
  for(int i = 0; i < LED_PIN_NUM; i++)
    state[i] = false;
  state[1] = true;
  state[3] = true;
  state[7] = true;
  for(int i = 0; i < LED_PIN_NUM; i++){
    digitalWrite(ledpin[i], state[i]);
  }
  BuzzerOn(buzzerpin);
  delay(exp(6.214 - i) + 20);
  state[1] = false;
  state[3] = false;
  state[7] = false;
   for(int i = 0; i < LED_PIN_NUM; i++){
    digitalWrite(ledpin[i], state[i]);
  }
  BuzzerOff(buzzerpin);
  delay(exp(6.214 - i) + 20);
}



bool isFinger(int pin){
   if(digitalRead(pin)) return true;
   else return false;
}
const int baud = 115200;
void setup(){
  pinMode(ledpin[0],OUTPUT);
  pinMode(ledpin[1],OUTPUT);
  pinMode(ledpin[2],OUTPUT);
  pinMode(ledpin[3],OUTPUT);
  pinMode(ledpin[4],OUTPUT);
  pinMode(ledpin[5],OUTPUT);
  pinMode(ledpin[6],OUTPUT);
  pinMode(ledpin[7],OUTPUT);
  pinMode(touchpin,INPUT);
  pinMode(lightpin,INPUT);
  pinMode(buzzerpin,OUTPUT);
  pinMode(fingerpin,INPUT);
  Serial.begin(115200);
}

int point = 0;
void Point(){
  point++;
  //BlueLightOn();
  BuzzerOn(buzzerpin);
  delay(100);
  BuzzerOff(buzzerpin);
  //BlueLightOff();
  Serial.print("point;");
  Serial.print(point);
  Serial.print("\n");
  Serial.flush();
  delay(100);
}

void loop(){
  if(isFinger(fingerpin)){
    Point(); 
  }
  if(isLight(lightpin)){
    if(isTouched(touchpin)){
      Emergency();
    }
    else{
      fadeLight();
    }
  }
  else{
    spinLight();//NightMode();
  }
}


