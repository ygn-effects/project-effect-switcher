#include "list_menu.h"
#include "utils/logging.h"

void ListMenu::update() {
  m_layoutManager->clear();

  // Set header
  m_layoutManager->setHeader("Settings");

  // Set footer
  const char* footItems[] = {"Back"};
  m_layoutManager->setFooter(footItems, 1);

  // Visible Content Rows
  uint8_t endIndex = m_startIndex + m_visibleRowCount;
  if (endIndex > m_itemCount) {
    endIndex = m_itemCount;
  }

  uint8_t rowIndex = 0;
  for (uint8_t i = m_startIndex; i < endIndex; i++) {

    Row row;
    uint8_t m_footerColumnCount = 1;
    row.alignment = Row::kLeft;
    row.columnsCount = m_footerColumnCount;
    row.columns[0] = { Column::kLabel, Column::kNormal, m_menuItems[i], 0, 0 };

    m_layoutManager->addRow(row);

    m_rowCounts[rowIndex] = i;
    m_rowColumnCounts[rowIndex] = row.columnsCount;
    rowIndex++;
  }

  LOG_DEBUG("Selected row %d:", m_selectedRow);
  LOG_DEBUG("Selected column %d:", m_selectedColumn);

  m_layoutManager->setActiveRow(m_selectedRow);
  m_layoutManager->setActiveColumn(m_selectedColumn);
  m_layoutManager->render();
}

void ListMenu::reset() {
  m_selectedRow = 0;
  m_selectedColumn = 0;
  m_startIndex = 0;
  m_isFooterActive = false;
  m_layoutManager->setIsFooterActive(false);
}

void ListMenu::handleInput(MenuInputAction t_action) {
  switch (t_action) {
    case MenuInputAction::kUp: {
      if (!m_isFooterActive && m_selectedRow == m_itemCount - 1) {
        // Activate footer navigation when fully scrolled down
        m_isFooterActive = true;
        m_layoutManager->setIsFooterActive(true);
        m_selectedColumn = 0; // Start at the first column of the footer
      } else if (m_isFooterActive) {
        // Handle footer navigation
        if (m_selectedColumn < m_footerColumnCount - 1) {
          m_selectedColumn++;
        }
      } else if (m_selectedRow < m_itemCount - 1) {
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
    }

    case MenuInputAction::kDown: {
      LOG_DEBUG("DOWN");
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
          m_selectedRow = m_itemCount - 1;
          // Last column of the last row
          m_selectedColumn = m_rowColumnCounts[m_selectedRow] - 1;
        }
      } else if (m_selectedRow > 0) {
        // Normal navigation (not in the footer)
        if (m_selectedColumn == 0) {
          m_selectedRow--;
          m_selectedColumn = m_rowColumnCounts[m_selectedRow] - 1;
        } else {
          m_selectedColumn--;
        }

        // Scroll down if necessary
        if (m_selectedRow < m_startIndex) {
          m_startIndex--;
        }
      }
      break;
    }

    case MenuInputAction::kPress:
      if (m_isFooterActive) {
        if (m_selectedColumn == 0) {
          m_backrequested = true;
        }
        else {
          m_itemSelected = true;
        }
      }
      break;

    default:
      break;
  }
}


uint8_t ListMenu::getSelectedIndex() {
  return m_startIndex + m_selectedRow;
}

bool ListMenu::isItemSelected() {
  bool selected = m_itemSelected;
  m_itemSelected = false;

  return selected;
}

bool ListMenu::isBackRequested() {
  bool requested = m_backrequested;
  m_backrequested = false;

  return requested;
}
