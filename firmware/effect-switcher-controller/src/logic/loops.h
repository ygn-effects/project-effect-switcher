#pragma once

#include <Arduino.h>

/// @brief Represents an audio loop where each loop has a state,
/// order, and a send/return loop mapped to a switch matrix.
class Loop {
  private:
    uint8_t m_loopState;   // State of the loop (on/off).
    uint8_t m_loopOrder;   // Order of the loop.
    uint8_t m_loopSend;    // Send channel of the loop.
    uint8_t m_loopReturn;  // Return channel of the loop.

  public:
    /// @brief Default constructor that initializes all loop parameters to 0.
    Loop() : m_loopState(0), m_loopOrder(0), m_loopSend(0), m_loopReturn(0) {}

    /// @brief Parametrized constructor to set the initial state, order, send, and return.
    /// @param t_state Initial state of the loop (on/off).
    /// @param t_order Initial order of the loop.
    /// @param t_send Initial send channel of the loop.
    /// @param t_return Initial return channel of the loop.
    Loop(uint8_t t_state, uint8_t t_order, uint8_t t_send, uint8_t t_return) :
      m_loopState(t_state),
      m_loopOrder(t_order),
      m_loopSend(t_send),
      m_loopReturn(t_return) { };

    /// @brief Get the current state of the loop.
    /// @return uint8_t The current state (on/off).
    uint8_t getLoopState() const;

    /// @brief Set the state of the loop (on/off).
    /// @param t_state The new state of the loop.
    void setLoopState(uint8_t t_state);

    /// @brief Get the current order of the loop.
    /// @return uint8_t The current order of the loop.
    uint8_t getLoopOrder() const;

    /// @brief Set the order of the loop.
    /// @param t_order The new order for the loop.
    void setLoopOrder(uint8_t t_order);

    /// @brief Get the current send channel of the loop.
    /// @return uint8_t The current send channel.
    uint8_t getLoopSend() const;

    /// @brief Set the send channel of the loop.
    /// @param t_send The new send channel for the loop.
    void setLoopSend(uint8_t t_send);

    /// @brief Get the current return channel of the loop.
    /// @return uint8_t The current return channel.
    uint8_t getLoopReturn() const;

    /// @brief Set the return channel of the loop.
    /// @param t_ret The new return channel for the loop.
    void setLoopReturn(uint8_t t_ret);

    /// @brief Toggle the current state of the loop (on/off).
    void toggleLoopState();
};
