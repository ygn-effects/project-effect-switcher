#include "layout_manager.h"

void LayoutManager::clearRows() {
  m_rowsCount = 0;

  for (uint8_t i = 0; i < c_maxRowsPerLayout; i++) {
    m_rows[i] = {};
  }
}

void LayoutManager::renderHeader() {
  if (m_header.columnsCount > 0) {
    m_display->drawInvertedLine(0);
    uint8_t xPosition = (m_screenWidth - m_display->calcTextWidth(m_header.columns[0].text)) / 2;
    m_display->printItem(m_header.columns[0].text, xPosition, 0);
  }
}

void LayoutManager::renderFooter() {

}

uint8_t LayoutManager::getVisibleRowsCount() {
  return m_visibleRowsCount;
}

void LayoutManager::clear() {
  m_display->reset();
  clearRows();
}

void LayoutManager::setActiveRow(uint8_t t_row) {
  m_activeRow = t_row;
}

void LayoutManager::setActiveColumn(uint8_t t_column) {
  m_activeColumn = t_column;
}

void LayoutManager::setHeader(const char* t_title) {
  m_header.alignment = Row::kCenter;
  m_header.columnsCount = 1;
  m_header.columns[0] = { Column::kLabel, Column::kNormal, t_title, 0, 0 };
}

void LayoutManager::setFooter(const char* t_items[], uint8_t t_itemsCount) {
  m_footer.alignment = Row::kCenter;
  m_footer.columnsCount = t_itemsCount;

  for (uint8_t i = 0; i < t_itemsCount; i++) {
    m_footer.columns[i] = { Column::kLabel, Column::kNormal, t_items[i], 0, 0 };
  }
}

void LayoutManager::addRow(Row& t_row) {
  m_rows[m_rowsCount] = t_row;
  m_rowsCount++;
}

void LayoutManager::render()  {
  // Get formatting values
  uint8_t newLine = m_display->getNewLine();
  uint8_t newTab = m_display->getNewTab();

  renderHeader();

  // Determine the number of visible rows
  uint8_t endIndex = m_contentStartIndex + m_visibleRowsCount;
  if (endIndex > m_rowsCount) {
    endIndex = m_rowsCount;  // Avoid overflow
  }

  // Render visible rows
  for (uint8_t i = m_contentStartIndex; i < endIndex; i++) {
    // Get the current row
    Row& row = m_rows[i];

    // Calculate Y position for the row and account for the header
    uint8_t yPosition = (i - m_contentStartIndex) * newLine + newLine;

    // Calculate total row width
    uint8_t totalRowWidth = 0;
    for (uint8_t j = 0; j < row.columnsCount; j++) {
      totalRowWidth += m_display->calcTextWidth(row.columns[j].text) + newTab;
    }

    // Calculate spacing between columns for justify alignment
    uint8_t gap = 0;
    if (row.alignment == Row::kJustify && row.columnsCount > 1) {
      uint8_t remainingSpace = m_screenWidth - totalRowWidth;
      gap = remainingSpace / (row.columnsCount - 1);  // Distribute space evenly
    }

    // Set starting X position based on alignment
    uint8_t xPosition = 0;
    if (row.alignment == Row::kCenter) {
      xPosition = (m_screenWidth - totalRowWidth) / 2;
    }
    else if (row.alignment == Row::kRight) {
      xPosition = m_screenWidth - totalRowWidth;
    }
    else if (row.alignment == Row::kLeft || row.alignment == Row::kJustify) {
      xPosition = newTab;
    }

    // Render each column
    for (uint8_t j = 0; j < row.columnsCount; j++) {
      Column& column = row.columns[j];

      // If this is the active row and column, print the cursor
      if (i == m_activeRow && j == m_activeColumn) {
        m_display->printItem(">", xPosition - newTab, yPosition); // Print cursor at the start of the row
      }

      // Print the column text
      m_display->printItem(column.text, xPosition, yPosition);

      // Update X position for the next column
      xPosition += m_display->calcTextWidth(column.text) + newTab;

      // Add gap if justified
      if (row.alignment == Row::kJustify && j < row.columnsCount - 1) {
        xPosition += gap;
      } else {
        xPosition += newTab;  // Default tab spacing
      }
    }
  }

  // Render the display
  m_display->render();
}