#pragma once

#include <Arduino.h>
#include "peripherals/eeprom.h"
#include "logic/preset.h"
#include "logic/footswitch.h"

constexpr uint16_t c_deviceStateAddress = 0x0;
constexpr uint16_t c_banksStartAddress = 0x20;
constexpr uint16_t c_footSwitchConfigStartAddress = 0x900;

/*
 * Memory Map for Preset Storage in EEPROM
 * Total Size per Preset: 128 bytes
 * This is the absolute max values, if there are only 8 loops for example the MIDI message storage
 * will start right after the loops storage
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

/*
 * Memory Map for FootSwitchConfig in EEPROM
 * Total Size per FootSwitchConfig: 11 bytes
 * This layout supports a configuration for each footswitch, including latching mode, loop toggling,
 * bank/preset selection, and up to two MIDI messages.
 *
 * Byte Range       Field Name            Description                             Example Value
 * ---------------------------------------------------------------------------------------------
 * 0                mode                  Footswitch mode (ToggleLoop, SendMidiMessage, etc.)   2
 * 1                latching              Latching setting for the footswitch (0 = momentary, 1 = latching)  1
 * 2                loopIndex             Index of the loop to toggle (if applicable)          3
 * 3                targetBank            Target bank number for bank change mode              1
 * 4                targetPreset          Target preset number for preset select mode          2
 *
 * 5-7              midiMessage[0]        First MIDI message configuration                     (3 bytes)
 *                   |                      - statusByte                                       0xB0
 *                   |                      - dataByte1                                        32
 *                   |                      - dataByte2                                        64
 *
 * 8-10             midiMessage[1]        Second MIDI message configuration                    (3 bytes)
 *                   |                      - statusByte                                       0xC0
 *                   |                      - dataByte1                                        64
 *                   |                      - dataByte2                                        127
 */
constexpr uint8_t c_footSwitchConfigSize = 11;
constexpr uint8_t c_footSwitchConfigPerBank = 6;

class MemoryManager {
  private:
    Eeprom eeprom;

    /// @brief Calculate the memory address of a preset
    /// @param t_bank Preset bank
    /// @param t_presetIndex Preset index in the bank
    /// @return uint16_t Address of the preset
    uint16_t calculatePresetAddress(uint8_t t_bank, uint8_t t_presetIndex) const;

    /// @brief Calculate the memory address of a footswitch config
    /// @param t_bank Preset bank
    /// @param t_footSwitchIndex Footswitch index
    /// @return uint16_t Address of the footswitch config
    uint16_t calculateFootSwitchConfigAddress(uint8_t t_bank, uint8_t t_footSwitchIndex) const;

    /// @brief Serialize a Preset object into a byte array
    /// @param t_preset Preset to serialize
    /// @param t_buffer Data buffer
    void serializePreset(const Preset& t_preset, uint8_t* t_buffer) const;

    /// @brief Deserialize a byte array into a buffer object
    /// @param t_buffer Data buffer
    /// @param t_preset Preset to deserialize the data into
    void deserializePreset(const uint8_t* t_buffer, Preset& t_preset) const;

    /// @brief Serialize a FootSwitchConfig object into a byte array
    /// @param t_config FootSwitchConfig to serialize
    /// @param t_buffer Data buffer
    void serializeFootSwitchConfig(const FootSwitchConfig& t_config, uint8_t* t_buffer) const;

    /// @brief Deserialize a byte array into a FootSwitchConfig
    /// @param t_buffer Data buffer
    /// @param t_config FootSwitchConfig object to deserialize data into
    void deserializeFootSwitchConfig(const uint8_t* t_buffer, FootSwitchConfig& t_config) const;


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

    /// @brief Save a FootSwitchConfig to EEPROM
    /// @param t_bank Current bank
    /// @param t_footSwitchIndex FootSwitchConfig index in the bank
    /// @param t_config Reference to the FootSwitchConfig to save
    void saveFootSwitchConfig(uint8_t t_bank, uint8_t t_footSwitchIndex, const FootSwitchConfig& t_config);

    /// @brief Load a FootSwitchConfig from EEPROM
    /// @param t_bank Target bank
    /// @param t_footSwitchIndex FootSwitchConfig index in the bank
    /// @param t_config Reference to the FootSwitchObject to load data into
    void loadFootSwitchConfig(uint8_t t_bank, uint8_t t_footSwitchIndex, FootSwitchConfig& t_config);


    /// Test functions
    void initializeTestData();
    void readTestData();
};
