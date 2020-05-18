#include <ArduinoJson.h>

char start_buffer[128];
char end_buffer[128];

void StartPost(){
  DynamicJsonDocument root(400);
  now_time = time(NULL);
  timeinfo = localtime(&now_time);
  sprintf(current_date, "%04d-%02d-%02d %02d:%02d:%02d",timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
 
  root["start_time"] = current_date;
  root["dev_id"] = device_ID;

  serializeJson(root,Serial);
  Serial.println(" ");
  serializeJson(root, start_buffer, sizeof(start_buffer));
  int postcode = client.POST((uint8_t *) start_buffer, strlen(start_buffer));
  
  Serial.print("PostCode = ");
  Serial.println(postcode);
  
  String response = client.getString();
  StaticJsonDocument<200> doc;
  JsonObject object = doc.as<JsonObject>();
  deserializeJson(doc, response);
  measurement_ID = doc["id"];

}
