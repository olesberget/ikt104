#include "mbed.h"
#include "HTS221Sensor.h"
#include "DFRobot_RGBLCD.h"
#include <cstdio>

DevI2C i2c(PB_11, PB_10);
HTS221Sensor hts221(&i2c);

DigitalIn button(PB_8, PullUp);

DFRobot_RGBLCD lcd(16, 2, PC_1, PC_0);

int main() {
  if (hts221.init(NULL) != 0) {
      printf("Failed to initilize HTS221 device\n");
  }
  if (hts221.enable() != 0){
      printf("Failed to power up HTS221 device\n");
  }

  lcd.init();
  lcd.display();

  int showTemperature = 1;

  while (true) {
    float temperature;
    float humidity;
    hts221.get_temperature(&temperature);
    hts221.get_humidity(&humidity);
    printf("Temperature:%.2fC  Humidity: %.2f \n", temperature, humidity);

    if (button == 0) {
      showTemperature = !showTemperature;
    }

    lcd.clear();
    lcd.setCursor(0, 0);

    if (showTemperature) {
      lcd.printf("Temperature:%.2fC", temperature);

      if (temperature < 20) {
        lcd.setRGB(0, 0, 255);
      } else if (temperature >= 20 && temperature <= 24) {
        lcd.setRGB(255, 165, 0);
      } else {
        lcd.setRGB(255, 0, 0);
      }
    } else {
      lcd.printf("Humidity: %.2f", humidity);

      int blue = (int) (humidity * 2.55);
      lcd.setRGB(0, 0, blue);
    }

    wait_us(100000);
  }

  return 0;
}

