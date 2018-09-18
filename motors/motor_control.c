#include <avr/io.h>
#include <util/delay.h>

//Define BAUD rate here
#define BAUD 9600

#include <util/setbaud.h>

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

void init_interrupt() {  
    EICRA |= (1 << ISC00);    // set INT0 to trigger on ANY logic change
    EIMSK |= (1 << INT0);     // Turns on INT0
}


ISR(INT0_vect)
{
    PORTB ^= (1<<PB5);
    char msg = Serial_read();
    Serial_println(msg);
    switch_direction();
}
void init_controls() {
  
    // set all control pins to output
    DDRB |= (1 << PB0);
    DDRB |= (1 << PB1);
    DDRB |= (1 << PB2);
    DDRB |= (1 << PB3);
    
    // set all control pins to output
    DDRD |= (1 << PD4);
    DDRD |= (1 << PD5);
    DDRD |= (1 << PD6);
    DDRD |= (1 << PD7);

    // set all enable pins to ouput
    DDRC |= (1 << PC0);
    DDRC |= (1 << PC1);
    DDRC |= (1 << PC2);
    DDRC |= (1 << PC3);

    // enable all 
    PORTC |= (1 << PC0);
    PORTC |= (1 << PC1);
    PORTC |= (1 << PC2);
    PORTC |= (1 << PC3);
}

// output high on each wheels
void forward() {
    PORTB |= (1 << PB0);
    PORTB |= (1 << PB2);
    PORTD |= (1 << PD4);
    PORTD |= (1 << PD6);
}

// flips the bits to change direction
// this function gets executed in the ISR
void switch_direction() {
    PORTB ^= (1 << PB0);
    PORTB ^= (1 << PB1);
    PORTB ^= (1 << PB2);
    PORTB ^= (1 << PB3);

    PORTD ^= (1 << PD4);
    PORTD ^= (1 << PD5);
    PORTD ^= (1 << PD6);
    PORTD ^= (1 << PD7);
}

int main(void) {
    Serial_init();
    DDRB |= (1 << PB5);     //built in LED
    init_controls();
    init_interrupt();    
    forward();
    sei();
    
    while(1) {
     
    } 
}

