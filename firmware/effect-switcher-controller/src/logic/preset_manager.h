#pragma once

#include <Arduino.h>
#include "logic/preset.h"
#include "logic/memory.h"

constexpr uint8_t c_maxPresetBanks = 16;
constexpr uint8_t c_maxPresetsPerBank = 4;

/// @brief Interface between the HAL and the presets to
/// securely handle banks and presets switching operations
/// and storage
class PresetManager {
  private:
    MemoryManager& m_memoryManager;

    uint8_t m_currentPresetBank;
    uint8_t m_currentPresetIndex;

    Preset m_presetBanks[c_maxPresetsPerBank];
    Preset* p_currentPreset;

    void loadPresetBank(uint8_t t_bank);

  public:
    /// @brief Default constructor
    /// @param t_memoryManager Reference to the MemoryManager object
    PresetManager(MemoryManager& t_memoryManager) :
      m_memoryManager(t_memoryManager),
      m_currentPresetBank(0),
      m_currentPresetIndex(0),
      p_currentPreset(nullptr) {
        for (uint8_t i = 0; i < c_maxPresetsPerBank; i++) {
          m_presetBanks[i] = Preset();
        }
      }

    /// @brief Load the last bank and preset from storage and
    /// initialize the object's members
    void initialize();

    /// @brief Get the current bank
    /// @return uint8_t Current bank
    uint8_t getCurrentBank() const;

    /// @brief Set the current bank and load it from storage
    /// @param t_bank Bank number to load
    void setPresetBank(uint8_t t_bank);

    /// @brief Go up a bank
    void setPresetBankUp();

    /// @brief Go down a bank
    void setPresetBankDown();

    /// @brief Get a pointer to the current Preset object
    /// @return Pointer*
    Preset* getCurrentPreset() const;

    /// @brief Get the current preset index
    /// @return uint8_t Preset index
    uint8_t getCurrentPresetIndex() const;

    /// @brief Set the current preset
    /// @param t_preset Preset index
    void setCurrentPreset(uint8_t t_presetIndex);

    /// @brief Save the current preset to storage
    void saveCurrentPreset();
};
