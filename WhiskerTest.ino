/*
  Exercise WHISKERS
  A basic example of how we should setup our whiskers for our smart car
  Note that the whiskers act similarly to that of toggle switches, and should use internal resistors
  (Internal pull-up resistor used)
  By: Robert Guziec
  Written: April 7, 2025
  Edited: April 7, 2025
  I/O Pins
  A0:
  A1:
  A2:
  A3:
  A4:
  A5:
  D0:
  D1:
  D2:
  D3:
  D4: RIGHT whisker LED
  D5: LEFT whisker LED
  D6:
  D7:
  D8:  
  D9:
  D10:
  D11:
  D12: LEFT whisker
  D13: RIGHT whisker
*/

void setup() {

  DDRD = 0x30;  // Setup outputs on D4 and D5
  PORTB |= 0x30;  // Setup internal resistos
}

void loop() {
  
  unsigned char whiskLeft = PINB & 0x20;  // Initialize left whisker variable
  unsigned char whiskRight = PINB & 0x10;  // Initialize right whisker variable

  // Decision tree that determines which whisker is being asserted. If none are being asserted,
  // the LED should be off

  if (!whiskLeft){
    PORTD |= 0x20;
  }
  else if (!whiskRight){
    PORTD |= 0x10;
  }
  else{
    PORTD &= 0xCF;
  }
}
