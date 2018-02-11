/*******************************************************************************
 * ttn-mapper
 * 
 * Send dummy TTN packets for tracking via ttnmapper.org
 * 
 * This code runs on an Adafruit Feather M0 with LoRa, status displayed on 
 * Adafruit OLED FeatherWing
 *
 *******************************************************************************/

#include <lmic.h>
#include "OLED_Display.h"

// Display
OLED_Display display = OLED_Display();

// Init radio
extern osjob_t init_lora_job;
extern void init_lora (osjob_t* j);

// Update the display every seccond
const uint8_t update_display_interval = 1;
static osjob_t update_display_job;
static void update_display(osjob_t* j)
{
  // Update screen
  display.update();
  // Reschedule
  os_setTimedCallback(&update_display_job, os_getTime() + sec2osticks(update_display_interval), update_display);
}

void setup() {
  // Display splash screen
  display.init();
  delay(1000);
  
  // initialize the scheduler
  os_init();

  // Schedule jobs
  // Display updates
  os_setCallback(&update_display_job, update_display);
  // Initialize radio
  os_setCallback(&init_lora_job, init_lora);  
}

void loop() {
  // Run the scheduler
  os_runloop_once();
}
