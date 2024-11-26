#include "loop_menu.h"

void LoopOrderMenu::update() {
  m_layoutManager->clear();

  // Set header
  m_layoutManager->setHeader("Loops Order");

  // Set footer
  m_footerColumnCount = 2;
  const char* footItems[] = {"Cancel", "Save"};
  m_layoutManager->setFooter(footItems, 2);

  // Gather data from the preset
  const uint8_t loopsCount = m_currentPreset->getLoopsCount();
  uint8_t loopIndexes[loopsCount];
  uint8_t loopOrders[loopsCount];
  uint8_t loopStates[loopsCount];

  // Construct the arrays by loops order to send ordered data to the display
  for (uint8_t i = 0; i < loopsCount; i++) {
    const uint8_t loopIndex = m_currentPreset->getLoopIndexByOrder(i);

    if (loopIndex < loopsCount) {
      loopIndexes[i] = loopIndex;
      loopOrders[i] = i;
      loopStates[i] = m_currentPreset->getLoopState(loopIndex);
    }
  }

  char loopNumbers[LayoutConstants::c_maxRowsPerLayout][5];

  uint8_t rowIndex = 0;
  for (uint8_t i = 0; i < loopsCount; i += LayoutConstants::c_maxColumnsPerRow) {
    uint8_t columnIndex = 0;
    Row row;
    row.alignment = Row::kJustify;
    row.columnsCount = LayoutConstants::c_maxColumnsPerRow;

    for (uint8_t j = 0; j < LayoutConstants::c_maxColumnsPerRow; j++) {
      uint8_t loopIndex = i + j;

      if (loopIndex < loopsCount) {
        uint8_t number = loopIndexes[loopIndex];
        char* buffer = loopNumbers[rowIndex * LayoutConstants::c_maxColumnsPerRow + j];
        uint8_t index = 0;

        if (number >= 10) {
          buffer[index++] = '0' + (number / 10);
        }
        buffer[index++] = '0' + (number % 10);

        buffer[index++] = ' ';
        buffer[index++] = 0x10;
        buffer[index] = '\0';

        if (loopStates[loopIndex]) {
          row.columns[j] = { Column::kLabel, Column::kHighlighted, buffer, 0, 0 };
        }
        else {
          row.columns[j] = { Column::kLabel, Column::kNormal, buffer, 0, 0 };
        }

        columnIndex++;
      }
      else {
        row.columns[j] = { Column::kLabel, Column::kNormal, "", 0, 0 };
      }

      m_rowColumnCounts[rowIndex] = columnIndex;
    }

    m_layoutManager->addRow(row);
    m_rowCounts[rowIndex] = i;
    rowIndex++;

    m_itemsCount = rowIndex;
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
  m_toggleRequested = false;
  m_swapRequested = false;
  m_goBackRequested = false;
  m_sourceLoop = 0;
  m_targetLoop = 0;
  m_selectedLoop = 0;
}

void LoopOrderMenu::handleInput(MenuInputAction t_action) {
    switch (t_action)
    {
      case MenuInputAction::kUp:
        if (!m_isFooterActive && m_selectedRow == m_itemsCount - 1 && m_selectedColumn == m_rowColumnCounts[m_selectedRow] - 1) {
          // Activate footer navigation when fully scrolled down
          m_isFooterActive = true;
          m_layoutManager->setIsFooterActive(true);
          // Start at the first column of the footer
          m_selectedColumn = 0;
        }
        else if (m_isFooterActive) {
          // Handle footer navigation
          if (m_selectedColumn < m_footerColumnCount - 1) {
            m_selectedColumn++;
          }
        }
        else if (m_selectedRow < m_itemsCount) {
          // Normal navigation (not in the footer)
          if (m_selectedColumn == m_rowColumnCounts[m_selectedRow] - 1) {
            m_selectedRow++;
            m_selectedColumn = 0;
          } else {
            m_selectedColumn++;
          }

          // Scroll up if necessary
          if (m_selectedRow >= m_startIndex + m_visibleRowCount) {
            m_startIndex++;
          }
        }
        break;

      case MenuInputAction::kDown:
        if (m_isFooterActive) {
          // Handle navigation in the footer
          if (m_selectedColumn > 0) {
            // Scroll down the available columns
            m_selectedColumn--;
          } else {
            // Exit footer navigation and resume normal navigation
            m_isFooterActive = false;
            m_layoutManager->setIsFooterActive(false);
            // Last row
            m_selectedRow = m_itemsCount - 1;
            // Last column of the last row
            m_selectedColumn = m_rowColumnCounts[m_selectedRow] - 1;
          }
        }
        else if (m_selectedRow > 0) {
          // Normal navigation (not in the footer)
          if (m_selectedColumn == 0) {
            m_selectedRow--;
            m_selectedColumn = m_rowColumnCounts[m_selectedRow] - 1;
          }
          else {
            m_selectedColumn--;
          }

          // Scroll down if necessary
          if (m_selectedRow < m_startIndex) {
            m_startIndex--;
          }
        }
        else if (m_selectedRow == 0) {
          if (m_selectedColumn > 0 ) {
            m_selectedColumn--;
          }
        }
        break;

      default:
        break;
    }
  }

void LoopOrderMenu::setCurrentPreset(Preset* t_currentPreset) {
  m_currentPreset = t_currentPreset;
}

bool LoopOrderMenu::isToggleRequested() {
  bool requested = m_toggleRequested;
  m_toggleRequested = false; // Reset after checking

  return requested;
}

bool LoopOrderMenu::isSwapRequested() {
  bool requested = m_swapRequested;
  m_swapRequested = false; // Reset after checking

  return requested;
}

bool LoopOrderMenu::isGoBackRequested() {
  bool requested = m_goBackRequested;
  m_goBackRequested = false;

  return requested;
}

uint8_t LoopOrderMenu::getSelectedLoop() const {
  return m_selectedLoop;
}

uint8_t LoopOrderMenu::getSourceLoop() const {
  return m_sourceLoop;
}

uint8_t LoopOrderMenu::getTargetLoop() const {
  return m_targetLoop;
}