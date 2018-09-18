#include <avr/io.h>

// Alternative version of binary counter with debouncing and state keeping

// counter to keep track of value (from 0 to 15)
unsigned char counter = 0;
int previous_state;
int current_state;

int main(void)
{
    Serial.begin(9600);
    // initializes output pins using the Data Direction Register B (DDRB)
    DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB2);
    DDRB |= (1 << PB3);
  
    // initalizes input pin
    DDRD &= ~(1 << PD2);
    
    // main loop
    while(1)
    {

        // set state of button
        current_state = PIND & (1<<PD2);

        // for debouncing
        if((previous_state != current_state) & isPressed())
        {
            // increment counter
            counter++;

            Serial.println(counter);
            
            // resets counter if needed
            counter %= 16;

            // set PORTx register to light the LEDs
            PORTB = counter;
        }

        // change the state
        previous_state = current_state;
    }
}

bool isPressed() {
  return PIND & _BV(PD2);
}



