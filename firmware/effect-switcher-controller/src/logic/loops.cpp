#include "loops.h"

void Loops::loopsSetup() {
  for (uint8_t i = 0; i < m_loopsCount; i++) {
    m_loopsId[i] = i;
    m_loopsState[i] = 0;  // Set all loops to off by default
  }
}

uint8_t* Loops::getLoopsId() {
  return m_loopsId;
}

uint8_t* Loops::getLoopsState() {
  return m_loopsState;
}

uint8_t Loops::getLoopState(uint8_t t_loop) {
  return m_loopsState[t_loop];
}

void Loops::setLoopState(uint8_t t_loop, uint8_t t_state) {
  m_loopsState[t_loop] = t_state;
}

uint8_t* Loops::getLoopsOrder() {
  return m_loopsOrder;
}

uint8_t Loops::getLoopOrder(uint8_t t_loop) {
  return m_loopsOrder[t_loop];
}

uint8_t Loops::getLoopIdByOrder(uint8_t t_order) {
  for (uint8_t i = 0; i < m_loopsCount; i++) {
    if (m_loopsOrder[i] == t_order) {
      return i;
    }
  }

  return 0xFF;
}

void Loops::setLoopOrder(uint8_t t_loop, uint8_t t_order) {
  m_loopsOrder[t_loop] = t_order;
}

uint8_t Loops::getLoopSend(uint8_t t_loop) {
  return m_loopsSend[t_loop];
}

void Loops::setLoopSend(uint8_t t_loop, uint8_t t_send) {
  m_loopsSend[t_loop] = t_send;
}

uint8_t Loops::getLoopReturn(uint8_t t_loop) {
  return m_loopsReturn[t_loop];
}

void Loops::setLoopReturn(uint8_t t_loop, uint8_t t_ret) {
  m_loopsReturn[t_loop] = t_ret;
}

uint8_t Loops::getLoopsCount() {
  return m_loopsCount;
}

void Loops::setLoopsCount(uint8_t t_count) {
  m_loopsCount = t_count;
}

void Loops::toggleLoopsState(uint8_t t_loop) {
  m_loopsState[t_loop] = !m_loopsState[t_loop];
}
