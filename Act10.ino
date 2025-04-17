#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/io.h>

// Encoder Count Variables
volatile unsigned int leftWheelCount = 0;
volatile unsigned int rightWheelCount = 0;
volatile uint8_t prevPINC = 0x30;  // Assume pull-up (HIGH) initially

// Wheel Constants
#define WHEEL_CIRCUMFERENCE 0.650  // meters (65 cm)
#define ENCODER_TICKS_PER_REV 192  // hall actuations per rev

void setup() {
  cli();  // Disable global interrupts

  // Configure PC4 (A4) and PC5 (A5) as inputs
  DDRC &= ~(0x30);  // Clear bits 4 and 5
  PORTC |= 0x30;    // Enable pull-up resistors on PC4 and PC5

  // Enable Pin Change Interrupts on PCINT12 (PC4) and PCINT13 (PC5)
  PCICR |= 0x02;   // Enable PCINT1 (for Port C)
  PCMSK1 |= 0x30;  // Enable PCINT12 and PCINT13

  // Configure Timer1 for 50 Hz Fast PWM (for OC1A and OC1B)
  DDRB |= 0x06;   // Set PB1 (OC1A) and PB2 (OC1B) as output
  TCCR1A = 0xA1;  // Non-inverting Fast PWM mode
  TCCR1B = 0x1A;  // Fast PWM, ICR1 as TOP, Prescaler = 8
  ICR1 = 39999;   // TOP value for 50 Hz PWM (16MHz / (8 * 50Hz)) - 1
  OCR1A = 19999;  // 50% duty cycle
  OCR1B = 19999;  // same

  // Initialize UART for serial output at 9600 baud (fancy shit, getting in the habit!!!)
  UBRR0L = 103;   // 16MHz, 9600 baud
  UCSR0B = 0x18;  // TX  RX enable
  UCSR0C = 0x06;  // 8-bit data

  sei();  // Enable global interrupts
}

ISR(PCINT1_vect) {
  uint8_t currentPINC = PINC & 0x30;
  uint8_t changed = currentPINC ^ prevPINC; 

  // Left Encoder (PC4) falling edge (previous state compare req)
  if ((changed & 0x10) && !(currentPINC & 0x10) && (prevPINC & 0x10)) {
    leftWheelCount++;
  }

  // Right Encoder (PC5) falling edge
  if ((changed & 0x20) && !(currentPINC & 0x20) && (prevPINC & 0x20)) {
    rightWheelCount++;
  }

  prevPINC = currentPINC;
}

void loop() {
  unsigned int averageTicks = (leftWheelCount + rightWheelCount) / 2;
  float distanceTraveled = ((float)averageTicks / ENCODER_TICKS_PER_REV) * WHEEL_CIRCUMFERENCE;
  unsigned int distance_mm = (unsigned int)(distanceTraveled * 1000.0);

  char buffer[80];
  sprintf(buffer, "Left Ticks: %u | Right Ticks: %u | Distance: %u mm\n",
          leftWheelCount, rightWheelCount, distance_mm);

  for (char* ptr = buffer; *ptr; ptr++) {
    while (!(UCSR0A & 0x20))
      ;  // Wait for UART transmit ready
    UDR0 = *ptr;
  }

  _delay_ms(500);  // Update every 0.5s
}

int main() {
  setup();
  while (1) {
    loop();
  }
}
