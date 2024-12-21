#pragma once

#include <Arduino.h>
#include "logic/loops.h"
#include "logic/midi_message.h"
#include "utils/logging.h"

constexpr uint8_t c_maxLoops = 16;           // Maximum number of loops per preset.
constexpr uint8_t c_maxMidiMessages = 20;    // Maximum number of MIDI messages per preset.

/// @brief Represents a preset that contains a bank, a preset number,
/// and an array of loops and MIDI messages.
class Preset {
  private:
    uint8_t m_bank;                                 // Bank number for the preset.
    uint8_t m_preset;                               // Preset number within the bank.
    uint8_t m_loopsCount;                           // Number of active loops in the preset.
    uint8_t m_midiMessagesCount;                    // Number of MIDI messages in the preset.
    Loop m_loops[c_maxLoops];                       // Array of loops.
    MidiMessage m_midiMessages[c_maxMidiMessages];  // Array of MIDI messages.

  public:
    /// @brief Default constructor that initializes the preset with default values.
    Preset() : m_bank(0), m_preset(0) { };

    /// @brief Parameterized constructor to initialize bank, preset, and loops count.
    /// @param t_bank Bank number.
    /// @param t_preset Preset number.
    /// @param t_loopsCount Number of loops in this preset.
    Preset(uint8_t t_bank, uint8_t t_preset, uint8_t t_loopsCount) :
      m_bank(t_bank),
      m_preset(t_preset),
      m_loopsCount(t_loopsCount) { };

    /// @brief Parameterized constructor to initialize bank, preset, loops count, and MIDI messages count.
    /// @param t_bank Bank number.
    /// @param t_preset Preset number.
    /// @param t_loopsCount Number of loops in this preset.
    /// @param t_midiMessagesCount Number of MIDI messages in this preset.
    Preset(uint8_t t_bank, uint8_t t_preset, uint8_t t_loopsCount, uint8_t t_midiMessagesCount) :
      m_bank(t_bank),
      m_preset(t_preset),
      m_loopsCount(t_loopsCount),
      m_midiMessagesCount(t_midiMessagesCount) { }

    /// @brief Get the bank number.
    /// @return uint8_t Bank number.
    uint8_t getBank() const;

    /// @brief Set the bank number.
    /// @param t_bank Bank number.
    void setBank(uint8_t t_bank);

    /// @brief Get the preset number.
    /// @return uint8_t Preset number.
    uint8_t getPreset() const;

    /// @brief Set the preset number.
    /// @param t_preset Preset number.
    void setPreset(uint8_t t_preset);

    /// @brief Get the number of loops in the preset.
    /// @return uint8_t Number of loops.
    uint8_t getLoopsCount() const;

    /// @brief Set the number of loops in the preset.
    /// @param t_count Loops count.
    void setLoopsCount(uint8_t t_count);

    /// @brief Get the state of a specific loop.
    /// @param t_loop Index of the loop.
    /// @return uint8_t Loop state (on/off).
    uint8_t getLoopState(uint8_t t_loop) const;

    /// @brief Set a loop's state.
    /// @param t_loop Index of the loop.
    /// @param t_state New state of the loop.
    void setLoopState(uint8_t t_loop, uint8_t t_state);

    /// @brief Get the order of a specific loop.
    /// @param t_loop Index of the loop.
    /// @return uint8_t Loop order.
    uint8_t getLoopOrder(uint8_t t_loop) const;

    /// @brief Set the order of a loop.
    /// @param t_loop Index of the loop.
    /// @param t_order New order of the loop.
    void setLoopOrder(uint8_t t_loop, uint8_t t_order);

    /// @brief Get the send channel of a specific loop.
    /// @param t_loop Index of the loop.
    /// @return uint8_t Send channel of the loop.
    uint8_t getLoopSend(uint8_t t_loop) const;

    /// @brief Set the send channel of a loop.
    /// @param t_loop Index of the loop.
    /// @param t_send New send channel.
    void setLoopSend(uint8_t t_loop, uint8_t t_send);

    /// @brief Get the return channel of a specific loop.
    /// @param t_loop Index of the loop.
    /// @return uint8_t Return channel of the loop.
    uint8_t getLoopReturn(uint8_t t_loop) const;

    /// @brief Set the return channel of a loop.
    /// @param t_loop Index of the loop.
    /// @param t_return New return channel.
    void setLoopReturn(uint8_t t_loop, uint8_t t_return);

    /// @brief Toggle the state of a specific loop.
    /// @param t_loop Index of the loop.
    void toggleLoopState(uint8_t t_loop);

