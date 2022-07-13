#include "Credentials.h"
#include "DataStructures.h"
#include "ElegantOta.h"
#include "Blynklegacy.h"
#include "VictronReceiver.h"

void setup()
{
  delay(2000);
  xTaskCreate(victron_task, "VE_HANDLE",     10000, &ve_data,   1, NULL);
  delay(10);
  xTaskCreate(ota_task,     "OTA_HANDLE",    10000, NULL,       1, NULL);
  delay(10);
  xTaskCreate(blynk_task,   "BLYNK_HANDLE",  10000, NULL,       1, NULL);
}

void loop()
{
  
}
