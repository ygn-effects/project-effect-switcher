#pragma once

#include <Arduino.h>
#include "peripherals/eeprom.h"
#include "logic/preset.h"

constexpr uint16_t c_deviceStateAddress = 0x0;
constexpr uint16_t c_banksStartAddress = 0x80;

/*
 * Memory Map for Preset Storage in EEPROM
 * Total Size per Preset: 128 bytes
 *
 * Byte Range       Field Name         Description                             Example Value
 * -----------------------------------------------------------------------------------------
 * 0                bank               Stores the preset bank number           1
 * 1                preset             Stores the preset number within bank    5
 * 2                loopsCount         Number of audio loops in this preset    16
 * 3                midiMessagesCount  Number of MIDI messages in this preset  20
 *
 * 4-67             loops              Audio loop configuration                (4 bytes per loop)
 *                   |                   - loopState                           (1 byte each)
 *                   |                   - loopOrder                           (1 byte each)
 *                   |                   - loopSend                            (1 byte each)
 *                   |                   - loopReturn                          (1 byte each)
 *                  Range: 4 * maxLoops = 4 * 16 = 64 bytes max
 *
 * 68-127           midiMessages       MIDI message configuration              (4 bytes per message)
 *                   |                   - statusByte                          (1 byte each)
 *                   |                   - dataByte1                           (1 byte each)
 *                   |                   - dataByte2                           (1 byte each)
 *                  Range: 4 * maxMIDI = 3 * 20 = 60 bytes max
 */
constexpr uint16_t c_presetSize = 128;
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
