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
  m_goBackRequested = false;
  m_selectedIndex = 0;
  m_startIndex = 0;
}

void MidiMessageMenu::handleInput(MenuInputAction t_action) {

}

void MidiMessageMenu::setCurrentPreset(Preset* t_currentPreset) {
  m_currentPreset = t_currentPreset;
}
