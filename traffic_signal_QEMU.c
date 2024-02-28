#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// Typedef for GPIO pin path
typedef char gpio_path[50];


#define DELAY_TWOSECONDS 2000000

#define DELAY_FIVESECONDS 2000000
static void initialize_gpios();

// Function to initialize GPIO pins (with print statements)
static void initialize_gpios() 
{
    (void)printf("Initializing GPIO pins...\n");
    return;
}

// Function to write to GPIO (with print statements)
static void writeGPIO(gpio_path light, int value);
static void writeGPIO(gpio_path light, int value) 
{
    (void)printf("Writing to GPIO: %s, value: %d\n", light, value);   
    return;
}

static void traffic_light_control(int green_duration_signal1,int green_duration_signal2);
// Function to control traffic lights (with print statements)
static void traffic_light_control(int green_duration_signal1,int green_duration_signal2) 
{
    (void)printf("Starting traffic light control...\n");
    while (1) 
    {
        (void)printf("Red light 1 is on\n");
        (void)printf("Green light 2 is on\n");
        (void)usleep(green_duration_signal2); // Wait for the duration of the green light for signal 2
        (void)printf("Yellow light 1 is on\n");
        (void)usleep(DELAY_TWOSECONDS);
        (void)printf("All lights off for transition\n");
        // Traffic Signal 2 Sequence
        (void)usleep(DELAY_FIVESECONDS);
        (void)printf("Red light 2 is on\n");
        (void)printf("Green light 1 is on\n");
        (void)usleep(green_duration_signal1); // Wait for the duration of the green light for signal 1
        (void)printf("Yellow light 2 is on\n");
        (void)usleep(DELAY_TWOSECONDS); // Wait for 2 seconds
        (void)printf("All lights off for transition\n"); // Turn off all lights for transition
    }
    return;
}

int main(void) {
    // Variables to store GPIO pin paths
    gpio_path red1_direction="";
    gpio_path  yellow1_direction="";
    gpio_path green1_direction="";
    gpio_path red1_value="";
    gpio_path yellow1_value="";
    gpio_path  green1_value="";
    gpio_path red2_direction=""; 
    gpio_path yellow2_direction="";
    gpio_path green2_direction="";
    gpio_path red2_value="";
    gpio_path yellow2_value="";
    gpio_path green2_value="";
    int green_duration_signal1=0;
    int green_duration_signal2=0;

    // Prompt user for GPIO pin numbers
    (void)printf("Enter GPIO pin numbers for Traffic Signal 1 (Red Yellow Green): ");
    (void)scanf("%s %s %s", red1_direction, yellow1_direction, green1_direction);
    (void)printf("Enter GPIO pin numbers for Traffic Signal 2 (Red Yellow Green): ");
    (void)scanf("%s %s %s", red2_direction, yellow2_direction, green2_direction);

    // Prompt user for Green light duration
    (void)printf("Enter the duration of green lights for Traffic Signal 1 (in minutes): ");
    (void)scanf("%d", &green_duration_signal1);
    (void)printf("Enter the duration of green lights for Traffic Signal 2 (in minutes): ");
    (void)scanf("%d", &green_duration_signal2);

    // Initialize GPIO pins
    initialize_gpios();
    

    // Call traffic light control function
    traffic_light_control(green_duration_signal1,green_duration_signal2);

    return 0;
}
