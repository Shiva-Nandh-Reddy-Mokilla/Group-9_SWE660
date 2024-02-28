#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

// Typedef for GPIO pin path

typedef char gpio_path[50]; // Define gpio_path as an array of characters

static int initialize_gpios(gpio_path red_direction, gpio_path yellow_direction, gpio_path green_direction);
// Function to initialize GPIO pins
static int initialize_gpios(gpio_path red_direction, gpio_path yellow_direction, gpio_path green_direction) {
    int file = 0;
    int result = 0; // Initialize result variable to track success/failure

    // Open and set direction for red light
    file = open(red_direction, O_RDWR);
    if (file < 0) {
        perror("Error opening Red Direction");
        result = -1; // Set result to -1 if opening red direction fails
    } else {
        // Set direction as output
        if (write(file, "out", 3) != 3) {
            perror("Error setting direction for Red Light");
            result = -1; // Set result to -1 if setting direction fails
        }
        (void)close(file); // Close file descriptor
    }

    // Open and set direction for yellow light
    file = open(yellow_direction, O_RDWR);
    if (file < 0) {
        perror("Error opening Yellow Direction");
        result = -1; // Set result to -1 if opening yellow direction fails
    } else {
        // Set direction as output
        if (write(file, "out", 3) != 3) {
            perror("Error setting direction for Yellow Light");
            result = -1; // Set result to -1 if setting direction fails
        }
        (void)close(file); // Close file descriptor
    }

    // Open and set direction for green light
    file = open(green_direction, O_RDWR);
    if (file < 0) {
        perror("Error opening Green Direction");
        result = -1; // Set result to -1 if opening green direction fails
    } else {
        // Set direction as output
        if (write(file, "out", 3) != 3) {
            perror("Error setting direction for Green Light");
            result = -1; // Set result to -1 if setting direction fails
        }
        (void)close(file); // Close file descriptor
    }

    return result; // Return result after all operations (0 if successful, -1 if any operation fails)
}
static int writeGPIO(gpio_path light, int value);

// Function to write to GPIO
static int writeGPIO(gpio_path light, int value) {
    int file = 0;
    int result = 0; // Variable to store the result of write operation

    file = open(light, O_WRONLY);
    if (file < 0) {
        perror("Error opening GPIO");
        result = -1; // Record the error but continue the function
    } else {
        if (value == 1) {
            if (write(file, "1", 1) != 1) {
                perror("Error writing to GPIO");
                result = -1; // Record the error if write operation fails
            }
        } else {
            if (write(file, "0", 1) != 1) {
                perror("Error writing to GPIO");
                result = -1; // Record the error if write operation fails
            }
        }
        (void)close(file);
    }
    return result; // Return the result after all operations
}

static void traffic_light_control(gpio_path red1_value, gpio_path yellow1_value, gpio_path green1_value,
                           gpio_path red2_value, gpio_path yellow2_value, gpio_path green2_value,
                           int green_duration_signal1, int green_duration_signal2);
/// Function to control traffic lights
static void traffic_light_control(gpio_path red1_value, gpio_path yellow1_value, gpio_path green1_value,
                           gpio_path red2_value, gpio_path yellow2_value, gpio_path green2_value,
                           int green_duration_signal1, int green_duration_signal2) 
{
    while (1) 
    {
        // Traffic Signal 1 Sequence
        (void)printf("Red light 1 is on\n");
        (void)writeGPIO(red1_value, 1);
        (void)writeGPIO(green2_value, 1);
        (void)usleep(green_duration_signal2); // Wait for the duration of the green light for signal 2


        (void)printf("Yellow light 1 is on\n");
        (void)writeGPIO(red1_value, 1);
        (void)writeGPIO(yellow1_value, 1);

        (void)usleep(2000000);// Wait for 2 seconds

        (void)printf("Green light 2 is on\n");
        (void)writeGPIO(green2_value, 1);

    
        (void)usleep(5000000); // Wait for 5 seconds

        (void)printf("All lights off for transition\n");
        (void)writeGPIO(red1_value, 0);
        (void)writeGPIO(yellow1_value, 0);
        (void)writeGPIO(green2_value, 0);

        // Traffic Signal 2 Sequence
        (void)printf("Red light 2 is on\n");
        (void)writeGPIO(red2_value, 1);
        (void)writeGPIO(green1_value, 1);
        (void)usleep(green_duration_signal1); // Wait for the duration of the green light for signal 1

        (void)printf("Yellow light 2 is on\n");
        (void)writeGPIO(red2_value, 1);
        (void)writeGPIO(yellow2_value, 1);
        (void)usleep(2000000); // Wait for 2 seconds
        (void)printf("Green light 1 is on\n");
        (void)writeGPIO(green1_value, 1);

        (void)usleep(5000000); // Wait for 5 seconds

        (void)printf("All lights off for transition\n"); // Turn off all lights for transition
        (void)writeGPIO(red2_value, 0);
        (void)writeGPIO(yellow2_value, 0);
        (void)writeGPIO(green1_value, 0);
    }
}


