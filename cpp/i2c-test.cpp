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

auto main() -> int
{

    rpi::i2c i2c_device = rpi::i2c(1);

    i2c_device.send(0x27, 0x08);

    return 0;
}