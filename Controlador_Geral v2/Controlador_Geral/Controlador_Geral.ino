#include <WiFi.h>
#include <esp_now.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <HTTPClient.h>

HTTPClient http;
WiFiUDP UDP;
#define CHANNEL 6
uint8_t macBroadCast[][6] = {{0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}};
bool Broadcast = 1;
byte rgb[3] = {0, 0, 0};


const char* ssid = "LenHide";
const char* password = "01010101";

#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS_ram1    7
#define NUM_LEDS_ram2    7
#define NUM_LEDS_1070   9
#define NUM_LEDS_cooler3   16
#define NUM_LEDS_cooler2   16
#define NUM_LEDS_cooler1   16

#define rastroCooler 8
#define rastro1070 3
#define rastroRam 3
byte i = 0;
int j;
int L = (NUM_LEDS_1070 - 2) * NUM_LEDS_cooler1 * NUM_LEDS_ram1;
int y;
int T;

bool Mode [7] [5];  // saida efeito        Ram1 Ram2 1070 Cooler1 Cooler2 Cooler3
bool onoff = 0;
int passo = 0;
float tempo = millis();

#define PinFAN 23
#define LED_PIN_ram1 17
#define LED_PIN_ram2 16
#define LED_PIN_1070 32
#define LED_PIN_cooler3 33
#define LED_PIN_cooler2 25
#define LED_PIN_cooler1 26
//27

int BRIGHTNESSMAX = 255;
int BRIGHTNESSMIN = 0;
int US = 3;
CRGB ledsRam1[NUM_LEDS_ram1];
CRGB ledsRam2[NUM_LEDS_ram2];
CRGB leds1070[NUM_LEDS_1070];
CRGB ledsCooler3[NUM_LEDS_cooler3];
CRGB ledsCooler2[NUM_LEDS_cooler2];
CRGB ledsCooler1[NUM_LEDS_cooler1];

/// 12 13 14 rgb  12 27 14
#define pin_R 19
#define pin_G 5
#define pin_B 18
#define cnR 5
#define cnG 7
#define cnB 6

WiFiServer server(80);
String currentLine;
byte t = 0;

/////////////////////////////////////////////////////////////////////////////////////
void setup() {
  pinMode(pin_R, OUTPUT);
  pinMode(pin_G, OUTPUT);
  pinMode(pin_B, OUTPUT);
  pinMode(PinFAN, OUTPUT);


  ledcAttachPin(pin_R, 5);  //Atribuimos o pino 2 ao canal 0.
  ledcSetup(5, 1000, 8);  //Atribuimos ao canal 0 a frequencia de 1000Hz com resolucao de 10bits.
  ledcAttachPin(pin_G, 7);
  ledcSetup(7, 1000, 8);
  ledcAttachPin(pin_B, 6);
  ledcSetup(6, 1000, 8);
  ledcAttachPin(PinFAN, 3);
  ledcSetup(3, 1000, 8);

  digitalWrite(PinFAN, LOW);
  digitalWrite(pin_R, LOW);
  digitalWrite(pin_G, LOW);
  digitalWrite(pin_B, LOW);

  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }


  ArduinoOTA.setHostname("Controlador Geral");
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


  FastLED.addLeds<LED_TYPE, LED_PIN_ram1, COLOR_ORDER>(ledsRam1, NUM_LEDS_ram1).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESSMAX );
  FastLED.addLeds<LED_TYPE, LED_PIN_ram2, COLOR_ORDER>(ledsRam2, NUM_LEDS_ram2).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESSMAX );
  FastLED.addLeds<LED_TYPE, LED_PIN_1070, COLOR_ORDER>(leds1070, NUM_LEDS_1070).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESSMAX );
  FastLED.addLeds<LED_TYPE, LED_PIN_cooler3, COLOR_ORDER>(ledsCooler3, NUM_LEDS_cooler3).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESSMAX );
  FastLED.addLeds<LED_TYPE, LED_PIN_cooler2, COLOR_ORDER>(ledsCooler2, NUM_LEDS_cooler2).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESSMAX );
  FastLED.addLeds<LED_TYPE, LED_PIN_cooler1, COLOR_ORDER>(ledsCooler1, NUM_LEDS_cooler1).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESSMAX );


  fill_solid( ledsCooler1, NUM_LEDS_cooler1, CHSV (i, 255, 0));
  delay(2);
  fill_solid( ledsCooler2, NUM_LEDS_cooler2, CHSV (i, 255, 0));
  delay(2);
  fill_solid( ledsCooler3, NUM_LEDS_cooler3, CHSV (i, 255, 0));
  delay(2);
  fill_solid( leds1070, NUM_LEDS_1070, CHSV (i, 255, 0));
  delay(2);
  fill_solid( ledsRam1, NUM_LEDS_ram1, CHSV (i, 255, 0));
  delay(2);
  fill_solid( ledsRam2, NUM_LEDS_ram2, CHSV (i, 255, 0));
  FastLED.show();
  ledcWrite(5, 1023);
  ledcWrite(7, 1023); 
  ledcWrite(6, 1023);

  if (esp_now_init() != 0)
    Serial.println("ESPNow Init Failed");
  int slavesCount = 1;

  esp_now_peer_info_t slave;
  slave.channel = CHANNEL;
  slave.encrypt = 0;
  memcpy(slave.peer_addr, macBroadCast, sizeof(macBroadCast));
  esp_now_add_peer(&slave);

  esp_now_register_send_cb(OnDataSent);
  //send();
  uint8_t broadcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_err_t result = esp_now_send(broadcast, (uint8_t*) &rgb, sizeof(rgb));
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  delay(1);
  WifiVerify();
  ArduinoOTA.handle();
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
    WiFi.begin(ssid, password);

  if (onoff && BRIGHTNESSMAX != 0) {
    if (Mode[1][2] || Mode[2][2] || Mode[3][2] || Mode[4][2] || Mode[5][2] || Mode[6][2] )
      MudaCorSolido();
    else if (Mode[1][1] || Mode[2][1] || Mode[3][1] || Mode[4][1] || Mode[5][1] || Mode[6][1] )
      MudaCorCobra();
  }

}
