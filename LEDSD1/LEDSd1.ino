#include <ESP8266WiFi.h> //lib do wifi para o ESP8266
#include <ESP8266WiFiMulti.h> //lib do wifi para o ESP8266
#include <ArduinoOTA.h> //lib do ArduinoOTA
#include <ESPAsyncTCP.h>
#include <WiFiClient.h>
#include <espnow.h>

ESP8266WiFiMulti wifiMulti;
#include <ESP8266HTTPClient.h>
HTTPClient http;
const char* ssid = "LenHide";
const char* password = "01010101";
int t;

/// 12 13 14 rgb  12 27 14
#define pin_R 14
#define pin_G 12
#define pin_B 16
//#define cnR 5
//#define cnG 7
//#define cnB 6

WiFiServer server(80);
String currentLine;

void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, 0);/////////////////////////////

  pinMode(pin_R, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_B, OUTPUT);
  digitalWrite(pin_R, HIGH);
  digitalWrite(pin_G, HIGH);
  digitalWrite(pin_B, HIGH);

  Serial.begin(115200);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print('.');
  }

  ArduinoOTA.setHostname("LedsD1");
  ArduinoOTA.onStart( startOTA );
  ArduinoOTA.onEnd( endOTA );
  ArduinoOTA.onProgress( progressOTA );
  ArduinoOTA.onError( errorOTA );
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  delay( 100 ); // power-up safety delay

  if (esp_now_init() != 0)
    Serial.println("ESPNow Init Failed");
  esp_now_register_recv_cb(OnDataRecv);
}


void loop() {
  ArduinoOTA.handle();
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
    WiFi.begin(ssid, password);
  WifiVerify();

  if (millis() - t > 1000) {
    analogWrite(pin_R, 1023);
    analogWrite(pin_G, 1023);
    analogWrite(pin_B, 1023);
  }
}






















void WifiVerify() {
  WiFiClient client = server.available();
  if (client) {
    //client.println("<!DOCTYPE html><html><head><title>Page Title</title></head><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>");
    client.println("ok"); //Algum client.print é obrigatorio
    currentLine = "";
    t = 1;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (Serial)
          Serial.print(c);
        if (c != '\r')
          currentLine += c;
      }
    }
    client.stop();
  }



  if (currentLine.substring(5, 6) == "S") {
    digitalWrite(5, 0); //ON
  }
  else if (currentLine.substring(5, 6) == "s") {
    digitalWrite(5, 1); //OFF
  }
}
