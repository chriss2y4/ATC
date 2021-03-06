//Author: Mohit Bhole, Github: moiiiiit
//Project: Sensory Cuffs with Autism Treatment Center, Dallas
//Engineering Project in COmmunity Service

#include <WiFi.h>          // Replace with WiFi.h for ESP32
#include <WebServer.h>     // Replace with WebServer.h for ESP32
#include <AutoConnect.h>
#include <HTTPClient.h>
#include "AsyncTCP.h"
#include "ESPAsyncWebServer.h"
#include <U8x8lib.h>

// the OLED used
U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);

int timer=0;
WebServer Server;          // Replace with WebServer for ESP32
AutoConnect      Portal(Server);
AsyncWebServer mainServer(81);
AutoConnectConfig  Config;

const int motorPin = 12; ////Pin where the motor air pump is connected to
const int valvePin = 23; //Pin where the solenoid valve is connected to

void rootPage() {
  char content[] = "Austism Treatment Center. You have set up the device!";
  Server.send(200, "text/plain", content);
}

void setup() {

  pinMode(motorPin, OUTPUT);
  pinMode(valvePin, OUTPUT);
  
  delay(1000);
  Serial.begin(9600);
  Serial.println();
  u8x8.begin();
  u8x8.setFont(u8x8_font_torussansbold8_r);
  u8x8.drawString(6,1, "ATC");
  u8x8.drawString(2, 3, "Setting up");
  u8x8.drawString(2, 5, "WiFi Server");
  Server.on("/", rootPage);
//  Config.staip = IPAddress(192,168,1,184);
//  Config.staGateway = IPAddress(192,168,1,1);
//  Config.staNetmask = IPAddress(255,255,255,0);
//  Config.dns1 = IPAddress(192,168,1,1);
  Portal.config(Config);
  if (Portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
    u8x8.clearDisplay();
    u8x8.drawString(3, 3, "WiFi connected");
    mainServer.on("/turnon", HTTP_GET, [](AsyncWebServerRequest *request){
      String inputMessage;
      String inputParam;
      if (request->hasParam("time")) {
        inputMessage = request->getParam("time")->value();
        inputParam = "time";
      }
      Serial.println(inputMessage);
      Serial.println(inputParam);
      timer = inputMessage.toInt();
      if(timer==1){
        u8x8.clearDisplay();
        u8x8.drawString(6,1, "ATC");
        u8x8.drawString(1, 3, "Cuff is READY");
        u8x8.drawString(7, 5, ":)");
      }
      request->send(200, "text/plain", "");
    });
    mainServer.begin();
    u8x8.clearDisplay();
    u8x8.drawString(6,1, "ATC");
    u8x8.drawString(1, 3, "Cuff is READY");
    u8x8.drawString(7, 5, ":)");
  }
}

void loop() {
    Portal.handleClient();

    int inflateTime = 12000;
    int deflateTime = 10000;
    
    if(timer==100){
      u8x8.clearDisplay();
      u8x8.drawString(6,1, "ATC");
      u8x8.drawString(1, 3, "Inflating Cuff");

      digitalWrite(motorPin, HIGH);
      delay(inflateTime);
      digitalWrite(motorPin, LOW);
      timer--;
    }
      if(timer==0){
        u8x8.clearDisplay();
        u8x8.drawString(6,1, "ATC");
        u8x8.drawString(1, 3, "Deflating Cuff");

        digitalWrite(valvePin, HIGH);
        delay(deflateTime);
        digitalWrite(valvePin, LOW);
        u8x8.clearDisplay();
        timer = 1;
      }
      if(timer==1){
        u8x8.drawString(6,1, "ATC");
        u8x8.drawString(1, 3, "Cuff is READY");
        u8x8.drawString(7, 5, ":)");
        
      }
}
