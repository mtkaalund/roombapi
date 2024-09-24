#include <iostream>
#include <string>
// Linux specific headers
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>

// https://raspberry-projects.com/pi/programming-in-c/i2c/using-the-i2c-interface

auto main() -> int {

    int file_i2c;
    int length;
    unsigned char buffer[60] = {0};

    // Opening i2c bus
    std::string filename = "/dev/i2c-1";
    if( (file_i2c = open(filename.c_str(), O_RDWR)) < 0 )
    {
        // TODO: Check errno to get what went wrong
        int err = errno;
        std::cerr << "Failed to open the i2c bus: " << std::endl;
        return -1;
    }

    int i2c_addr = 0x27;
    if( ioctl(file_i2c, I2C_SLAVE, i2c_addr) < 0 )
    {
        // TODO: Check errno to get what went wrong
        close(file_i2c);
        int err = errno;
        std::cerr << "Failed to acquire bus access and/or talk to slave." << std::endl;
        return -1;
    }

    // Read bytes
    length = 4; // Number of bytes to read
    if( read(file_i2c, buffer, length) != length ) {
        // Read
        close(file_i2c);
        int err = errno;
        std::cerr << "Failed to read from the i2c bus." << std::endl;
        return -1;
    }

    std::cout << "Data read: " << buffer << std::endl;

    buffer[0] = 0x01;
    buffer[1] = 0x02;
    length = 2;
    if( write(file_i2c, buffer, length) != length ) {
        close(file_i2c);
        int err = errno;
        std::cerr << "Failed to write to the i2c bus." << std::endl;
        return -1;
    }

    close(file_i2c);

    return 0;
}