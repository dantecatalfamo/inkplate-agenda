#include <Inkplate.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "secrets.h"
#define TEXT_SCALE_X 2
#define TEXT_SCALE_Y 3
#define DELAY_REFETCH 300000
#define CLEAN_REFRESHES 72


Inkplate display(INKPLATE_1BIT);
int refreshes;

void setup() {
    Serial.begin(115200);
    display.begin();
    display.setTextSize(TEXT_SCALE_X, TEXT_SCALE_Y);
    Serial.print("Hello from Inkplate!\n");
    display.clearDisplay();
    display.display();
    display.println("Hello there friend! :)");
    display.println("How are you?");
    display.printf("Connecting to %s\n", ssid);
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
    refreshes = 0;
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        display.setCursor(0, 0);
        display.println("\nWiFi disconnected.");
        display.partialUpdate();
        return;
    }
    HTTPClient http;
    http.begin(url);
    http.setAuthorization(httpUsername, httpPassword);
    int httpCode = http.GET();
    display.setCursor(0, 0);
    if (httpCode > 0) {
        display.clearDisplay();
        display.println(http.getString());
    } else {
        display.printf("\nError in HTTP, got code %d\n", httpCode);
    }
    http.end();
    refreshes++;
    if (refreshes > CLEAN_REFRESHES) {
        display.display();
        refreshes = 0;
    } else {
        display.partialUpdate();
    }
    delay(DELAY_REFETCH);
}

// Local Variables:
// mode: c++
// End:
