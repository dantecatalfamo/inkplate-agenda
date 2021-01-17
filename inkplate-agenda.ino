#include <Inkplate.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"
#define DELAY_REFETCH 300000


Inkplate display(INKPLATE_1BIT);

void setup() {
    Serial.begin(115200);
    display.begin();
    display.setTextSize(2);
    Serial.print("Hello from Inkplate!\n");
    display.clearDisplay();
    display.display();
    display.println("Hello there friend! :)");
    display.println("How are you?");
    display.partialUpdate();

    WiFi.begin(ssid, wifiPass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        display.print(".");
        display.partialUpdate();
    }
    display.println();
    display.printf("%s connected!\n", ssid);
    display.printf("MAC Address: %s\n", WiFi.macAddress().c_str());
    display.print("IP: ");
    display.println(WiFi.localIP());
    display.partialUpdate();
    delay(5000);
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        display.println("\nWiFi disconnected.");
        display.partialUpdate();
        return;
    }
    HTTPClient http;
    http.begin(url);
    http.setAuthorization(httpUsername, httpPassword);
    int httpCode = http.GET();
    if (httpCode > 0) {
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println(http.getString());
    } else {
        display.printf("\nError in HTTP, got code %d\n", httpCode);
    }
    display.display();
    delay(DELAY_REFETCH);
}
