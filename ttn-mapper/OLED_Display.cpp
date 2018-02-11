/*
 * OLED_Display.cpp - Helper class for Adafruit OLED Featherwing
 * 
 * The OLED FeatherWing is a 128x32 pixels display
 * Characters are 6x8 pixels -- 4 lines of 21 chars
 * 
 */

#include "OLED_Display.h"

// Reads battery voltage
void OLED_Display::_readBatteryVotlatge( void )
{
  _battery = analogRead(battery_pin);
  _battery *= 2;    // we divided by 2, so multiply back
  _battery *= 3.3;  // Multiply by 3.3V, our reference voltage
  _battery /= 1024; // convert to voltage
}

// Converts an "os time" to a String: "hh:mm:ss"
String OLED_Display::hms(ostime_t t) {
  s4_t h, m, s;
  String txt;

  s = osticks2ms(t) / 1000;
  m = s / 60;
  s -= m * 60;
  h = m / 60;
  m -= h * 60;

  txt = String(h);
  txt += String(m < 10 ? ":0" : ":");
  txt += String(m);
  txt += String(s < 10 ? ":0" : ":");
  txt += String (s);

  return txt;
}

// Returns the uptime in seconds
s4_t OLED_Display::_uptime( void )
{
  return osticks2ms(os_getTime()) / 1000;
}

// Initialize the display (show TTN splash screen)
void OLED_Display::init (void)
{
  // initialize with the I2C addr 0x3C (for the 128x32)
  begin(SSD1306_SWITCHCAPVCC, 0x3C);
  setTextSize(1);
  setTextColor(WHITE);
  for ( int8_t i = -30; i <= 0; i += 2) {
    clearDisplay(); 
    drawBitmap(0, i, ttn_logo, ttn_logo_w, ttn_logo_h, WHITE);
    display();
  }
  setCursor(128 - 3 * oled_char_w - 3 * oled_char_w / 2, 5);
  print("TTN");
  setCursor(128 - 6 * oled_char_w, 2 * 5 + oled_char_h);
  print("Mapper");
  display();
}

// Clears single line of display
void OLED_Display::clearLine ( uint8_t line)
{
  fillRect(0, oled_char_h * (line - 1), oled_w, oled_char_h, BLACK);
  setCursor(0, oled_char_h * (line - 1));
}

// Clears middle lines of display
void OLED_Display::clearText ( void )
{
  fillRect(0, oled_char_h, oled_w, oled_char_h * 2, BLACK);
  setCursor(0, oled_char_h);
}

// Increments sent counter
void OLED_Display::addSent ( void )
{
  _sent++;
}

// Increments error counter
void OLED_Display::addError ( void )
{
  _error++;
}

// Increments Send complete counter
void OLED_Display::addComplete ( void )
{
  _complete++;
}

// Updates top / Bottom status lines
void OLED_Display::update ( void )
{
  s4_t uptime;
  s4_t h, m, s;
  
  clearLine(1);
  print("S:");
  print(_sent);
  setCursor(oled_char_w * 8, 0);
  print(" C:");
  print(_complete);
  setCursor(oled_char_w * 16, 0);
  print(" E:");
  print(_error);
  
  clearLine(4);
  print ("Up ");
  print (hms(os_getTime()));

  setCursor(oled_w - (oled_char_w * 8), oled_char_h * 3);
  print ("Bt ");
  _readBatteryVotlatge();
  print (_battery,2);
  print("V");
  display();
}

// Returns last voltage
float OLED_Display::getBatteryVoltage ( void )
{
  return _battery;
}

