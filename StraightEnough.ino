/*Straight Enough
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
  // Configure TCCR0A, TCCR0B, OCR0A
  TCCR0A = 0xA3;
  TCCR0B = 0x11;
  TIMSK0 = 0x00;
  // Configure TCCR2A, TCCR2B, OCR2A
  TCCR2A = 0xA3;
  TCCR2B = 0x11;
  TIMSK2 = 0x00;
  // Set D3, D5, D6 as output
  DDRD = 0x68;
  // Set D11 as output
  DDRB = 0x08;
  // Enable internal pull-up D8
  PORTB |= 0x01;
  // Configure Pin Change Interrupts
  PCICR = 0x01;
  PCMSK0 = 0x01;
  sei();
}
void loop() {
// LEFT WHEEL (L:R RATIO 14:15, 93%)
  if (forward) {
    OCR0A = 8;
    OCR0B = 237;
  } else {
    OCR0A = 237;
    OCR0B = 8;
  }
// RIGHT WHEEL (FULL SEND)
  if (forward) {
    OCR2A = 8;
    OCR2B = 255;
  } else {
    OCR2A = 255;
    OCR2B = 8;
  }
}
// Direction Switch Interrupt
ISR(PCINT0_vect) {
  forward = PINB & 0x01;
}
