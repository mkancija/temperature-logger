// VARIABLES
// *********************************************
uint16_t startAddr = 0x0000;            // Start address to store in the NV-RAM
uint16_t lastAddr;                      // new address for storing in NV-RAM
uint16_t TimeIsSet = 0xaa55;            // Helper that time must not set again



/**
 * format time - mysql style
 *
 * @return string
 */
String timeLogName()
{

  char outstr[15];
  String tmp = "";

  RTC.getTime();

  // dohvat godine
  tmp += String(RTC.year, DEC);
  tmp += ""; 

  // dohvat mjeseca
  if (RTC.month < 10)                   // correct month if necessary
  {
    tmp += '0'; 
  }
  tmp += String(RTC.month, DEC);
  tmp += ""; 

  // dohvat dana
  if (RTC.day < 10)                   // correct month if necessary
  {
    tmp += '0'; 
  }
  tmp += String(RTC.day, DEC);

  return tmp;
}

/**
 * 
 * 
 */
void timeInit()
{
  
  pinMode(2, INPUT);                    // Test of the SQW pin, D2 = INPUT
  digitalWrite(2, HIGH);                // Test of the SQW pin, D2 = Pullup on
  //Serial.begin(9600);

  bool repTime = false;
  
  RTC.getRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));

  // RTC ram status:43605. 2018-01-06 - OK!
  // ---
  // RTC ram status:37179. 2099-15-31 - greška

  String tempRam = String(TimeIsSet);
  Serial.println("RTC ram status:"+tempRam+".");

  // Ako postoji greška u nvramu rtc-a, resetiram vrijeme na zadnje poznato.
  if (tempRam == "37179") {  // greška s vremenom.
    repTime = true;
  }
  else if (tempRam.toInt() < 10000) {
    repTime = true;
  }

  if (repTime == true) {

    Serial.println("RTC greska, popravljam vrijeme: 2018-01-06 10:10:10.");    
    repTime = false;
    delay(100);
    
    RTC.stopClock();
        
    RTC.fillByYMD(2018,1,6);
    RTC.fillByHMS(10,10,10);
    
    RTC.setTime();
    TimeIsSet = 0xaa55;
    RTC.setRAM(54, (uint8_t *)&TimeIsSet, sizeof(uint16_t));
    RTC.startClock();
  }

  RTC.getTime();
  Serial.println("RTC inicijaliziran.");
}

