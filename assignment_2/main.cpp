
#include <chrono>
#include <cstdio>
#include "mbed.h"

// Define the digital input for the button
DigitalIn button(PC_1, PullUp);

// Define the analog input for the potentiometer
AnalogIn potentiometer(PC_4);

// Define the PWM outputs for the LEDs
PwmOut led1(PA_7);
PwmOut led2(PA_2);
PwmOut led3(PA_15);

// Define a variable to store the potentiometer value
float potentiometerValue;

int main() {
    // Initialize the LEDs with a duty cycle of 0
    led1.write(0);
    led2.write(0);
    led3.write(0);
    
    // Set the period of the PWM outputs
    led1.period(0.01);
    led2.period(0.01);
    led3.period(0.01);
    
    while(1) {
        // Read the potentiometer value
        potentiometerValue = potentiometer.read();
        //printf("%f", potentiometerValue);
        
        // Check if the button is pressed
        if(button == 0) {
            // If the button is pressed, set all LEDs to full brightness
            led1.write(1);
            led2.write(1);
            led3.write(1);
        } else {
            // If the button is not pressed, set the brightness of the LEDs based on the potentiometer value
            

        if (potentiometerValue <= (1.0/3.0)) { // first 1/3 of potentiometer range
            led1 = potentiometerValue * 3; // set brightness of first LED
            led2 = 0; // turn off second LED
            led3 = 0; // turn off third LED
        } else if (potentiometerValue > (1.0/3.0) && potentiometerValue <= (2.0/3.0)) { 
            led1 = 1; 
            led2 = (potentiometerValue - (1.0/3.0)) * 3; 
            led3 = 0; 
        } else { 
            led1 = 1; 
            led2 = 1;
            led3 = (potentiometerValue - (2.0/3.0)) * 3; // set brightness of third LED
        }
        }
        // Wait for 0.1 seconds before reading the potentiometer value again
       ThisThread::sleep_for(std::chrono::milliseconds(100));
    
}
}