#include "midi_menu.h"

void MidiMessageMenu::update() {

}

void MidiMessageMenu::reset() {
  m_editRequested = false;
  m_addRequested = false;
  m_goBackRequested = false;
  m_selectedIndex = 0;
  m_startIndex = 0;
}

void MidiMessageMenu::handleInput(MenuInputAction t_action) {

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

}

void MidiMessageEditMenu::reset() {
  m_midiMessageIndex = 0;
  m_messageEditMode = false;
  m_fieldEditMode = false;
  m_cancelRequested = false;
  m_deleteRequested = false;
  m_saveRequested = false;
  m_addRequested = false;
  m_selectedIndex = 0;
  m_newMessageType = 0xB0;
  m_newMessageChannel = 0;
  m_newMessageDataByte1 = 64;
  m_newMessageDataByte2 = 64;
  m_NewMessageHasDataByte2 = true;
}

void MidiMessageEditMenu::handleInput(MenuInputAction t_action) {

}

uint8_t MidiMessageEditMenu::getMidiMessageIndex() {
  return m_midiMessageIndex;
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

bool MidiMessageEditMenu::isDeleteRequested(){
  bool requested = m_deleteRequested;
  m_deleteRequested = false;

  return requested;
}

bool MidiMessageEditMenu::isSaveRequested() {
  bool requested = m_saveRequested;
  m_saveRequested = false;

  return requested;
}

bool MidiMessageEditMenu::isAddRequested() {
  bool requested = m_addRequested;
  m_addRequested = false;

  return requested;
}

uint8_t MidiMessageEditMenu::getNewMessageType() const {
  return m_newMessageType;
}

uint8_t MidiMessageEditMenu::getNewMessageChannel() const {
  return m_newMessageChannel;
}

uint8_t MidiMessageEditMenu::getNewMessageDataByte1() const {
  return m_newMessageDataByte1;
}

uint8_t MidiMessageEditMenu::getNewMessageDataByte2() const {
  return m_newMessageDataByte2;
}

bool MidiMessageEditMenu::getNewMessageHasDataByte2() const {
  return m_NewMessageHasDataByte2;
}
