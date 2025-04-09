/*
  Do a Doughnut!
  By: Robert J. Guziec and Jacob JM Horstman
  Written: March 14, 2025
  Edited: April 9, 2025
  I/O Pins for MOTORS
  D3:  [OUTPUT] PC PWM 8-Bit, Output B, RIGHT MOTOR FORWARD
  D5:  [OUTPUT] PC PWM 8-Bit, Output B, LEFT MOTOR REVERSE 
  D6:  [OUTPUT] PC PWM 8-Bit, Output A, LEFT MOTOR FORWARD
  D8:  [INPUT]  Toggle Direction, uses Pin Change Interrupt 0, Port B
  D11: [OUTPUT] PC PWM 8-Bit, Output B, RIGHT MOTOR REVERSE 
*/
volatile unsigned char forward = 0;
void setup() {
  cli();
  // Configure serial monitor baudrate to 9600
  Serial.begin(9600);
  // PC PWM 8-Bit Mode
  // Configure TCCR0A, TCCR0B, OCR0A
  TCCR0A = 0xA3;
  TCCR0B = 0x11;
  TIMSK0 = 0x00;
  OCR0A = 40;
  OCR0B = 8;
  // Set D3, D5, D6 as output
  DDRD = 0x68;
  // Set D11 as output
  DDRB = 0x08;

  // Configure TCCR2A, TCCR2B, OCR2A
  TCCR2A = 0xA3;
  TCCR2B = 0x11;
  TIMSK2 = 0x00;
  OCR2A = 40;
  OCR2B = 8;
  
  // Enable internal pull-up D8
  PORTB |= 0x01;
  // Configure Pin Change Interrupts
  PCICR = 0x01;
  PCMSK0 = 0x01;

  sei();
}

void loop() {

// TURNING LEFT WHEEL
  if (forward) {
    // CCW (Left)
    OCR0A = 200;
    OCR0B = 8;
  } else {
    // CW (Right)
    OCR0A = 8;
    OCR0B = 200;
  }
// TURNING RIGHT WHEEL
  if (forward) {
    // CCW (Left)
    OCR2A = 8;
    OCR2B = 200;
  } else {
    // CW (Right)
    OCR2A = 200;
    OCR2B = 8;
  }
}

// Direction Switch Interrupt
ISR(PCINT0_vect) {
  forward = PINB & 0x01;
}
