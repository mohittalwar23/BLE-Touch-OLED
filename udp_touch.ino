#include <WiFi.h>
#include <WiFiUdp.h>

#define SERIAL_BAUD 115200
const char *udpAddress = "192.168.218.75"; // Destination IP address
const int udpPort = 4444;                   // Destination UDP port

const char *ssid = "rizzler";        // Your WiFi SSID
const char *password = "12345678"; // Your WiFi password

const int touchPin = 4; // Touch pin to read from

WiFiUDP udp;

void setup()
{
    Serial.begin(SERIAL_BAUD);
    Serial.println("Setup done!");
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected with IP address:");
    Serial.println(WiFi.localIP());
}

void loop()
{
    int touchValue = touchRead(touchPin); // Read touch value from touch pin

    Serial.print("Touch Value: ");
    Serial.println(touchValue);

    // Send touch value via UDP
    udp.beginPacket(udpAddress, udpPort);
    udp.print(touchValue);
    udp.endPacket();

    delay(10); // Delay for 1 second before reading again
}
