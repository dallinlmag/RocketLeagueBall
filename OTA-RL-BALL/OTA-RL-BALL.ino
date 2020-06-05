#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <RgbLed.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "WIFI-NAME"
#define STAPSK  "WIFI-PASSWORD"
#endif

//Function Declarations because I am too lazy for a header and I want to add them at the end of this file.
void handleLEDon();
void handleALLoff();
void handleGoal();
void handleWin();
void handleFade();
void wait();


const char* ssid = STASSID;
const char* password = STAPSK;
ESP8266WebServer server(80); //Server on port 80
bool OTA_Enabled = true;

const int NUM_LEDS = 3;
RgbLed leds[3] {RgbLed(1,3,5,COMMON_TYPE::ANODE), RgbLed(4,0,2,COMMON_TYPE::ANODE), RgbLed(13,12,14,COMMON_TYPE::ANODE) };

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("RLBall");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Start web server now that we are booted and connected to WIFI
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/allon", handleLEDon); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/alloff", handleALLoff);
  server.on("/goal", handleGoal);
  server.on("/win", handleWin);
  server.on("/fade", handleFade);
  server.on("/ota", [](){OTA_Enabled = !OTA_Enabled;});

  server.begin();                  //Start server
  
}

void loop() {
  server.handleClient();          //Handle client requests
  if (OTA_Enabled)
  {
    ArduinoOTA.handle();
    fadeWait(1000);
  }
  else
  {
    //fadeWait(100);
  }
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 Serial.println("You called root page");
 String s = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>ROCKET LEAGUE LED BALL</h1><br>
Click to turn <a href="allon" target="myIframe">LED ON</a><br>
Click to turn <a href="alloff" target="myIframe">LED OFF</a><br>
Click to score a goal <a href="goal" target="myIframe">GOAL</a><br>
Click to win <a href="win" target="myIframe">WIN</a><br>
Click to fade <a href="fade" target="myIframe">FADE</a><br>
Click to enable/disable OTA <a href="ota" target="myIframe">OTA</a><br>
LED State:<iframe name="myIframe" width="100" height="25" frameBorder="0"><br>
<hr>
<hr>
</center>

</body>
</html>
)====="; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleLEDon() { 
 Serial.println("LED on page");
 setLEDs(leds, colors[RED], NUM_LEDS);
 delay(1000);
 setLEDs(leds, colors[GREEN], NUM_LEDS);
 delay(1000);
 setLEDs(leds, colors[BLUE], NUM_LEDS);
 delay(1000);
 server.send(200, "text/html", "ON"); //Send ADC value only to client ajax request
}

void handleALLoff() { 
 Serial.println("LED off page");
 setLEDs(leds, {0,0,0}, NUM_LEDS);
 server.send(200, "text/html", "OFF"); //Send ADC value only to client ajax request
}

void handleWin(){
  server.send(200, "text/html", "WIN"); 
  for (int i = 0; i < 20; i++){
    setLEDs(leds, colors[GREEN], NUM_LEDS);
    delay(100);
    setLEDs(leds, colors[WHITE], NUM_LEDS);
    delay(100);
  }
}

void handleGoal(){
  server.send(200, "text/html", "GOAAAALLLLL"); 
  for (int i = 0; i < 50; i++){
    leds[0].setColor(colors[random(0,7)]);
    leds[1].setColor(colors[random(0,7)]);
    leds[2].setColor(colors[random(0,7)]);
    delay(100);
  }
}

void handleFade()
{
  setLEDs(leds, colors[RED], NUM_LEDS);
  for (auto i = 0; i < 6; i++)
  {
    delay(1000);
    fadeLEDs(leds, colors[i], colors[i+1], 2000, NUM_LEDS);
  }
}

int currentColor = 0;
void wait(){
  setLEDs(leds, colors[currentColor], NUM_LEDS);
  currentColor = (currentColor+1)%7;
}

void fadeWait(int duration)
{
   fadeLEDs(leds, colors[currentColor], colors[(currentColor+1)%7], duration, NUM_LEDS, MEDIUM_B);
   currentColor = (currentColor+1)%7;
}
