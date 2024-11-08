#include "midi_menu.h"

void MidiMessageMenu::update() {
  uint8_t messagesCount = m_currentPreset->getMidiMessagesCount();
  uint8_t messageTypes[messagesCount] = {0};
  uint8_t messageChannels[messagesCount] = {0};
  uint8_t messageDataBytes1[messagesCount] = {0};
  uint8_t messageDataBytes2[messagesCount] = {0};

  for (uint8_t i = 0; i < messagesCount; i++) {
    messageTypes[i] = m_currentPreset->getMidiMessageType(i);
    messageChannels[i] = m_currentPreset->getMidiMessageChannel(i);
    messageDataBytes1[i] = m_currentPreset->getMidiMessageDataByte1(i);
    messageDataBytes2[i] = m_currentPreset->getMidiMessageDataByte2(i);
  }

  m_display->clear();
  m_display->renderMidiMessages(messageTypes, messageChannels, messageDataBytes1, messageDataBytes2, messagesCount, m_selectedIndex, m_startIndex);
}

void MidiMessageMenu::reset() {
  m_editRequested = false;
  m_addRequested = false;
  m_goBackRequested = false;
  m_selectedIndex = 0;
  m_startIndex = 0;
}

void MidiMessageMenu::handleInput(MenuInputAction t_action) {
  uint8_t messagesCount = m_currentPreset->getMidiMessagesCount();
  uint8_t maxIndex = messagesCount + 1;  // Includes the "Add" and "Back" options

  switch (t_action) {
    case MenuInputAction::kUp:
      if (m_selectedIndex > 0) {
        m_selectedIndex--;
        if (m_selectedIndex < m_startIndex) m_startIndex--;
      }
      break;

    case MenuInputAction::kDown:
      if (m_selectedIndex < maxIndex) {
        m_selectedIndex++;
        if (m_selectedIndex >= m_startIndex + (m_display->getHeight() / m_display->getLineHeight()))
          m_startIndex++;
      }
      break;

    case MenuInputAction::kPress:
      if (m_selectedIndex < messagesCount) {
        // Edit existing message
        m_editRequested = true;
      } else if (m_selectedIndex == messagesCount) {
        // Add new message
        m_addRequested = true;
      } else if (m_selectedIndex == messagesCount + 1) {
        // Go back
        m_goBackRequested = true;
      }
      break;

    default:
      break;
  }
}

void MidiMessageMenu::setCurrentPreset(Preset* t_currentPreset) {
  m_currentPreset = t_currentPreset;
}

bool MidiMessageMenu::isEditRequested() {
  bool requested = m_editRequested;
  m_editRequested = false;

  return requested;
}

bool MidiMessageMenu::isAddRequested() {
  bool requested = m_addRequested;
  m_addRequested = false;

  return requested;
}

bool MidiMessageMenu::isGoBackRequested() {
  bool requested = m_goBackRequested;
  m_goBackRequested = false;

  return requested;
}

uint8_t MidiMessageMenu::getSelectedIndex() const {
  return m_selectedIndex;
}

uint8_t MidiMessageMenu::getStartIndex() const {
  return m_startIndex;
}

void MidiMessageEditMenu::update() {
  const char* messageFields[4] = {"Type:", "Channel:", "Byte1:", "Byte2:"};
  uint8_t messageValues[4] = {0};

  // Populate the value arrays with either the retrieved or default values
  messageValues[0] = m_newMessageType;
  messageValues[1] = m_newMessageChannel;
  messageValues[2] = m_newMessageDataByte1;
  messageValues[3] = m_newMessageDataByte2;

  m_display->clear();

  if (m_messageEditMode){
    m_display->renderHeader("Edit MIDI message");
  }
  else {
    m_display->renderHeader("Add MIDI message");
  }

  m_display->renderMidiMessageEdit(messageFields, messageValues, 4, m_NewMessageHasDataByte2, m_selectedIndex, m_fieldEditMode, m_messageEditMode);
}

void MidiMessageEditMenu::reset() {
  m_messageEditMode = false;
  m_fieldEditMode = false;
  m_cancelRequested = false;
  m_saveRequested = false;
  m_selectedIndex = 0;
  m_newMessageType = 0xB0;
  m_newMessageChannel = 0;
  m_newMessageDataByte1 = 64;
  m_newMessageDataByte2 = 64;
}

