/* Barrier Detection v1.0
   By: Sasha Dauz, Robert J. Guziec, Jacob JM Horstman, and The Weirdo
   Written: April 10, 2025
   Purpose:
   - Navigate a maze using two motors and two tactile (whisker) sensors
   - Reverse and turn upon obstacle detection
   - Watchdog reset on corner trapping or system hang

   I/O Pins:
   A0 - 
   A1 - 
   A2 - 
   A3 - 
   A4 - 
   A5 - 
   D0 - 
   D1 - 
   D2 - 
   D3 - Right Motor Forward (digital output)
   D4 - Left Motor Direction/Control (digital output)
   D5 - Left Motor Reverse PWM (Timer0, OCR0B)
   D6 - Left Motor Forward PWM (Timer0, OCR0A)
   D7 - 
   D8 - 
   D9 - 
   D10 - 
   D11 - Right Motor Reverse PWM (Timer2, OCR2A)
   D12 - Left Whisker (digital input, pin change interrupt)
   D13 - Right Whisker (digital input, pin change interrupt)
*/

// *** *** *** *** GLOBAL VARIABLES *** *** *** ***
#include <avr/wdt.h>  // Watchdog Timer library

// Tactile sensors input pins
volatile unsigned char barrierLeft = 1;  // Active-LOW
volatile unsigned char barrierRight = 1; // Active-LOW
unsigned char bumpLoopCount = 0;         // Counter for repeated bump triggers

// *** *** *** *** SETUP FUNCTION *** *** *** ***
void setup() {
  cli();  // Disable global interrupts

  // Configure Timer0 (Left motor PWM)
  TCCR0A = 0xA1;  // Phase Correct PWM, Clear OC0A on Compare Match
  TCCR0B = 0x01;  // No prescaler

  // Configure Timer2 (Right motor PWM)
  TCCR2A = 0xA1;  // Phase Correct PWM, Clear OC2A on Compare Match
  TCCR2B = 0x01;  // No prescaler

  // Configure motor pins as outputs
  DDRD = 0x68;  // D3 (Right motor), D5, D6 (Left motor)
  DDRB = 0x08;  // D11 (Right motor reverse)

  // Enable internal pull-ups on whisker input pins
  PORTB |= 0x30;  // D12, D13

  // Enable pin change interrupt on D12, D13 (PCINT4 and PCINT5)
  PCICR = 0x01;   // Enable PCINT[7:0]
  PCMSK0 = 0x30;  // Mask PCINT4 (D12), PCINT5 (D13)

  sei();  // Enable global interrupts

  // Enable watchdog timer with 1 second timeout
  wdt_enable(WDTO_1S);
}

// Drive forward at calibrated speeds
void moveForward() {
  OCR0A = 8;     // Left motor forward (OCR0A → D6)
  OCR0B = 237;   // Left motor trim
  OCR2A = 8;     // Right motor forward (OCR2A → D11)
  OCR2B = 255;   // Right motor trim
}

// Reverse briefly
void moveBackward() {
  OCR0A = 237;
  OCR0B = 8;
  OCR2A = 255;
  OCR2B = 8;
  _delay_ms(50);
}

// Pivot left
void turnLeft() {
  OCR0A = 0;
  OCR0B = 200;
  OCR2A = 200;
  OCR2B = 0;
  _delay_ms(50);
}

// Pivot right
void turnRight() {
  OCR0A = 200;
  OCR0B = 0;
  OCR2A = 0;
  OCR2B = 200;
  _delay_ms(50);
}

// *** *** *** *** MAIN LOOP *** *** *** ***
void loop() {
  wdt_reset();         // Feed the watchdog
  bumpLoopCount = 0;   // Reset bump counter when moving freely
  moveForward();
}

// *** *** *** *** ISR: Whisker Contact Detection *** *** ***
ISR(PCINT0_vect) {
  barrierLeft = (PINB & 0x20) >> 5;   // D13
  barrierRight = (PINB & 0x10) >> 4;  // D12

  bumpLoopCount++;  // Count consecutive bumps

  // Reset if bumping too often without escaping
  if (bumpLoopCount > 6) {
    wdt_enable(WDTO_15MS);  // Force quick reset
    while (1);              // Wait for watchdog reset
  }

  // Handle right whisker hit
  if (barrierRight == 0) {
    for(unsigned char i = 0; i < 8; i++) moveBackward(); 
    for(unsigned char j = 0; j < 13; j++) turnRight();
  }

  // Handle left whisker hit
  if (barrierLeft == 0) {
    for(unsigned char i = 0; i < 8; i++) moveBackward(); 
    for(unsigned char j = 0; j < 10; j++) turnLeft();
  }
}
