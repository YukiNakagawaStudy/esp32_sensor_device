#include <HTTPClient.h>

void HttpConnect(){
  client.begin(url);
  Serial.print("[***] Success HttpConnect url: ");
  Serial.println(url);
  client.addHeader("Content-Type", "application/json");
}

void HttpConnectStart(){
  client.begin(url_start);
  Serial.print("[***] Success HttpConnect url: ");
  Serial.println(url_start);
  client.addHeader("Content-Type", "application/json");
}

void HttpDisConnect(){  
  client.end();
  Serial.println("[***] Success HttpDisConnect");
}
