/*
 * ttn_mapper.cpp - Handles the radio
 * 
 * The lmic/config.h is by default configured for the RFM95 radio
 * and the EU868 frequency plan. For the US915 frequency plan you need 
 * to update that file.
 * 
 * Important: see "Pin mapping" below
 * 
 */

#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include "OLED_Display.h"
#include "ttn_secrets.h"

// Display handler
extern OLED_Display display;

// Send packet interval (in seconds) -- respect duty cycle!
const uint8_t send_packet_interval = 60;

// LoRaWan keys
static const u1_t app_eui[8]  = SECRET_APP_EUI;
static const u1_t dev_eui[8]  = SECRET_DEV_EUI;
static const u1_t app_key[16] = SECRET_APP_KEY;

// Getters for LMIC
void os_getArtEui (u1_t* buf) 
{
  memcpy(buf, app_eui, 8);
}

void os_getDevEui (u1_t* buf)
{
  memcpy(buf, dev_eui, 8);
}

void os_getDevKey (u1_t* buf)
{
  memcpy(buf, app_key, 16);
}

// Pin mapping
// The Feather M0 LoRa does not map RFM95 DIO1 to an M0 port. LMIC needs this signal 
// in LoRa mode, so you need to bridge IO1 to an available port -- I have bridged it to 
// Digital pin #10
// We do not need DIO2 for LoRa communication.
const lmic_pinmap lmic_pins = {
    .nss = 8,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 4,
    .dio = {3, 10, LMIC_UNUSED_PIN},
};

// Init job -- Actual message message loop will be initiated when join completes
osjob_t init_lora_job;
void init_lora (osjob_t* j)
{
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();
  // start joining
  LMIC_startJoining();
}


// Send job
static osjob_t send_packet_job;
static void send_packet(osjob_t* j)
{
  static uint8_t message[] = "*";
  
  display.clearText();
  display.print(display.hms(os_getTime()));
    
  // Check if there is not a current TX/RX job running   
  if (LMIC.opmode & OP_TXRXPEND) {
    display.println("Prv not sent");
    display.print("Waiting...");
    display.addError();
  } else {
    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, message, sizeof(message)-1, 0);
    display.println(": Sending msg");
    display.addSent();
  }
  display.display();

  // Reschedule
  os_setTimedCallback(&send_packet_job, os_getTime()+sec2osticks(send_packet_interval), send_packet);
}

// LoRa event handler
// We look at more events than needed, to track potential issues
void onEvent (ev_t ev) {
    display.clearText();
    display.print(display.hms(os_getTime()));
    display.print(": Evt ");

    switch(ev) {
        case EV_SCAN_TIMEOUT:
            display.println("Scan Timeout");
            break;
        case EV_BEACON_FOUND:
            display.println("Beacon Found");
            break;
        case EV_BEACON_MISSED:
            display.println("Beacon Missed");
            break;
        case EV_BEACON_TRACKED:
            display.println("Beacon Tracked");
            break;
        case EV_JOINING:
            display.println("Joining");
            break;
        case EV_JOINED:
            display.println("Joined");
            // Disable link check validation (automatically enabled
            // during join, but not supported by TTN at this time).
            LMIC_setLinkCheckMode(0);
            // Start sending packets
            os_setCallback(&send_packet_job, send_packet);
            break;
        case EV_RFU1:
            display.println("RFU1");
            break;
        case EV_JOIN_FAILED:
            display.println("Join Failed");
            // Re-try!
            os_setCallback(&init_lora_job, init_lora);
            break;
        case EV_REJOIN_FAILED:
            display.println("Rejoin Failed");
            break;
        case EV_TXCOMPLETE:
            display.addComplete();
            display.print("TX Complete");
            if (LMIC.txrxFlags & TXRX_ACK) {
              display.println(" - Received ack");
            }
            // We could re-schedule from here, but it would break the loop if a TX never completes...
            // os_setTimedCallback(&send_packet_job, os_getTime()+sec2osticks(send_packet_interval), send_packet);
            break;
        case EV_LOST_TSYNC:
            display.println("Lost Sync");
            break;
        case EV_RESET:
            display.println("Reset");
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            display.println("RX Complete");
            break;
        case EV_LINK_DEAD:
            display.println("Link Dead");
            break;
        case EV_LINK_ALIVE:
            display.println("Link Alive");
            break;
         default:
            display.println("Unknown event");
            break;
    }
    display.display();
}

