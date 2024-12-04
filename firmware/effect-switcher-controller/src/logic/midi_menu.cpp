#include "midi_menu.h"

void MidiMessageMenu::update() {
  m_layoutManager->clear();

  // Set header
  m_layoutManager->setHeader("MIDI messages");

  // Set footer
  m_footerColumnCount = 2;
  const char* footItems[] = {"Add", "Back"};
  m_layoutManager->setFooter(footItems, m_footerColumnCount);

  // Visible Content Rows
  m_itemsCount = m_presetView->midiMessagesCount;
  uint8_t endIndex = m_startIndex + m_visibleRowCount;
  if (endIndex > m_itemsCount) {
    endIndex = m_itemsCount;
  }

  uint8_t rowIndex = 0;
  for (uint8_t i = m_startIndex; i < endIndex; i++) {
    Row row;
    row.alignment = Row::kLeft;
    row.columnsCount = 4;

    MidiMessageView& message = m_presetView->midiMessages[i];

    StaticString type;
    StaticString channel(message.channel);
    StaticString byte1(message.byte1);
    StaticString byte2(message.byte2);

    switch (message.type) {
      case 0xB0:
        type.append('C');
        type.append('C');
        break;

      case 0xC0:
        type.append('P');
        type.append('C');
        break;

      default:
        break;
    }

    row.columns[0] = { Column::kLabel, Column::kNormal, "T:", type.c_str(), 0 };
    row.columns[1] = { Column::kLabel, Column::kNormal, "C:", channel.c_str(), 0 };
    row.columns[2] = { Column::kLabel, Column::kNormal, "B1:", byte1.c_str(), 0 };

    if (message.hasDataByte2) {
      row.columns[3] = { Column::kLabel, Column::kNormal, "B2:", byte2.c_str(), 0 };
    }
    else {
      row.columns[3] = { Column::kLabel, Column::kNormal, "B2:", nullptr, 0 };
    }

    m_layoutManager->addRow(row);

    m_rowCounts[rowIndex] = i;
    m_rowColumnCounts[rowIndex] = 1;

    rowIndex++;
  }

  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
  m_layoutManager->render();
}

void MidiMessageMenu::reset() {
  m_selectedRow = 0;
  m_selectedColumn = 0;
  m_startIndex = 0;
  m_isFooterActive = false;
  m_editRequested = false;
  m_addRequested = false;
  m_backRequested = false;
  m_itemSelected = false;
  m_layoutManager->setIsFooterActive(false);
  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
}

