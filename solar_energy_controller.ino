#include "Credentials.h"
#include "ElegantOta.h"
#include "Blynklegacy.h"

void setup()
{
  delay(2000);
  xTaskCreate(ota_task,     "OTA_HANDLE",    10000, NULL,       1, NULL);
  delay(10);
  xTaskCreate(blynk_task,   "BLYNK_HANDLE",  10000, NULL,       1, NULL);
}

void loop()
{
  
}
