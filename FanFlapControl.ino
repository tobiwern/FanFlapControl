/* Luefterklappensteuerung
  Um Energie zu sparen ist es sinnvoll eine zusätzliche Ventilklappe im Lüftermauerkasten zu haben.
  Allerdings ist der Lüfter nicht stark genug die zusätzliche Klappe zu öffnen.
  Daher wird detektiert, wann der Lüfter anläuft (grüne LED) und der Servo öffnet die Ventilklappen.
  Sobald der Servo eine Bewegung durchgeführt hat, wird der Strom zum Sevo nach 2 Sekunden unterbrochen um Strom zu sparen und um unnötige Geräusche zu vermeiden.  
  ToDo: 
  - Send to sleep.
  - Webserver mit open/close
  - Send on/off times to database
  - 
*/

#include <Servo.h>
#define pinFanLed 5  //D1
#define pinServo 4   //D2
#define pinPower 0   //D3
#define pinLed 2     //D4 /InternalLED
int lastState = -1;
int flapState = -2;
unsigned long triggerMillis = 0;  //set everytime the pinFanLed is LOW (LED on fan turned on)
unsigned long lastMillis = 0;
unsigned long onDuration = 1000;                 //ms => 1 sec
unsigned long detectOffTimeout = 2000;            //ms
unsigned long trailingDuration = 5 * 60 * 1000;  //ms => 5 min

Servo myServo;

#include <OTA.h>
#include <credentials.h>

#define LED_ON LOW  //pinFanLed pulled low
#define CLOSE 0
#define OPEN 1

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

//  setupOTA("FanFlapControl", mySSID, myPASSWORD);

  // Your setup code
  myServo.attach(pinServo);
  Serial.begin(115200);
  pinMode(pinFanLed, INPUT_PULLUP);
  pinMode(pinLed, OUTPUT);
  pinMode(pinPower, OUTPUT);  //in order to save power when
  lastMillis = millis();
}

void loop() {
//  ArduinoOTA.handle();
  // Your code here
  int fanLedState = digitalRead(pinFanLed);
  unsigned long now = millis();
  if (fanLedState == LED_ON) {
    triggerMillis = now;
    flapState = OPEN; //while there are "turn on" pulses... (LED iss pulsed)
  }
  if ((now - triggerMillis) > detectOffTimeout) {  //if there was no more on pulse since a while => fan is off => close the lid
    //    Serial.println("now (" + String(now) + ") - triggerMillis (" + triggerMillis + " > detectOffTimeout (" + detectOffTimeout + ")");
    flapState = CLOSE; //if no more "turn on" pulses...
  }
  if(flapState==CLOSE){Serial.println("CLOSE");} else {Serial.println("OPEN");}
  if (flapState != lastState) {
    Serial.println("flapState = " + String(flapState));
    lastState = flapState;
    lastMillis = now;  //reset timer
    Serial.println("Turning power on");
    digitalWrite(pinPower, HIGH);  //turn power on
    digitalWrite(pinLed, LOW);     //lowActive LED
  }
  if ((now - lastMillis) < onDuration) {
    if (flapState == CLOSE) {
      myServo.write(0);  //close flap
    } else {
      myServo.write(180);  //open flap
    }
  } else {
    if (digitalRead(pinPower) != LOW) { Serial.println("Turning power off"); }
    digitalWrite(pinPower, LOW);  //turn power off
    digitalWrite(pinLed, HIGH);   //lowActive LED
  }
    delay(500);
}
