#include "loop_menu.h"

void LoopOrderMenu::update() {
  m_layoutManager->clear();

  // Set header
  m_layoutManager->setHeader("Loops Order");

  // Set footer
  m_footerColumnCount = 2;
  const char* footItems[] = {"Back", "Save"};
  m_layoutManager->setFooter(footItems, 2);

  char loopNumbers[LayoutConstants::c_maxRowsPerLayout][5];

  m_itemsCount = m_presetView->loopsCount / LayoutConstants::c_maxColumnsPerRow;
  uint8_t rowIndex = 0;
  for (uint8_t i = 0; i < m_presetView->loopsCount; i += LayoutConstants::c_maxColumnsPerRow) {
    uint8_t columnIndex = 0;
    Row row;
    row.alignment = Row::kJustify;
    row.columnsCount = LayoutConstants::c_maxColumnsPerRow;

    for (uint8_t j = 0; j < LayoutConstants::c_maxColumnsPerRow; j++) {
      uint8_t loopIndex = i + j;

      if (loopIndex < m_presetView->loopsCount) {
        uint8_t number = m_presetView->loops[loopIndex].index;
        char* buffer = loopNumbers[rowIndex * LayoutConstants::c_maxColumnsPerRow + j];
        uint8_t index = 0;

        if (number >= 10) {
          buffer[index++] = '0' + (number / 10);
        }
        buffer[index++] = '0' + (number % 10);

        buffer[index++] = ' ';
        buffer[index++] = 0x10;
        buffer[index] = '\0';

        if (m_presetView->loops[loopIndex].isActive) {
          if (loopIndex == m_sourceLoop && m_swappingMode) {
            row.columns[j] = { Column::kLabel, Column::kHighlightedAndAnnotated, buffer, nullptr, 0 };
          }
          else {
            row.columns[j] = { Column::kLabel, Column::kHighlighted, buffer, nullptr, 0 };
          }
        }
        else {
          if (loopIndex == m_sourceLoop && m_swappingMode) {
            row.columns[j] = { Column::kLabel, Column::kAnnotated, buffer, nullptr, 0 };
          }
          else {
            row.columns[j] = { Column::kLabel, Column::kNormal, buffer, nullptr, 0 };
          }
        }

        columnIndex++;
      }
      else {
        row.columns[j] = { Column::kLabel, Column::kNormal, "", nullptr, 0 };
      }

      m_rowColumnCounts[rowIndex] = columnIndex;
    }

    m_layoutManager->addRow(row);
    m_rowCounts[rowIndex] = i;
    rowIndex++;
  }

  if (m_swappingMode && !m_isFooterActive) {
    m_layoutManager->setCursor(Cursor::kAsterisk);
  }
  else {
    m_layoutManager->setCursor(Cursor::kArrow);
  }

  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
  m_layoutManager->render();
}

void LoopOrderMenu::reset() {
  m_selectedRow = 0;
  m_selectedColumn = 0;
  m_startIndex = 0;
  m_isFooterActive = false;
  m_swappingMode = false;
  m_saveRequested = false;
  m_backRequested = false;
  m_sourceLoop = 0;
  m_targetLoop = 0;
  m_layoutManager->setIsFooterActive(false);
  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
  m_layoutManager->setCursor(Cursor::kArrow);
}

void LoopOrderMenu::handleAction(MenuInputAction t_action) {
  switch (t_action) {
    case MenuInputAction::kPress:
      if (!m_swappingMode) {
        if (!m_isFooterActive) {
          m_presetView->loops[m_selectedRow * LayoutConstants::c_maxColumnsPerRow + m_selectedColumn].isActive = !m_presetView->loops[m_selectedRow * LayoutConstants::c_maxColumnsPerRow + m_selectedColumn].isActive;
        }
        else {
          if (m_selectedColumn == 0) {
            m_backRequested = true;
          }
          if (m_selectedColumn == 1) {
            m_saveRequested = true;
          }
        }
      }
      else {
        m_targetLoop = m_selectedRow * LayoutConstants::c_maxColumnsPerRow + m_selectedColumn;

        if (m_sourceLoop < m_presetView->loopsCount && m_targetLoop < m_presetView->loopsCount) {
          uint8_t sourceLoopIndex = m_presetView->loops[m_sourceLoop].index;
          uint8_t sourceLoopActive = m_presetView->loops[m_sourceLoop].isActive;
          uint8_t targetLoopIndex = m_presetView->loops[m_targetLoop].index;
          uint8_t targetLoopActive = m_presetView->loops[m_targetLoop].isActive;

          m_presetView->loops[m_sourceLoop].index = targetLoopIndex;
          m_presetView->loops[m_sourceLoop].isActive = targetLoopActive;
          m_presetView->loops[m_targetLoop].index = sourceLoopIndex;
          m_presetView->loops[m_targetLoop].isActive = sourceLoopActive;
        }

        m_swappingMode = false;
      }
      break;

    case MenuInputAction::kLongPress:
      if (!m_swappingMode) {
        m_swappingMode = true;
        m_sourceLoop = m_selectedRow * LayoutConstants::c_maxColumnsPerRow + m_selectedColumn;
      }
      break;

    case MenuInputAction::kUp:
    case MenuInputAction::kDown:
    default:
      break;
  }
}

bool LoopOrderMenu::isSaveRequested() {
  bool requested = m_saveRequested;
  m_saveRequested = false;

  return requested;
}

bool LoopOrderMenu::isBackRequested() {
  bool requested = m_backRequested;
  m_backRequested = false;

  return requested;
}
