
#include "freertos/task.h"
#include <WiFi.h>
#include <HTTPClient.h>
HTTPClient client;

#define sensor_pin 34
#define onoff_pin 25
#define led_pin 26
#define wifi_pin 13

#define JST 3600* 9

volatile int sample_count = 0;
volatile int count = 0;
volatile int calibration = 0;

String url = "";
String url_start = "";

time_t now_time;
struct tm *timeinfo;  //時刻を格納するオブジェクト
char current_time[10];
char current_date[30];

volatile int measurement_ID;
volatile int signal;
volatile int signal_pre =1800;

//[Wi-Fi環境・サーバ]が変更される場合は以下を変更
//*********************************************
const char* ssid = "elecom-58179b";
const char* password = "cmp574fn3em4";
const String host ="192.168.2.105";
#define port 8000
int device_ID = 92407;
//*********************************************

void setup() {  
  Serial.begin(115200); 
  while (!Serial);
  
  pinMode(sensor_pin, INPUT);
  pinMode(onoff_pin, INPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(wifi_pin, OUTPUT);

  url = "http://";
  url += host;
  url += ":";
  url += port;
  url += "/v1/api";
  url_start = url + "/measurement/start/";
  url += "/calc_data/";
  WiFiDisConnect();
  WiFiConnect();
  
}

void loop() {
  int sensor_power = digitalRead(onoff_pin);
  if (sensor_power == HIGH) {

    Serial.println("[ON ] Start Calibulation");
    Serial.print("[ON ] ");
    while (calibration <=  6){
      if (calibration % 2 == 0){
        digitalWrite(led_pin, LOW);
      }else {
        digitalWrite(led_pin, HIGH);
      }
      delay(1000);
      Serial.print("◼");
      calibration += 1 ;
    }
    calibration = 0;
    digitalWrite(led_pin, HIGH);
    Serial.println(" ");
    Serial.println("[ON ] Success Calibulation");
    
    now_time = time(NULL);
    timeinfo = localtime(&now_time);

    HttpConnectStart();
    StartPost();
    HttpDisConnect();

    int sensor_power = digitalRead(onoff_pin);
    if (sensor_power == HIGH){
      Serial.println("[ON ] Start MultiTask");
      
      xTaskCreatePinnedToCore(TaskSave, "tasksave", 4096, NULL, 2, NULL, 1);
      xTaskCreatePinnedToCore(TaskPost, "taskpost", 4096, NULL, 2, NULL, 0);

      while(sensor_power == HIGH){
        
        if(sensor_power == LOW){
          break;
        }
      }
    }
       
  }else if (sensor_power == LOW){
    digitalWrite(led_pin, LOW);
    
    now_time = time(NULL);
    timeinfo = localtime(&now_time);  
    sprintf(current_time, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    Serial.print("[OFF] ");
    Serial.println(current_time);
    
    delay(2000);
  }

}
