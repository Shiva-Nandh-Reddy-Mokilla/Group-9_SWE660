#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include "common.h"


// Function prototypes
void *air_quality_monitor(void *arg);
void *ph_thread(void *arg);
void *display_values(void *arg);
void *feed_fish(void *arg);
void timer_handler(int signum);

// Mutexes for thread synchronization
pthread_mutex_t air_quality_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ph_value_mutex = PTHREAD_MUTEX_INITIALIZER;

// Global variables to store sensor values
int air_quality = 80;
float ph_value = 8.5;
int display_value = 50;

int32_t servo_angle(int32_t angle_deg) {
    int32_t duty_ns = (0.5 + (angle_deg / 180.0) * 2.0) * 1000000;

    FILE *period_file = fopen("/sys/class/pwm/pwm-4:0/period", "w");
    FILE *duty_cycle_file = fopen("/sys/class/pwm/pwm-4:0/duty_cycle", "w");
    FILE *enable_file = fopen("/sys/class/pwm/pwm-4:0/enable", "w");

    if (period_file == NULL || duty_cycle_file == NULL || enable_file == NULL) {
        perror("Error opening PWM files");
        return -1;
    }

    fprintf(period_file, "%d", 20000000); // Corrected fprintf usage
    fprintf(duty_cycle_file, "%d", duty_ns); // Corrected fprintf usage
    fprintf(enable_file, "1"); // Corrected fprintf usage

    fclose(period_file);
    fclose(duty_cycle_file);
    fclose(enable_file);

    return 0;
}

int main() {
    pthread_t thread1, thread2, thread3, feed_thread;
    int priority1, priority2, priority3, timer_priority;
    system("config-pin p9.14 pwm");
    // Assign priorities for each thread
    priority1 = 50; // Lower priority for air quality monitor
    priority2 = 60; // Slightly higher priority for pH value
    priority3 = 70; // Highest priority for display values
    timer_priority = 40; // Priority for the timer thread

    // Assign priorities before creating threads
    struct sched_param param1, param2, param3, timer_param;
    param1.sched_priority = priority1;
    param2.sched_priority = priority2;
    param3.sched_priority = priority3;
    timer_param.sched_priority = timer_priority;

    // Create threads with specified priorities
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param1);
    pthread_create(&thread1, NULL, air_quality_monitor, NULL);

    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param2);
    pthread_create(&thread2, NULL, ph_thread, NULL);

    pthread_setschedparam(pthread_self(), SCHED_FIFO, &param3);
    pthread_create(&thread3, NULL, display_values, NULL);

    // Set priority for timer thread
    pthread_setschedparam(pthread_self(), SCHED_FIFO, &timer_param);
    pthread_create(&feed_thread, NULL, feed_fish, NULL);

    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    pthread_join(feed_thread, NULL);

    return 0;
}

void *air_quality_monitor(void *arg) {
    while (1) {
        // Simulated air quality data
        pthread_mutex_lock(&air_quality_mutex);
        air_quality = get_airquality();
        pthread_mutex_unlock(&air_quality_mutex);

        usleep(1000 * 1000); // Sleep for 1 second
    }
}

void *ph_thread(void *arg) {
    while (1) {
        // Simulated pH value data
        pthread_mutex_lock(&ph_value_mutex);
        ph_value = get_ph_value();
        pthread_mutex_unlock(&ph_value_mutex);

        usleep(2000 * 1000); // Sleep for 2 seconds
    }
}

void *display_values(void *arg) {
    char buffer[16];
        printf("*******************************************************************************\n");
        printf("*                                                                             *\n");
        printf("*                      FISHTANK MONITORING SYSTEM                              *\n");
        printf("*                                                                             *\n");
        printf("*    Group 9: Priyesh   Shiva    Anusha     Charan                            *\n");
        printf("*******************************************************************************\n");
        sleep(5);
    while (1) {
        //system("clear");

        // Display air quality, pH value, and display value
        pthread_mutex_lock(&air_quality_mutex);
        pthread_mutex_lock(&ph_value_mutex);
        // Display air quality
        printf("Air Quality: ");
        for (int i = 0; i < air_quality; i++) {
            printf("*");
        }
        printf(" %d%%\n", air_quality);

        // Display pH value
        printf("pH Value: ");
        for (int i = 0; i < ph_value ; i++) {
            printf("*");
        }
        printf(" %.1f\n", ph_value);
        pthread_mutex_unlock(&ph_value_mutex);
        pthread_mutex_unlock(&air_quality_mutex);

        usleep(5000 * 1000); // Sleep for 5 second
    }
}

void *feed_fish(void *arg) {
    int remaining_time = 20; // Initial time until next feed (seconds)

    while (1) {
        // Display time remaining until next feed

        lcd_data(air_quality, ph_value,remaining_time);

        if (remaining_time == 0) {
            printf("Feeding the Fish tank\n");
            servo_angle(180);
            sleep(5);
            remaining_time = 20; // 20 seconds until next feed
        }
        servo_angle(0);
        // Sleep for 1 second
        sleep(5);
        remaining_time-=5;
    }
}

