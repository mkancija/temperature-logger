/**
 *
 */
// #include <LiquidCrystal.h>
#include <OneWire.h>
#include <Wire.h>                       // For some strange reasons, Wire.h must be included here
#include <DallasTemperature.h>
#include <SD.h>
#include <DS1307new.h>
#include "DHT.h"

// Debug - serial out
boolean DEBUG = true;
boolean lcdActive = false;

//---DHT22
#define DHTPIN 9     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// Generalne definicije varijabli
File file;
int time_count = 0;
int min_count  = 0;
String time_value = "";
String humid_value = "";
char bt_in; // bluetooth variable...

   temp_value = "";
String data_string = "";

String bmp_temp_value = "";
String bmp_pressure_value = "";
String bmp_alt_value = "";

int time_limit = 0;
int time_second = -1;

unsigned long time_milis_curr = millis();
unsigned long time_milis_prev = millis();
const long    time_interval   = 20000; // interval at which i send data (milliseconds)

// Data wire DS18b20 pin
#define ONE_WIRE_BUS 8
#define TEMPERATURE_PRECISION 9

DeviceAddress Probe01 = {
  //0x28, 0xFF, 0x9A, 0x02, 0x64, 0x04, 0x00, 0x45
  //0x28, 0xFF, 0xB6, 0x08, 0x65, 0x04, 0x00, 0x2C
  0x28, 0xFF, 0xC8, 0x07, 0x63, 0x04, 0x00, 0xA8

};
DeviceAddress Probe02 = {
  //0x28, 0xFF, 0x56, 0x02, 0x64, 0x04, 0x00, 0x46
  //0x28, 0xFF, 0x6F, 0x0B, 0x65, 0x04, 0x00, 0x09
  0x28, 0xFF, 0x2E, 0x02, 0x64, 0x04, 0x00, 0x15

};
DeviceAddress Probe03 = {
  //0x28, 0xFF, 0x3D, 0x0A, 0x65, 0x04, 0x00, 0x90
  //0x28, 0xFF, 0x7A, 0x09, 0x63, 0x04, 0x00, 0x71
  //0x28, 0xFF, 0x87, 0xC7, 0x21, 0x17, 0x04, 0xB1 //petar1
  0x28, 0xFF, 0x5A, 0xD1, 0x21, 0x17, 0x04, 0x32   // petar2



};

 float t18_t1 = 0;
 float t18_t2 = 0;
 float t18_t3 = 0;
 float t18_t4 = 0;
 float t18_t5 = -127;
 float t18_t6 = -127;

 String bmp_t1 = "0";
 String bmp_p1 = "0";
 String dht_t1 = "0";
 String dht_h1 = "0";


//gorice
// s1: 0x28, 0xFF, 0xB6, 0x08, 0x65, 0x04, 0x00, 0x2C
// s2: 0x28, 0xFF, 0x6F, 0x0B, 0x65, 0x04, 0x00, 0x09
// s3: 0x28, 0xFF, 0x7A, 0x09, 0x63, 0x04, 0x00, 0x71



// CS pin
const int SD_CS = 53;

//---BMP085
#define BMP085_ADDRESS 0x77   // I2C address of BMP085
const unsigned char OSS = 0;  // Oversampling Setting

// Calibration values
int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;

long b5;
short temperature;
long  pressure;
float altitude;

// Use these for altitude conversions
const float p0 = 101325;     // Pressure at sea level (Pa)

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

void dht22_read();
void timeInit();
void sdInit();
void ethernetInit();

/**
 * format time.
 *
 * @return string time
 */
String time()
{

  char outstr[15];
  String tmp = "";

  RTC.getTime();

  // dohvat godine
  tmp += String(RTC.year, DEC);
  tmp += '-';

  // dohvat mjeseca
  if (RTC.month < 10)                   // correct month if necessary
  {
    tmp += '0';
  }
  tmp += String(RTC.month, DEC);
  tmp += '-';

  // dohvat dana
  if (RTC.day < 10)                   // correct month if necessary
  {
    tmp += '0';
  }
  tmp += String(RTC.day, DEC);
  tmp += ' ';

  // dohvat sata
  if (RTC.hour < 10)                    // correct hour if necessary
  {
    tmp += '0';
  }
  tmp += String(RTC.hour, DEC);
  tmp += ":";

  // dohvat minute
  if (RTC.minute < 10)                  // correct minute if necessary
  {
    tmp += '0';
  }
  tmp += String(RTC.minute, DEC);
  tmp += ":";

  // dohvat sekunde
  if (RTC.second < 10)                  // correct sec if necessary
  {
    tmp += '0';
  }
  tmp += String(RTC.second, DEC);
  tmp += "";
  time_second = (RTC.second);

  time_value = tmp;

  return tmp;
}



