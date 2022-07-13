#define BLYNK_TEMPLATE_ID SOLAR_ENERGY_CONTROLLER_BLYNK_TEMPLATE_ID
#define BLYNK_DEVICE_NAME SOLAR_ENERGY_CONTROLLER_BLYNK_DEVICE_NAME
#define BLYNK_AUTH_TOKEN SOLAR_ENERGY_CONTROLLER_BLYNK_AUTH_TOKEN


// Comment this out to disable prints and save space
//#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

void blynk_task(void * parameter){
  Blynk.begin(auth, SOLAR_ENERGY_CONTROLLER_SSID, SOLAR_ENERGY_CONTROLLER_PASS, SOLAR_ENERGY_CONTROLLER_BLYNK_ADDRESS, 8080);
  
  for (;;) {
    Blynk.run();
    int count = millis() / 1000;
    Blynk.virtualWrite(V0, count);

    Blynk.run();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
