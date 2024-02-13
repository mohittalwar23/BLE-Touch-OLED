#include <Arduino.h>
#include <Wire.h>
#include <OLED_SSD1306_Chart.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define SDA_PIN D1
#define SCL_PIN D2
const int TOUCH_PIN = 4; // GPIO4 // Assuming the touch sensor is connected to digital pin 4

OLED_SSD1306_Chart display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
#if defined(ESP8266)
  Wire.begin(SDA_PIN, SCL_PIN);
#else
  Wire.begin();
#endif


  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setChartCoordinates(0, 60);
  display.setChartWidthAndHeight(123, 55);
  display.setXIncrement(5);
  display.setYLimits(0, 80); // Adjust according to your touch sensor's range (0 to 80)
  display.setYLimitLabels("0", "80");
  display.setYLabelsVisible(true);
  display.setAxisDivisionsInc(12, 6);
  display.setPlotMode(SINGLE_PLOT_MODE);

  display.drawChart();
  display.display();
}

void loop() {
  int touchValue = touchRead(TOUCH_PIN);
  if (touchValue >= 0 && touchValue <= 80) { // Ensure touch value is within the range
    if (!display.updateChart(touchValue)) {
      display.clearDisplay();
      display.drawChart();
    }
    display.display();
  }
  delay(100);
}