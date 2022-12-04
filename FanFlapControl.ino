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
#define pinFan    5 //D1
#define pinServo  4 //D2
#define pinPower 14 //D5
int lastState = -1;
unsigned long lastMillis;
unsigned long onDuration = 1000; //ms => 2 sec  

Servo myServo; 

#include <OTA.h>
#include <credentials.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");

  setupOTA("FanFlapControl", mySSID, myPASSWORD);

  // Your setup code
  myServo.attach(pinServo); 
  Serial.begin(115200);
  pinMode(pinFan, INPUT_PULLUP);
  pinMode(pinPower, OUTPUT); //in order to save power when 
  lastMillis = millis();  
}

void loop() {
  ArduinoOTA.handle();
  // Your code here
  int nowState = digitalRead(pinFan);
  if(nowState != lastState){
    lastState = nowState;
    lastMillis = millis(); //reset timer
    Serial.println("Turning power on");
    digitalWrite(pinPower, HIGH); //turn power on
  }
  if(millis()-lastMillis < onDuration){
    if(digitalRead(pinFan) == HIGH){
      myServo.write(180);
    } else {  
      myServo.write(0);  
    }
  } else {
    Serial.println("Turning power off"); 
    digitalWrite(pinPower, LOW); //turn power off 
  }
//  delay(100);

}
