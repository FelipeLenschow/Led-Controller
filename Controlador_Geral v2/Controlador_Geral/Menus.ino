void MudaCorSolido() {

  T = millis();

  if (T - tempo >= 1000 / US) {
    tempo += 1000 / US;
    y += passo + 1;
  }

  j = y + (T - tempo) / (1000 / US) * (passo + 1);

  getRGB(j , 1023, BRIGHTNESSMAX);

  if (Mode[1][2])
    fill_solid( ledsRam1, NUM_LEDS_ram1, CHSV (j, 255, BRIGHTNESSMAX));
  if (Mode[2][2])
    fill_solid( ledsRam2, NUM_LEDS_ram2, CHSV (j, 255, BRIGHTNESSMAX));
  if (Mode[3][2])
    fill_solid( leds1070, NUM_LEDS_1070 - 2, CHSV (j, 255, BRIGHTNESSMAX));
  if (Mode[4][2])
    fill_solid( ledsCooler1, NUM_LEDS_cooler1, CHSV (j, 255, BRIGHTNESSMAX));
  if (Mode[5][2])
    fill_solid( ledsCooler2, NUM_LEDS_cooler2, CHSV (j, 255, BRIGHTNESSMAX));
  if (Mode[6][2])
    fill_solid( ledsCooler3, NUM_LEDS_cooler3, CHSV (j, 255, BRIGHTNESSMAX));

  FastLED.show();
  ArduinoOTA.handle();

}