// Stores all of the bmp085's calibration values
void bmp085Calibration()
{
  ac1 = bmp085ReadInt(0xAA);
  ac2 = bmp085ReadInt(0xAC);
  ac3 = bmp085ReadInt(0xAE);
  ac4 = bmp085ReadInt(0xB0);
  ac5 = bmp085ReadInt(0xB2);
  ac6 = bmp085ReadInt(0xB4);
  b1  = bmp085ReadInt(0xB6);
  b2  = bmp085ReadInt(0xB8);
  mb  = bmp085ReadInt(0xBA);
  mc  = bmp085ReadInt(0xBC);
  md  = bmp085ReadInt(0xBE);
}

// Read the uncompensated temperature value
unsigned int bmp085ReadUT()
{
  unsigned int ut;

  // Write 0x2E into Register 0xF4
  // This requests a temperature reading
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x2E);
  Wire.endTransmission();

  // Wait at least 4.5ms
  delay(5);

  // Read two bytes from registers 0xF6 and 0xF7
  ut = bmp085ReadInt(0xF6);
  return ut;
}

// Read 1 byte from the BMP085 at 'address'
char bmp085Read(unsigned char address)
{
  unsigned char data;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 1);
  while(!Wire.available());

  return Wire.read();
}

// Read 2 bytes from the BMP085
// First byte will be from 'address'
// Second byte will be from 'address'+1
int bmp085ReadInt(unsigned char address)
{
  unsigned char msb, lsb;

  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(BMP085_ADDRESS, 2);
  while(Wire.available()<2);
  msb = Wire.read();
  lsb = Wire.read();

  return (int) msb<<8 | lsb;
}

// Read the uncompensated pressure value
unsigned long bmp085ReadUP()
{
  unsigned char msb, lsb, xlsb;
  unsigned long up = 0;

  // Write 0x34+(OSS<<6) into register 0xF4
  // Request a pressure reading w/ oversampling setting
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF4);
  Wire.write(0x34 + (OSS<<6));
  Wire.endTransmission();

  // Wait for conversion, delay time dependent on OSS
  delay(2 + (3<<OSS));

  // Read register 0xF6 (MSB), 0xF7 (LSB), and 0xF8 (XLSB)
  Wire.beginTransmission(BMP085_ADDRESS);
  Wire.write(0xF6);
  Wire.endTransmission();
  Wire.requestFrom(BMP085_ADDRESS, 3);

  // Wait for data to become available
  while(Wire.available() < 3);

  msb = Wire.read();
  lsb = Wire.read();
  xlsb = Wire.read();

  up = (((unsigned long) msb << 16) | ((unsigned long) lsb << 8) | (unsigned long) xlsb) >> (8-OSS);

  return up;
}

/**
 * Calculate temperature given ut.
 * Value returned will be in units of 0.1 deg C
 */
short bmp085GetTemperature(unsigned int ut)
{
  long x1, x2;

  x1 = (((long)ut - (long)ac6)*(long)ac5) >> 15;
  x2 = ((long)mc << 11)/(x1 + md);
  b5 = x1 + x2;

  return (b5 + 8)>>4;
}

