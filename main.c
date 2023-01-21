#pragma config FEXTOSC = HS     // External Oscillator mode Selection bits (HS (crystal oscillator) above 8 MHz; PFM set to high power)
#pragma config RSTOSC = EXTOSC_4PLL// Power-up default value for COSC bits (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)
#pragma config WDTE = OFF        // WDT operating mode (WDT enabled regardless of sleep)

#include <xc.h>
#include "LEDarray.h"
#include "ADC.h"

#define _XTAL_FREQ 64000000 //note intrinsic _delay function is 62.5ns at 64,000,000Hz  

void main(void) 
{
	unsigned int count=0;
    LEDarray_init(); // Initialize LED array, RF2 button and ADC converter for pin RA3
    button_init();
    ADC_init();
  
    while (1) {
        /*
         * TASK_1
		count++; // increment count
		if (count>511) {count=0;} //reset a when it gets too big
		LEDarray_disp_bin(count); //output a on the LED array in binary
        */
        
        /*
        // TASK_2 with button press to initiate light
        if (!PORTFbits.RF2) {
            LEDarray_disp_bin(count);
            __delay_ms(75); // Delay so human eye can see change
            count = count << 1;
            if (count > 256) {
                while (count > 1) {
                    if (!PORTFbits.RF2) {
                    count = count >> 1;
                    LEDarray_disp_bin(count);
                    __delay_ms(75);
                    }
                }
            }
        }
        */
        
        /*
        // TASK_3 & 4
        if (!PORTFbits.RF2) { //if button is pressed
            count++; // increment count
            LEDarray_disp_bin(count); //output a on the LED array in binary
            __delay_ms(500); // delay so it can be seen, or else 1 press may increment count by an unholy amount!
        }
        if (count>511) {count=0;} //reset a when it gets too big
        
        */
        
        /*
        // TASK_5
        if (!PORTFbits.RF2) { // if button pressed increment count
            count++;
            if (count>=100) {count=10;} // once count reaches 100, set count back to 10
            LEDarray_disp_dec(count); // call function
            __delay_ms(500); // delay 
        }
        */
        
        /*
        // Task 3 & 4 improved :)
        // Here we improve by making a delay that accelerates and saturates
        unsigned int dly; // set the delay as a variable
        dly = 2000; // Initial delay
        unsigned int count2; // another count variable to decrease the delay
        count2 = 0;
        while (!PORTFbits.RF2) { //if button is pressed
            count++; // increment count
            count2++; // increment count2
            LEDarray_disp_bin(count);
            delay_ms(dly);
            dly = dly/(count2); // decrease delay periodically as button is held
            if (dly<=50) {dly = 50;} // saturate delay at 50ms
            
        }
        if (count>511) {count=0;} //reset count when it gets too big
        */
        
        /*
        // Task 6 & 7 & 8
        unsigned int val, max;
        val = ADC_getval(); // Initiate the current val and max, 
        max = val; // max at the start is the the current value
        while (!PORTFbits.RF2){ // Button press to start sampling
            if (max>=90) {max=0;} // For some reason max becomes a huge ass number after 0, so set this just incase!
            max = (max/10)*10; // Turn max to a multiple of 10 integer
            if (max>100) {max = 90;} // Saturate max value (as we are only using 9 LEDs)
            LEDarray_disp_PPM(ADC_getval(),max); // Call function
            __delay_ms(250); // Delay 
            max = maximum(max,ADC_getval()); // Update max if the current value increases over the previous maximum
            if (max>ADC_getval()) {max -= 10;} // Move the max value down sequentially by 1 LED every 0.5s/1s depending on delay defined
            
        }
        */
        
        // Task 6 & 7 & 8 improved
        unsigned int val, max, mili;
        val = ADC_getval(); // Initiate the current val and max, 
        max = val; // max at the start is the the current value
        while (1){ // Button press to start sampling
            if (max>=90) {max=0;} // For some reason max becomes a huge ass number after 0, so set this just incase!
            max = (max/10)*10; // Turn max to a multiple of 10 integer
            mili = 150; // Set value for mili
            while(mili > 0) { // While loop for general delay improvements
                if (max>90) {max = 90;} // Saturate max value (as we are only using 9 LEDs)
                LEDarray_disp_PPM(ADC_getval(),max); // Call function to display LEDs
                max = maximum(max,ADC_getval()); // Update max if the current value increases over the previous maximum
                __delay_ms(3); // Delay
                mili--; // Decrement mili
            }
            if (max>ADC_getval()) {max -= 10;} // Move the max value down sequentially by 1 LED every 0.5s/1s depending on delay defined
            
        }  
    }
}