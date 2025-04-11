/* Barrier Detection v1.0
   By: Sasha Dauz, Robert J. Guziec, Jacob JM Horstman, and The Weirdo
   Written: April 10, 2025
   I/O Pins:
  A0:
  A1:
  A2:
  A3:
  A4: 
  A5: 
  D0:
  D1:
  D2:
  D3: Right Motor Forward
  D4: Left Motor Control
  D5: Left Motor Reverse Speed
  D6: Left motor Forward Speed
  D7:
  D8:
  D9:
  D10:
  D11: Right Motor Reverse Speed
  D12: Left Whisker
  D13: Right Whisker
  To-Do:
  - Fill in I/O pins in header
  - Edit header
  - Edit comments to be professional
*/

// *** *** *** *** GROSS GLOBAL VARIABLES *** *** *** ***
#include <avr/wdt.h>  // <-- ADD: Include watchdog library

// Tactile sensors input pins
volatile unsigned char barrierLeft = 1;  // Active-LOW
volatile unsigned char barrierRight = 1; // Active-LOW

// *** *** *** *** SETUP *** *** *** ***
void setup() {
  // Disable global interrupts
  cli();

  // Left Motor Setup
  TCCR0A = 0xA1;    // Configure Timer 0 for PWM (Left motor)
  TCCR0B = 0x01;

  // Right Motor Setup
  TCCR2A = 0xA1;    // Configure Timer 2 for PWM (Right motor)
  TCCR2B = 0x01;

  // Configure motor control pins as output
  // [D3 & D11 Right] [D5 & D6 Left]
  DDRD = 0x68;  // D3, D5, D6 as output for motors
  DDRB = 0x08;  // D11 as output for motor control

  // Enable internal pull-up on D12 and D13
  PORTB |= 0x30;  // Pin D12 and D13

  // Configure pin change interrupts
  PCICR = 0x01;  // Enable PORT B
  PCMSK0 = 0x30; // PORT B, D12 and D13

  // Enable global interrupts
  sei();

  // <-- ADD: Enable watchdog timer to reset if stalled
  wdt_enable(WDTO_1S);
}

void moveForward() {
  // Keep moving straight, the lower bound is 82.9 percent and upper is 93 percent
  // left wheel is stronger than right wheel (LEFT BIG, RIGHT SMOL BOI)
  OCR0A = 8;    // Left motor forward speed
  OCR0B = 237;  // Left motor forward speed
  OCR2A = 8;    // Right motor forward speed
  OCR2B = 255;  // Right motor forward speed
}

// Move backward (whoa!)
void moveBackward() {
  // Reverse direction by reversing motor speeds
  OCR0A = 237;  // Left motor backward speed
  OCR0B = 8;    // Left motor backward speed
  OCR2A = 255;  // Right motor backward speed
  OCR2B = 8;    // Right motor backward speed
  _delay_ms(50);
}

// Turn left
void turnLeft() {
  // Left motor turns in reverse and right motor moves forward
  OCR0A = 0;    // Left motor reverse speed
  OCR0B = 200;  // Left motor reverse speed
  OCR2A = 200;  // Right motor forward speed
  OCR2B = 0;    // Right motor forward speed
  _delay_ms(50);
}

// Turn right
void turnRight() {
  // Right motor turns in reverse and right motor moves forward
  OCR0A = 200;    // Left motor reverse speed
  OCR0B = 0;  // Left motor reverse speed
  OCR2A = 0;  // Right motor forward speed
  OCR2B = 200;    // Right motor forward speed
  _delay_ms(50);
}

// *** *** *** *** LOOP *** *** *** ***
void loop() {
  wdt_reset();    // <-- ADD: Feed the watchdog
  moveForward();
}

// *** *** *** *** ISR's *** *** *** ***
// PORT B - Whisker logic :D
ISR(PCINT0_vect) {
  // Active-LOW Contact Switch
  barrierLeft = (PINB & 0x20) >> 5;  // D13
  barrierRight = (PINB & 0x10) >> 4; // D12
  if (barrierRight == 0){
    for(unsigned char i = 0; i < 8; i++){
      moveBackward(); 
    }
    for( unsigned char j = 0; j < 13; j++){
      turnRight();
    }
  }
  if (barrierLeft == 0){
    for(unsigned char i = 0; i < 8; i++){
      moveBackward(); 
    }
    for(unsigned char j = 0; j < 10; j++){
      turnLeft();
    }
  }
}