void MudaCorCobra() {

  T = millis();

  if (L >= 2 * ((NUM_LEDS_1070 - 2) * NUM_LEDS_cooler1 * NUM_LEDS_ram1))
    L = ((NUM_LEDS_1070 - 2) * NUM_LEDS_cooler1 * NUM_LEDS_ram1);


  j = i + (T - tempo) / (1000 / US) * (passo + 1);
  getRGB(j, 255, BRIGHTNESSMAX);


  if (Mode[4][1]) {
    ledsCooler1[(L + 1) % NUM_LEDS_cooler1] =  CHSV (i + passo + 1, 255, map((T - tempo), 0, 1000 / US, BRIGHTNESSMIN, BRIGHTNESSMAX));
    for (int h = 0; h < rastroCooler; h++) {
      ledsCooler1[(L - h) % NUM_LEDS_cooler1] =  CHSV (i - (h * passo), 255, BRIGHTNESSMAX - (h * (BRIGHTNESSMAX - BRIGHTNESSMIN) / rastroCooler) - map((T - tempo), 0, 1000 / US, 0, (BRIGHTNESSMAX - BRIGHTNESSMIN) /  rastroCooler ));
    }
    ledsCooler1[(L - rastroCooler) % NUM_LEDS_cooler1] =  CHSV (i - (rastroCooler * passo), 255, BRIGHTNESSMIN);
  }


  if (Mode[5][1]) {
    ledsCooler2[(L + 1) % NUM_LEDS_cooler2] =  CHSV (i + passo + 1, 255, map((T - tempo), 0, 1000 / US, BRIGHTNESSMIN, BRIGHTNESSMAX));
    for (int h = 0; h < rastroCooler; h++) {
      ledsCooler2[(L - h) % NUM_LEDS_cooler2] =  CHSV (i - (h * passo), 255, BRIGHTNESSMAX - (h * (BRIGHTNESSMAX - BRIGHTNESSMIN) / rastroCooler) - map((T - tempo), 0, 1000 / US, 0, (BRIGHTNESSMAX - BRIGHTNESSMIN) /  rastroCooler ));
    }
    ledsCooler2[(L - rastroCooler) % NUM_LEDS_cooler2] =  CHSV (i - (rastroCooler * passo), 255, BRIGHTNESSMIN);
  }


  if (Mode[6][1]) {
    ledsCooler3[(L + 1) % NUM_LEDS_cooler3] =  CHSV (i + passo + 1, 255, map((T - tempo), 0, 1000 / US, BRIGHTNESSMIN, BRIGHTNESSMAX));
    for (int h = 0; h < rastroCooler; h++) {
      ledsCooler3[(L - h) % NUM_LEDS_cooler3] =  CHSV (i - (h * passo), 255, BRIGHTNESSMAX - (h * (BRIGHTNESSMAX - BRIGHTNESSMIN) / rastroCooler) - map((T - tempo), 0, 1000 / US, 0, (BRIGHTNESSMAX - BRIGHTNESSMIN) /  rastroCooler ));
    }
    ledsCooler3[(L - rastroCooler) % NUM_LEDS_cooler3] =  CHSV (i - (rastroCooler * passo), 255, BRIGHTNESSMIN);
  }



  if (Mode[3][1]) {
    leds1070[(L + 1) % (NUM_LEDS_1070 - 2)] =  CHSV (i + passo + 1, 255, map((T - tempo), 0, 1000 / US, BRIGHTNESSMIN, BRIGHTNESSMAX));
    for (int h = 0; h < rastro1070; h++) {
      leds1070[(L - h) % (NUM_LEDS_1070 - 2)] =  CHSV (i - (h * passo), 255, BRIGHTNESSMAX - (h * (BRIGHTNESSMAX - BRIGHTNESSMIN) / rastro1070) - map((T - tempo), 0, 1000 / US, 0, (BRIGHTNESSMAX - BRIGHTNESSMIN) /  rastro1070 ));
    }
    leds1070[(L - rastro1070) % (NUM_LEDS_1070 - 2)] =  CHSV (i - (rastro1070 * passo), 255, BRIGHTNESSMIN);
  }


  if (Mode[1][1]) {
    ledsRam1[(L + 1) % NUM_LEDS_ram1] =  CHSV (i + passo + 1, 255, map((T - tempo), 0, 1000 / US, BRIGHTNESSMIN, BRIGHTNESSMAX));
    for (int h = 0; h < rastroRam; h++) {
      ledsRam1[(L - h) % NUM_LEDS_ram1] =  CHSV (i - (h * passo), 255, BRIGHTNESSMAX - (h * (BRIGHTNESSMAX - BRIGHTNESSMIN) / rastroRam) - map((T - tempo), 0, 1000 / US, 0, (BRIGHTNESSMAX - BRIGHTNESSMIN) /  rastroRam ));
    }
    ledsRam1[(L - rastroRam) % NUM_LEDS_ram1] =  CHSV (i - (rastroRam * passo), 255, BRIGHTNESSMIN);
  }


  if (Mode[2][1]) {
    ledsRam2[(L + 1) % NUM_LEDS_ram2] =  CHSV (i + passo + 1, 255, map((T - tempo), 0, 1000 / US, BRIGHTNESSMIN, BRIGHTNESSMAX));
    for (int h = 0; h < rastroRam; h++) {
      ledsRam2[(L - h) % NUM_LEDS_ram2] =  CHSV (i - (h * passo), 255, BRIGHTNESSMAX - (h * (BRIGHTNESSMAX - BRIGHTNESSMIN) / rastroRam) - map((T - tempo), 0, 1000 / US, 0, (BRIGHTNESSMAX - BRIGHTNESSMIN) /  rastroRam ));
    }
    ledsRam2[(L - rastroRam) % NUM_LEDS_ram2] =  CHSV (i - (rastroRam * passo), 255, BRIGHTNESSMIN);
  }


  if (T - tempo >= 1000 / US) {
    L++;
    i += passo + 1;
    if (Serial)
      Serial.println(i);

    tempo += 1000 / US;
  }

  leds1070[8] = CHSV (5, 255, 0);
  leds1070[7] = CHSV (5, 255, 0);
  //  leds1070[8] = CRGB::Black;
  FastLED.show();
  ArduinoOTA.handle();

}
