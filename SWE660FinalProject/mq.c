#include "common.h"

#define AIN_PATH "/sys/bus/iio/devices/iio:device0/in_voltage0_raw"
#define BUFFER_SIZE 10

int read_analog_input() {
    int fd;
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    int analog_value;

    // Open the AIN0 file for reading
    fd = open(AIN_PATH, O_RDONLY);
    if (fd == -1) {
        perror("Error opening AIN0");
        return -1;
    }

    // Read analog value
    bytes_read = read(fd, buffer, BUFFER_SIZE);
    if (bytes_read == -1) {
        perror("Error reading AIN0");
        close(fd);
        return -1;
    }

    // Convert string to integer
    analog_value = atoi(buffer);

    // Close the file descriptor
    close(fd);

    return analog_value;
}

int get_airquality()
{
    int analog_value = read_analog_input();
    double percentage = ((double)analog_value / 4095.0) * 100.0;
    return (int)percentage;
}


// int main() {
//     int analog_value;

//     while (1) {
//         // Read analog input
//         analog_value = read_analog_input();
//         if (analog_value == -1) {
//             fprintf(stderr, "Failed to read analog input\n");
//             break;
//         }

//         // Do something with the analog value (e.g., print it)
//         printf("Analog value: %d\n", analog_value);

//         // Sleep for one second
//         sleep(1);
//     }

//     return 0;
// }
