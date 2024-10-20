#include <SPI.h>

#include "leddriver.h"
#include "utils/logging.h"

void LedDriver::setup() {
  pinMode(m_csPin, OUTPUT);
  digitalWrite(m_csPin, LOW);
  SPI.begin();
}

void LedDriver::select() {
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  digitalWrite(m_csPin, LOW);
}

void LedDriver::deselect() {
  digitalWrite(m_csPin, HIGH);
  SPI.endTransaction();
}

void LedDriver8::lightLed(uint8_t t_led) {
  select();
  SPI.transfer(1 << t_led);
  deselect();

  LOG_DEBUG("8-bit driver : LED %d on", t_led);
}

void LedDriver8::turnOffAll() {
  select();
  SPI.transfer(0);
  deselect();

  LOG_DEBUG("8-bit driver : All LEDs off");
}

void LedDriver16::lightLed(uint8_t t_led) {
  select();
  SPI.transfer16(1 << t_led);
  deselect();

  LOG_DEBUG("16-bit driver : LED %d on", t_led);
}

void LedDriver16::lightTwoLeds(uint8_t t_led) {
  select();
  SPI.transfer16((1 << t_led) | ((1 << t_led) << 8));
  deselect();

  LOG_DEBUG("16-bit driver : LEDs %d/%d on", t_led, t_led + 8);
}

void LedDriver16::turnOffAll() {
  select();
  SPI.transfer16(0);
  deselect();

  LOG_DEBUG("16-bit driver: All LEDs off");
}

void LedDriver16::blinkLed(uint8_t t_led, uint8_t t_interval) {
  m_blinkTime = millis();

  if ((m_blinkTime - m_lastBlinkTime) >= t_interval) {
    if (m_lastBlinkState) {
      m_lastBlinkState = 0;
      turnOffAll();
    } else {
      m_lastBlinkState = 1;
      lightLed(t_led);
    }

    m_lastBlinkTime = m_blinkTime;
  }
}

void LedDriver16::blinkTwoLeds(uint8_t t_led, uint8_t t_interval) {
  m_blinkTime = millis();

  if ((m_blinkTime - m_lastBlinkTime) >= t_interval) {
    if (m_lastBlinkState) {
      m_lastBlinkState = 0;
      turnOffAll();
    } else {
      m_lastBlinkState = 1;
      lightTwoLeds(t_led);
    }

    m_lastBlinkTime = m_blinkTime;
  }
}

void LedDriver16::resetBlink() {
  m_lastBlinkTime = 0;
  m_lastBlinkState = 0;

  LOG_DEBUG("16-bit driver: Blink state reset");
}

void LedDriver16::setLedStateByMask(uint16_t t_mask) {
  m_ledDriverMask = t_mask;

  select();
  SPI.transfer16(t_mask);
  deselect();

  LOG_DEBUG("16-bit driver: LED state set by mask 0x%04X", t_mask);
}

uint16_t LedDriver16::getLedStateByMask() const {
  return m_ledDriverMask;
}
