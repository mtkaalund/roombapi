#ifndef RPI_I2C_LCD_H
#define RPI_I2C_LCD_H

#include <chrono> // Time library
#include <thread>
#include <mutex>
#include <memory>
#include <string>

#include <i2c.h>

namespace rpi
{

    namespace lcd
    {
        enum class hw
        {
            enable = 0x04, // 0b00000100,
            backlight = 0x08,
            rs = 0x01,
        };

        enum class reg
        {
            instruction = 0x00,
        };

        enum class func
        {
            ClearDisplay = 0x01,
            ReturnHome = 0x02,
            FunctionSet = 0x04,
            Mode4Bit = 0x00,
            Lines2 = 0x08,
            Char5x8 = 0x00,
        };
    }

    class lcd_i2c
    {
    private:
        static lcd_i2c *pInstance;
        static std::mutex mutex;

        std::unique_ptr<rpi::i2c> device;
        unsigned int BusId;

        bool bEnableBacklight = true;

    protected:
        lcd_i2c(const int i2c_bus = 1, const int bus_id) : BusId(bus_id)
        {
            device = std::make_unique<rpi::i2c>(i2c_bus);
            Initialize();
        }

        ~lcd_i2c()
        {
        }

    public:
        // Singletons should not be cloneable.
        lcd_i2c(lcd_i2c &other) = delete;
        // Singletons should not be assignable
        void operator=(const lcd_i2c &) = delete;
        static lcd_i2c *GetInstance(const int &i2c_bus, const int &bus_id);

        // Now we will define the class' functions
    private:
        void Enable(const uint8_t value)
        {
            device.get()->send(BusId, value | static_cast<uint8_t>(lcd::hw::enable));
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            device.get()->send(BusId, value & ~static_cast<uint8_t>(lcd::hw::enable));
            std::this_thread::sleep_for(std::chrono::microseconds(50));
        }

        // Push value out on the i2c bus
        void Push(const uint8_t value, const uint8_t mode)
        {
            uint8_t high = value & 0xF0;
            uint8_t low = (value << 4) & 0xF0;

            device.get()->send(BusId, high | mode | (bEnableBacklight == true ? static_cast<uint8_t>(lcd::hw::backlight) : 0x00));
            Enable(high | mode | (bEnableBacklight == true ? static_cast<uint8_t>(lcd::hw::backlight) : 0x00));
            device.get()->send(BusId, low | mode | (bEnableBacklight == true ? static_cast<uint8_t>(lcd::hw::backlight) : 0x00));
            Enable(low | mode | (bEnableBacklight == true ? static_cast<uint8_t>(lcd::hw::backlight) : 0x00));
        }

        void Instruction(const uint8_t value)
        {
            Push(value, static_cast<uint8_t>(lcd::reg::instruction));
        }

        void Initialize()
        {
            uint8_t mode = static_cast<uint8_t>(lcd::hw::rs);

            Push((0x03 << 4), mode);
            std::this_thread::sleep_for(std::chrono::microseconds(4500));
            Push((0x03 << 4), mode);
            std::this_thread::sleep_for(std::chrono::microseconds(4500));
            Push((0x03 << 4), mode);
            std::this_thread::sleep_for(std::chrono::microseconds(150));
            Push((0x02 << 4), mode);

            // Setting up the display
            uint8_t lcd_functions = static_cast<uint8_t>(lcd::func::Mode4Bit) | static_cast<uint8_t>(lcd::func::Lines2) | static_cast<uint8_t>(lcd::func::Char5x8);

            Instruction(0x20 | lcd_functions);

            // Clearing display
            ClearDisplay();
        }

    public:
        void ClearDisplay()
        {
            Instruction(static_cast<uint8_t>(lcd::func::ClearDisplay));
            std::this_thread::sleep_for(std::chrono::microseconds(2000));
        }

        void ReturnHome()
        {
            Instruction(static_cast<uint8_t>(lcd::func::ReturnHome));
            std::this_thread::sleep_for(std::chrono::microseconds(2000));
        }

        void DisableBacklight()
        {
            bEnableBacklight = false;
        }
        void EnableBacklight()
        {
            bEnableBacklight = true;
        }

        void Character(const uint8_t c)
        {
            Push(c, static_cast<uint8_t>(lcd::hw::rs));
        }

        void String(const std::string str)
        {
            for (auto c : str)
            {
                Character(c);
            }
        }
    };

    lcd_i2c *lcd_i2c::pInstance{nullptr};
    std::mutex lcd_i2c::mutex;

    lcd_i2c *lcd_i2c::GetInstance(const int &i2c_bus, const int &bus_id)
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (pInstance == nullptr)
        {
            pInstance = new lcd_i2c(i2c_bus, bus_id);
        }

        return pInstance;
    }

};

#endif