int main(void) {

    // Variables to store GPIO pin paths
    gpio_path red1_direction, yellow1_direction, green1_direction;
    gpio_path red1_value, yellow1_value, green1_value;
    gpio_path red2_direction, yellow2_direction, green2_direction;
    gpio_path red2_value, yellow2_value, green2_value;
    int red1_num, yellow1_num, green1_num, red2_num, yellow2_num, green2_num;
    int green_duration_signal1, green_duration_signal2;

    // Prompt user for GPIO pin numbers
    (void)printf("Enter GPIO pin numbers for Traffic Signal 1 (Red Yellow Green): ");
    (void)scanf("%d %d %d", &red1_num, &yellow1_num, &green1_num);
    (void)snprintf(red1_direction, sizeof(red1_direction), "/sys/class/gpio/gpio%d/direction", red1_num);
    (void)snprintf(yellow1_direction, sizeof(yellow1_direction), "/sys/class/gpio/gpio%d/direction", yellow1_num);
    (void)snprintf(green1_direction, sizeof(green1_direction), "/sys/class/gpio/gpio%d/direction", green1_num);
    (void)snprintf(red1_value, sizeof(red1_value), "/sys/class/gpio/gpio%d/value", red1_num);
    (void)snprintf(yellow1_value, sizeof(yellow1_value), "/sys/class/gpio/gpio%d/value", yellow1_num);
    (void)snprintf(green1_value, sizeof(green1_value), "/sys/class/gpio/gpio%d/value", green1_num);

    (void)printf("Enter GPIO pin numbers for Traffic Signal 2 (Red Yellow Green): ");
    (void)scanf("%d %d %d", &red2_num, &yellow2_num, &green2_num);
    (void)snprintf(red2_direction, sizeof(red2_direction), "/sys/class/gpio/gpio%d/direction", red2_num);
    (void)snprintf(yellow2_direction, sizeof(yellow2_direction), "/sys/class/gpio/gpio%d/direction", yellow2_num);
    (void)snprintf(green2_direction, sizeof(green2_direction), "/sys/class/gpio/gpio%d/direction", green2_num);
    (void)snprintf(red2_value, sizeof(red2_value), "/sys/class/gpio/gpio%d/value", red2_num);
    (void)snprintf(yellow2_value, sizeof(yellow2_value), "/sys/class/gpio/gpio%d/value", yellow2_num);
    (void)snprintf(green2_value, sizeof(green2_value), "/sys/class/gpio/gpio%d/value", green2_num);

    // Prompt user for Green light duration
    (void)printf("Enter the duration of green lights for Traffic Signal 1 (in minutes): ");
    (void)scanf("%d", &green_duration_signal1);
    (void)printf("Enter the duration of green lights for Traffic Signal 2 (in minutes): ");
    (void)scanf("%d", &green_duration_signal2);

    // Convert green light durations from minutes to microseconds
    int green_duration_micro_signal1 = green_duration_signal1 * 60 * 1000000;
    int green_duration_micro_signal2 = green_duration_signal2 * 60 * 1000000;

    // Initialize GPIO pins
    if (initialize_gpios(red1_direction, yellow1_direction, green1_direction) == -1 ||
        initialize_gpios(red2_direction, yellow2_direction, green2_direction) == -1) {
        (void)printf("Error with GPIO initialization \n");
        return -1;
    }

    // Call traffic light control function
    traffic_light_control(red1_value, yellow1_value, green1_value,
                          red2_value, yellow2_value, green2_value,
                          green_duration_micro_signal1, green_duration_micro_signal2);

    return 0;
}
