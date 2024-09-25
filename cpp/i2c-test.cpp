#include <iostream>

#include "i2c.h"

namespace lcd {
enum class command {
    clear_display   = 0x01,
    return_home     = 0x02,
    entry_mode_set  = 0x04,
    display_ctrl    = 0x08,
    cursor_shift    = 0x10,
    function_set    = 0x20,
    set_CGRAM_addr  = 0x40,
    set_DRAM_addr   = 0x80
};

enum class display_entry_mode
{
    entry_right     = 0x00,
    entry_left      = 0x02,
    increment       = 0x01,
    decrement       = 0x00
};

enum class display_control
{
    display_on      = 0x04,
    display_off     = 0x00,
    cursor_on       = 0x02,
    cursor_off      = 0x00,
    blink_on        = 0x01,
    blink_off       = 0x00
};

};


// Pinout PCF8574 -> LCD
// P0 = RS
// P1 = RW
// P2 = CS
// P3 = P3
// P4 = DB4
// P5 = DB5
// P6 = DB6
// P7 = DB7
#define RS (1 << 0)
#define RW (1 << 1)
#define CS (1 << 2)
#define P3 (1 << 3)
#define DB4 (1 << 4)
#define DB5 (1 << 5)
#define DB6 (1 << 6)
#define DB7 (1 << 7)

auto main() -> int
{

    rpi::i2c i2c_device = rpi::i2c(1);

    i2c_device.send(0x27, P3);

    return 0;
}