#include "mbed.h"
#include "DFRobot_RGBLCD.h"

// I2C pins for the LCD
#define SDA_PIN PC_1
#define SCL_PIN PC_0

// Button pins
#define BUTTON1_PIN PB_0
#define BUTTON2_PIN PB_1
#define BUTTON3_PIN PA_2
#define BUTTON4_PIN PA_3

// Buzzer pin
#define BUZZER_PIN PB_4

// Create an I2C object for the LCD
I2C i2c(SDA_PIN, SCL_PIN);

// Create an RGBLCD object with the I2C object
DFRobot_RGBLCD lcd(16, 2, PC_1, PC_0);

// Create button objects
DigitalIn button1(BUTTON1_PIN, PullUp);
DigitalIn button2(BUTTON2_PIN, PullUp);
DigitalIn button3(BUTTON3_PIN, PullUp);
DigitalIn button4(BUTTON4_PIN, PullUp);

// Create buzzer object
PwmOut buzzer(BUZZER_PIN);

// Timer object
Timer timer;

// Timer variables
uint32_t remaining_time_s = 60;
bool timer_running = false;

// Debounce time
const int debounce_ms = 20;

// Update LCD function
void update_lcd() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("%02d:%02d", remaining_time_s / 60, remaining_time_s % 60);
}

// Sound alarm function
void sound_alarm() {
    buzzer.period(1.0 / 1000.0); // Set the buzzer frequency to 1000 Hz
    buzzer.write(0.5f);          // Set the duty cycle to 50%
    ThisThread::sleep_for(5s);   // Sound the alarm for 5 seconds
    buzzer.write(0.0f);          // Turn off the buzzer
}

int main() {
    // Initialize the LCD
    lcd.init();
    lcd.setBacklight(0);
    update_lcd();

    while (true) {
        if (!button1.read()) {
            ThisThread::sleep_for(debounce_ms);
            if (!button1.read()) {
                timer_running = !timer_running;
                if (timer_running) {
                    timer.start();
                } else {
                    timer.stop();
                }
                update_lcd();
            }
        }

        if (!button2.read()) {
            ThisThread::sleep_for(debounce_ms);
            if (!button2.read()) {
                remaining_time_s = 60;
                timer_running = false;
                timer.stop();
                update_lcd();
            }
        }

        if (!button3.read()) {
            ThisThread::sleep_for(debounce_ms);
            if (!button3.read()) {
                remaining_time_s += 5;
                update_lcd();
            }
        }

        if (!button4.read()) {
            ThisThread::sleep_for(debounce_ms);
            if (!button4.read()) {
                if (remaining_time_s >= 5) {
                    remaining_time_s -= 5;
                } else {
                    remaining_time_s = 0;
                }
                update_lcd();
            }
        }

        if (timer_running) {
    uint32_t elapsed_time_s = timer.read();
    if (elapsed_time_s >= 1) {
        timer.reset();
        if (remaining_time_s > 0) {
            remaining_time_s--;
            update_lcd();
        } else {
            timer_running = false;
            sound_alarm();
        }
    }
}

ThisThread::sleep_for(10ms);


    }
}