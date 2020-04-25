/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-date-time-ntp-client-server-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include "time.h"
#include <U8g2lib.h>
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ 15, /* data=*/ 4, /* reset=*/ 16);


const char* ssid     = "SSID";
const char* password = "password";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 3600;
char dateStringBuffer[32];
char timeStringBuffer[32];
char secondsBuffer;
int secondsValue;
int dateStringLength;
int x_pos;

void setup(){
  Serial.begin(115200);
  u8g2.begin();
  u8g2.clearBuffer();
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  
  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop(){
  delay(1000);
  printLocalTime();
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
 // strftime(secondsBuffer, 32, "%s", &timeinfo);
  secondsValue=timeinfo.tm_sec;
  if(timeinfo.tm_sec % 10 < 5){
    strftime(dateStringBuffer, 32, "%d %b %Y", &timeinfo);
  }
  else if (timeinfo.tm_sec > 54){
    strftime(dateStringBuffer, 32, "day %j wk %W", &timeinfo);
  }
  else 
  { 
    strftime(dateStringBuffer, 32, " %A", &timeinfo);

  }
  Serial.println(dateStringBuffer);
  dateStringLength = strlen(dateStringBuffer);
  x_pos = 64 - ((dateStringLength / 2) * 9 );
  
  strftime(timeStringBuffer, 32, " %H:%M:%S", &timeinfo);
  Serial.println(timeStringBuffer);

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_9x18_tf);
  u8g2.setCursor(x_pos,20);
  u8g2.print(dateStringBuffer);
  u8g2.setCursor(28,50);
  u8g2.print(timeStringBuffer);
  u8g2.sendBuffer();

}
