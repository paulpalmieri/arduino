// Controls the LED brightness with the Infrared sensor

#include <avr/io.h>
#include <util/delay.h>

// the analog to digital converter is prescaled to 16 and in AVcc mode
void converter_init() 
{
    ADCSRA |= (1 << ADEN);    // enable analog/digital conversion
    ADCSRA |= (1 << ADPS2);   // set prescaler to 16
    ADMUX  |= (1 << REFS0);   // AVcc with external capacitor mode
}

// timer2 is initialized in fast PWM mode to change the LED's brightness
void timer2_init() 
{   
    TCCR2A |= _BV(WGM21) | _BV(WGM20);                  // set to fast PWM
    TCCR2A |= (1 << COM2B1);                            // set to clear OC2B on Compare Match
    TCCR2B |= (1 << CS20) | (1 << CS21) | (1 << CS22);  // set prescaler to 1024
    OCR2B = 0;                                          // init output compare register
}

// simple conversion function
void convert_analog_digital() 
{
    ADCSRA |= (1 << ADSC);                // start conversion
    while(!(ADCSRA & (1 << ADIF))) { }    // waits for conversion to finish
    ADCSRA |= (1 << ADIF);                // resets ADIF to 1 to be able to start the next conversion
}

// fits the ADC readings into the 8-bit OCR2B register
int scale_brightness(int digital_value) 
{
    return digital_value % 255;   // mod 255 the reading
}

int main(void) 
{
    Serial.begin(9600);     // serial monitor 9600 baud
    DDRD |= (1 << PD3);     // connect led to pin PD3
    timer2_init();          // init timer2
    converter_init();       // init ADC

    while(1) 
    {
        convert_analog_digital();         // start conversion
        Serial.println(ADC, DEC);         // prints IR reading to monitor
        OCR2B = scale_brightness(ADC);    // scales the brightness with the sensor reading

        _delay_ms(100);
    }
}

