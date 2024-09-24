#ifndef RPI_I2C_H
#define RPI_I2C_H

#include <iostream>
#include <string>
#include <cerrno>
#include <cstring>
// Linux specific headers
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
// #include <linux/input.h>
#include <linux/i2c-dev.h>
// #include <linux/i2c.h>

namespace rpi {

// https://raspberry-projects.com/pi/programming-in-c/i2c/using-the-i2c-interface
// I2C for Raspberry Pi
class i2c
{
private:
    int i2c_file;

private:
    void error(std::string msg)
    {
        int err = errno;
        std::cerr << msg << " error: " << strerror(err) << std::endl;
    }

public:
    i2c(int bus)
    {
        std::string file = "/dev/i2c-" + std::to_string(bus);
        if ((this->i2c_file = open(file.c_str(), O_RDWR)) < 0)
        {
            // Failed state
            error("i2c::i2c failed to open");
            exit(1);
        }
    }

    ~i2c()
    {
        close(i2c_file);
    }

    void send(unsigned char addr, unsigned char byte)
    {

        if (ioctl(this->i2c_file, I2C_SLAVE, addr) < 0)
        {
            error("i2c::send Failed to acquire bus access.");
            exit(1);
        }

        // We write the bytes
        unsigned char bytes[1] = {0};
        bytes[0] = byte;
        if (write(this->i2c_file, bytes, 1) != 1)
        {
            error("i2c::send failed to send to i2c bus");
            exit(1);
        }
    }

    void receive( unsigned char * bytes, int length )
    {

        if( read(this->i2c_file, bytes, length) != length)
        {
            error("Failed to read from the i2c");
            exit(1);
        }
    }
};

};

#endif