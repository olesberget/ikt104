#include "mbed.h"
#include <cstdio>

#define WAIT_TIME_MS 500ms

#define MAXIMUM_BUFFER_SIZE                                                  32

DigitalOut led1(PA_0);           // PA_0 = (D1 on the board)
static BufferedSerial pc(USBTX, USBRX);


int main() {
    // Configure the serial port with a baud rate of 115200
    pc.set_baud(115200);

    // Continuously read a byte from the serial port and control the LED
    while(1) {
        // Wait for a byte to be available
        if (pc.readable()){
            char c;
            pc.read(&c, 1);
            // Check if the byte is '0' or '1'
            if (c == '0') {
                // Turn the LED off
                printf("LED is off! \n");
                led1 = 0;}
            if (c == '1') {
                // Turn the LED on
                printf("LED is on! \n");
                led1 = 1;}   
        }
    }
}