void MidiMessageMenu::handleAction(MenuInputAction t_action) {
  switch (t_action) {
    case MenuInputAction::kPress:
      if (m_isFooterActive) {
        switch (m_selectedColumn) {
          case 0:
            m_addRequested = true;
            break;

          case 1:
            m_backRequested = true;
            break;

          default:
            break;
        }
      }
      else {
        m_editRequested = true;
      }
      break;

    case MenuInputAction::kUp:
      handleNavigation(t_action);
      break;

    case MenuInputAction::kDown:
      handleNavigation(t_action);
      break;

    case MenuInputAction::kLongPress:
    default:
      break;
  }
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

bool MidiMessageMenu::isBackRequested() {
  bool requested = m_backRequested;
  m_backRequested = false;

  return requested;
}

void MidiMessageEditMenu::update() {
  m_layoutManager->clear();

  // Set header
  m_layoutManager->setHeader("MIDI message edit");

  // Set footer
  m_footerColumnCount = 2;

  if (m_messageEditMode) {
    const char* footItems[] = {"Cancel", "Save"};
    m_layoutManager->setFooter(footItems, m_footerColumnCount);
  }
  else {
    const char* footItems[] = {"Cancel", "Add"};
    m_layoutManager->setFooter(footItems, m_footerColumnCount);
  }

  StaticString type;
  StaticString channel(m_newMessageChannel);
  StaticString byte1(m_newMessageDataByte1);
  StaticString byte2(m_newMessageDataByte2);

  switch (m_newMessageType) {
    case 0xB0:
      type.append('C');
      type.append('C');
      break;

    case 0xC0:
      type.append('P');
      type.append('C');
      break;

    default:
      break;
  }

  uint8_t rowIndex = 0;
  for (uint8_t i = 0; i < 2; i++) {
    Row row;
    row.alignment = Row::kCenter;
    row.columnsCount = 2;

    auto getColumnStyle = [this, rowIndex](uint8_t columnIndex) {
      return (m_selectedRow == rowIndex && m_selectedColumn == columnIndex && !m_isNavigationActive)
                  ? Column::kValueHighLighted
                  : Column::kNormal;
    };

    if (rowIndex == 0) {
      row.columns[0] = { Column::kLabel, getColumnStyle(0), "T:", type.c_str(), 0 };
      row.columns[1] = { Column::kLabel, getColumnStyle(1), "C:", channel.c_str(), 0 };
    }
    else {
      row.columns[0] = { Column::kLabel, getColumnStyle(0), "B1:", byte1.c_str(), 0 };

      if (m_NewMessageHasDataByte2) {
        row.columns[1] = { Column::kLabel, getColumnStyle(1), "B2:", byte2.c_str(), 0 };
      }
      else {
        row.columns[1] = { Column::kLabel, getColumnStyle(1), "B2:", nullptr, 0 };
      }
    }

    m_layoutManager->addRow(row);

    m_rowCounts[rowIndex] = i;
    m_rowColumnCounts[rowIndex] = row.columnsCount;
    rowIndex++;
    m_itemsCount = rowIndex;
  }

  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
  m_layoutManager->render();
}

void MidiMessageEditMenu::reset() {
  m_selectedRow = 0;
  m_selectedColumn = 0;
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
  m_layoutManager->setIsFooterActive(false);
  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
}

void MidiMessageEditMenu::handleAction(MenuInputAction t_action) {
  switch (t_action) {
    case MenuInputAction::kUp:
      if (m_isNavigationActive) {
        handleNavigation(t_action);
      }
      else {
        switch (m_selectedRow * 2 + m_selectedColumn) {
          case 0:
            break;

          case 1:
            m_newMessageChannel=+ (m_newMessageChannel + 1) % 16;
            break;

          case 2:
            m_newMessageDataByte1 = (m_newMessageDataByte1 + 1) % 128;
            break;

          case 3:
            if (m_NewMessageHasDataByte2) {
              m_newMessageDataByte2 = (m_newMessageDataByte2 + 1) % 128;
            }
            break;

          default:
            break;
        }
      }
      break;

    case MenuInputAction::kDown:
      if (m_isNavigationActive) {
        handleNavigation(t_action);
      }
      else {
        switch (m_selectedRow * 2 + m_selectedColumn) {
          case 0:
            break;

          case 1:
            m_newMessageChannel = (m_newMessageChannel - 1) % 16;
            break;

          case 2:
            m_newMessageDataByte1 = (m_newMessageDataByte1 - 1) % 128;
            break;

          case 3:
            if (m_NewMessageHasDataByte2) {
              m_newMessageDataByte2 = (m_newMessageDataByte2 - 1) % 128;
            }
            break;

          default:
            break;
        }
      }
      break;

    case MenuInputAction::kPress:
      if (!m_isFooterActive) {
        m_isNavigationActive = !m_isNavigationActive;
      }
      else {
        switch (m_selectedColumn) {
          case 0:
            m_cancelRequested = true;
            break;

          case 1:
            if (m_messageEditMode) {
              m_addRequested = true;
            }
            else {
              m_saveRequested = true;
            }
            break;

          default:
            break;
        }
      }
      break;

    case MenuInputAction::kLongPress:
      break;

    default:
      break;
  }
}

void MidiMessageEditMenu::setPresetView(PresetView* t_view) {
  m_presetView = t_view;
}

uint8_t MidiMessageEditMenu::getMidiMessageIndex() {
  return m_midiMessageIndex;
}

void MidiMessageEditMenu::setMidiMessageIndex(uint8_t t_index) {
  m_midiMessageIndex = t_index;
}

void MidiMessageEditMenu::setMessageEditMode(bool t_mode) {
  m_messageEditMode = t_mode;

  if (m_messageEditMode) {
    const MidiMessageView message = m_presetView->midiMessages[m_midiMessageIndex];
    m_newMessageType = message.type;
    m_newMessageChannel = message.channel;
    m_newMessageDataByte1 = message.byte1;
    m_newMessageDataByte2 = message.byte2;
    m_NewMessageHasDataByte2 = message.hasDataByte2;
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
