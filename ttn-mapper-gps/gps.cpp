/*
 * gps.cpp - Handles Adafruit GPS FeatherWing
 *
 * The following libraries are used:
 *  Adafruit_GPS - https://github.com/adafruit/Adafruit_GPS
 *  Adafruit_ZeroTimer - https://github.com/adafruit/Adafruit_ZeroTimer
 *  Adafruit_ASFcore - https://github.com/adafruit/Adafruit_ASFcore
 *
 */

#include <Adafruit_ZeroTimer.h>
#include "gps.h"

// Connect to the GPS
Adafruit_GPS GPS(&GPSSerial);

// Timer for reading GPS data
Adafruit_ZeroTimer timer = Adafruit_ZeroTimer(timer_number);

// Timer interrupt handler
void timer_callback(struct tc_module *const module_inst)
{
  GPS.read();
}

// GPS setup
void gps_init()
{
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's
  GPS.begin(9600);

  // Only interrested in GGA, no antenna status
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PGCMD_NOANTENNA);

  // Update every second
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate

  // Configure timer
  timer.configure(TC_CLOCK_PRESCALER_DIV16,    // prescaler
                  TC_COUNTER_SIZE_16BIT,       // bit width of timer
                  TC_WAVE_GENERATION_MATCH_PWM // frequency or PWM mode
                 );

  // Set timer period.
  // Transmission speed is 9600bps, wich is about 1byte/ms
  // (The GPS module isn't sending that fast, we can reliably read data with
  // 5 ms polling!)
  // With DIV16 prescaler, 1 ms is F_CPU/16000
  timer.setPeriodMatch(F_CPU/16000, 1, 0);
  timer.setCallback(true, TC_CALLBACK_CC_CHANNEL0, timer_callback);
  // The interrupt handler does not seem to affect radio (LMIC) operation.
  // If it would, we could disable the timer during transmission.
  timer.enable(true);
}

