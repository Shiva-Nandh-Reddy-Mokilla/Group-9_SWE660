#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define ADS111x_ADDR 0x48 // Default I2C address for ADS111x
#define ADS111x_CONFIG_REG 0x01 // Address of the configuration register
#define ADS111x_CONVERSION_REG 0x00 // Address of the conversion register

#define CALIBRATION_VALUE 19633
#define CALIBRATION_PH_14_ADC_VALUE 27223
#define CALIBRATION_PH_14_PH_VALUE 14.0

// Function to open the I2C bus and check for ADS111x presence
float get_ph_value() {
    int i2c_fd = open("/dev/i2c-2", O_RDWR);
    if (i2c_fd < 0) {
        perror("Failed to open the I2C bus");
        return -1;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, ADS111x_ADDR) < 0) {
        perror("Failed to acquire bus access and/or talk to slave");
        close(i2c_fd);
        return -1;
    }

    // Check if ADS111x is present by reading its configuration register
    unsigned char config[2] = {ADS111x_CONFIG_REG};
    if (write(i2c_fd, config, 1) != 1) {
        perror("Failed to write to the I2C bus");
        close(i2c_fd);
        return -1;
    }
    if (read(i2c_fd, config, 2) != 2) {
        perror("Failed to read from the I2C bus");
        close(i2c_fd);
        return -1;
    }

    // Check if the most significant bit of the configuration register is set
    if ((config[0] & 0x80) == 0) {
        printf("ADS111x not detected at address 0x%x\n", ADS111x_ADDR);
        close(i2c_fd);
        return -1;
    }

    // Config for A0 single-ended input, +/-6.144V range, continuous conversion mode
    unsigned char config_data[3] = {ADS111x_CONFIG_REG, 0xC3, 0x83};
    if (write(i2c_fd, config_data, 3) != 3) {
        perror("Failed to write to the I2C bus");
        close(i2c_fd);
        return -1;
    }

    // Wait for conversion to complete
    sleep(1);

    // Read ADC value
    unsigned char conversion[2] = {ADS111x_CONVERSION_REG};
    if (write(i2c_fd, conversion, 1) != 1) {
        perror("Failed to write to the I2C bus");
        close(i2c_fd);
        return -1;
    }
    if (read(i2c_fd, conversion, 2) != 2) {
        perror("Failed to read from the I2C bus");
        close(i2c_fd);
        return -1;
    }
    
    int adc_value = (conversion[0] << 8) | conversion[1];
    close(i2c_fd);

    // Calculate the slope (m) and y-intercept (c) using two calibration points
    float slope = (CALIBRATION_PH_14_PH_VALUE - 7.0) / (CALIBRATION_PH_14_ADC_VALUE - CALIBRATION_VALUE);
    float y_intercept = CALIBRATION_PH_14_PH_VALUE - slope * CALIBRATION_PH_14_ADC_VALUE;
    
    // Convert ADC value to pH using the linear equation: pH = m * ADC + c
    float pH = slope * adc_value + y_intercept;
    return pH;
}
