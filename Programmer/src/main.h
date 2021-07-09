#include <SoftwareSerial.h>

#define NRFTYPE 24

// Specify pins in use
#define PROG      8   // nRF24LE1 Program
#define _RESET_   9   // nRF24LE1 Reset
#define _FCSN_    10  // nRF24LE1 Chip select

// nRF24LE1 Serial port connections.  These will differ with the different chip
// packages
#define nRF24LE1_TXD   10   // nRF24LE1 UART/TXD
#define nRF24LE1_RXD    7   // nRF24LE1 UART/RXD

SoftwareSerial nRF24LE1Serial(nRF24LE1_TXD, nRF24LE1_RXD);
#define NRF24LE1_BAUD  19200

#define FLASH_TRIGGER   0x01    // Magic character to trigger uploading of flash


// SPI Flash operations commands
#define WREN 		0x06  // Set flase write enable latch
#define WRDIS		0x04  // Reset flash write enable latch
#define RDSR		0x05  // Read Flash Status Register (FSR)
#define WRSR		0x01  // Write Flash Status Register (FSR)
#define READ		0x03  // Read data from flash
#define PROGRAM		0x02  // Write data to flash
#define ERASE_PAGE	0x52  // Erase addressed page
#define ERASE_ALL	0x62  // Erase all pages in flash info page^ and/or main block
#define RDFPCR		0x89  // Read Flash Protect Configuration Register (FPCR)
#define RDISMB		0x85  // Enable flash readback protection
#define ENDEBUG		0x86  // Enable HW debug features


/* NOTE: The InfoPage area DSYS are used to store nRF24LE1 system and tuning
 * parameters. Erasing the content of this area WILL cause changes to device
 * behavior and performance. InfoPage area DSYS should ALWAYS be read out and
 * stored prior to using ERASE ALL. Upon completion of the erase, the DSYS
 * information must be written back to the flash InfoPage.
 *
 * Use the Read_Infopage sketch to make a backup.
 */

// Flash Status Register (FSR) bits
#define FSR_STP                 B01000000  // Enable code execution start from protected flash area
#define FSR_WEN                 B00100000  // Write enable latch
#define FSR_RDYN                B00010000  // Flash ready flag - active low
#define FSR_INFEN               B00001000  // Flash InfoPage enable


// Hex file processing definitions
#define HEX_REC_START_CODE             ':'

#define HEX_REC_TYPE_DATA               0
#define HEX_REC_TYPE_EOF                1
#define HEX_REC_TYPE_EXT_SEG_ADDR	2
#define HEX_REC_TYPE_EXT_LIN_ADDR	4

#define	HEX_REC_OK                      0
#define HEX_REC_ILLEGAL_CHARS          -1
#define HEX_REC_BAD_CHECKSUM           -2
#define HEX_REC_NULL_PTR               -3
#define HEX_REC_INVALID_FORMAT         -4
#define HEX_REC_EOF                    -5

