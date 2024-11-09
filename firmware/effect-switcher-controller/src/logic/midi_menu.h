#pragma once

#include "menu_base.h"
#include "peripherals/display.h"
#include "logic/preset.h"

class MidiMessageMenu : public MenuBase {
  private:
    Preset* m_currentPreset;
    uint8_t m_selectedIndex;
    uint8_t m_startIndex;
    bool m_editRequested;
    bool m_addRequested;
    bool m_goBackRequested;

  public:
    MidiMessageMenu(Display* t_display, Preset* t_curentPreset) :
      MenuBase(t_display),
      m_currentPreset(t_curentPreset),
      m_selectedIndex(0),
      m_startIndex(0),
      m_editRequested(false),
      m_goBackRequested(false) { };

    void update() override;
    void reset() override;
    void handleInput(MenuInputAction t_action) override;

    void setCurrentPreset(Preset* t_currentPreset);
    bool isEditRequested();
    bool isAddRequested();
    bool isGoBackRequested();
    uint8_t getSelectedIndex() const;
    uint8_t getStartIndex() const;
};

class MidiMessageEditMenu : public MenuBase {
  private:
    Preset* m_currentPreset;
    uint8_t m_midiMessageIndex;
    uint8_t m_selectedIndex;
    bool m_messageEditMode;
    bool m_fieldEditMode;
    bool m_cancelRequested;
    bool m_deleteRequested;
    bool m_saveRequested;
    bool m_addRequested;
    uint8_t m_messagesCount = 4;
    uint8_t m_newMessageType = 0xB0; // Default value for new message
    uint8_t m_newMessageChannel = 0; // Default value for new message
    uint8_t m_newMessageDataByte1 = 64; // Default value for new message
    uint8_t m_newMessageDataByte2 = 64; // Default value for new message
    bool m_NewMessageHasDataByte2 = true;

  public:
    MidiMessageEditMenu(Display* t_display, Preset* t_currentPreset, uint8_t t_midiMessageIndex) :
      MenuBase(t_display),
      m_currentPreset(t_currentPreset),
      m_midiMessageIndex(t_midiMessageIndex),
      m_selectedIndex(0),
      m_messageEditMode(false),
      m_fieldEditMode(false),
      m_cancelRequested(false),
      m_saveRequested(false) { };

    void update() override;
    void reset() override;
    void handleInput(MenuInputAction t_action) override;
    void setCurrentPreset(Preset* t_currentPreset);
    uint8_t getMidiMessageIndex();
    void setMidiMessageIndex(uint8_t t_index);
    void setMessageEditMode(bool t_mode);
    bool isCancelRequested();
    bool isDeleteRequested();
    bool isSaveRequested();
    bool isAddRequested();
    uint8_t getNewMessageType() const;
    uint8_t getNewMessageChannel() const;
    uint8_t getNewMessageDataByte1() const;
    uint8_t getNewMessageDataByte2() const;
    bool getNewMessageHasDataByte2() const;
};