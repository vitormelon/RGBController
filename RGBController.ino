/*
 * ==== RGB Controller ====
 * Project description here
 * 
 * ------------ TODO --------------
 * Check inativity to get back state zero
 * ADD potentiometer and do the map thing
 * add to github
 */

#include <EEPROM.h>

/* ==== Defines ==== */

#define LED_RED         2 //Red LED info port
#define LED_GREEN       4 //Green LED info port
#define LED_BLUE        7 //Blue LED info port
#define BUTTON          8 //Push-button port
#define POTENTIOMETER   A0//Potentiometer port
#define OUT_RED         3 //Red LED info port
#define OUT_GREEN       5 //Green LED info port
#define OUT_BLUE        6 //Blue LED info port

#define SERIAL_BAUD     115200
#define MENU_MAX        3 //MAX menu states ( + the 0 zero)
#define TIME_OUT        10*1000   //x * 1000 = convertion from miliseconds to seconds
#define MAX_VARIATION   5 //max variation on the potentiometer read to start editing.
#define RED             1 //array position of each color
#define GREEN           2
#define BLUE            3
#define GENERAL         0

/* ==== END Defines ==== */

/* ==== Global Variables ==== */
/*--- Menu--- 
 * 0 = All LEDs off, potentiometer controls general bight
 * 1 = Red LED on, potentiometer controls red light
 * 2 = Green LED on, potentiometer controls green light
 * 3 = Blue LED on, potentiometer controls blue light
 * if nothing happens in TIME_OUT seconds, the menu returns
 * to the 0 (zero) state
 */
int menuState = 0;
long timeout = 0;
int ledValue[4] = {255, 255, 255, 255}; //colors itensity begns with max, generating "white" color
int potentiometerValue = 0;
boolean isEditing = false;
/* ==== END Global Variables ==== */



void setup() {
  Serial.begin(SERIAL_BAUD);
  buttonToLow();
  ledToLow(); 
  ledTest(); 
  readFromEprom();
}

void loop() {
  checkButtonPress();
  checkPotentiometerChange();
  menu();
  setOutLedValue();
}
/* ==== Functions ==== */
void menu(){
  switch(menuState){
  case 0:
    menuAction(0, GENERAL);
    break;
  case RED:
    menuAction(LED_RED, RED);//red
    break;
  case GREEN:
    menuAction(LED_GREEN, GREEN);//green
    break;
  case BLUE:
    menuAction(LED_BLUE, BLUE);//blue
    break;
  }
}

void menuAction(int monitorPort, int color){
  printStatusOnSerial();
  digitalWrite(monitorPort, HIGH);
  if(isEditing){
    ledValue[color] = readPotentiometer();
  }
}

void printStatusOnSerial(){
  Serial.print("Menu: ");
  Serial.print(menuState);
  
  Serial.print(" IsEditing: ");
  Serial.print(isEditing);
  
  Serial.print(" Potentiometer: ");
  Serial.print(readPotentiometer());
  
  Serial.print(" General: ");
  Serial.print(map(ledValue[GENERAL], 0, 255, 0, 100)/100.0);
  
  Serial.print(" Red: ");
  Serial.print(ledValue[RED]);
  
  Serial.print(" Green: ");
  Serial.print(ledValue[GREEN]);
  
  Serial.print(" Blue: ");
  Serial.println(ledValue[BLUE]);
  
}

void checkButtonPress(){
  int menuStateBefore = menuState;
  if(digitalRead(BUTTON) == HIGH){
    writeOnEprom();
    menuState ++;
    delay(250);
    ledToLow();
  }
  if(menuState > MENU_MAX) menuState = 0;
  if(menuState != menuStateBefore) onMenuStateChange();
}

void checkPotentiometerChange(){
  if(!isEditing){
    int valueNow = readPotentiometer();
    if(getPotentiometerVariation(valueNow) > MAX_VARIATION){
      isEditing = true;
    }
  }
}

void setOutLedValue(){
  float general  = map(ledValue[GENERAL], 0, 255, 0, 100)/100.0;
  analogWrite(OUT_RED,ledValue[RED] * general);
  analogWrite(OUT_GREEN,ledValue[GREEN] * general);
  analogWrite(OUT_BLUE,ledValue[BLUE] * general);
}

int getPotentiometerVariation(int valueNow){
  return abs(valueNow-potentiometerValue);
}

void onMenuStateChange(){
  isEditing = false;
  potentiometerValue = readPotentiometer();
}

void readFromEprom(){
  ledValue[RED] = EEPROM.read(RED);
  ledValue[GREEN] = EEPROM.read(GREEN);
  ledValue[BLUE] = EEPROM.read(BLUE);
  ledValue[GENERAL] = EEPROM.read(GENERAL);  
}

void writeOnEprom(){
  EEPROM.write(RED, ledValue[RED]);
  EEPROM.write(GREEN, ledValue[GREEN]);
  EEPROM.write(BLUE, ledValue[BLUE]);
  EEPROM.write(GENERAL, ledValue[GENERAL]);
}

int readPotentiometer(){
  int analogicValue = analogRead(POTENTIOMETER);
  return map(analogicValue, 0, 1023, 0, 255);  
}

void ledToLow(){  
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
}

void buttonToLow(){
  digitalWrite(BUTTON, LOW);
}

void ledTest(){
  blinkLed(LED_RED, 500);
  blinkLed(LED_GREEN, 500);
  blinkLed(LED_BLUE, 500);
}

void blinkLed(int led, int time){
  digitalWrite(led, HIGH);
  delay(time);
  digitalWrite(led, LOW);
}


