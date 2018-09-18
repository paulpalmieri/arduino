#include <avr/io.h>
#include <util/delay.h>

//Define BAUD rate here for UBRR values
#define BAUD 9600

#include <util/setbaud.h>

// the analog to digital converter is prescaled to 16 and in AVcc mode
void converter_init() 
{
    ADCSRA |= (1 << ADEN);    // enable analog/digital conversion
    ADCSRA |= (1 << ADPS2);   // set prescaler to 16
    ADMUX  |= (1 << REFS0);   // AVcc with external capacitor mode
}

// simple conversion function
void convert_analog_digital() 
{
    ADCSRA |= (1 << ADSC);                // start conversion
    while(!(ADCSRA & (1 << ADIF))) { }    // waits for conversion to finish
    ADCSRA |= (1 << ADIF);                // resets ADIF to 1 to be able to start the next conversion
}

// Configuring UART Communication in order to send data to the serial monitor
void Serial_init(void)
{

    // USART baud rate registers
    // These are set based on the BAUD variable that is defined at the top of the program
    UBRR0H = UBRRH_VALUE;            
    UBRR0L = UBRRL_VALUE;


// double the transmission speed (if USE_2X is defined)
// otherwise set this bit to 0
#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    // choose data format with character size register (and table in datasheet)
    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); // 8 bit data format
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   // enable RX/TX (receiver/transmitter)
}

/* Sends 1 byte of data to the serial Monitor
 * Waits for USART Data Register to be empty by checking if the UDRE flag is set
 */
void Serial_write(char c)
{
    loop_until_bit_is_set(UCSR0A, UDRE0); /* Wait until data register empty. */
    UDR0 = c;
}

/* Recieves  1 byte of data from the serial Monitor
 * Waits for UDR0 Data Register to have data that can be read by checking the RXC0 flag
 */
char Serial_read(void)
{
    loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
    return UDR0;
}

/*
 * Prints string with new line
 */
 
void Serial_println(const char *s){
    // loop through entire string
    while (*s) { 
        Serial_write(*s);
        s++;
    }
    Serial_write('\n');
}


/*
 * Prints string
 */
 
void Serial_print(const char *s){
    // loop through entire string
    while (*s) { 
        Serial_write(*s);
        s++;
    }
}

int main(void) 
{
    Serial_init();          // init usart
    converter_init();       // init ADC
    DDRD |= (1 << PD3);     // init output pin


    while(1) 
    {
        convert_analog_digital();         // start conversion
        if(ADC > 100) {
            PORTD |= (1 << PD3);          // output when the ADC value > 100
            Serial_write('r');
        } else {
            PORTD &= (0 << PD3);          // otherwise, turn off
        }
        _delay_ms(100);
    }
}


