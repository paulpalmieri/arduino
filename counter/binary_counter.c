#include <avr/io.h>
#include <util/delay.h>

// Binary counter implemented in C

// counter to keep track of value
int counter = 0;

int main(void)
{

    Serial.begin(9600);
  // initialize output pins using the Data Direction Register B (DDRB)
    DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB2);
    DDRB |= (1 << PB3);

    // initalize input pin
    DDRD &= ~(1 << PD2);
    

    // main loop
    while(1)
    {

      // if button is pressed
      if(PIND & (1<<PD2))
      {
          // increments the counter on each button press
          counter++;

          Serial.println(counter);
          
          // resets counter everytime we reach 16
          counter %= 16;
          
          // assigns the counter value to the port register
          PORTB = counter;
          
          // small delay for debouncing effect
          _delay_ms(200);
      }
    }
}





