#include <WiFi.h>
#include <WebSocketsClient.h>
#include "Position.h"
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include <base64.h>

// const char* ssid = "Device-Northwestern"; // Make sure to register with device northwestern prior to using this

/* In case you want to use eduroam*/
const char *ssid = "AndroidAPPM";
// const char* eap_anonymous_id = "anonymous@northwestern.edu";
// const char* eap_id = "YOUR_NETID@northwestern.edu"; // replace "YOUR_NETID" with your net ID
const char *eap_password = "paint278"; // replace with your password

WebSocketsClient websockets_client;
// int adc_pin = 32; // replace with the ADC pin you're using

void websockets_event(WStype_t type, uint8_t *payload, size_t length);
void sendImage(const char *imagePath);
void websetup();
void webloop();

void websetup()
{
    Serial.begin(115200);
    SPIFFS.begin();
    Serial.println(WiFi.macAddress()); // displays MAC address to register with device northwestern, if desired

    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, eap_password);

    /* In case you want to use eduroam*/
    // Serial.println("Connecting to WPA2 Enterprise network");
    // esp_wifi_sta_wpa2_ent_set_identity((uint8_t*) eap_anonymous_id, strlen(eap_anonymous_id));
    // esp_wifi_sta_wpa2_ent_set_username((uint8_t*) eap_id, strlen(eap_id));
    // esp_wifi_sta_wpa2_ent_set_password((uint8_t*) eap_password, strlen(eap_password));
    // esp_wifi_sta_wpa2_ent_enable();

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // websockets_client.begin("localhost", 8080); // replace with your server's info
    // websockets_client.begin("192.168.107.28", 8080); // replace with your server's info
    websockets_client.begin("192.168.56.1", 8080); // replace with your server's info
    // websockets_client.begin("34.218.247.252", 8888, "/websocket_esp32"); // replace with your server's info
    websockets_client.onEvent(websockets_event);
}

void webloop()
{
    Serial.print("1");
    // Sending IMU data as JSON
    float *sensorData = readSensorData();
    float yaw = sensorData[0];
    float pitch = sensorData[1];
    float roll = sensorData[2];
    Serial.print("2");
    StaticJsonDocument<1024> doc;
    doc["type"] = "imu";
    doc["yaw"] = yaw;
    doc["pitch"] = pitch;
    doc["roll"] = roll;
    Serial.print("3");
    String jsonData;
    serializeJson(doc, jsonData);
    websockets_client.sendTXT(jsonData);
    Serial.print("4");
    // Sending the image
    sendImage("/image.jpg");
    Serial.print("5");
    websockets_client.loop();
    delay(1000);
}

void websockets_event(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.println("Disconnected from WebSocket server");
        break;
    case WStype_CONNECTED:
        Serial.println("Connected to WebSocket server");
        break;
    case WStype_TEXT:
        Serial.println("Received message from WebSocket server: " + String((char *)payload));
        break;
    }
}

void sendImage(const char *imagePath)
{
    if (!SPIFFS.exists(imagePath))
    {
        Serial.println("Image file not found");
        return;
    }

    File imageFile = SPIFFS.open(imagePath, "r");
    if (!imageFile)
    {
        Serial.println("Failed to open image file");
        return;
    }

    websockets_client.sendTXT("IMAGE_START");

    while (imageFile.available())
    {
        uint8_t buffer[512];
        size_t bytesRead = imageFile.read(buffer, sizeof(buffer));
        String data = base64::encode(buffer, bytesRead);

        StaticJsonDocument<1024> doc;
        doc["type"] = "image";
        doc["data"] = data;

        String jsonData;
        serializeJson(doc, jsonData);
        websockets_client.sendTXT(jsonData);
    }

    websockets_client.sendTXT("IMAGE_END");
    imageFile.close();
}
