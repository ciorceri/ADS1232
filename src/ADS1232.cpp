/*
  ADS1232.cpp - Library for TI ADS1232
  24-Bit Analog-to-Digital Converter For Bridge Sensors
  Created by Sorin Ciorceri, 2017.
  Released into the public domain.
*/

#include "Arduino.h"
#include "ADS1232.h"

ADS1232::ADS1232(byte pdwn, byte sclk, byte dout) {
  PDWN = pdwn;
  SCLK = sclk;
  DOUT = dout;

  pinMode(PDWN, OUTPUT);
  pinMode(SCLK, OUTPUT);
  pinMode(DOUT, INPUT);
}

ADS1232::~ADS1232() {
}

bool ADS1232::is_ready() {
  return digitalRead(DOUT) == LOW;
}
  
void ADS1232::power_up() {
  digitalWrite(PDWN, HIGH);
  while (!is_ready()) {};
}

void ADS1232::power_down() {
  digitalWrite(PDWN, LOW);
}

void ADS1232::set_offset(long offset) {
  OFFSET = offset;
}

void ADS1232::set_scale(float scale) {
  SCALE = scale;
}

long ADS1232::_raw_read() {
  // From datasheet (page 19): The data must be retrieved before
  // new data are updated or else it will be overwritten
  // This means that I have to wait DOUT to change from LOW to HIGH and back
  // to LOW (that's the signal I have new data) and get it in 12.5 or 100 ms 100ms
  
  while (is_ready()) {};
  while (!is_ready()) {};

  long value = 0;
  long to_add = 0;
  byte data[3] = { 0 };

  // pulse the clock pin 24 times to read the data
  data[2] = shiftIn(DOUT, SCLK, MSBFIRST);
  data[1] = shiftIn(DOUT, SCLK, MSBFIRST);
  data[0] = shiftIn(DOUT, SCLK, MSBFIRST);

  // From datasheet : output 24 bits of data in binary two's complement &
  // The positive full-scale input produces an output code of 7FFFFFh
  // and the negative full-scale input produces an output code of 800000h
  
  // If we have a negative number, this will be translated between 0 -> 0x7FFFFF
  if (data[2] & B1000000) {
    data[2] &= B01111111;
    to_add = 0x0UL;
  }
  // If we have a positive number, this will be translated between 0x800000 -> 0x1000000
  else {
    to_add = 0x7FFFFFUL;  // 2^23
  }
  value = ( static_cast<long>(data[2]) << 16
          | static_cast<long>(data[1]) << 8
          | static_cast<long>(data[0]) );
  value += static_cast<long>(to_add); 
  value -= static_cast<long>(OFFSET);

  return static_cast<long>(value);
}

long ADS1232::raw_read(byte times) {
  long sum = 0;
  for (byte i=0; i<times; i++) {
    sum += _raw_read();;
  }
  return sum/times;
}

float ADS1232::units_read(byte times) {
  return raw_read(times) / SCALE;
}

