#pragma once

#include <Arduino.h>

/// @brief Represents a MIDI message at the byte level
class MidiMessage {
  private:
    uint8_t m_statusByte;
    uint8_t m_dataByte1;
    uint8_t m_dataByte2;

  public:
    MidiMessage() :
      m_statusByte(0),
      m_dataByte1(0),
      m_dataByte2(255) { };  // Set default to 255 for no data

    /// @brief Constructor for a 2 bytes MIDI message
    /// @param t_type MIDI message type
    /// @param t_channel MIDI message channel
    /// @param t_data1 MIDI message byte 1
    /// @param t_data2 MIDI message byte 2
    MidiMessage(uint8_t t_type, uint8_t t_channel, uint8_t t_data1, uint8_t t_data2) :
      m_statusByte(t_type | (t_channel & 0x0F)),
      m_dataByte1(t_data1),
      m_dataByte2(t_data2) { };

    /// @brief Constructor for a 1 byte MIDI message
    /// @param t_type MIDI message type
    /// @param t_channel MIDI message channel
    /// @param t_data1 MIDI message byte 1
    MidiMessage(uint8_t t_type, uint8_t t_channel, uint8_t t_data1) :
      m_statusByte(t_type | (t_channel & 0x0F)),
      m_dataByte1(t_data1),
      m_dataByte2(255) { };  // Set m_dataByte2 to 255 to indicate it's unused

    /// @brief Get the MIDI message status byte
    /// @return uint8_t
    uint8_t getMessageStatusByte() const {
      return m_statusByte;
    }

    /// @brief Set the MIDI message status byte
    /// @param t_byte Status byte
    void setMessageStatusByte(uint8_t t_byte) {
      m_statusByte = t_byte;
    }

    /// @brief Get the type of the MIDI message
    /// @return uint8_t first 4 bits of the status byte
    uint8_t getMessageType() const {
      return m_statusByte & 0xF0;
    }

    /// @brief Get the channel of the MIDI message
    /// @return uint8_t last 4 bits of the status byte
    uint8_t getChannel() const {
      return m_statusByte & 0x0F;
    }

    /// @brief Get the first data byte
    /// @return uint8_t
    uint8_t getDataByte1() const {
      return m_dataByte1;
    }

    /// @brief Set the MIDI message first data byte
    /// @param t_byte Data byte 1
    void setMessageDataByte1(uint8_t t_byte) {
      m_dataByte1 = t_byte;
    }

    /// @brief Get the second data byte
    /// @return uint8_t
    uint8_t getDataByte2() const {
      return m_dataByte2;
    }

    /// @brief Set the MIDI message second data byte
    /// @param t_byte Data byte 2
    void setMessageDataByte2(uint8_t t_byte) {
      m_dataByte2 = t_byte;
    }


    /// @brief Check if the message has 2 data bytes
    /// @return true if the message has 2 data bytes
    bool hasDataByte2() const {
      return m_dataByte2 != 255;
    }

    /// @brief Send the MIDI message
    void send() const {
      Serial.write(m_statusByte);
      Serial.write(m_dataByte1);
      if (hasDataByte2()) {
        Serial.write(m_dataByte2);
      }
    }
};
