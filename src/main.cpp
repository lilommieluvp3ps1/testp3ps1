#include <arduino.h>
#define INT0_vect _VECTOR(1)
#define TIMER1_OVF_vect _VECTOR(13)

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

char red = 1;
char green = 0;
char yellow = 0;

ISR(INT0_vect)
{
  if (red == 1)
  {
    red = 0;
    green = 1;
    PORTB &= 0b11111011; // Red off
    PORTB |= 0b00000001; // Green on
    TCNT1 = 3036;
  }
}

ISR(TIMER1_OVF_vect)
{
  // Check Green
  if (green == 1)
  {
    PORTB &= 0b11111110; // Green Off
    PORTB |= 0b00000010; // Yellow On
    yellow = 1;
    green = 0;
  }
  else if (yellow == 1)
  {
    PORTB &= 0b11111110; // Green Off
    PORTB &= 0b11111101; // Yellow Off
    PORTB |= 0b00000100; // Red On
    yellow = 0;
    red = 1;
  }
  TCNT1 = 49911; // Reset Timer1
}

int main()
{
  // กำหนด PORT Inpur/Output ที่ใช้
  DDRB = 0b00000111;  // PB0 PB1 PB2 Output
  PORTB = 0b00000100; // Red On

  // External Interrupt
  EICRA = EICRA | 0b00000010;
  EIMSK = 1;

  // Timer1 Interrupt
  TCCR1A = 0b00000000;
  TCCR1B = 0b00000101;
  TCNT1 = 3036;
  TIMSK1 = 0b00000001;

  // Turn On Global Interrupt
  sei();

  // รอการกดปุ่มด้วยลูปอนันต์
  while (1)
    ;
}
// iluvuEarth