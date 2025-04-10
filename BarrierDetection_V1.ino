/* Barrier Detection
   By: Sasha Dauz, Robert J. Guziec, Jacob JM Horstman
   Written: April 10, 2025
   I/O Pins:

   Possible issues I see:
   - Current logic is to hug the left wall. What happens when it encounters the first barrier?
     I think it'll exit the maze from the beginning, where it came from??? Like hear me out on this.
  
  To-Do:
  - Write code for turn180degrees function
  - Connect tactile sensor
  - Fill in I/O pins in header
  - Edit header
  - Edit comments to be professional
*/



// *** *** *** *** GLOBAL VARIABLES *** *** *** ***
// Encoder calibration
volatile unsigned long tcks = 80;  // Number of encoder ticks per wheel revolution

// Variables to track the number of encoder ticks
volatile unsigned long counterLeft = 0;
volatile unsigned long counterRight = 0;

// Tactile sensors input pins
volatile unsigned char barrierLeft = 0;  // Left tactile sensor state
volatile unsigned char barrierRight = 0; // Right tactile sensor state

// Motor control variables
volatile unsigned char forward = 0;



// *** *** *** *** SETUP *** *** *** ***
void setup() {
  // Disable global interrupts
  cli();  

  // Motor PWM Setup
  // Left Motor Setup
  TCCR0A = 0xA1;    // Configure Timer 0 for PWM (Left motor)
  TCCR0B = 0x11;
  OCR0A = 40;       // Set motor speed (left motor)
  OCR0B = 8;        // Set motor speed (left motor)

  // Right Motor Setup
  TCCR2A = 0xA1;    // Configure Timer 2 for PWM (Right motor)
  TCCR2B = 0x11;
  OCR2A = 40;       // Set motor speed (right motor)
  OCR2B = 8;        // Set motor speed (right motor)

  // Configure motor control pins as output
  DDRD = 0x68;  // D3, D5, D6 as output for motors
  DDRB = 0x08;  // D11 as output for motor control

  // Enable internal pull-up on D8 (direction toggle switch)
  PORTB |= 0x01;

  // Configure pin change interrupt on D8 for motor direction switch
  PCICR = 0x01;
  PCMSK0 = 0x01;

  // Enable global interrupts
  sei();  
}



// *** *** *** *** LOOP *** *** *** ***
void loop() {
  // Continuously move forward till death
  moveForward();

  // Read tactile sensor state
  barrierLeft = PINB & 0x04;  // D10
  barrierRight = PINB & 0x08; // D11

  // Barrier logic
  // If a barrier is detected on either sensor
  if (barrierLeft == 0 || barrierRight == 0) {  
    stopMotors();      // Stop the motors
    _delay_ms(5000);   // Wait before moving backward
    moveBackward();    // Move backward to avoid the barrier
    _delay_ms(5000);   // Wait
    turnLeft();        // Turn left
    moveForward();     // Move forward

    // Check if barrier is still detected and turn accordingly
    // Barrier detected again, turn 180 degrees
    if (barrierLeft == 0 || barrierRight == 0) {
      moveBackward();   // Move backward 
      _delay_ms(5000);  // Wait before turning
      turn180Degrees(); // Turn 180 degrees
      _delay_ms(5000);  // Wait to complete the turn
      moveForward();    // Continue moving forward after turning 180 degrees
    }
}


// *** *** *** *** OTHER FUNCTIONS *** *** *** ***
// Move forward
void moveForward() {
  // Keep moving straight
  OCR0A = 8;    // Left motor forward speed
  OCR0B = 237;  // Left motor forward speed
  OCR2A = 8;    // Right motor forward speed
  OCR2B = 255;  // Right motor forward speed
}

// Stop both motors
void stopMotors() {
  OCR0A = 0;
  OCR0B = 0;
  OCR2A = 0;
  OCR2B = 0;
}

// Move backward
void moveBackward() {
  // Reverse direction by reversing motor speeds
  OCR0A = 237;  // Left motor backward speed
  OCR0B = 8;    // Left motor backward speed
  OCR2A = 255;  // Right motor backward speed
  OCR2B = 8;    // Right motor backward speed
}

// Turn left
void turnLeft() {
  // Left motor turns in reverse and right motor moves forward
  OCR0A = 8;    // Left motor reverse speed
  OCR0B = 200;  // Left motor reverse speed
  OCR2A = 200;  // Right motor forward speed
  OCR2B = 8;    // Right motor forward speed
}

// Turn 180 degrees
void turn180Degrees() {
  // WRITE CODE 
}



// *** *** *** *** ISR's *** *** *** ***
// Encoder ISR - Updates encoder counters on each interrupt
ISR(ADC_vect) {
  // Read encoder state (left and right)
  unsigned char leftB = PINC & 0x20;
  unsigned char rightB = PINC & 0x10;

  // Increment counters
  if (leftB < leftA) {
    counterLeft++;
  }
  if (rightB < rightA) {
    counterRight++;
  }

  // Debugging Code for Serial Monitor
  // Serial.print("Left: ");
  // Serial.println(counterLeft);
  // Serial.print("Right: ");
  // Serial.println(counterRight);

  // Save current state for the next interrupt
  leftA = leftB;
  rightA = rightB;
}