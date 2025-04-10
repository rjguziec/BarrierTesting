/*
  Activity 10 Q5
  By: Jacob Horstman, Sasha Dauz
  Written: March 14, 2025
  Edited: --
  I/O Pins for ENCODERS
  ...
  A4: [Input] Right Wheel Encoder
  A5: [Input] Left Wheel Encoder


  I/O Pins for MOTORS
  A0: [INPUT] DC motor Potentiometer
  ...
  D5: [OUTPUT] PC PWM 8-Bit, Output B, REVERSE
  D6: [OUTPUT] PC PWM 8-Bit, Output A, FORWARD
  D8: [INPUT] Toggle Direction, uses Pin Change Interrupt0, Port B
  ...
*/
void setup() {
  cli();

  // Configure serial monitor baudrate to 9600
  Serial.begin(9600);


  // FOR ENCODERS

  // Configure internal pull-up resistors on pins A5 and A4, left and right respectively
  PORTC |= 0x30;
  // Configure Pin Change Interrupts on PORTC, pins A5 and A4
  PCICR = 0x02;
  PCMSK1 = 0x30;


  // END ENCODERS
  // FOR MOTORS

  // PC PWM 8-Bit Mode
  // Configure TCCR0A, TCCR0B, OCR0A
  TCCR0A = 0xA3;
  TCCR0B = 0x11;
  TIMSK0 = 0x00;
  OCR0A = 40;
  OCR0B = 8;
  // Set D5, D6 as output
  DDRD = 0x60;

  // Configure ADC, 8-Bit ADLAR = 1
  ADCSRA = 0xEF;
  ADCSRB = 0x00;
  ADMUX = 0x60;
  // Enable internal pull-up D8
  PORTB |= 0x01;

  // Configure Pin Change Interrupts
  PCICR = 0x01;
  PCMSK0 = 0x01;

  // END MOTORS

  sei();

}

void loop() {
  // Nothing to see here...
}



// BEGIN ENCODER ISR CODE

// In order to get FALLING EDGES in the ISR keep track of the previous state of the PIN and compare it against the new state after the transition.
volatile unsigned long counterRight = 0;
volatile unsigned long counterLeft = 0;
volatile unsigned char leftA = 1;
volatile unsigned char rightA = 1;
volatile unsigned char leftB = PINC & 0x20;
volatile unsigned char rightB = PINC & 0x10;

ISR(PCINT1_vect) {
  // Do nothing ?? previously all the encoder logic
}


// END ENCODER ISR CODE
// BEGIN MOTOR ISR CODE


volatile unsigned char forward = 0;

ISR(PCINT0_vect) {
  forward = PINB & 0x01;
}

ISR(ADC_vect) {
  if (forward) {
    OCR0A = 8;
    OCR0B = ADCH;
    //Serial.println(OCR0B);
  } else {
    OCR0A = ADCH;
    OCR0B = 8;
    //Serial.println(OCR0A);
  }
  //Serial.println(ADCH);


  // ENCODER LOGIC BEGIN

  leftB = PINC & 0x20;
  rightB = PINC & 0x10;
  if (leftB < leftA) {
    counterLeft++;
    //Serial.print("Left: " );
    //Serial.println( counterLeft );
  }
  if (rightB < rightA) {
    counterRight++;
    Serial.print("Right: " );
    Serial.println( counterRight );
  }
  leftA = leftB;
  rightA = rightB;

  //Serial.print("Average: ");
  //Serial.println((counterLeft + counterRight) / 2 );


  // ENCODER LOGIC END
}

// END MOTOR ISR CODE
