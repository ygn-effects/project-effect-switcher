#pragma once

#include <Arduino.h>
#include "utils/logging.h"

/// @brief Represents an array of audio loops, where each loop has an ID, state, order, send/return
class Loops {
  private:
    uint8_t m_loopsId[16];      // Loop IDs
    uint8_t m_loopsState[16];   // Loop states (on/off)
    uint8_t m_loopsOrder[16];   // Loop order
    uint8_t m_loopsSend[16];    // Loop send state
    uint8_t m_loopsReturn[16];  // Loop return state
    uint8_t m_loopsCount;       // Number of loops

  public:
    /// @brief Default constructor
    Loops() : m_loopsCount(0) { };

    /// @brief Parameterized constructor
    /// @param t_count Number of loops
    Loops(uint8_t t_count) :
        m_loopsCount(t_count) { };

    /// @brief Initialize the loop IDs and states
    void loopsSetup();

    /// @brief Get the loop IDs
    /// @return Pointer to the loop IDs
    uint8_t* getLoopsId();

    /// @brief Get the loop states
    /// @return Pointer to the loop states
    uint8_t* getLoopsState();

    /// @brief Get the state of a loop
    uint8_t getLoopState(uint8_t t_loop);

    /// @brief Set the state of a loop (on/off)
    void setLoopState(uint8_t t_loop, uint8_t t_state);

    /// @brief Get the loop order
    uint8_t* getLoopsOrder();

    /// @brief Get the order of a specific loop
    uint8_t getLoopOrder(uint8_t t_loop);

    /// @brief Get the loop ID by its order in the chain
    uint8_t getLoopIdByOrder(uint8_t t_order);

    /// @brief Set the order of a specific loop
    void setLoopOrder(uint8_t t_loop, uint8_t t_order);

    /// @brief Get the send state of a loop
    uint8_t getLoopSend(uint8_t t_loop);

    /// @brief Set the send state of a loop
    void setLoopSend(uint8_t t_loop, uint8_t t_send);

    /// @brief Get the return state of a loop
    uint8_t getLoopReturn(uint8_t t_loop);

    /// @brief Set the return state of a loop
    void setLoopReturn(uint8_t t_loop, uint8_t t_ret);

    /// @brief Get the number of loops
    uint8_t getLoopsCount();

    /// @brief Set the number of loops
    void setLoopsCount(uint8_t t_count);

    /// @brief Toggle the state of a loop (on/off)
    void toggleLoopsState(uint8_t t_loop);
};
