//Define BAUD rate here
#define BAUD 9600
#include <util/setbaud.h>
#include <avr/io.h>

// timer2 is initialized in fast PWM mode to change the LED's brightness
void timer2_init() 
{   
    TCCR2A |= _BV(WGM21) | _BV(WGM20);                  // set to fast PWM
    TCCR2A |= (1 << COM2B1);                            // set to clear OC2B on Compare Match
    TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);  // set prescaler to 1024
    OCR2B = 0;                                          // init output compare register
}

// Configuring UART Communication in order to send data to the serial monitor
void Serial_init(void)
{
    UBRR0H = UBRRH_VALUE;
    UBRR0L = UBRRL_VALUE;

#if USE_2X
    UCSR0A |= _BV(U2X0);
#else
    UCSR0A &= ~(_BV(U2X0));
#endif

    UCSR0C = _BV(UCSZ01) | _BV(UCSZ00); /* 8-bit data */
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);   /* Enable RX and TX */
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

    DDRD |= (1<< PD3);              // connect LED to pin PD3
    timer2_init();                  // init timer 2
    Serial_init();                  // init UART communication
    char buffer [4];                // init buffer to get input

    while (1) 
    {
        Serial_println("Enter a value");  // prompt user
        int index = 0;              // index to access buffer

        while (1) {
            char c;                 // new char to read input
            c = Serial_read();      // put input into char
            
            // if user is done, put null at the end of the buffer and break
            if (c == '\n') {
                buffer[index] = '\0';
                break;
            } else {
                buffer[index] = c;  // put input into buffer
                index++;            // increment index to read the next char
            }
        }
        
        Serial_print("User entered: ");
        Serial_println(buffer); 
        OCR2B = atoi(buffer);       // converts buffer to integer and light the LED
    }
}



