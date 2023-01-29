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
#define pinFan 5  //D1
#define pinServo 4   //D2
#define pinPower 0   //D3
#define pinLed 2     //D4 /InternalLED
int fanState = -1;
int lastState = -1;
int flapState = -2;
int flapRequest = -1;
bool serverAcknowledge = false;
unsigned long triggerMillis = 0;  //set everytime the pinFan is LOW (LED on fan turned on)
unsigned long lastMillis = 0;
unsigned long onDuration = 1000;                 //ms => 1 sec, How long power is supplied
unsigned long detectOffTimeout = 500;            //ms => 100ms, detect the fan was turned off

Servo myServo;
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager

//#include <OTA.h>

#define FAN_ON LOW  //pinFan pulled low
#define CLOSE 0
#define OPEN 1

#include "webpage.h"
#include "webserver.h"  //separate file for webserver functions
#include "credentials.h"

void setup() {
  Serial.begin(115200);
  while (!Serial) { ; }

  Serial.println("Booting");

  //setupOTA("FanFlapControl", mySSID, myPASSWORD);
  WiFiManager wm;
  //  wm.resetSettings();
  if (wm.autoConnect("FanFlapControl")) {
    Serial.println("Successfully connected.");
  } else {
    Serial.println("Failed to connect.");
  }
  WiFi.hostname("FanFlapControl");
  // Your setup code
  myServo.attach(pinServo, 600, 2300);  //500, 2400
  pinMode(pinFan, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinFan), fanTurnedOn, CHANGE); //as long as there is the 50Hz toggling the fan is turned on
  pinMode(pinLed, OUTPUT);
  pinMode(pinPower, OUTPUT);  //in order to save power when
  lastMillis = millis();
  startWebServer();
}

ICACHE_RAM_ATTR void fanTurnedOn() {
  fanState = FAN_ON;
  triggerMillis = millis();  //set everytime the pinFan is LOW (LED on fan turned on)
}

void loop() {
//  ArduinoOTA.handle();
  server.handleClient();

  // Your code here
  unsigned long now = millis();
  if ((fanState == FAN_ON) || (flapRequest == OPEN)) {
    flapState = OPEN;  //while there are "turn on" pulses... (LED iss pulsed)
    if(flapRequest == OPEN){triggerMillis = now;}
    if (serverAcknowledge) { serverSendFlapState(); }
  }
  if ((now - triggerMillis + 10) > detectOffTimeout) {  //if there was no more on pulse since a while => fan is off => close the lid, due to the interrupt the trigger millis could be larger then now => leaving the allowed unsigned long range (-1 => very LARGE number)! => + 10 prevents this!
    Serial.println("now (" + String(now) + ") - triggerMillis (" + triggerMillis + ") > detectOffTimeout (" + detectOffTimeout + ")");
    flapState = CLOSE;  //if no more "turn on" pulses...
    if (serverAcknowledge) { serverSendFlapState(); }
  }
  if (flapState != lastState) {
    Serial.println("flapState = " + String(flapState));
    lastState = flapState;
    lastMillis = now;  //power timer
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
}