void MidiMessageEditMenu::handleInput(MenuInputAction t_action) {
  switch (t_action)
  {
    case MenuInputAction::kUp:
      if (m_fieldEditMode) {
        // Adjust the value of the selected field when in field edit mode
        switch (m_selectedIndex)
        {
          case 0: // Type
            m_newMessageType = (m_newMessageType + 0x10) & 0xF0;
            if (m_newMessageType > 0xC0) m_newMessageType = 0xB0;
            if (m_newMessageType == 0xC0) {
              m_NewMessageHasDataByte2 = false;
            }
            else {
              m_NewMessageHasDataByte2 = true;
            }
            break;

          case 1: // Channel
            m_newMessageChannel = (m_newMessageChannel + 1) % 16;
            break;

          case 2: // Data byte 1
            m_newMessageDataByte1 = (m_newMessageDataByte1 + 1) % 128;
            break;

          case 3: // Data Byte 2
            m_newMessageDataByte2 = (m_newMessageDataByte2 + 1) % 128;
            break;

          default:
            break;
        }
      }
      else {
        // Navigate up through fields if not in edit mode
        if (m_selectedIndex > 0) m_selectedIndex--;
        if (m_selectedIndex == 3 && !m_NewMessageHasDataByte2) m_selectedIndex--;
      }
      break;

    case MenuInputAction::kDown:
      if (m_fieldEditMode) {
        // Adjust the value of the selected field when in field edit mode
        switch (m_selectedIndex)
        {
          case 0: // Type
            m_newMessageType = (m_newMessageType - 0x10) & 0xF0;
            if (m_newMessageType < 0xB0) m_newMessageType = 0xC0;
            if (m_newMessageType == 0xC0) {
              m_NewMessageHasDataByte2 = false;
            }
            else {
              m_NewMessageHasDataByte2 = true;
            }
            break;

          case 1: // Channel
            m_newMessageChannel = (m_newMessageChannel - 1) % 16;
            break;

          case 2: // Data byte 1
            m_newMessageDataByte1 = (m_newMessageDataByte1 - 1) % 128;
            break;

          case 3: // Data Byte 2
            m_newMessageDataByte2 = (m_newMessageDataByte2 - 1) % 128;
            break;

          default:
            break;
        }
      }
      else {
        // Navigate up through fields if not in edit mode
        if (m_selectedIndex < 5) m_selectedIndex++;
        if (m_selectedIndex == 3 && !m_NewMessageHasDataByte2) m_selectedIndex++;
      }
      break;

    case MenuInputAction::kPress:
      if (m_selectedIndex < m_messagesCount) {
        m_fieldEditMode = !m_fieldEditMode;
      }
      else if (m_selectedIndex == m_messagesCount) {
        m_cancelRequested = true;
      }
      else if (m_selectedIndex > m_messagesCount) {
        m_saveRequested = true;
      }
      break;

    default:
      break;
  }
}

void MidiMessageEditMenu::setCurrentPreset(Preset* t_currentPreset) {
  m_currentPreset = t_currentPreset;
}

void MidiMessageEditMenu::setMidiMessageIndex(uint8_t t_index) {
  m_midiMessageIndex = t_index;
}

void MidiMessageEditMenu::setMessageEditMode(bool t_mode) {
  m_messageEditMode = t_mode;

  // Edit mode, retrieve data from the current message to the local members
  if (m_messageEditMode) {
    m_newMessageType = m_currentPreset->getMidiMessageType(m_midiMessageIndex);
    m_newMessageChannel = m_currentPreset->getMidiMessageChannel(m_midiMessageIndex);
    m_newMessageDataByte1 = m_currentPreset->getMidiMessageDataByte1(m_midiMessageIndex);
    m_newMessageDataByte2 = m_currentPreset->getMidiMessageDataByte2(m_midiMessageIndex);
    m_NewMessageHasDataByte2 = m_currentPreset->getMidiMessageHasDataByte2(m_midiMessageIndex);
  }
}

bool MidiMessageEditMenu::isCancelRequested() {
  bool requested = m_cancelRequested;
  m_cancelRequested = false;

  return requested;
}

bool MidiMessageEditMenu::isSaveRequested() {
  bool requested = m_saveRequested;
  m_saveRequested = false;

  return requested;
}
