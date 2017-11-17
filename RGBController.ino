/*
 * ==== RGB Controller ====
 * Project description here
 * 
 * ------------ TODO --------------
 * Check inativity to get back state zero
 * ADD potentiometer and do the map thing
 * add to github
 */
 

/* ==== Defines ==== */

#define SERIAL_BAUD 115200
#define LED_RED   2
#define LED_GREEN 3
#define LED_BLUE  4
#define BUTTON    5
#define MENU_MAX  3
#define ANALOG    A1
#define TIME_OUT  10*1000   //x * 1000 = convertion from miliseconds to seconds

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
int redIntesity = 1023, greenIntensity = 1023, blueIntesity = 1023; //colors itensity begns with "white"
/* ==== END Global Variables ==== */



void setup() {
  Serial.begin(SERIAL_BAUD);
  buttonToLow();
  ledToLow(); 
  ledTest(); 
}

void loop() {
  checkButtonPress();
  menu();
}
/* ==== Functions ==== */
void menu(){
  switch(menuState){
    case 0:
      //general
      break;
    case 1:
      digitalWrite(LED_RED, HIGH);//red
      break;
    case 2:
      digitalWrite(LED_GREEN, HIGH);//green
      break;
    case 3:
      digitalWrite(LED_BLUE, HIGH);//blue
      break;
  }
}

void checkButtonPress(){
  if(digitalRead(BUTTON) == HIGH){
    menuState ++;
    delay(250);
    ledToLow();
  }
  if(menuState > MENU_MAX) menuState = 0;
    
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

