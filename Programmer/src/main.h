/*
  Arduino Uno connections:

  See nRF24LE1 Product Specification for corresponding pin numbers.

  NOTE: nRF24LE1 is a 3.3V device.  Level converters are required to connect it to a
  5V Arduino.

 * D00: Serial RX
 * D01: Serial TX
 * D02:
 *~D03:
 * D04:
 *~D05:
 *~D06:
 * D07: nRF24LE1 UART/RXD
 * D08: nRF24LE1 PROG
 *~D09: nRF24LE1 _RESET_
 *~D10: nRF24LE1 FCSN, nRF24LE1 UART/TXD
 *~D11: SPI MOSI, nRF24LE1 FMOSI
 * D12: SPI MISO, nRF24LE1 FMISO
 * D13: SPI SCK, On board Status LED, nRF24LE1 FSCK
 * A0:
 * A1:
 * A2:
 * A3:
 * A4: I2C SDA
 * A5: I2C SCL
 * 5V:
 * 3.3V: nRF24LE1 VDD
 * AREF:
 * GND:  nRF24LE1 VSS

 (~ PWM)

 Interupts:
 0:
 1:

 Pin-Mapping:
 Arduino	         24Pin		 32Pin		48Pin
 D07 (RXD)	    12 P0.6		10 P0.4		15 P1.1
 D08 (PROG)	     5 PROG		 6 PROG		10 PROG
 D09 (RESET)	  13 RESET	19 RESET	30 RESET
 D10 (FCSN,TXD)	11 P0.5		15 P1.1		22 P2.0
 D11 (FMOSI)	   9 P0.3		13 P0.7		19 P1.5
 D12 (FMISO)	  10 P0.4		14 P1.0		20 P1.6
 D13 (FSCK)	     8 P0.2		11 P0.5 	16 P1.2

 */

#include <SoftwareSerial.h>

#define NRFTYPE 32

// Specify pins in use
#define PROG 8    // nRF24LE1 Program
#define _RESET_ 9 // nRF24LE1 Reset
#define _FCSN_ 10 // nRF24LE1 Chip select

// nRF24LE1 Serial port connections.  These will differ with the different chip
// packages
#define nRF24LE1_RXD 6 // nRF24LE1 UART/TXD
#define nRF24LE1_TXD 7  // nRF24LE1 UART/RXD

SoftwareSerial nRF24LE1Serial(nRF24LE1_RXD, nRF24LE1_TXD);
#define NRF24LE1_BAUD 19200
#define PROG_BAUD 57600

#define FLASH_TRIGGER 0x01 // Magic chaNRF24LE1_BAUDracter to trigger uploading of flash

// SPI Flash operations commands
#define WREN 0x06       // Set flase write enable latch
#define WRDIS 0x04      // Reset flash write enable latch
#define RDSR 0x05       // Read Flash Status Register (FSR)
#define WRSR 0x01       // Write Flash Status Register (FSR)
#define READ 0x03       // Read data from flash
#define PROGRAM 0x02    // Write data to flash
#define ERASE_PAGE 0x52 // Erase addressed page
#define ERASE_ALL 0x62  // Erase all pages in flash info page^ and/or main block
#define RDFPCR 0x89     // Read Flash Protect Configuration Register (FPCR)
#define RDISMB 0x85     // Enable flash readback protection
#define ENDEBUG 0x86    // Enable HW debug features

/* NOTE: The InfoPage area DSYS are used to store nRF24LE1 system and tuning
 * parameters. Erasing the content of this area WILL cause changes to device
 * behavior and performance. InfoPage area DSYS should ALWAYS be read out and
 * stored prior to using ERASE ALL. Upon completion of the erase, the DSYS
 * information must be written back to the flash InfoPage.
 *
 * Use the Read_Infopage sketch to make a backup.
 */

// Flash Status Register (FSR) bits
#define FSR_STP B01000000   // Enable code execution start from protected flash area
#define FSR_WEN B00100000   // Write enable latch
#define FSR_RDYN B00010000  // Flash ready flag - active low
#define FSR_INFEN B00001000 // Flash InfoPage enable

// Hex file processing definitions
#define HEX_REC_START_CODE ':'

#define HEX_REC_TYPE_DATA 0
#define HEX_REC_TYPE_EOF 1
#define HEX_REC_TYPE_EXT_SEG_ADDR 2
#define HEX_REC_TYPE_EXT_LIN_ADDR 4

#define HEX_REC_OK 0
#define HEX_REC_ILLEGAL_CHARS -1
#define HEX_REC_BAD_CHECKSUM -2
#define HEX_REC_NULL_PTR -3
#define HEX_REC_INVALID_FORMAT -4
#define HEX_REC_EOF -5

#define PINS_PROG_MODE_ON      \
  pinMode(MOSI, OUTPUT);       \
  pinMode(SCK, OUTPUT);        \
  pinMode(PROG, OUTPUT);       \
  digitalWrite(PROG, LOW);     \
  pinMode(_RESET_, OUTPUT);    \
  digitalWrite(_RESET_, HIGH); \
  pinMode(_FCSN_, OUTPUT);     \
  digitalWrite(_FCSN_, HIGH);

#define PINS_PROG_MODE_OFF \
  SPI.end();               \
  pinMode(MOSI, INPUT);    \
  pinMode(SCK, INPUT);     \
  pinMode(PROG, INPUT);    \
  pinMode(_RESET_, INPUT); \
  pinMode(_FCSN_, INPUT);

void flash();
byte ConvertHexASCIIDigitToByte(char c);
byte ConvertHexASCIIByteToByte(char msb, char lsb);
int ParseHexRecord(struct hexRecordStruct *record, char *inputRecord, int inputRecordLen);
