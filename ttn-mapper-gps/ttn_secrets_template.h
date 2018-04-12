/*
 * ttn_secrets.h - Store your keys here!
 *
 */

// Application EUI
// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
#define SECRET_APP_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0xD5, 0xB3, 0x70 }

// Device EUI
// This should also be in little endian format, see above.
// The Adafruit Feather / RFM95 does not have a unique id. To ensure uniqueness
// generate the Device EUI in the TTN console
#define SECRET_DEV_EUI { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

// App key
// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from the TTN console can be copied as-is.
#define SECRET_APP_KEY { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

