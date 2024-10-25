#pragma once

#include <Arduino.h>
#include "peripherals/eeprom.h"
#include "logic/preset.h"

constexpr uint16_t c_deviceStateAddress = 0x0;
constexpr uint16_t c_banksStartAddress = 0x80;
constexpr uint16_t c_presetSize = 64;
constexpr uint8_t c_presetsPerBank = 4;

class MemoryManager {
  private:
    Eeprom eeprom;

    /// @brief Calculate the memory address of a preset
    /// @param t_bank Preset bank
    /// @param t_presetIndex Preset index in the bank
    /// @return uint16_t Address of the preset
    uint16_t calculatePresetAddress(uint8_t t_bank, uint8_t t_presetIndex) const;

    /// @brief Serialize a Preset object into a byte array
    /// @param t_preset Preset to serialize
    /// @param t_buffer Data buffer
    void serializePreset(const Preset& t_preset, uint8_t* t_buffer) const;

    /// @brief Deserialize a byte array into a buffer object
    /// @param t_buffer Data buffer
    /// @param t_preset Preset to deserialize the data into
    void deserializePreset(const uint8_t* t_buffer, Preset& t_preset) const;

  public:
    /// @brief Constructor for an SPI EEPROM
    /// @param t_csPin EEPROM CS pin
    MemoryManager(uint8_t t_csPin) :
      eeprom(t_csPin) {
        eeprom.setup();
      };

    /// @brief Saves the device's current state (last bank and preset) to EEPROM
    /// @param t_bank Current bank
    /// @param t_preset Current preset
    void saveDeviceState(uint8_t t_bank, uint8_t t_preset);

    /// @brief Loads the last saved device state (bank and preset) from EEPROM
    /// @param t_bank Saved bank
    /// @param t_preset Saved preset
    void loadDeviceState(uint8_t& t_bank, uint8_t& t_preset);

    /// @brief Saves a specific preset to EEPROM
    /// @param t_bank Current bank
    /// @param t_presetIndex Preset index in the bank
    /// @param t_preset Reference to the preset to save
    void savePreset(uint8_t t_bank, uint8_t t_presetIndex, const Preset& t_preset);

    /// @brief Load a preset from EEPROM
    /// @param t_bank Target bank
    /// @param t_presetIndex Preset index in the bank
    /// @param t_preset Reference to the preset to load into
    void loadPreset(uint8_t t_bank, uint8_t t_presetIndex, Preset& t_preset);

    /// Test functions
    void initializeTestData();
    void readTestData();
};
