#pragma once

#include <Arduino.h>
#include "loops.h"
#include "utils/logging.h"

/// @brief Represents a preset that contains a bank, a preset number, and its associated loops.
class Preset {
  private:
    uint8_t m_bank;          // Bank number
    uint8_t m_preset;        // Preset number
    Loops* m_presetLoops;    // Pointer to the associated loops

  public:
    /// @brief Default constructor
    Preset() : m_bank(0), m_preset(0), m_presetLoops(nullptr) { };

    /// @brief Parameterized constructor
    /// @param t_bank Bank number
    /// @param t_preset Preset number
    /// @param t_loops Pointer to the loops object
    Preset(uint8_t t_bank,
      uint8_t t_preset,
      Loops* t_loops
    ) :
      m_bank(t_bank),
      m_preset(t_preset),
      m_presetLoops(t_loops) { };

    /// @brief Setup the preset with the given bank, preset number, and loops
    /// @param t_bank Bank number
    /// @param t_preset Preset number
    /// @param t_loops Pointer to the loops object
    /// @param t_loopsCount Number of loops
    void setup(uint8_t t_bank, uint8_t t_preset, Loops* t_loops, uint8_t t_loopsCount);

    /// @brief Get the bank number
    /// @return Bank number
    uint8_t getBank() const;

    /// @brief Set the bank number
    /// @param t_bank Bank number
    void setBank(uint8_t t_bank);

    /// @brief Get the preset number
    /// @return Pointer to the preset number
    uint8_t* getPreset();

    /// @brief Set the preset number
    /// @param t_preset Preset number
    void setPreset(uint8_t t_preset);

    /// @brief Get the loop IDs for this preset
    /// @return Pointer to the loop IDs
    uint8_t* getPresetLoopsId();

    /// @brief Get the loop states for this preset
    /// @return Pointer to the loop states
    uint8_t* getPresetLoopsStates();

    /// @brief Get the order of loops for this preset
    /// @return Pointer to the loop order
    uint8_t* getPresetLoopsOrder();

    /// @brief Swap the order of two loops in the preset
    /// @param t_id1 Loop 1 ID
    /// @param t_order1 Loop 1 order
    /// @param t_id2 Loop 2 ID
    /// @param t_order2 Loop 2 order
    void swapPresetLoopsOrder(uint8_t t_id1, uint8_t t_order1, uint8_t t_id2, uint8_t t_order2);

    /// @brief Get the loop ID based on its order in the preset
    /// @param t_order Loop order
    /// @return Loop ID
    uint8_t getPresetLoopIdByOrder(uint8_t t_order);

    /// @brief Set the send and return state of a loop
    void setPresetLoopSendReturn(uint8_t t_loop, uint8_t t_send, uint8_t t_ret);

    /// @brief Get the send state of a loop
    uint8_t getPresetLoopSend(uint8_t t_loop);

    /// @brief Get the return state of a loop
    uint8_t getPresetLoopReturn(uint8_t t_loop);

    /// @brief Set the state of a loop (on/off)
    void setLoopState(uint8_t t_loop, uint8_t t_state);

    /// @brief Get the state of a loop (on/off)
    uint8_t getLoopState(uint8_t t_loop);

    /// @brief Toggle the state of a loop (on/off)
    void toggleLoopState(uint8_t t_loop);

    /// @brief Get the number of active loops in the preset
    /// @return Number of active loops
    uint8_t getActiveLoopsCount();
};
