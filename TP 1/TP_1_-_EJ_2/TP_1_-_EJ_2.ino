#include <ESP32Time.h>
#include "time.h"
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64


#define BUTTON_PIN1 34
#define BUTTON_PIN2 35
int readButton1;
int readButton2;
int buttonState1;
int buttonState2;
int buttonState3;
int buttonState4;
int screenState;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void askTime(void);
void setup_rtc_ntp(void);


struct tm timeinfo;
ESP32Time rtc;


long unsigned int timestamp;
const char *ntpServer = "south-america.pool.ntp.org";
long gmtOffset_sec = -10800;
const int daylightOffset_sec = 0;


const char* ssid = "ORT-IoT";
const char* password = "OrtIOTnew22$2";

int sM;


int hrs;
int mins;
int GMT = -3;

void setup() {

  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);

  Serial.begin(115200);
  Serial.println("Connecting to Wi-Fi...");
  initWiFi();
  setup_rtc_ntp();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

}

void loop() {
  readButton1 = digitalRead(BUTTON_PIN1);
  readButton2 = digitalRead(BUTTON_PIN2);

  if  (gmtOffset_sec > 43200) {
    gmtOffset_sec = 43200;
  }

  if  (gmtOffset_sec < -43200) {
    gmtOffset_sec = -43200;
  }

  if  (GMT < -12) {
    GMT = -12;
  }

  if  (GMT > 12) {
    GMT = 12;
  }

  switch (sM) {
    case 0:
      askTime();
      delay(500);
      display.clearDisplay();

      if (readButton1 == LOW && readButton2 == LOW ) {
        buttonState4 = HIGH;
      }

      if (readButton1 == HIGH && readButton2 == HIGH && buttonState4 == HIGH) {
        buttonState4 = LOW;
        sM = 1;
      }

      break;

    case 1:
      screenState = 1;

      display.clearDisplay();

      display.setCursor(0, 25);
      display.println("GMT:");
      display.print (GMT);
      display.display();


      if (readButton1 == LOW && readButton2 == LOW ) {
        buttonState4 = HIGH;
      }

      if (readButton1 == HIGH && readButton2 == HIGH && buttonState4 == HIGH) {
        buttonState4 = LOW;
        sM = 0;
        display.clearDisplay();
        break;

      }

      if (readButton1 == LOW && readButton2 == HIGH && buttonState4 == LOW) {
        buttonState2
        = HIGH;
      }

      if (readButton1 == HIGH && buttonState2
      == HIGH && buttonState4 == LOW) {
        gmtOffset_sec -= 3600;
        GMT = GMT - 1;
        setup_rtc_ntp();
        buttonState2
        = LOW;
      }

      if (readButton2 == LOW && readButton1 == HIGH && buttonState4 == LOW) {
        buttonState3 = HIGH;
      }

      if (readButton2 == HIGH && buttonState3 == HIGH && buttonState4 == LOW) {
        gmtOffset_sec += 3600;
        GMT = GMT + 1;
        setup_rtc_ntp();
        buttonState3 = LOW;
      }

      break;


  }
}


// Initialize WiFi
void initWiFi() {
  WiFi.begin(ssid , password );
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  Serial.println();
}


void setup_rtc_ntp(void)
{
  configTime (gmtOffset_sec, daylightOffset_sec, ntpServer);
  timestamp = time(NULL);
  rtc.setTime(timestamp + gmtOffset_sec);
}


void askTime(void)
{
  mins = timeinfo.tm_min;
  if (!getLocalTime(&timeinfo))
  {
    display.setCursor(0, 25);
    display.println("veo la hrs del rtc interno ");
    timestamp = rtc.getEpoch() - gmtOffset_sec;
    timeinfo = rtc.getTimeStruct();
    display.println(&timeinfo, "%H:%M:%S");


  }
  else
  {
    display.setCursor(0, 25);
    display.print("NTP Time:");
    timestamp = time(NULL);
    display.println(&timeinfo, "%H:%M:%S");



  }
  display.display();
  return;
}
