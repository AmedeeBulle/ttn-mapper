/*
 * gps.h - Handles Adafruit GPS FeatherWing
 *
 */

#ifndef _gps_h
#define _gps_h

#include <Adafruit_GPS.h>

// We are only interrested in GGA messages
#define PMTK_SET_NMEA_OUTPUT_GGA "$PMTK314,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"

// GPS serial port
#define GPSSerial Serial1

extern Adafruit_GPS GPS;
extern void gps_init();

// Timer handling
const uint8_t timer_number = 3;

#endif /* _gps_h */

