void startOTA()
{
  String type;
  if (ArduinoOTA.getCommand() == U_FLASH)
    type = "flash";
  else
    type = "filesystem"; // U_SPIFFS
  Serial.println("Start updating " + type);
}

void endOTA()
{
  Serial.println("\nEnd");
}
void progressOTA(unsigned int progress, unsigned int total)
{
  Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
}
void errorOTA(ota_error_t error)
{
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


void OnDataRecv(uint8_t *mac_addr, byte *data, uint8_t data_len) {

  Serial.println(data[0]);
  Serial.println(data[1]);
  Serial.println(data[2]);

  analogWrite(pin_R, 1023 - 2 * data[0]);
  analogWrite(pin_G, 1023 - 2 * data[1]);
  analogWrite(pin_B, 1023 - 2 * data[2]);
  t = millis();

}
