#include "LiquidCrystal.h"

// boolean lcdActive=false // defined in tLogger.ino
// LiquidCrystal lcd(8,9,4,5,6,7); // initialize the library by providing the nuber of pins to it

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// https://www.allaboutcircuits.com/projects/interface-an-lcd-with-an-arduino/


void lcdInit() {

  lcd.begin(16,2);

  // Turn off the display:
  lcd.noDisplay();
  delay(500);
  // Turn on the display:
  lcd.display();
  delay(500);

  // set cursor position to start of first line on the LCD
  lcd.setCursor(0,0);
  //text to print
  lcd.print("  temp logger  ");
  // set cusor position to start of next line
  lcd.setCursor(0,1);
  lcd.print("starting...");

  lcdActive = true;
}
