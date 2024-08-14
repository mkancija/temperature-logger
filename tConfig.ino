/*
*
*/

String cfgFile = "TLOGGER.CFG";
String cfgLogFile = "tlogger";
String logLastTime = "";

int logCounter = 0;

float exFloat = 1.12345;
boolean exBoolean = true;
long exLong = 2123456789;


void readSDSettings()
{
    char character;
    String settingName;
    String settingValue;
    
    Serial.println("citam config datoteku \""+cfgFile+"\"..."); 
    
    File myFile = SD.open(cfgFile, FILE_READ);
    
    if (myFile) {
        while (myFile.available()) {
            
            character = myFile.read();
            while((myFile.available()) && (character != '[')){
                character = myFile.read();
            }
            character = myFile.read();
            while((myFile.available()) && (character != '=')){
                settingName = settingName + character;
                character = myFile.read();
            }
            character = myFile.read();
            while((myFile.available()) && (character != ']')){
                settingValue = settingValue + character;
                character = myFile.read();
            }
            if(character == ']'){
               
                //Debuuging Printing
                Serial1.println("Name "+String(settingName)+"="+String(settingValue));      

                // Apply the value to the parameter
                applySetting(settingName,settingValue);
                // Reset Strings
                settingName = "";
                settingValue = "";
            }
        }
        
        Serial1.println("citanje postavki zavrseno.");
    } 
    else {
      // if the file didn't open, print an error:
      Serial.println("error opening \""+cfgFile+"\".");
    }
    
    // close the file:
    myFile.close();    
}

String cfgGetFileName()
{
  return cfgLogFile;  
}

/* Apply the value to the parameter by searching for the parameter name
  Using String.toInt(); for Integers
  toFloat(string); for Float
  toBoolean(string); for Boolean
  toLong(string); for Long
*/
void applySetting(String sn, String sv) 
{
    // INT
    if(sn == "logCounter") {
        logCounter = sv.toInt();
    }
    // FLOAT
    else if(sn == "exFloat") {
        //exFloat = toFloat(sv);
    }
    // BOOL
    else if(sn == "exBoolean") {
        //exBoolean = toBoolean(sv);
    }
    // LONG
    else if(sn == "exLong") {
        //exLong = toLong(sv);
    }
    
    // STRING
    else if(sn == "cfgLogFile") {
        cfgLogFile = String(sv);
    }
    else if(sn == "logLastTime") {
        logLastTime = String(sv);
    }
}

// converting string to Float
float toFloat(String settingValue)
{
    char floatbuf[settingValue.length()+1];
    settingValue.toCharArray(floatbuf, sizeof(floatbuf));
    float f = atof(floatbuf);
    return f;
}

long toLong(String settingValue)
{
    char longbuf[settingValue.length()+1];
    settingValue.toCharArray(longbuf, sizeof(longbuf));
    long l = atol(longbuf);
    return l;
}

// Converting String to integer and then to boolean
// 1 = true
// 0 = false
boolean toBoolean(String settingValue) 
{
    if(settingValue.toInt()==1){
        return true;
    } else {
        return false;
    }
}

// Writes A Configuration file
void writeSDSettings() {
    
    // Delete the old One
    SD.remove(cfgFile);
    // Create new one
    File myFile = SD.open(cfgFile, FILE_WRITE);
    
      // writing in the file works just like regular print()/println() function
      myFile.print("[");
      myFile.print("cfgLogFile=");
      myFile.print(cfgLogFile);
      myFile.println("]");
      
      myFile.print("[");
      myFile.print("logCounter=");
      myFile.print(logCounter);
      myFile.println("]");
      
      //myFile.print("[");
      //myFile.print("logCounter=");
      //myFile.print(exFloat,5);
      //myFile.println("]");

      
    // close the file:
    myFile.close();
    //Serial.println("Writing done.");
}
