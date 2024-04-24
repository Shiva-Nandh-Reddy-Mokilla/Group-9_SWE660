
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "lcd.h"

#define PWM_PERIOD_PATH "/sys/class/pwm/pwm-4:0/period"
#define PWM_ENABLE_PATH "/sys/class/pwm/pwm-4:0/enable"
#define PWM_DUTY_CYCLE_PATH "/sys/class/pwm/pwm-4:0/duty_cycle"

#define SERVO_PERIOD_NS 20000000 // 20ms period (50 Hz)
#define SERVO_MIN_DUTY_NS 500000  // 0.5ms pulse width (0 degrees)
#define SERVO_MAX_DUTY_NS 2500000 // 2.5ms pulse width (180 degrees)

float get_ph_value();
int get_airquality();