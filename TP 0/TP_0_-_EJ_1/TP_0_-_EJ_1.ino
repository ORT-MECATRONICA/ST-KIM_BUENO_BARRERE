#include <Adafruit_Sensor.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>

#define OLED_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16

Adafruit_SSD1306 display(OLED_RST);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Hola mundo");
  display.display();
}

void loop(){
  
}
