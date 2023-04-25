#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <DHT.h>

#define BTN_PIN 11
#define BTN_PIN2 12
bool doubleBtnSt;
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
#define WAIT 4
int state = 1;
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
  pinMode(RELAY_PIN, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  
  dht.begin();
  
}

void loop() {
  
  timer();

}

void stateMachine(){
  switch(sM){
    case INICIAL:
    {
      
      delay(2000);
      temperature = dht.readTemperature();
      
      if(ms >= 500){
        
        display.clearDisplay();
        display.display();
        ms = 0;
        
      }
      
      sM = CHOOSE; 

    }
    break;

    case CHOOSE:
      
      if(BTN_PIN == LOW && BTN_PIN2 == LOW && doubleBtnSt == 1){
        
        state++;
        
        if (state >= 3){
          state = 1;
          
        }
        
        doubleBtnSt = 0;
      }
      
      sM = WAIT;
      
    break;
    
    case STILL:
    
      display.setCursor(0, 0);
      display.print("Temp: ");
      display.print(temperature);
      display.print(" C");
      display.setCursor(1, 0);
      display.print("Temp U: 28°C");

      sM = INICIAL;
      
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
        btnState == 0;
        
      }
      
      if(BTN_PIN2 == LOW && btnState2 == 1){
        
        umbral++;
        btnState2 == 0;
        
      }
      
      sM = WAIT;
      
    break;

    case WAIT:
    sM = INICIAL;
    
      if(BTN_PIN == HIGH && BTN_PIN2 == HIGH && doubleBtnSt == 0){
        
        doubleBtnSt == 1;
        
        if(state == 1){
          sM = STILL;
        }else{
          sM = MOD;
        }  
      }
      
      if(BTN_PIN == HIGH && BTN_PIN2 == HIGH && btnState == 0){
        
        btnState == 1;
        
      }
      
      if(BTN_PIN == HIGH && BTN_PIN2 == HIGH && btnState2 == 0){
        
        btnState2 == 1;

      }
      
    break;
  }
}

void timer(){
  ms++;
}
