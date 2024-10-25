#pragma once

#include <Arduino.h>


/// @brief Represents an audio loop where each loop has a state,
/// order, and a send/return loop mapped to a switch matrix
class Loop {
  private:
    uint8_t m_loopState;
    uint8_t m_loopOrder;
    uint8_t m_loopSend;
    uint8_t m_loopReturn;

  public:
    /// @brief Default constructor
    Loop() : m_loopState(0), m_loopOrder(0), m_loopSend(0), m_loopReturn(0) {}

    /// @brief Parametrized constructor
    /// @param t_state Loop state
    /// @param t_order Loop order
    /// @param t_send Loop send #
    /// @param t_returnLoop return #
    Loop(uint8_t t_state,
      uint8_t t_order,
      uint8_t t_send,
      uint8_t t_return
    ) :
      m_loopState(t_state),
      m_loopOrder(t_order),
      m_loopSend(t_send),
      m_loopReturn(t_return) { };

    /// @brief Get the state of a loop
    uint8_t getLoopState() const;

    /// @brief Set the state of a loop (on/off)
    void setLoopState(uint8_t t_state);

    /// @brief Get the order of a loop
    uint8_t getLoopOrder() const;

    /// @brief Set the order of a loop
    void setLoopOrder(uint8_t t_order);

    /// @brief Get the send state of a loop
    uint8_t getLoopSend() const;

    /// @brief Set the send state of a loop
    void setLoopSend(uint8_t t_send);

    /// @brief Get the return state of a loop
    uint8_t getLoopReturn() const;

    /// @brief Set the return state of a loop
    void setLoopReturn(uint8_t t_ret);

    /// @brief Toggle the state of a loop (on/off)
    void toggleLoopState();
};