// Calculate pressure given up
// calibration values must be known
// b5 is also required so bmp085GetTemperature(...) must be called first.
// Value returned will be pressure in units of Pa.
long bmp085GetPressure(unsigned long up)
{
  long x1, x2, x3, b3, b6, p;
  unsigned long b4, b7;

  b6 = b5 - 4000;
  // Calculate B3
  x1 = (b2 * (b6 * b6)>>12)>>11;
  x2 = (ac2 * b6)>>11;
  x3 = x1 + x2;
  b3 = (((((long)ac1)*4 + x3)<<OSS) + 2)>>2;

  // Calculate B4
  x1 = (ac3 * b6)>>13;
  x2 = (b1 * ((b6 * b6)>>12))>>16;
  x3 = ((x1 + x2) + 2)>>2;
  b4 = (ac4 * (unsigned long)(x3 + 32768))>>15;

  b7 = ((unsigned long)(up - b3) * (50000>>OSS));
  if (b7 < 0x80000000)
    p = (b7<<1)/b4;
  else
    p = (b7/b4)<<1;

  x1 = (p>>8) * (p>>8);
  x1 = (x1 * 3038)>>16;
  x2 = (-7357 * p)>>16;
  p += (x1 + x2 + 3791)>>4;

  return p;
}

/**
 * BMP085 READ
 */
void bmp085_read()
{
  temperature = bmp085GetTemperature(bmp085ReadUT());
  pressure = bmp085GetPressure(bmp085ReadUP());
  altitude = (float)44330 * (1 - pow(((float) pressure/p0), 0.190295));

  bmp_temp_value = String((float(temperature)/10), 2);
  bmp_pressure_value = String((float(pressure)/100), 2);
  bmp_alt_value = String(altitude, 2);

  bmp_t1 = bmp_temp_value;
  bmp_p1 = bmp_pressure_value;

  if (DEBUG) {
    Serial.println(String("BMP085 Temperature: ")+bmp_temp_value);
    Serial.println(String("BMP085 Pressure: ")+bmp_pressure_value);
    Serial.println(String("BMP085 Altitude: ")+bmp_alt_value);
  }
}


/**
*
*/
void combine_data_val()
{

  temp_value.replace(".", ",");
  humid_value.replace(".", ",");
  bmp_temp_value.replace(".", ",");
  bmp_pressure_value.replace(".", ",");
  bmp_alt_value.replace(".", ",");

  data_string = String(time_value) + String(";") + String(humid_value) + String("; ") + bmp_temp_value + String("; ")
  + bmp_pressure_value + String("; ") + bmp_alt_value + String("; ") + temp_value;


}


/**
 *
 */
void combine_temp_values()
{

  sensors.requestTemperatures();

  t18_t1 = sensors.getTempC(Probe01);
  t18_t2 = sensors.getTempC(Probe02);
  t18_t3 = sensors.getTempC(Probe03);

  // Grab a count of devices on the wire
  int nd = sensors.getDeviceCount();
  int c = 0;


  while (t18_t1 < -125 && c <= 10) {
    Serial.print("R1["+String(c)+"]:"+String(t18_t1)+",");
    delay(500);
    sensors.requestTemperatures();
    t18_t1 = sensors.getTempC(Probe01);
    c++;
    if (c >= 10){t18_t1=-124;}
  }
  c = 0;

  while (t18_t2 < -125 && c <= 10) {
    Serial.print("R2["+String(c)+"]:"+String(t18_t2)+",");
    delay(500);
    sensors.requestTemperatures();
    t18_t2 = sensors.getTempC(Probe02);
    c++;
    if (c >= 10){t18_t2=-124;}
  }
  c = 0;


  while (t18_t3 < -125 && c <= 10) {
    Serial.print("R3["+String(c)+"]:"+String(t18_t3)+",");
    delay(500);
    sensors.requestTemperatures();
    t18_t3 = sensors.getTempC(Probe03);
    c++;
    if (c >= 10){t18_t3=-124;}
  }
  c = 0;


  if (DEBUG) {
    float tempZ = sensors.getTempCByIndex(0);
    Serial.println(String("Board temperature: ")+tempZ);
  }

  temp_value = String(t18_t1)+String("; ")+String(t18_t2)+String("; ")+String(t18_t3)+String(";");
  Serial.println(String("Broj senzora:")+String(nd)+String(". "));
  Serial.println(temp_value);
}

