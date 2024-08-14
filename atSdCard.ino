
String logFile = "";

/**
 *
 *
 */
void sdInit()
{
  // testiraj je li SD kartica prisutna i komunikacija je moguća.
  if (!SD.begin(SD_CS)) {
    Serial.println("SD CARD greska!");
    // zaustaviti izvodenje?
    // return;
  }
  else {
    Serial.println("SD CARD OK!");
  }
}

void sdConfig()
{
  readSDSettings();

  String base = cfgGetFileName();
  String ext  = "TXT";
  String pfix = timeLogName();

  // logFile = pfix+String("")+base+String(".")+ext;
  logFile = pfix+String(".")+ext;

  Serial.println("Log datoteka pripremljena:"+String(logFile));
  // npr. 2018-01-06_tlogger.txt

  File file = SD.open(logFile, FILE_WRITE);
       file.println("TLOGGER restarted.");
  file.close();
}
/**
 * pisanje na SD karticu
 *
 * https://www.youtube.com/watch?v=Caw5PiIFk-w
 *
 * SD CARD SETTINGS
 * MISO - ARDUINO 12
 * SCK  - ARDUINO 13
 * MOSI - ARDUINO 11
 * CS   - ARDUINO 10
 *
 * ATMEGA2560 pins
 *
 * int TFTsclk = 52;
 * int TFTmosi = 51;
 * int TFTmiso = 50;
 * int TFTdc   = 49;
 * int TFTrst  = 48;
 * int TFTcs   = 53;
 */
void sd_write()
{
  // otvaranje datoteke na kartici.
  // char * fname = "temp.txt";
  // file = SD.open(fname, FILE_WRITE);
  File file = SD.open(logFile, FILE_WRITE);

  if (file) {

    digitalWrite(13, HIGH);  // LED ON

    temp_value.replace(".", ",");
    humid_value.replace(".", ",");
    bmp_temp_value.replace(".", ",");
    bmp_pressure_value.replace(".", ",");
    bmp_alt_value.replace(".", ",");

    file.println(String(time_value) + String(";") + String(humid_value) + String("; ") + bmp_temp_value + String("; ")
      + bmp_pressure_value + String("; ") + bmp_alt_value + String("; ") + temp_value);

    file.close();    // close the file: (ako ne zatvorim, ne mogu otvoriti drugu)
  }
  else {
    if (DEBUG) {
      Serial.println(String("greska, ne mogu otvoriti datoteku:"+logFile));

      /*
        Serial.println(String("pokusavam reset SD kartice..."));

        pinMode(SD_CS, OUTPUT);
        digitalWrite(SD_CS, LOW);
        delay(50);
        digitalWrite(SD_CS, HIGH);

        //----------SD CARD----------
        // see if the card is present and can be initialized:
        if (!SD.begin(SD_CS)) {
          Serial.println("SD CARD greska!");
          // zaustaviti izvodenje?
          // return;
        }
        else {
          Serial.println("SD CARD OK!");
        }
        */
    }
    // file.close();    // close the file: (ako ne zatvorim, ne mogu otvoriti drugu)
    // greska, ako ne mogu otvoriti datoteku.
  }

  digitalWrite(13, LOW);  // LED ON
  //file.close();    // close the file: (ako ne zatvorim, ne mogu otvoriti drugu)
}

/**
* DEPRECATED
*/
void ____sd_read()
{
  //char * fname = "temp.txt";
  File file = SD.open(logFile, FILE_READ);

  long byte_start = 0;
  byte outbyte;

  digitalWrite(13, HIGH);  // LED ON

  // if the file is available, write to it:
  if (file) {

    Serial1.println("saljem datoteku...");
    Serial1.println("velicina datoteke: "+String(file.size()));

    // read from the file until there's nothing else in it:
    while (file.available()) {
      Serial1.write(file.read());
    }

/*
    // send data one byte at a time
    outbyte = file.read();
    while(outbyte>-1)
    {
      Serial.write(outbyte);
      outbyte = file.read();
    }
  */
      file.close();

      Serial1.println("datoteka poslana.");
  }
  // if the file isn't open, pop up an error:
  else {
    Serial1.println("greska - ne mogu procitati datoteku");
  }
  file.close();

  digitalWrite(13, LOW);  // LED OFF
}
