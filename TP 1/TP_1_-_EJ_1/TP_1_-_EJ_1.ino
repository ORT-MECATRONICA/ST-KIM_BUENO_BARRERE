#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <DHT.h>

#define RELAY_PIN 18

#define BTN_PIN 11
#define BTN_PIN2 12
bool readBtn;
bool readBtn2;
bool btnState;
bool btnState2;

#define OLED_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16

int sM;
#define DOUBLE 0
#define SINGLE 1
#define WAIT 2
bool plusM;
bool plusH;

#define DHTPIN 23
#define DHTTYPE DHT11
float temperature;

int ms;
int ms2;
int m;
int h;

Adafruit_SSD1306 display(OLED_RST);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  
  pinMode(BTN_PIN,INPUT_PULLUP);
  pinMode(BTN_PIN2,INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC,OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  
  dht.begin();

}

void loop() {



  readBtn = digitalRead(BTN_PIN);
  readBtn2 = digitalRead(BTN_PIN2);
      
  if(ms2 >= 500){
    display.clearDisplay();
    display.display();
    ms2 = 0;
  }
  
  timer();
  stateMachine();
  
}

int stateMachine(){
  switch(sM){
    
    case DOUBLE:

      display.setCursor(0, 0);
      display.print("Hora: ");
      if(h < 10){
        display.print("0");
      }
      display.print(h);
      display.print(":");
      if(m < 10){
        display.print("0");
      }
      display.print(m);
      
      display.setCursor(1, 0);
      display.print("Temp: ");
      display.print(temperature);
      display.print(" C");

      if(readBtn == LOW && readBtn2 == LOW && btnState == 1){
        btnState = 0;
        sM = WAIT;
      }
    
    break;
    
    case SINGLE:

      display.setCursor(0, 6);
      display.print("Son las: ");
      
      display.setCursor(1, 0);
      if(h < 10){
        display.print("0");
      }
      display.print(h);
      display.print(":");
      if(m < 10){
        display.print("0");
      }
      display.print(m);
      
      if(readBtn == LOW && readBtn2 == HIGH && plusH == 1){
        sM = WAIT;
        plusH = 0;
        h++;
      }

      if(readBtn == HIGH && readBtn2 == LOW && plusM == 1){
        sM = WAIT;
        plusM = 0;
        m++;
      }

    break;
    
    case WAIT:

    if(readBtn == HIGH && readBtn2 == HIGH && btnState == 0){
      btnState = 1;
      sM = SINGLE;
    }

    if(readBtn == HIGH && readBtn2 == HIGH && btnState2 == 0){
      btnState2 = 1;
      sM = DOUBLE;
    }

    if(readBtn == HIGH && readBtn2 == HIGH && plusH == 0){
      sM = SINGLE;
    }

    if(readBtn == HIGH && readBtn2 == HIGH && plusM == 0){
      sM = SINGLE;
    }
    
    break;
    
  }
}

int timer(){
  ms++;
  
  if(ms >= 60000){
    m++;
    ms = 0;
  }

  if (m >= 60){
    h++;
    m = 0;
  }
  
  if(h > 24){
    h = 0;
  }

  ms2++;
}
