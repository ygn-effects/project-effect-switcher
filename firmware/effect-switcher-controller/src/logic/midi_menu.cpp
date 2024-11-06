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