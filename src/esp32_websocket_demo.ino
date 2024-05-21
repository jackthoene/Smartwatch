#include <WiFi.h>
#include <WebSocketsClient.h>

// const char* ssid = "Device-Northwestern"; // Make sure to register with device northwestern prior to using this

/* In case you want to use eduroam*/
const char *ssid = "AndroidAPPM";
// const char* eap_anonymous_id = "anonymous@northwestern.edu";
// const char* eap_id = "YOUR_NETID@northwestern.edu"; // replace "YOUR_NETID" with your net ID
const char *eap_password = "paint278"; // replace with your password

WebSocketsClient websockets_client;
// int adc_pin = 32; // replace with the ADC pin you're using

void websetup()
{
    Serial.begin(115200);
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
    websockets_client.begin("192.168.107.28", 8080); // replace with your server's info
    // websockets_client.begin("34.218.247.252", 8888, "/websocket_esp32"); // replace with your server's info
    websockets_client.onEvent(websockets_event);
}

void webloop()
{
    // float adc_value = analogRead(adc_pin);
    // float voltage = adc_value * (3.3 / 4096.0);
    String message = "Hello other side!";
    websockets_client.sendTXT(message);
    websockets_client.loop();
    delay(100);
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
