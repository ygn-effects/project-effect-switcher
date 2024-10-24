#include "loops.h"


uint8_t Loop::getLoopState() const {
  return m_loopState;
}

void Loop::setLoopState(uint8_t t_state) {
  m_loopState = t_state;
}

uint8_t Loop::getLoopOrder() const {
  return m_loopOrder;
}

void Loop::setLoopOrder(uint8_t t_order) {
  m_loopOrder = t_order;
}

uint8_t Loop::getLoopSend() const {
  return m_loopSend;
}

void Loop::setLoopSend(uint8_t t_send) {
  m_loopSend = t_send;
}

uint8_t Loop::getLoopReturn() {
  return m_loopReturn;
}

void Loop::setLoopReturn(uint8_t t_ret) {
  m_loopReturn = t_ret;
}

void Loop::toggleLoopState() {
  m_loopState = !m_loopState;
}