/**
*
*/
void bluetoothCommand()
{

  Serial1.println ("Press 1/0 to test Arduino, 2 to send data.");

  //while (!Serial1.available()); // stay here so long as COM port is empty
  bt_in = Serial1.read();         // read next available byte

  // Test if its alive!
  if (bt_in == '0') {
    digitalWrite(30, LOW);  // if it's a 0 (zero) turn LED off
    Serial.println(String("LED 12 OFF"));
  }
  if (bt_in == '1') {

    digitalWrite(30, HIGH); // if it's a 1 (one) turn LED on
    Serial.println(String("LED 12 ON"));

    //bt_in = 0;
    //digitalWrite(30, LOW);  //
  }

  if (bt_in == '2') {

    digitalWrite(30, HIGH); // if it's a 2 (two) turn LED on

    //temp sensors (all)
    combine_temp_values();

    // humitidy
    dht22_read();

    // read pressure sensor
    bmp085_read();

    // kombiniraj vrijednosti u jednu varijablu...
    combine_data_val();

    Serial1.println (String("Arduino time: ")+time_value);
    Serial1.println (String("")+data_string);

    bt_in = 0;
    digitalWrite(30, LOW);  //
  }

}

/**
 * Start function
 */
void setup() {

  // definiraj vrstu lcd-a:
  // lcd.begin(16, 2);
  // lcd.clear();


  Serial.begin(9600);
   while (!Serial) {
   ;  // wait for serial port to connect. Needed for Leonardo only
  }

  // CS pin mora biti output, kod SPI komunikacije, iako nije u upotrebi
  //  pinMode(10, OUTPUT); uno:pin 10, mega: pin 53.
  //  digitalWrite(10, HIGH);

  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  pinMode (53, OUTPUT);
  digitalWrite(SD_CS, HIGH);

  //----------RTC----------
  timeInit();
  //----------SD CARD----------
  sdInit();
  //----------DS18b20----------
  sensors.begin();

  sensors.setResolution(Probe01, 11);
  sensors.setResolution(Probe02, 11);
  sensors.setResolution(Probe03, 11);

  Serial.println("Termometri startani.");

  //----------BMP085----------
  bmp085Calibration();

  Serial.println("Barometar kalibriran.");

  //----------DHT22----------
  dht.begin();
  //dht22_read(); // inicijalno citanje
  Serial.println("DHT22 inicijaliziran.");

  //----------BLUETOOTH------
  //Serial1.begin(9600);

  // led test
  pinMode(13, OUTPUT);
  Serial.println("Bluetooth inicijaliziran.");

  //----SD-CARD-SETTINGS-----
  //Serial.println("Setup postavki...");
  //  sdConfig();
  //Serial.println("Postavke ucitane.");

  //----LCD-INIT-----
  //Serial.print("Setup lcd prikaza...");
  //  lcdInit();
  //Serial.println("ok.");


  //----Ethernet-INIT-----
  Serial.print("Setup Ethernet clienta...");
    ethernetInit();
  Serial.println("ok.");

  Serial.println(String("--------------STARTING------------------"));
}

/**
 *
 */
void loop() {

  int write_flag = false;
  byte curr_sec   = 1;

  int test_led = 13;


  // Posalji zahtjev za dohvat temperatura / Send the command to get temperatures
  // sensors.requestTemperatures();

  // recheck time (global var time_value)
    //time();
    //curr_sec = time_second;

  time_milis_curr = millis();

//  if (time_second == 0 && time_second != curr_sec) {

  // if (time_second % 10 == 0) {
  if (time_milis_curr - time_milis_prev >= time_interval)
  {
    write_flag = true;
    digitalWrite(13, HIGH); // if it's a 1 (one) turn LED on
    digitalWrite(30, LOW); // OFF
  }

  //Serial.println(String("RTC time:")+time_value);

  if (write_flag) {

    write_flag = false;

    if (DEBUG) {
      //Serial.println(String("----------------------------------------"));
      Serial.println(String("RTC time:")+time_value);
      delay(100);
    }

    //temp sensors (all)
    combine_temp_values();

    // humitidy
    dht22_read();

    // read pressure sensor
    bmp085_read();

    // write to SD card.
    // sd_write();
    digitalWrite(13, LOW); // off

    ethernetStart();

    time_milis_prev = millis();
    Serial.println(String("-------------------------------------END"));
  } else {
      digitalWrite(30, HIGH); // OFF
      bluetoothCommand();
  }

}

