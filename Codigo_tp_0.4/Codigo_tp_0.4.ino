#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <DHT.h>

#define BTN_PIN 11
#define BTN_PIN2 12
bool btnState;
bool btnState2;

#define OLED_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16

int sM;
#define INICIAL 0
#define CHOOSE 1
#define STILL 2
#define MOD 3
int state;
int umbral = 28;

#define DHTPIN 23
#define DHTTYPE DHT11
float temperature;

#define RELAY_PIN 18

int ms;

Adafruit_SSD1306 display(OLED_RST);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(BTN_PIN,INPUT_PULLUP);
  pinMode(BTN_PIN2,INPUT_PULLUP);
  
  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Hola mundo");
  display.display();
  dht.begin();
  pinMode(RELAY_PIN, OUTPUT);
}

void loop() {
  switch(sM){
    case INICIAL:
    {
      delay(2000);
      temperature = dht.readTemperature();
      timer();
      sM = CHOOSE;
      if(ms == 500){
      display.clearDisplay();
      display.display();
      ms = 0;
      }
    }
    break;

    case CHOOSE:
      
      if(BTN_PIN == LOW && BTN_PIN2 == LOW){
        state++;
        if (state == 3){
          state = 1;
        }
      }
      
      if(state == 1){
        sM = STILL;
      }else{
        sM = MOD;
      }  
    break;
    
    case STILL:
      display.setCursor(0, 0);
      display.print("Temp: ");
      display.print(temperature);
      display.print(" C");
      display.setCursor(1, 0);
      display.print("Temp U: 28°C");
    break;
    
    case MOD:
      display.setCursor(0, 0);
      display.print("Temp: ");
      display.print(temperature);
      display.print(" C");
      display.setCursor(1, 0);
      display.print("Temp U: ");
      display.print(umbral);
      display.print(" C");

      if(BTN_PIN == LOW && btnState == 1){
        umbral--;
      }
      if(BTN_PIN2 == LOW && btnState2 == 1){
        umbral++;
      }
      
      
    break;
    
  }
}

void timer(){
  ms++;
}
