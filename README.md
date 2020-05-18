# EPS32 + Arduino IDE  
## Abstract  
ESP32 + PulseSensor により脈拍を読み込み  
サーバ側にデータをPOSTするためのスクリプトだ！  
  
## Tools  
* ESPr Developer 32  
https://www.switch-science.com/catalog/3210/  
  
* PulseSensor  
https://www.switch-science.com/catalog/1135/  
  
* Arduino IDE (1.8.10)  
https://www.arduino.cc/en/main/software  
  
* Library to install  
ArduinoJson ( by Benoit Blanchon) 6.14  

## About Code  
### ESP_Sensor.ino  
<code>void setup()</code>  
* PinModeの設定  
* URLの生成  
* WiFiとの接続 ( <code>void WiFiConnect()</code> )  

<code>void loop()</code>  
1. 現在時刻の取得  
2. Switch On ⇨ センサの校正 ( 20 sec )  
3. サーバとの接続 ( <code>void HttpConnect()</code> )  
4. データをJSON形式でPOST ( <code>void CreateJson()</code> )  
5. Switch Off ⇨ 全接続の解除  
  
### TaskFunction.ino  
<code>void SaveBeatData()</code>  
* Buffer
POSTするデータ数を変更する時は, timeInterval(測定時間幅)とarrayNum(要素数)を変更  
arrayNumの値により, バッファの大きさが決まる
```php
char buffer[strNum]   
```
strNum = [括弧など] + [要素数*桁数] + [カンマの数] + [余分に]  
  
* Json
Jsonの生成は以下のURL参照  
https://arduinojson.org/v6/api/dynamicjsondocument/  
https://arduinojson.org/v6/example/generator/  
  
* RC Filter
```php
void fuction(){
　Signal = 0.8 * S + 0.2 * analogRead(SensorPin);  
　// Any Process  
　S = Signal;  
}  
```  

RCフィルタはアナログフィルタだが, 差分方程式を用いることで,  
サンプリングした離散データに対して, 同様の特性を持つデジタルフィルタになる.  
  
<code>y[n] = a*y[n-1] + (1-a)*x[n]</code>  
  
<code>a = CR / (CR + T )</code>  
  
<code>fc = 1 / 2πCR</code>  
  
遮断周波数 <code>fc</code> , サンプリング周期 <code>T</code>, 電気容量 <code>C</code>, 抵抗値 <code>R</code> とした時,  
<code>T</code> はTimeInterval=10ms, 任意の値 <code>fc</code> を用いて, <code>a</code> を算出する.  
  
https://bluefish.orz.hm/sdoc/digifil2.html#RCフィルタ

## How to use  
1. 電源と接続  
2. 真ん中のスイッチ(電源)をONにする  
3. 緑LEDが点灯し, WiFiと接続できたことを確認する  
4. もう一つのスイッチ(センサ)をONにする  
5. 指にセンサを装着したまま, 20秒待つ  
6. 赤LEDの点滅が終わったら測定開始  
7. 測定が終了したら, センサ側のスイッチをOFFにする  
8. 赤LEDの消灯を確認後, 電源スイッチをOFFにする

