#include <WiFi.h>

void WiFiConnect(){

  while(1){
    WiFi.begin(ssid, password); 
    count = 0;
    
    if (WiFi.status() == WL_CONNECTED){
      Serial.println("[!!!] Failed WiFiConnect");
      Serial.println("      Try Again ...");
    }
    while (WiFi.status() != WL_CONNECTED){
      
      if(count == 5){
        break;
      }else if (count % 2 == 0){
        digitalWrite(wifi_pin, LOW);
      }else {
        digitalWrite(wifi_pin, HIGH);
      }
      delay(1000);
      count += 1;
    }

    if (WiFi.status() == WL_CONNECTED){
      Serial.println("[***] Success WiFiConnect");
      digitalWrite(wifi_pin, HIGH);
      configTime( JST, 0, "ntp.nict.jp");
      break;
    }
  }
     
}


void WiFiDisConnect() {
  WiFi.disconnect();
  Serial.println("[***] Success WiFiDisConnect");
  digitalWrite(wifi_pin, LOW);
}
