/* Barrier Detection v1.0
   By: Sasha Dauz, Robert J. Guziec, Jacob JM Horstman
   Written: April 10, 2025
   I/O Pins:
  A0:
  A1:
  A2:
  A3:
  A4: Left Encoder
  A5: Right Encoder
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
// Encoder variables
volatile unsigned long counterLeft = 0;
volatile unsigned long counterRight = 0;
volatile unsigned char leftA = 0;
volatile unsigned char rightA = 0;
volatile unsigned char leftB = 0;
volatile unsigned char rightB = 0;
// Tactile sensors input pins
volatile unsigned char barrierLeft = 1;  // Active-LOW
volatile unsigned char barrierRight = 1; // Active-LOW
// Motor control variables
volatile unsigned char forward = 0;
// *** *** *** *** SETUP *** *** *** ***
void setup() {
  // Disable global interrupts
  cli();
  // Left Motor Setup
  TCCR0A = 0xA1;    // Configure Timer 0 for PWM (Left motor)
  TCCR0B = 0x11;
  // Right Motor Setup
  TCCR2A = 0xA1;    // Configure Timer 2 for PWM (Right motor)
  TCCR2B = 0x11;
  // Configure motor control pins as output
  // [D3 & D11 Right] [D5 & D6 Left]
  DDRD = 0x68;  // D3, D5, D6 as output for motors
  DDRB = 0x08;  // D11 as output for motor control
  // Enable internal pull-up on D8 (direction toggle switch)
  PORTB |= 0x31;  // Pin D12 and D13 and D8
  PORTC |= 0x30;  // A4 and A5 pull ups
  // Configure pin change interrupts
  PCICR = 0x03;  // Enable PORTS B & C
  PCMSK0 = 0x31; // PORT B, pin D8 and D12 and D13
  PCMSK1 = 0x30; // PORT C, pins A4 and A5
  //Baudrate
  Serial.begin(9600);
  // Enable global interrupts
  sei();
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
  moveForward();
}
// *** *** *** *** ISR's *** *** *** ***
// PORT B - ForwardToggle (Currently not in use)
ISR(PCINT0_vect) {

  // static char scaryCounter = 0;  // Counter to detect if our car encountered a scary corner D:
  // Active-LOW Contact Switch
  barrierLeft = (PINB & 0x20) >> 5;  // D13
  barrierRight = (PINB & 0x10) >> 4; // D12
  if (barrierRight == 0){
    for(unsigned char b = 0; b < 8; b++){
      moveBackward(); 
    }
    for( unsigned char r = 0; r < 13; r++){
      turnRight();
    }
  }
  if (barrierLeft == 0){
    for(unsigned char b = 0; b < 8; b++){
      moveBackward(); 
    }
    for(unsigned char l = 0; l < 10; l++){
      turnLeft();
    }
  }
}
// PORT C - Encoders and whiskers
ISR(PCINT1_vect) {
  // Read encoder state (left and right)
  leftB = PINC & 0x20;  //A5
  rightB = PINC & 0x10; //A4
  // Increment counters on a falling edge
  if (leftB < leftA) {
    counterLeft++;
    //Serial.print("Left: ");
    //Serial.println(counterLeft);
  }
  if (rightB < rightA) {
    counterRight++;
    // Serial.print("Right: ");
    // Serial.println(counterRight);
  }
  // Save current state for the next interrupt
  leftA = leftB;
  rightA = rightB;
}
