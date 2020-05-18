#include <ArduinoJson.h>

int time_interval = 10;  //測定間隔(ms)
//***********************************
const int array_num = 3000;  //配列に入れる要素数
const int str_num = 40 + array_num*4 + array_num - 1 +  3000;
char buffer[str_num];
//***********************************
volatile int beat_array[3000];
volatile int beat_count = 0;
int last_beat_count = 0;
int tmp_beat_count = 0;
int beat_num_count = 0;
     
void SaveBeatData(char* task){
  for (int i = 0; i < 3000; i++ ) {
        beat_array[i] = 0;
  }
  while(1){
    signal = 0.8 * signal_pre + 0.2 * analogRead(sensor_pin); 
    if(beat_count < 3000){
      beat_array[beat_count] = signal;
      signal_pre = signal;
      beat_count += 1;
    } 
    else {
      beat_count = 0;
      beat_array[beat_count] = signal;
      signal_pre = signal;
      beat_count += 1;
    }
    int sensor_power = digitalRead(onoff_pin);
    if (sensor_power == LOW){
      break;
    }
    delay(time_interval);
  }
}

void PostBeat(char* task){
  while(1){
    int sensor_power = digitalRead(onoff_pin);
    delay(1000);  
    if (sensor_power == HIGH ){
      tmp_beat_count = beat_count;
      //beat_countが前回のpostの時と比較して3000を超えていない場合
      if (tmp_beat_count > last_beat_count){
        beat_num_count = tmp_beat_count - last_beat_count;
        if (beat_num_count > 1000){
          DynamicJsonDocument root(50000);
          JsonArray Beat = root.createNestedArray("beat");
          now_time = time(NULL);
          timeinfo = localtime(&now_time);
          sprintf(current_time, "%04d-%02d-%02d %02d:%02d:%02d",timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
          root["time"] = current_time;
          root["dev_id"] = device_ID;
          root["measurement_id"] = measurement_ID;
          for(int i=0; i <= beat_num_count; i++){
            root["beat"].add(*(beat_array+last_beat_count+i));
          }
          serializeJson(root, buffer, sizeof(buffer));
          HttpConnect();
          int postcode = client.POST((uint8_t *) buffer, strlen(buffer));
          last_beat_count = tmp_beat_count;
          for (int i=0; i <= beat_num_count; i++){
            root["beat"].remove(i);
          }
          root.clear();
        }
      }
      //beat_countが前回のpostの時と比較して3000を超えた場合
      else if (last_beat_count >= tmp_beat_count){
        beat_num_count = 3000 - last_beat_count + 1 + tmp_beat_count;
        if (beat_num_count > 1000) {
          DynamicJsonDocument root(50000);
          JsonArray Beat = root.createNestedArray("beat");
          now_time = time(NULL);
          timeinfo = localtime(&now_time);
          sprintf(current_time, "%04d-%02d-%02d %02d:%02d:%02d",timeinfo->tm_year+1900, timeinfo->tm_mon+1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
          root["time"] = current_time;
          root["dev_id"] = device_ID;
          root["measurement_id"] = measurement_ID;
          for(int i=0; i <= beat_num_count; i++){
            if (i < 3000-last_beat_count){
              root["beat"].add(*(beat_array+last_beat_count+i));
            } 
            else {
              root["beat"].add(*(beat_array+i+last_beat_count-3000));
            }
          }
          serializeJson(root, buffer, sizeof(buffer));
          HttpConnect();
          int postcode = client.POST((uint8_t *) buffer, strlen(buffer));
          last_beat_count = tmp_beat_count;
          for (int i=0; i <= beat_num_count; i++){
            root["beat"].remove(i);
          }
          root.clear();
        }
      }
    } 
    else if (sensor_power == LOW){
      break;
    }
  }
}
