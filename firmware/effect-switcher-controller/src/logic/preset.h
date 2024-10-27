#pragma once

#include <Arduino.h>
#include "logic/loops.h"
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
    /// @param t_loopsCount Loops count
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
    uint8_t getLoopsCount() const;

    /// @brief Set the number of loops in the preset
    /// @param t_count Loops count
    void setLoopsCount(uint8_t t_count);

    /// @brief Get the state of a specific loop
    uint8_t getLoopState(uint8_t t_loop) const;

    /// @brief Set a loop state
    /// @param t_loop Loop index
    /// @param t_state Loop state
    void setLoopState(uint8_t t_loop, uint8_t t_state);

    /// @brief Get the order of a specific loop
    uint8_t getLoopOrder(uint8_t t_loop) const;

    /// @brief Set a loop order
    /// @param t_loop Loop index
    /// @param t_order Loop order
    void setLoopOrder(uint8_t t_loop, uint8_t t_order);

    /// @brief Get the send value of a specific loop
    uint8_t getLoopSend(uint8_t t_loop) const;

    /// @brief Set a loop send value
    /// @param t_loop Loop index
    /// @param t_send Loop send value
    void setLoopSend(uint8_t t_loop, uint8_t t_send);

    /// @brief Get the return value of a specific loop
    uint8_t getLoopReturn(uint8_t t_loop) const;

    /// @brief Set a loop return value
    /// @param t_loop Loop index
    /// @param t_return Loop return value
    void setLoopReturn(uint8_t t_loop, uint8_t t_return);

    /// @brief Toggle the state of a specific loop
    void toggleLoopState(uint8_t t_loop);

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
