#define BLYNK_TEMPLATE_ID SOLAR_ENERGY_CONTROLLER_BLYNK_TEMPLATE_ID
#define BLYNK_DEVICE_NAME SOLAR_ENERGY_CONTROLLER_BLYNK_DEVICE_NAME
#define BLYNK_AUTH_TOKEN SOLAR_ENERGY_CONTROLLER_BLYNK_AUTH_TOKEN


// Comment this out to disable prints and save space
//#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = BLYNK_AUTH_TOKEN;

WidgetLED solar_charge_led(V100);

BLYNK_WRITE(V14){ //remote control solar charger
  if(param.asInt() == 1) ve_data.enable_charger = true;
  else                   ve_data.enable_charger = false;
}

char *uptime(unsigned long milli){
  static char _return[32];
  unsigned long secs=milli/1000, mins=secs/60;
  unsigned int hours=mins/60, days=hours/24;
  milli-=secs*1000;
  secs-=mins*60;
  mins-=hours*60;
  hours-=days*24;
  sprintf(_return,"Uptime: %d days %2.2d:%2.2d:%2.2d.%3.3d", (byte)days, (byte)hours, (byte)mins, (byte)secs, (int)milli);
  return _return;
}

char *uptime(){
  return (char *)uptime(millis()); // call original uptime function with unsigned long millis() value
}

void ve_data_update(){
  if(ve_data.panel_voltage.flags.blynk){
    Blynk.virtualWrite(V4, ve_data.panel_voltage.value);
    ve_data.panel_voltage.flags.blynk = false;
  }

  if(ve_data.battery_voltage.flags.blynk){
    Blynk.virtualWrite(V2, ve_data.battery_voltage.value);
    ve_data.battery_voltage.flags.blynk = false;
  }

  if(ve_data.battery_current.flags.blynk){
    Blynk.virtualWrite(V3, ve_data.battery_current.value);
    ve_data.battery_current.flags.blynk = false;
  }

  if(ve_data.power.flags.blynk){
    Blynk.virtualWrite(V1, ve_data.power.value);
    ve_data.power.flags.blynk = false;
  }

  if(ve_data.yield_total.flags.blynk){
    Blynk.virtualWrite(V8, ve_data.yield_total.value);
    ve_data.yield_total.flags.blynk = false;
  }

  if(ve_data.yield_today.flags.blynk){
    Blynk.virtualWrite(V9, ve_data.yield_today.value);
    ve_data.yield_today.flags.blynk = false;
  }

  if(ve_data.max_pwr_today.flags.blynk){
    Blynk.virtualWrite(V10, ve_data.max_pwr_today.value);
    ve_data.max_pwr_today.flags.blynk = false;
  }

  if(ve_data.yield_yesterday.flags.blynk){
    Blynk.virtualWrite(V11, ve_data.yield_yesterday.value);
    ve_data.yield_yesterday.flags.blynk = false;
  }

  if(ve_data.max_pwr_yesterday.flags.blynk){
    Blynk.virtualWrite(V12, ve_data.max_pwr_yesterday.value);
    ve_data.max_pwr_yesterday.flags.blynk = false;
  }

  if(ve_data.charger_state.flags.blynk){
    String ve_operation_state = "Unknown";
    switch((int)ve_data.charger_state.value){
            case 0: ve_operation_state = "Off";        break;
            case 2: ve_operation_state = "Fault";      break;
            case 3: ve_operation_state = "Bulk";       break;
            case 4: ve_operation_state = "Absorption"; break;
            case 5: ve_operation_state = "Float";      break;
    }
    Blynk.virtualWrite(V5, ve_operation_state);
    ve_data.charger_state.flags.blynk = false;
  }
  if(ve_data.enable_charger) solar_charge_led.on();
  else                       solar_charge_led.off();
}

void blynk_task(void * parameter){
  Blynk.begin(auth, SOLAR_ENERGY_CONTROLLER_SSID, SOLAR_ENERGY_CONTROLLER_PASS, SOLAR_ENERGY_CONTROLLER_BLYNK_ADDRESS, 8080);
  
  for (;;) {
    Blynk.run();
    Blynk.virtualWrite(V0, uptime());
    ve_data_update();

    Blynk.run();
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
