#include <Adafruit_Sensor.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <DHT.h>

#define RELAY_PIN 18

#define OLED_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16

#define DHTPIN 23
#define DHTTYPE DHT11
float temperature;
float umbral = 28.00;

int ms;

Adafruit_SSD1306 display(OLED_RST);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  dht.begin();
}

void loop() {
  delay(2000);
  temperature = dht.readTemperature();
  if(ms >= 500){
    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Temp: ");
    display.print(temperature);
    display.print(" C");
    display.display();
    ms = 0;
  }

  display.clearDisplay();
  display.setCursor(1, 0);
  display.print("Temp U: ");
  display.print(umbral);
  display.print(" C");
  display.display();

  if(umbral >= 28.00){
    digitalWrite(RELAY_PIN,HIGH);
  }
}

void timer(){
  ms++;
}
