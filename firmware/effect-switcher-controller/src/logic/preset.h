#pragma once

#include <Arduino.h>
#include "loops.h"
#include "utils/logging.h"

constexpr uint8_t c_maxLoops = 16;

/// @brief Represents a preset that contains a bank, a preset number,
/// and an array of loops
class Preset {
  private:
    uint8_t m_bank;
    uint8_t m_preset;
    uint8_t m_loopsCount;
    Loop m_loops[c_maxLoops];

  public:
    /// @brief Default constructor
    Preset() : m_bank(0), m_preset(0) { };

    /// @brief Parameterized constructor
    /// @param t_bank Bank number
    /// @param t_preset Preset number
    /// @param t_loopsCount Pointer to the loops object
    Preset(uint8_t t_bank,
      uint8_t t_preset,
      uint8_t t_loopsCount
    ) :
      m_bank(t_bank),
      m_preset(t_preset),
      m_loopsCount(t_loopsCount) { };

    /// @brief Get the bank number
    uint8_t getBank() const;

    /// @brief Set the bank number
    void setBank(uint8_t t_bank);

    /// @brief Get the preset number
    uint8_t getPreset() const;

    /// @brief Set the preset number
    void setPreset(uint8_t t_preset);

    /// @brief Get the number of loops in the preset
    uint8_t getLoopsCount() const {
      return m_loopsCount;
    }

    /// @brief Get the state of a specific loop
    uint8_t getLoopState(uint8_t t_loop) const;

    /// @brief Toggle the state of a specific loop
    void toggleLoopState(uint8_t t_loop) {
      m_loops[t_loop].toggleLoopState();
    }

    /// @brief Swap the order of two loops in the preset
    /// @param t_id1 Loop 1
    /// @param t_order1 Loop 1 order
    /// @param t_id2 Loop 2
    /// @param t_order2 Loop 2 order
    void swapPresetLoopsOrder(uint8_t t_loop1, uint8_t t_order1, uint8_t t_loop2, uint8_t t_order2);

    /// @brief Set the send and return state of a loop
    void setPresetLoopSendReturn(uint8_t t_loop, uint8_t t_send, uint8_t t_return);

    /// @brief Get the send state of a loop
    uint8_t getPresetLoopSend(uint8_t t_loop);

    /// @brief Get the return state of a loop
    uint8_t getPresetLoopReturn(uint8_t t_loop);
};
