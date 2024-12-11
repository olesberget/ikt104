#include "mbed.h"
#include "DFRobot_RGBLCD.h"
#include "Watchdog.h"

// I2C pins for the LCD
#define SDA_PIN PC_1
#define SCL_PIN PC_0

// Button pins
#define BUTTON1_PIN PA_0
#define BUTTON2_PIN PA_1

// Create an I2C object for the LCD
I2C i2c(SDA_PIN, SCL_PIN);

// Create an RGBLCD object with the I2C object
DFRobot_RGBLCD lcd(16, 2, PC_1, PC_0);

// Create button interrupt objects
InterruptIn button1(BUTTON1_PIN);
InterruptIn button2(BUTTON2_PIN);

// Timer objects
Timer timer;
Timer debounce_timer;

// Watchdog object
Watchdog &wd = Watchdog::get_instance();

// Button flags
volatile bool button1_pressed = false;
volatile bool button2_pressed = false;

// Timer flag
volatile bool timer_paused = false;

// Debounce delay
const uint32_t debounce_delay_ms = 20;

// Debounce check function
bool debounce_check() {
    return (debounce_timer.elapsed_time().count() / 1000 >= debounce_delay_ms);
}

// Button 1 interrupt handler
void button1_handler() {
    if (debounce_check()) {
        debounce_timer.reset();
        button1_pressed = true;
    }
}

// Button 2 interrupt handler
void button2_handler() {
    if (debounce_check()) {
        debounce_timer.reset();
        button2_pressed = true;
    }
}

int main() {
    // Initialize the LCD
    lcd.init();

    // Set the LCD's backlight color to your desired color (e.g., white)
    lcd.setBacklight(0); // 0 is the predefined value for white

    // Set up button interrupts
    button1.fall(&button1_handler);
    button2.fall(&button2_handler);

    // Set interrupt trigger mode to falling edge
    button1.mode(PullUp);
    button2.mode(PullUp);

    // Start the debounce timer
    debounce_timer.start();

    // Configure the watchdog to trigger after 10 seconds
    wd.start(10000);

    // Start the timer
    timer.start();

    // Initialize the previous stopwatch time
    int prev_ms = -1;

    // Run the main loop
    while (true) {
        if (button1_pressed) {
            button1_pressed = false;
            wd.kick();
        }

        if (button2_pressed) {
            button2_pressed = false;
            timer_paused = !timer_paused;
            if (timer_paused) {
                timer.stop();
            } else {
                timer.start();
            }
        }

        if (!timer_paused) {
            // Calculate the current stopwatch time
            int ms = timer.elapsed_time().count() / 1000;

            // Only update the LCD if the time has changed
            if (ms != prev_ms) {
                // Set the cursor to the first row
                lcd.setCursor(0, 0);

                // Print the current stopwatch time on the LCD with two decimal points
                lcd.printf("%d.%02d", ms / 100, ms % 100);

                // Update the previous stopwatch time
                prev_ms = ms;
            }
        }

        // Wait for a short period
        ThisThread::sleep_for(50ms);
    }
}