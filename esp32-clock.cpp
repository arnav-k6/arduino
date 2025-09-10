int latch = 3;
int clockPin = 2;
int data = 8;
int comPins[4] = {4, 5, 6, 7}; // pins for setting the digit "HIGH" (off) 
int timer = 60;
unsigned char table[] = {//binary digits which are inputted to the 74HC shift register to input the 1/0 to each 7-seg digit 
  B00111111, //0
  B00000110, //1
  B01011011, //2
  B01001111, //3
  B01100110, //4
  B01101101, //5
  B01111101, //6
  B00000111, //7
  B01111111, //8
  B01101111, //9
  B00000000 //blank digit
};
//setup NTP protocol for time synchronization // copied off internet source 
#include <WiFi.h>
#include "time.h"

const char* ssid     = "MBBBKNSS_U3";
const char* password = "ShreeJeeBabaShreeNathJee@215";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = -28800; // vancouver time is GMT-7, so this is the offset in seconds
const int   daylightOffset_sec = 3600;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void setup() {
  pinMode(latch, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(data, OUTPUT);
  for(int i=0; i<4; i++){
    pinMode(comPins[i], OUTPUT);
    digitalWrite(comPins[i], HIGH); // all digits are turned off
  }

  //initialize serial monitor to display time to ensure the correct data is parsed into the display
  Serial.begin(115200);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);//prints name of network which is connected to ESP32
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");//prints while initiating connection
  }
  Serial.println(" CONNECTED");
  
  //initialize and gets the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

// display a single digit
void DisplayDigit(int pos, int num){
  for(int i=0; i<4; i++) digitalWrite(comPins[i], HIGH); // all off
  digitalWrite(latch, LOW);
  shiftOut(data, clockPin, MSBFIRST, table[num]);
  digitalWrite(latch, HIGH);
  digitalWrite(comPins[pos], LOW); // turn on the specific digit, here "LOW" is "on"
}

// displays the 4-digit number
void DisplayNumber(int num){
  int digits[4] = { (num/1000)%10, (num/100)%10, (num/10)%10, num%10 }; // division removes digits to the right of it's position using int division, then finds the remainder when / by 10 to get the corresponding digit to display
  for(int i=0;i<4;i++){
    DisplayDigit(i, digits[i]);//displays the digits left to right sequentially with a 7 ms delay
    delay(7); 
  }
}

void loop(){//copy pasted from online source about ESP32 NTP protocol
  printLocalTime();
  struct tm timeinfo;
  if(getLocalTime(&timeinfo)){
      int hh = timeinfo.tm_hour;  // HH in 24-hour format
      int mm = timeinfo.tm_min;   // MM in minutes

      int timeNumber = hh * 100 + mm; // e.g., 14:35 -> 1435
      DisplayNumber(timeNumber);       // send integer to 7-segment function
  }
}
