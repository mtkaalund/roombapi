#include <iostream>

#include <thread>
#include <chrono>

#include "i2c.h"

namespace lcd
{
    enum class command
    {
        clear_display = 0x01,
        return_home = 0x02,
        entry_mode_set = 0x04,
        display_ctrl = 0x08,
        cursor_shift = 0x10,
        function_set = 0x20,
        set_CGRAM_addr = 0x40,
        set_DRAM_addr = 0x80
    };

    enum class display_entry_mode
    {
        entry_right = 0x00,
        entry_left = 0x02,
        increment = 0x01,
        decrement = 0x00
    };

    enum class display_control
    {
        display_on = 0x04,
        display_off = 0x00,
        cursor_on = 0x02,
        cursor_off = 0x00,
        blink_on = 0x01,
        blink_off = 0x00
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
#define BL (1 << 3) // Backlight
#define DB4 (1 << 4)
#define DB5 (1 << 5)
#define DB6 (1 << 6)
#define DB7 (1 << 7)

auto main() -> int
{

    rpi::i2c i2c_device = rpi::i2c(1);

    // unsigned char c = 0x00;

    // This will be 4-bit operation mode
    // Setup lcd display
    //       RS   RW  DB7     DB6     DB5     DB4
    // 1:      0    0   0       0       1       0      // Sets to 4-bit operations
    // 2:      0    0   0       0       1       0      // Sets 4-bit operations and 1-line 5x8
    //        0    0   0       0       *       *
    // 3:      0    0   0       0       0       0      // Display on. Cursor on
    //        0    0   0       1       1       0
    // 4:      0    0   0       0       0       0      // Set mode to increment and shift right
    //        0    0   0       1       1       0
    // 5:      1    0   0       1       0       0      // Writes H
    //        1    0   1       0       0       0
    //
    const uint8_t i2c_addr = 0x27;
    const uint8_t backlight = 0x08;
    const uint8_t enable = 0b00000100;

    const uint8_t rs = 0x01;
    uint8_t mode = rs;

    auto lcd_nibble = [&](uint8_t value)
    {
        // enable
        i2c_device.send(i2c_addr, value | enable);
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        i2c_device.send(i2c_addr, value & ~enable);
        std::this_thread::sleep_for(std::chrono::microseconds(50));
    };

    auto lcd_send = [&](uint8_t value, uint8_t mode)
    {
        uint8_t high = value & 0xF0;
        uint8_t low = (value << 4) & 0xF0;

        i2c_device.send(i2c_addr, high | mode | backlight);
        lcd_nibble(high | mode | backlight);

        i2c_device.send(i2c_addr, low | mode | backlight);
        lcd_nibble(low | mode | backlight);
    };

    auto lcd_cmd = [&](uint8_t c) {
        lcd_send(c, 0x00);
    };

    auto lcd_clear = [&]()
    {
        // clear display
        lcd_cmd(0x01);
        std::this_thread::sleep_for(std::chrono::microseconds(2000));
    };

    auto lcd_init = [&]()
    {
        lcd_send((0x03 << 4), mode);
        std::this_thread::sleep_for(std::chrono::microseconds(4500));
        lcd_send((0x03 << 4), mode);
        std::this_thread::sleep_for(std::chrono::microseconds(4500));
        lcd_send((0x03 << 4), mode);
        std::this_thread::sleep_for(std::chrono::microseconds(150));
        lcd_send((0x02 << 4), mode);

        // Setting up the display
        const uint8_t lcd_functionset = 0x20;
        const uint8_t lcd_4bitmode = 0x00;
        const uint8_t lcd_2line = 0x08;
        const uint8_t lcd_char5x8dots = 0x00;

        uint8_t lcd_functions = lcd_4bitmode | lcd_2line | lcd_char5x8dots;
        lcd_cmd(lcd_functionset | lcd_functionset);

        // Clearing display
        lcd_clear();
    };

    auto lcd_char = [&](uint8_t c) {
        lcd_send(c, rs);
    };

    // command is send with mode set to 0x00
    std::cout << "lcd start" << std::endl;
    lcd_init();

    lcd_char('h');
    lcd_char('e');
    lcd_char('l');
    lcd_char('l');
    lcd_char('o');
    lcd_char(',');
    lcd_char(' ');
    lcd_char('w');
    lcd_char('o');
    lcd_char('r');
    lcd_char('l');
    lcd_char('d');

    std::cout << "End of program" << std::endl;

    return 0;
}