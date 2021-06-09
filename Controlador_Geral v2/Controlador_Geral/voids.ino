void startOTA() {
  String type;
  if (ArduinoOTA.getCommand() == U_FLASH)
    type = "flash";
  else
    type = "filesystem"; // U_SPIFFS
  Serial.println("Start updating " + type);

  Broadcast = 0;
}

void endOTA() {
  Serial.println("\nEnd");
}

void progressOTA(unsigned int progress, unsigned int total) {
  Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
}

void errorOTA(ota_error_t error) {
  //Broadcast = 1;
  Serial.printf("Error[%u]: ", error);
  if (error == OTA_AUTH_ERROR)
    Serial.println("Auth Failed");
  else if (error == OTA_BEGIN_ERROR)
    Serial.println("Begin Failed");
  else if (error == OTA_CONNECT_ERROR)
    Serial.println("Connect Failed");
  else if (error == OTA_RECEIVE_ERROR)
    Serial.println("Receive Failed");
  else if (error == OTA_END_ERROR)
    Serial.println("End Failed");
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

  if (t) {
    ExtrairInfo();
  }
}

void ExtrairInfo() {

  if (currentLine.substring(5, 7) == "ON") {
    tempo = millis();
    onoff = 1;
  }

  else if (currentLine.substring(5, 8) == "OFF") {
    onoff = 0;
    fill_solid( ledsCooler1, NUM_LEDS_cooler1, CHSV (i, 255, 0));
    fill_solid( ledsCooler2, NUM_LEDS_cooler2, CHSV (i, 255, 0));
    fill_solid( ledsCooler3, NUM_LEDS_cooler3, CHSV (i, 255, 0));
    fill_solid( leds1070, NUM_LEDS_1070, CHSV (i, 255, 0));
    fill_solid( ledsRam1, NUM_LEDS_ram1, CHSV (i, 255, 0));
    fill_solid( ledsRam2, NUM_LEDS_ram2, CHSV (i, 255, 0));
    getRGB(0, 0, 0);
    FastLED.show();
  }

  else if (currentLine.substring(5, 6) == "P") {
    passo = ExtrairValor(7, 9);
  }

  else if (currentLine.substring(5, 6) == "T") {
    tempo = millis();
    onoff = 1;
    getRGB(i - 256 , 255, BRIGHTNESSMAX);
    fill_solid( ledsCooler1, NUM_LEDS_cooler1, CHSV (i, 255, BRIGHTNESSMIN));
    fill_solid( ledsCooler2, NUM_LEDS_cooler2, CHSV (i, 255, BRIGHTNESSMIN));
    fill_solid( ledsCooler3, NUM_LEDS_cooler3, CHSV (i, 255, BRIGHTNESSMIN));
    fill_solid( leds1070, NUM_LEDS_1070 - 2, CHSV (i, 255, BRIGHTNESSMIN));
    fill_solid( ledsRam1, NUM_LEDS_ram1, CHSV (i, 255, BRIGHTNESSMIN));
    fill_solid( ledsRam2, NUM_LEDS_ram2, CHSV (i, 255, BRIGHTNESSMIN));
    FastLED.show();
    for (int p = 0; p <= 6; p++)
      Desliga(p);
    int valor = ExtrairValor(7, 8);
    for (int p = 0; p <= 6; p++)
      Mode [p] [valor] = HIGH;
  }

  else if (currentLine.substring(5, 7) == "BM") {
    BRIGHTNESSMAX = ExtrairValor(8, 11);
  }

  else if (currentLine.substring(5, 7) == "Bm") {
    BRIGHTNESSMIN = ExtrairValor(8, 11);
  }

  else if (currentLine.substring(5, 7) == "VR") {
    US = ExtrairValor(8, 10);
  }






  else if (ExtrairValor(5, 6))
    Mode[ExtrairValor(5, 6)][ExtrairValor(7, 8)];
  else
    Serial.println("digitou errado ai");


  t = 0;
}

void Desliga (int q) {
  for (int p = 0; p < 5; p++)
    Mode [q] [p] = 0;
}

int ExtrairValor(int DE, int ATE) {
  char Auxiliar[currentLine.substring(DE, ATE).length() + 1]; //determine size of the array
  currentLine.substring(DE, ATE).toCharArray(Auxiliar, sizeof(Auxiliar)); //put readstring into an array
  return atoi(Auxiliar); //convert the array into an Integer;
}

void getRGB(unsigned short int hue, int sat, int value) {

  int auxiliar = value ;
  int base;

  if (sat == 0) { // Acromatic color (gray). Hue doesn't mind.
    rgb[0] = value;
    rgb[1] = value;
    rgb[2] = value;
  } else  {

    base = 0;//((255 - sat) * value) >> 8;


    if (0 <= hue && hue <= 32) {
      rgb[0] = map(hue, 0, 32, value, auxiliar);
      rgb[1] = map(hue, 0, 96, base, value);
      rgb[2] = base;
    } else {
      if (32 < hue && hue <= 64) {
        rgb[0] = 4 * 170;
        rgb[1] = map(hue, 0, 96, base, value);
        rgb[2] = base;
      } else {
        if (64 < hue && hue <= 96) {
          rgb[0] = map(hue, 64, 96, auxiliar, base);
          rgb[1] = map(hue, 0, 96, base, value);
          rgb[2] = base;
        } else {
          if (96 < hue && hue <= 128) {
            rgb[0] = base;
            rgb[1] = map(hue, 96, 128, value, auxiliar);
            rgb[2] = map(hue, 96, 128, base, value - auxiliar);
          } else {
            if (128 < hue && hue <= 160) {
              rgb[0] = base;
              rgb[1] = map(hue, 128, 160, auxiliar, base);
              rgb[2] = map(hue, 128, 160, value - auxiliar, value);
            } else {
              if (160 < hue && hue <= 255) {
                rgb[0] = map(hue, 160, 255, base, value);
                rgb[1] = 0;
                rgb[2] = map(hue, 160, 255, value, base);
              }
            }
          }
        }
      }
    }
  }

  ledcWrite(5, 1023 -  4 * rgb[0]); //r
  ledcWrite(7, 1023 - 4 * rgb[1]); //g
  ledcWrite(6, 1023 - 4 * rgb[2]); //b
}

//String httpGETRequest(String serverName) {
//  //Serial.println("Chamando MousePad");
//  HTTPClient http;
//  http.setTimeout(10);
//
//  http.begin(serverName);
//
//  int httpResponseCode = http.GET();
//  String payload = "--";
//  if (Serial) {
//    if (httpResponseCode > 0) {
//      Serial.print("HTTP Response code: ");
//      Serial.println(httpResponseCode);
//      payload = http.getString();
//    }
//  }
//  return payload;
//}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if (Broadcast ) {
    Serial.println("broadcasting");

    uint8_t broadcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
    esp_err_t result = esp_now_send(broadcast, (uint8_t*) &rgb, sizeof(rgb));

    //    Serial.print("Status: ");
    //    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Success" : "Fail");

  }
}
