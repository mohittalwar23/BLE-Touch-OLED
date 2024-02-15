#include <Arduino.h>
#include <Wire.h>
#include <OLED_SSD1306_Chart.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

#define SDA_PIN D1
#define SCL_PIN D2

const int TOUCH_PIN1 = 4; // GPIO4 // Assuming the touch sensor is connected to digital pin 4
const int TOUCH_PIN2 = 2;
const int TOUCH_PIN3 = 32;
const int TOUCH_PIN4 = 33;
const int TOUCH_PIN5 = 15;

OLED_SSD1306_Chart display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
int currentGraph = 1;

void setup() {
#if defined(ESP8266)
  Wire.begin(SDA_PIN, SCL_PIN);
#else
  Wire.begin();
#endif

  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Graph: ");
  display.print(currentGraph);
  display.display();

  display.setChartCoordinates(0, 60);
  display.setChartWidthAndHeight(123, 55);
  display.setXIncrement(5);
  display.setYLimits(0, 80); // Adjust according to your touch sensor's range (0 to 80)
  display.setYLimitLabels("0", "80");
  display.setYLabelsVisible(true);
  display.setAxisDivisionsInc(12, 6);
  display.setPlotMode(SINGLE_PLOT_MODE);

  display.drawChart();
}

void loop() {
  int touchValue5 = touchRead(TOUCH_PIN5); // Read touch value from select pin

  if (touchValue5 >= 0 && touchValue5 <= 80) { // Ensure touch value is within the range
    // Increment currentGraph to toggle between graphs
    currentGraph++;
    if (currentGraph > 4) {
      currentGraph = 1;
    }

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Graph: ");
    display.print(currentGraph);
    display.display();
    delay(1000); // Display the current graph number for 1 second
    display.clearDisplay();
    display.drawChart();
    display.display();
  }

  int touchValue = 0;
  switch (currentGraph) {
    case 1:
      touchValue = touchRead(TOUCH_PIN1);
      break;
    case 2:
      touchValue = touchRead(TOUCH_PIN2);
      break;
    case 3:
      touchValue = touchRead(TOUCH_PIN3);
      break;
    case 4:
      touchValue = touchRead(TOUCH_PIN4);
      break;
  }

  if (touchValue >= 0 && touchValue <= 80) { // Ensure touch value is within the range
    // Update chart and check if the chart is at the end
    if (!display.updateChart(touchValue)) {
      // If chart is at the end, clear display and redraw the chart from the beginning
      display.clearDisplay();
      display.drawChart();
      display.display();
    } else {
      display.display();
    }
  }

  delay(100);
}
