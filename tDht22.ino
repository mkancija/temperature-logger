



/**
 * DHT22 READ
 */
void dht22_read()
{
  String tmp = "";
  char tmp_c[15];
  char tmp_t[15];

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings take up to 2 seconds wtf!!! (old slow sensor) - Kupi zadnju verziju!
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit
  //float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  //if (isnan(h) || isnan(t) || isnan(f)) {
  if (isnan(h)) {
    //failed!!
    if (DEBUG) {
      Serial.println(String("Dht22 fail!"));
    }
    return;
  }

  // Compute heat index
  // Must send in temp in Fahrenheit!
  //float hi  = dht.computeHeatIndex(f, h);
  //float hic = (hi-32) * 0.55555;

  dtostrf(h, 7, 3, tmp_c);
  humid_value = String(tmp_c);

  dtostrf(t, 7, 3, tmp_t);
  temp_value  = String(tmp_t);

  // global
  temp_value.trim();
  humid_value.trim();
  dht_t1 = temp_value;
  dht_h1 = humid_value;

  if (DEBUG) {
    Serial.println(String("humidity:")+humid_value+String(", temp:")+temp_value);
  }

}
