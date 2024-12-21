#pragma once

#include <Arduino.h>

/// @brief Base class for the LED current sink drivers using SPI
class LedDriver {
  private:
    uint8_t m_csPin;

  protected:
    uint32_t m_blinkTime = 0;
    uint32_t m_lastBlinkTime = 0;
    uint8_t m_lastBlinkState = 0;
    uint16_t m_ledDriverMask = 0;

    /// @brief Setup the SPI bus and select the chip
    void select();

    /// @brief Deselect the chip and close the SPI bus
    void deselect();

  public:
    /// @brief Construct a new LedDriver object
    /// @param t_cspin CS pin #
    LedDriver(uint8_t t_cspin) :
      m_csPin(t_cspin) { };

    /// @brief Setup the µC's pins and start the SPI bus
    void setup();
};

/// @brief Drives an 8-bit current sink driver with 8 inputs.
class LedDriver8 : public LedDriver {
  public:
    /// @brief Construct a new LedDriver8 object
    /// @param t_cspin CS pin #
    LedDriver8(uint8_t t_cspin) :
      LedDriver(t_cspin) { };

    /// @brief Light the selected LED
    /// @param t_led LED #
    void lightLed(uint8_t t_led);

    /// @brief Turn all LEDs off
    void turnOffAll();
};

/// @brief Drives a 16-bit current sink driver with 16 inputs.
/// It can manage 16 LEDs or 8 bicolor LEDs.
class LedDriver16 : public LedDriver {
  public:
    /// @brief Construct a new LedDriver16 object
    /// @param t_cspin CS pin #
    LedDriver16(uint8_t t_cspin) :
      LedDriver(t_cspin) { };

    /// @brief Light the selected LED
    /// @param t_led LED #
    void lightLed(uint8_t t_led);

    /// @brief Light two LEDs simultaneously
    /// @param t_led LED #
    void lightTwoLeds(uint8_t t_led);

    /// @brief Turn all LEDs off
    void turnOffAll();

    /// @brief Blink an LED with a given interval
    /// @param t_led LED #
    /// @param t_interval Interval in ms
    void blinkLed(uint8_t t_led, uint8_t t_interval);

    /// @brief Blink two LEDs with a given interval
    /// @param t_led LED #
    /// @param t_interval Interval in ms
    void blinkTwoLeds(uint8_t t_led, uint8_t t_interval);

    /// @brief Reset the blinking state
    void resetBlink();

    /// @brief Set the driver's state using a 16-bit mask variable
    /// @param t_mask Driver's mask
    void setLedStateByMask(uint16_t t_mask);

    /// @brief Get the current driver mask (LED state)
    /// @return uint16_t Current mask
    uint16_t getLedStateByMask() const;
};
