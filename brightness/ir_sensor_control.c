#include <avr/io.h>
#include <util/delay.h>

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
    
    // resets ADIF to 1 to be able to start the next conversion
    ADCSRA |= (1 << ADIF);                
}

int main(void) 
{
    Serial.begin(9600);     // serial monitor 9600 baud
    converter_init();       // init ADC

    while(1) 
    {
        convert_analog_digital();         // start conversion
        Serial.println(ADC, DEC);         // prints IR reading to monitor
        _delay_ms(100);
    }
}

