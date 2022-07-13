#define RXD2 16
#define TXD2 17
#define MySerial Serial2

void set_ve_value(data_field_t *field_ptr, float new_val, String field_name){
  data_field_t *field = (data_field_t*)field_ptr;
  field->value = new_val;
  field->update_timestamp = millis();
  field->flags.blynk = true;
  field->flags.mqtt = true;
  //TelnetStream.println("VE_RECEIVER: " + field_name + ": " + (String)new_val);
}

void victron_task(void * parameter){
  victron_data_t *ve_data = (victron_data_t*)parameter;
  
  MySerial.begin(19200, SERIAL_8N1);
  MySerial.setRxBufferSize(400);
  pinMode(TXD2, OUTPUT);
  
  String tempStr = "";
  
  for (;;) {    
    while (MySerial.available() > 0) {
        char byteFromSerial = MySerial.read();
        if(byteFromSerial != '\n'){
          tempStr += byteFromSerial;
        }else{
          float calculated_val;
          if(tempStr.charAt(0) == 'V' && tempStr.charAt(1) == 'P' && tempStr.charAt(2) == 'V'){ //Panel voltage
            String val = tempStr.substring(4);
            calculated_val = atoi(val.c_str()) / 1000.0;
            set_ve_value(&ve_data->panel_voltage, calculated_val, "Panel voltage");
          }else if(tempStr.charAt(0) == 'V' && tempStr.charAt(1) != 'P'){ //calc voltage
            String val = tempStr.substring(2);
            calculated_val = atoi(val.c_str()) / 1000.0;
            set_ve_value(&ve_data->battery_voltage, calculated_val, "Battery voltage");
          }else if(tempStr.charAt(0) == 'I'){ // calc current
            String val = tempStr.substring(2);
            calculated_val = atol(val.c_str()) / 1000.0;
            set_ve_value(&ve_data->battery_current, calculated_val, "Battery current");
          }else if(tempStr.charAt(0) == 'P' && tempStr.charAt(1) == 'P' && tempStr.charAt(2) == 'V'){ // Power
            String val = tempStr.substring(4);
            calculated_val = atoi(val.c_str());
            set_ve_value(&ve_data->power, calculated_val, "Power");
          }else if(tempStr.charAt(0) == 'H' && tempStr.charAt(1) == '1' && tempStr.charAt(2) == '9'){ // Yield total
            String val = tempStr.substring(4);
            calculated_val = atol(val.c_str()) / 100.0;
            set_ve_value(&ve_data->yield_total, calculated_val, "Yield total");
          }else if(tempStr.charAt(0) == 'H' && tempStr.charAt(1) == '2' && tempStr.charAt(2) == '0'){ // Yield today
            String val = tempStr.substring(4);
            calculated_val = atol(val.c_str()) / 100.0;
            set_ve_value(&ve_data->yield_today, calculated_val, "Yield today");
          }else if(tempStr.charAt(0) == 'H' && tempStr.charAt(1) == '2' && tempStr.charAt(2) == '1'){ // Maximum power today
            String val = tempStr.substring(4);
            calculated_val = atol(val.c_str());
            set_ve_value(&ve_data->max_pwr_today, calculated_val, "Max power today");
          }else if(tempStr.charAt(0) == 'H' && tempStr.charAt(1) == '2' && tempStr.charAt(2) == '2'){ // Yield yesterday
            String val = tempStr.substring(4);
            calculated_val = atol(val.c_str()) / 100.0;
            set_ve_value(&ve_data->yield_yesterday, calculated_val, "Yield yesterday");
          }else if(tempStr.charAt(0) == 'H' && tempStr.charAt(1) == '2' && tempStr.charAt(2) == '3'){ // Maximum power Yesterday
            String val = tempStr.substring(4);
            calculated_val = atol(val.c_str());
            set_ve_value(&ve_data->max_pwr_yesterday, calculated_val, "Max power yesterday");
          }else if(tempStr.charAt(0) == 'C' && tempStr.charAt(1) == 'S'){ // Charger State
            String val = tempStr.substring(3);
            calculated_val = atol(val.c_str());
            set_ve_value(&ve_data->charger_state, calculated_val, "Charger state");
          }
          tempStr = "";
        }
    }
    digitalWrite(TXD2, ve_data->enable_charger);
    vTaskDelay(pdMS_TO_TICKS(950));
  }
}
