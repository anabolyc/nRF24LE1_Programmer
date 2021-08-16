/*
 Read_Infopage

 Reads InfoPage contents from Nordic nRF24LE1 SOC RF chips using SPI interface from an Arduino.
 Useful for making a backup of the system variables stored in the InfoPage.

 Upload sketch, start serial monitor, type 'GO' to start sketch running.

 Copyright (c) 2014 Dean Cording

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

 */

#include <Arduino.h>
#include <SPI.h>
#include "main.h"

byte fsr;          // Flash status register buffer
byte spi_data;     // SPI data transfer buffer

void setup() {
  // start serial port:
  Serial.begin(PROG_BAUD);
  Serial.setTimeout(30000);

  // Initialise SPI
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();

  // Initialise control pins
  pinMode(PROG, OUTPUT);
  digitalWrite(PROG, LOW);
  pinMode(_RESET_, OUTPUT);
  digitalWrite(_RESET_, HIGH);
  pinMode(_FCSN_, OUTPUT);
  digitalWrite(_FCSN_, HIGH);

  Serial.println("READY");
  // Wait for GO command from Serial
  while (!Serial.find("GO\n"));
  Serial.println("READYING");
  delay(1000);
  Serial.println("SETTING UP");

  // Put nRF24LE1 into programming mode
  digitalWrite(PROG, HIGH);
  digitalWrite(_RESET_, LOW);
  delay(10);
  digitalWrite(_RESET_, HIGH);

  // Set InfoPage bit so InfoPage flash is read
  digitalWrite(_FCSN_, LOW);
  SPI.transfer(RDSR);
  fsr = SPI.transfer(0x00);
  digitalWrite(_FCSN_, HIGH);

  digitalWrite(_FCSN_, LOW);
  SPI.transfer(WRSR);
  SPI.transfer(fsr | FSR_INFEN);
  delay(1);
  digitalWrite(_FCSN_, HIGH);


  digitalWrite(_FCSN_, LOW);
  SPI.transfer(RDSR);
  fsr = SPI.transfer(0x00);
  digitalWrite(_FCSN_, HIGH);

  if (!(fsr & FSR_INFEN)) {
    Serial.println("INFOPAGE ENABLE FAILED");
    goto done;
  }


  delay(10);

  // Read InfoPage contents
  Serial.println("READING...");
  digitalWrite(_FCSN_, LOW);
  SPI.transfer(READ);
  SPI.transfer(0);
  SPI.transfer(0);
  for (int index = 1; index < 38; index++) {
    spi_data = SPI.transfer(0x00);
    Serial.print(index);
    Serial.print(": ");
    Serial.println(spi_data);
  }
  digitalWrite(_FCSN_, HIGH);

done:
  Serial.println("DONE");

  // Take nRF24LE1 out of programming mode
  digitalWrite(PROG, LOW);
  digitalWrite(_RESET_, LOW);
  delay(10);
  digitalWrite(_RESET_, HIGH);

  SPI.end();

}

void loop() {
  // Do nothing
  delay(1000);
}