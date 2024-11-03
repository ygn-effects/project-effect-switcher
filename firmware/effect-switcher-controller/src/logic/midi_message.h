#pragma once

#include <Arduino.h>

/// @brief Represents a MIDI message at the byte level
class MidiMessage {
  private:
    uint8_t m_statusByte;
    uint8_t m_dataByte1;
    uint8_t m_dataByte2;
    bool m_hasDataByte2;

  public:
    /// @brief Constructor for a 2 bytes MIDI message
    /// @param t_type MIDI message type
    /// @param t_channel MIDI message channel
    /// @param t_data1 MIDI message byte 1
    /// @param t_data2 MIDI message byte 2
    MidiMessage(uint8_t t_type, uint8_t t_channel, uint8_t t_data1, uint8_t t_data2) :
      m_statusByte(t_type | (t_channel & 0x0F)),
      m_dataByte1(t_data1),
      m_dataByte2(t_data2),
      m_hasDataByte2(true) { };

    /// @brief Constructor for a 1 byte MIDI message
    /// @param t_type MIDI message type
    /// @param t_channel MIDI message channel
    /// @param t_data1 MIDI message byte 1
    MidiMessage(uint8_t t_type, uint8_t t_channel, uint8_t t_data1) :
      m_statusByte(t_type | (t_channel & 0x0F)),
      m_dataByte1(t_data1),
      m_dataByte2(0),
      m_hasDataByte2(false) { };

    /// @brief Get the type of the MIDI message
    /// @return uint8_t first 4 bits of the status byte
    uint8_t getMessageType() {
      return m_statusByte & 0xF0;
    }

    /// @brief Get the channel of the MIDI message
    /// @return uint8_t last 4 bits of the status byte
    uint8_t getChannel() {
      return m_statusByte & 0xF;
    }

    /// @brief Get the first data byte
    /// @return uint8_t
    uint8_t getDataByte1() const {
      return m_dataByte1;
    }

    /// @brief Get the second data byte
    /// @return uint8_t
    uint8_t getDataByte2() const {
      return m_dataByte2;
    }

    /// @brief Check if the message has 2 data bytes
    /// @return true if the message has 2 data bytes
    bool hasDataByte2() const {
      return m_hasDataByte2;
    }

    /// @brief Send the MIDI message
    void send() const {
      Serial.write(m_statusByte);
      Serial.write(m_dataByte1);
      if (m_hasDataByte2) {
        Serial.write(m_dataByte2);
      }
    }
};