    /// @brief Returns the index of a loop by its order.
    /// @param t_order Order of the loop to find.
    /// @return uint8_t Index of the loop, or an invalid value if not found.
    uint8_t getLoopIndexByOrder(uint8_t t_order);

    /// @brief Swap the order of two loops in the preset.
    /// @param t_loop1 Index of the first loop.
    /// @param t_loop2 Index of the second loop.
    void swapPresetLoopsOrder(uint8_t t_loop1, uint8_t t_loop2);

    /// @brief Set both the send and return channels for a loop.
    /// @param t_loop Index of the loop.
    /// @param t_send Send channel.
    /// @param t_return Return channel.
    void setPresetLoopSendReturn(uint8_t t_loop, uint8_t t_send, uint8_t t_return);

    /// @brief Get the send channel for a loop.
    /// @param t_loop Index of the loop.
    /// @return uint8_t Send channel.
    uint8_t getPresetLoopSend(uint8_t t_loop);

    /// @brief Get the return channel for a loop.
    /// @param t_loop Index of the loop.
    /// @return uint8_t Return channel.
    uint8_t getPresetLoopReturn(uint8_t t_loop);

    /// @brief Get the number of MIDI messages in the preset.
    /// @return uint8_t Number of MIDI messages.
    uint8_t getMidiMessagesCount() const;

    /// @brief Set the MIDI messages count in the preset.
    /// @param t_count Number of MIDI messages.
    void setMidiMessagesCount(uint8_t t_count);

    /// @brief Get the status byte of a specific MIDI message.
    /// @param t_message Index of the MIDI message.
    /// @return uint8_t Status byte.
    uint8_t getMidiMessageStatusByte(uint8_t t_message) const;

    /// @brief Set the status byte of a MIDI message.
    /// @param t_message Index of the MIDI message.
    /// @param t_byte Status byte to set.
    void setMidiMessageStatusByte(uint8_t t_message, uint8_t t_byte);

    /// @brief Get the type of a specific MIDI message.
    /// @param t_message Index of the MIDI message.
    /// @return uint8_t MIDI message type.
    uint8_t getMidiMessageType(uint8_t t_message) const;

    /// @brief Set the type of a MIDI message.
    /// @param t_message Index of the MIDI message.
    /// @param t_type MIDI message type.
    void setMidiMessageType(uint8_t t_message, uint8_t t_type);

    /// @brief Get the channel of a specific MIDI message.
    /// @param t_message Index of the MIDI message.
    /// @return uint8_t MIDI channel.
    uint8_t getMidiMessageChannel(uint8_t t_message) const;

    /// @brief Set the channel of a MIDI message.
    /// @param t_message Index of the MIDI message.
    /// @param t_channel MIDI channel.
    void setMidiMessageChannel(uint8_t t_message, uint8_t t_channel);

    /// @brief Get the first data byte of a specific MIDI message.
    /// @param t_message Index of the MIDI message.
    /// @return uint8_t First data byte.
    uint8_t getMidiMessageDataByte1(uint8_t t_message) const;

    /// @brief Set the first data byte of a MIDI message.
    /// @param t_message Index of the MIDI message.
    /// @param t_byte Data byte 1.
    void setMidiMessageDataByte1(uint8_t t_message, uint8_t t_byte);

    /// @brief Get the second data byte of a specific MIDI message. Returns 255 if there’s no second byte.
    /// @param t_message Index of the MIDI message.
    /// @return uint8_t Second data byte or 255 if none.
    uint8_t getMidiMessageDataByte2(uint8_t t_message) const;

    /// @brief Set the second data byte of a MIDI message.
    /// @param t_message Index of the MIDI message.
    /// @param t_byte Data byte 2.
    void setMidiMessageDataByte2(uint8_t t_message, uint8_t t_byte);

    /// @brief Check if a MIDI message has a second data byte.
    /// @param t_message Index of the MIDI message.
    /// @return true if the message has a second data byte; otherwise false.
    bool getMidiMessageHasDataByte2(uint8_t t_message) const;

    /// @brief Add a new MIDI message to the messages array.
    /// @param t_type MIDI message type.
    /// @param t_channel MIDI message channel.
    /// @param t_byte1 First data byte.
    /// @param t_byte2 Second data byte.
    /// @param hasByte2 Specify if the MIDI message has two data bytes (default is true).
    void AddMidiMessage(uint8_t t_type, uint8_t t_channel, uint8_t t_byte1, uint8_t t_byte2, bool hasByte2 = true);

    /// @brief Remove a MIDI message from the messages array at the specified index.
    /// @param t_message Index of the MIDI message to delete.
    void removeMidiMessage(uint8_t t_message);
};
