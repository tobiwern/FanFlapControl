//https://arduinojson.org/v6/api/jsondocument/
//https://arduinojson.org/v6/doc/deserialization/
//https://arduinojson.org/v6/assistant/#/step1
//https://arduinogetstarted.com/reference/serial-readbytes
//https://makesmart.net/arduino-ide-arbeiten-mit-json-objekten-fur-einsteiger/
//https://arduinojson.org/v6/how-to/reduce-memory-usage/
//https://github.com/arkhipenko/Dictionary

#include <ESP8266WebServer.h>
ESP8266WebServer server(80);

void notFound() {
  Serial.println("Not found");
  server.send(404, "text/plain", "Not found");
}

void handleRoot() {
  String s = webpage;
  server.send(200, "text/html", s);
}

void sendSettingsToWebpage() {  //transferring ESP settings => Webpage
  String value;
  if(flapState == CLOSE){
    value = "closed";
  } else {
    value = "opened";
  }
  Serial.println("Sending settings: " + value);
  server.send(200, "text/plane", value);
}

void setFlap() {
  String text = server.arg("value");
  Serial.println("Receiving setting : " + text);
  if (text == "close") {
    flapState = CLOSE;
    text = "closed";
  } else {
    flapState = OPEN;
    text = "opened";
  }
  server.send(200, "text/plane", text);
}


void startWebServer() {
  Serial.println("Starting WebServer...");
  server.on("/", handleRoot);
  server.on("/set_flap", setFlap);
  server.on("/request_settings", sendSettingsToWebpage);
  server.onNotFound(notFound);
  server.begin();
  Serial.print("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print(", hostName: http://");
  Serial.println(WiFi.getHostname());
}

void stopWebServer() {
  Serial.println("Stopping WebServer...");
  server.stop();
}
