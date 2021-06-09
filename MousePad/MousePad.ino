#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoOTA.h>
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
#define pin_R 4
#define pin_G 5
#define pin_B 14
//#define cnR 5
//#define cnG 7
//#define cnB 6

WiFiServer server(80);

void setup() {

  pinMode(pin_R, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_B, OUTPUT);
  digitalWrite(pin_R, LOW);
  digitalWrite(pin_G, LOW);
  digitalWrite(pin_B, LOW);

  Serial.begin(115200);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print('.');
  }

  ArduinoOTA.setHostname("MousePad");
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
  //delay(20);
  //WifiVerify();
  ArduinoOTA.handle();
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
    WiFi.begin(ssid, password);

  if (millis() - t > 1000) {
    analogWrite(pin_R, 0);
    analogWrite(pin_G, 0);
    analogWrite(pin_B, 0);
  }

}
