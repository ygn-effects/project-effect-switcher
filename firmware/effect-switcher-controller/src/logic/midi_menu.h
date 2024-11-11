#pragma once

#include "menu_base.h"
#include "peripherals/display.h"
#include "logic/preset.h"

/// @brief Class representing the MIDI message menu, allowing the user to view, add, or edit MIDI messages.
///
/// The MidiMessageMenu class provides navigation and selection of MIDI messages, enabling the user to
/// select a message for editing, add a new message, or go back to the previous menu.
class MidiMessageMenu : public MenuBase {
  private:
    Preset* m_currentPreset;        // Pointer to the currently selected Preset.
    uint8_t m_selectedIndex;        // Index of the currently selected MIDI message.
    uint8_t m_startIndex;           // Index of the first visible item in the list.
    bool m_editRequested;           // Flag indicating if an edit action is requested.
    bool m_addRequested;            // Flag indicating if an add action is requested.
    bool m_goBackRequested;         // Flag indicating if a go-back action is requested.

  public:
    /// @brief Constructor to initialize the MIDI message menu with a display and preset.
    /// @param t_display Pointer to the Display object for rendering.
    /// @param t_currentPreset Pointer to the current Preset containing MIDI messages.
    MidiMessageMenu(Display* t_display, Preset* t_currentPreset) :
      MenuBase(t_display),
      m_currentPreset(t_currentPreset),
      m_selectedIndex(0),
      m_startIndex(0),
      m_editRequested(false),
      m_goBackRequested(false) { };

    /// @brief Updates the display to render the list of MIDI messages.
    void update() override;

    /// @brief Resets the menu's state, clearing selection flags and resetting indices.
    void reset() override;

    /// @brief Handles user input actions to navigate and select options within the menu.
    /// @param t_action The input action, such as moving up, down, or selecting an item.
    void handleInput(MenuInputAction t_action) override;

    /// @brief Sets the current preset.
    /// @param t_currentPreset Pointer to the current Preset.
    void setCurrentPreset(Preset* t_currentPreset);

    /// @brief Checks if an edit action was requested.
    /// @return true if an edit action is requested; false otherwise.
    bool isEditRequested();

    /// @brief Checks if an add action was requested.
    /// @return true if an add action is requested; false otherwise.
    bool isAddRequested();

    /// @brief Checks if a go-back action was requested.
    /// @return true if a go-back action is requested; false otherwise.
    bool isGoBackRequested();

    /// @brief Retrieves the index of the currently selected item.
    /// @return uint8_t The index of the selected item.
    uint8_t getSelectedIndex() const;

    /// @brief Retrieves the start index of the visible items in the list.
    /// @return uint8_t The start index of the visible items.
    uint8_t getStartIndex() const;
};

/// @brief Class representing the edit menu for a MIDI message, allowing modification of individual fields.
///
/// The MidiMessageEditMenu class provides detailed editing for each field in a MIDI message, including type,
/// channel, data bytes, and other settings.
class MidiMessageEditMenu : public MenuBase {
  private:
    Preset* m_currentPreset;              // Pointer to the current Preset object.
    uint8_t m_midiMessageIndex;           // Index of the MIDI message being edited.
    uint8_t m_selectedIndex;              // Index of the currently selected field in the message.
    bool m_messageEditMode;               // Flag indicating if edit mode is active for a MIDI message.
    bool m_fieldEditMode;                 // Flag indicating if field edit mode is active.
    bool m_cancelRequested;               // Flag indicating if a cancel action was requested.
    bool m_deleteRequested;               // Flag indicating if a delete action was requested.
    bool m_saveRequested;                 // Flag indicating if a save action was requested.
    bool m_addRequested;                  // Flag indicating if an add action was requested.
    uint8_t m_messagesCount = 4;          // Number of MIDI messages.
    uint8_t m_newMessageType = 0xB0;      // Default value for the MIDI message type.
    uint8_t m_newMessageChannel = 0;      // Default value for the MIDI message channel.
    uint8_t m_newMessageDataByte1 = 64;   // Default value for the first data byte.
    uint8_t m_newMessageDataByte2 = 64;   // Default value for the second data byte.
    bool m_NewMessageHasDataByte2 = true; // Default flag for the presence of a second data byte.

  public:
    /// @brief Constructor to initialize the MIDI message edit menu with a display, preset, and message index.
    /// @param t_display Pointer to the Display object.
    /// @param t_currentPreset Pointer to the current Preset.
    /// @param t_midiMessageIndex Index of the MIDI message to edit.
    MidiMessageEditMenu(Display* t_display, Preset* t_currentPreset, uint8_t t_midiMessageIndex) :
      MenuBase(t_display),
      m_currentPreset(t_currentPreset),
      m_midiMessageIndex(t_midiMessageIndex),
      m_selectedIndex(0),
      m_messageEditMode(false),
      m_fieldEditMode(false),
      m_cancelRequested(false),
      m_saveRequested(false) { };

    /// @brief Updates the display to show the current MIDI message edit fields.
    void update() override;

    /// @brief Resets the edit menu, clearing all flags and resetting fields.
    void reset() override;

    /// @brief Handles user input actions to navigate and modify fields within the edit menu.
    /// @param t_action The input action, such as selecting a field or modifying a value.
    void handleInput(MenuInputAction t_action) override;

    /// @brief Sets the current preset.
    /// @param t_currentPreset Pointer to the Preset containing the MIDI message to edit.
    void setCurrentPreset(Preset* t_currentPreset);

    /// @brief Retrieves the index of the MIDI message being edited.
    /// @return uint8_t The index of the MIDI message.
    uint8_t getMidiMessageIndex();

    /// @brief Sets the index of the MIDI message to be edited.
    /// @param t_index The index of the MIDI message.
    void setMidiMessageIndex(uint8_t t_index);

    /// @brief Sets the message edit mode flag and load the values from the preset if editing
    /// @param t_mode The mode flag; true for edit mode, false for add mode.
    void setMessageEditMode(bool t_mode);

    /// @brief Checks if a cancel action was requested.
    /// @return true if cancel action was requested; false otherwise.
    bool isCancelRequested();

    /// @brief Checks if a delete action was requested.
    /// @return true if delete action was requested; false otherwise.
    bool isDeleteRequested();

    /// @brief Checks if a save action was requested.
    /// @return true if save action was requested; false otherwise.
    bool isSaveRequested();

    /// @brief Checks if an add action was requested.
    /// @return true if add action was requested; false otherwise.
    bool isAddRequested();

    /// @brief Retrieves the type of the new MIDI message.
    /// @return uint8_t The type of the new MIDI message.
    uint8_t getNewMessageType() const;

    /// @brief Retrieves the channel of the new MIDI message.
    /// @return uint8_t The channel of the new MIDI message.
    uint8_t getNewMessageChannel() const;

    /// @brief Retrieves the first data byte of the new MIDI message.
    /// @return uint8_t The first data byte.
    uint8_t getNewMessageDataByte1() const;

    /// @brief Retrieves the second data byte of the new MIDI message.
    /// @return uint8_t The second data byte.
    uint8_t getNewMessageDataByte2() const;

    /// @brief Checks if the new MIDI message has a second data byte.
    /// @return true if there is a second data byte; false otherwise.
    bool getNewMessageHasDataByte2() const;
};
