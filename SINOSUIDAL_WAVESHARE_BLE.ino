#include <Arduino.h>
#include "LCD_Test.h"
#include <math.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

UWORD Imagesize = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
UWORD *BlackImage = NULL;
float acc[3], gyro[3];
unsigned int tim_count = 0;
uint16_t result;
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLEDescriptor *pDescr;
BLE2902 *pBLE2902;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

// Function to send coordinates over BLE
void sendCoordinates(int x, int y) {
    String data = String(x) + "," + String(y); // Format: "x,y"
    int dataLength = data.length();

    // Check if data exceeds maximum packet size
    if (dataLength > 20) { // Adjust based on the characteristic's maximum length
        Serial.println("Data exceeds maximum packet size. Please adjust.");
        return;
    }

    // Send data over BLE characteristic
    pCharacteristic->setValue(data.c_str());
    pCharacteristic->notify();
}

void setup() {
    Serial.begin(115200);

    if (psramInit()) {
        Serial.println("\nPSRAM is correctly initialized");
    } else {
        Serial.println("PSRAM not available");
    }
    if ((BlackImage = (UWORD *)ps_malloc(Imagesize)) == NULL) {
        Serial.println("Failed to apply for black memory...");
        exit(0);
    }

    if (DEV_Module_Init() != 0)
        Serial.println("GPIO Init Fail!");
    else
        Serial.println("GPIO Init successful!");

    LCD_1IN28_Init(HORIZONTAL);
    LCD_1IN28_Clear(WHITE);
    Paint_NewImage((UBYTE *)BlackImage, LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, CYAN);
    Paint_SetScale(65);
    Paint_SetRotate(ROTATE_0);
    Paint_Clear(CYAN);

    BLEDevice::init("ESP32");
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );                   
    pDescr = new BLEDescriptor((uint16_t)0x2901);
    pDescr->setValue("A very interesting variable");
    pCharacteristic->addDescriptor(pDescr);   
    pBLE2902 = new BLE2902();
    pBLE2902->setNotifications(true);
    pCharacteristic->addDescriptor(pBLE2902);
    pService->start();
    BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
    pAdvertising->addServiceUUID(SERVICE_UUID);
    pAdvertising->setScanResponse(false);
    pAdvertising->setMinPreferred(0x0);  
    BLEDevice::startAdvertising();
    Serial.println("Waiting a client connection to notify...");

    delay(2000);
}

void loop() {
    static int prevX = 20;
    static int prevY = 100 + 50 * sin(0);
    static int xOffset = 0;

    Paint_Clear(CYAN);
    Paint_DrawLine(20, 20, 20, 220, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(20, 120, 220, 120, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

    // Draw the sine wave
    String graphData = ""; // String to hold graph data
    for (int x = 0; x <= 200; x++) {
        int y = 100 + 50 * sin((x + xOffset) * 0.1); // Adjust amplitude and frequency as needed
        Paint_DrawLine(prevX, prevY, 20 + x, y, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
        prevX = 20 + x;
        prevY = y;

        // Append the x and y coordinates to graphData string
        graphData += String(x) + "," + String(y) + ";";
    }

    xOffset++; // Increment xOffset for animation effect

    // Reset prevX and prevY
    prevX = 20;
    prevY = 100 + 50 * sin(xOffset * 0.1);

    // Update the display
    LCD_1IN28_Display(BlackImage);

    // Send coordinates over BLE if device is connected
    if (deviceConnected) {
        sendCoordinates(prevX, prevY);
    }

    // Disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }

    // Connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
    
}
// need whole ass library with this
