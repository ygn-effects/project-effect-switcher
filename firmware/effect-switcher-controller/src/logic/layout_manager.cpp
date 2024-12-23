#include "layout_manager.h"

void LayoutManager::clearRows() {
  m_rowsCount = 0;

  for (uint8_t i = 0; i < LayoutConstants::c_maxRowsPerLayout; i++) {
    m_rows[i] = {};
  }

  m_footer.columnsCount = 0;
  m_footer.alignment = Row::kLeft;
  for (uint8_t i = 0; i < LayoutConstants::c_maxColumnsPerRow; i++) {
      m_footer.columns[i] = {};
  }

  m_header.columnsCount = 0;
  m_header.alignment = Row::kLeft;
  for (uint8_t i = 0; i < LayoutConstants::c_maxColumnsPerRow; i++) {
      m_header.columns[i] = {};
  }
}

uint8_t LayoutManager::calculateTotalRowWidth(Row& row) {
  uint8_t totalRowWidth = 0;
  for (uint8_t j = 0; j < row.columnsCount; j++) {
    if (row.columns[j].width > 0) {
      totalRowWidth = row.columns[j].width;
    }
    else {
      totalRowWidth += m_display->calcTextWidth(row.columns[j].text) + m_display->getNewTab();

      if (row.columns[j].value != nullptr) {
        totalRowWidth += m_display->calcTextWidth(row.columns[j].value);
      }
    }
  }

  return totalRowWidth;
}

uint8_t LayoutManager::calculateColumnWidth(Column& t_column) {
  uint8_t totalColumnWidth = 0;

  if (t_column.width > 0) {
    totalColumnWidth = t_column.width;
  }
  else {
    totalColumnWidth += m_display->calcTextWidth(t_column.text);

    if (t_column.value != nullptr) {
      totalColumnWidth += m_display->calcTextWidth(t_column.value);
    }
  }

  return totalColumnWidth;
}

uint8_t LayoutManager::calculateStartingX(Row& row, uint8_t totalRowWidth, uint8_t gap) {
  switch (row.alignment) {
    case Row::kCenter:
      return (m_screenWidth - totalRowWidth) / 2;
    case Row::kRight:
      return m_screenWidth - totalRowWidth;
    case Row::kLeft:
      return m_display->getNewTab();
    case Row::kJustify:
      // Justify alignment will have gaps calculated separately
      return m_display->getNewTab();
    default:
      return 0;
  }
}

void LayoutManager::renderColumns(Row& row, uint8_t rowIndex, uint8_t xPosition, uint8_t yPosition, uint8_t newTab, uint8_t gap) {
  for (uint8_t j = 0; j < row.columnsCount; j++) {
    Column& column = row.columns[j];
    uint8_t columnWidth = calculateColumnWidth(column);

    // Print the cursor if this is the active row and column
    if (rowIndex == m_activeRow && j == m_activeColumn && !m_isFooterActive) {
      const char* cursor;

      switch (m_cursor) {
        case Cursor::kArrow:
          cursor = ">";
          break;

        case Cursor::kAsterisk:
          cursor = "*";
          break;

        default:
          cursor = ">";
          break;
      }

      m_display->printItem(cursor, xPosition - newTab, yPosition);
    }

    switch (column.style) {
      case Column::kAnnotated:
        m_display->printItem("*", xPosition - newTab, yPosition);
        m_display->printItem(column.text, xPosition, yPosition);

        break;

      case Column::kHighlightedAndAnnotated:
        m_display->printItem("*", xPosition - newTab, yPosition);

      case Column::kHighlighted:
        m_display->printHighlightedItem(column.text, xPosition, yPosition);
        break;

      case Column::kNormal:
      case Column::kBold:
      case Column::kValueHighLighted:
        m_display->printItem(column.text, xPosition, yPosition);

      default:
        break;
    }

    // Render the value if present
    if (column.value != nullptr) {
      // Update X position for the next column
      uint8_t valuePosition = xPosition + m_display->calcTextWidth(column.text);

      // Use a specific style for highlighted values
      if (column.style == Column::kValueHighLighted) {
        m_display->printHighlightedItem(column.value, valuePosition, yPosition);
      }
      else {
        m_display->printItem(column.value, valuePosition, yPosition);
      }
    }

    // Update X position for the next column
    xPosition += columnWidth + newTab;

    // Add gap for justify alignment
    if (row.alignment == Row::kJustify && j < row.columnsCount - 1) {
      xPosition += gap;
    }
  }
}

void LayoutManager::renderHeader() {
  if (m_header.columnsCount > 0) {
    m_display->drawInvertedLine(0);
    uint8_t xPosition = (m_screenWidth - m_display->calcTextWidth(m_header.columns[0].text)) / 2;
    m_display->printHighlightedItem(m_header.columns[0].text, xPosition, 0);
  }
}

void LayoutManager::renderFooter() {
  uint8_t totalRawWidth = calculateTotalRowWidth(m_footer);
  uint8_t xPosition = calculateStartingX(m_footer, totalRawWidth, 0);
  uint8_t yPosition = m_display->getHeight() - m_display->getNewLine();

  for (uint8_t i = 0; i < m_footer.columnsCount; i++) {
    Column& column = m_footer.columns[i];

    if (m_isFooterActive && m_activeColumn == i) {
      m_display->printItem(">", xPosition - m_display->getNewTab(), yPosition);
    }

    m_display->printItem(column.text, xPosition, yPosition);
    xPosition += m_display->calcTextWidth(column.text) + m_display->getNewTab();
  }
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

void LayoutManager::setIsFooterActive(bool t_active) {
  m_isFooterActive = t_active;
}

void LayoutManager::setCursor(Cursor t_cursor) {
  m_cursor = t_cursor;
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

void LayoutManager::render() {
  // Get formatting values
  uint8_t newLine = m_display->getNewLine();
  uint8_t newTab = m_display->getNewTab();

  if (m_header.columnsCount > 0) {
    renderHeader();
  }

  if (m_footer.columnsCount > 0) {
    renderFooter();
  }

  // Determine visible rows
  uint8_t endIndex = m_contentStartIndex + m_visibleRowsCount;
  if (endIndex > m_rowsCount) {
    endIndex = m_rowsCount;
  }

  // Render rows
  for (uint8_t i = m_contentStartIndex; i < endIndex; i++) {
    Row& row = m_rows[i];

    // Calculate Y position
    uint8_t yPosition = (i - m_contentStartIndex) * newLine + newLine;

    // Total row width and gap for justify alignment
    uint8_t totalRowWidth = calculateTotalRowWidth(row);
    uint8_t gap = 0;

    if (row.alignment == Row::kJustify && row.columnsCount > 1) {
      uint8_t remainingSpace = m_screenWidth - totalRowWidth;
      gap = remainingSpace / (row.columnsCount - 1);
    }

    // Starting X position
    uint8_t xPosition = calculateStartingX(row, totalRowWidth, gap);

    switch (row.alignment)
    {
      // Full screen will only have one column
      case Row::kFullScreen:
        m_display->printFullScreenItem(row.columns[0].text, m_screenHeight, m_screenWidth);
        break;

      case Row::kLeft:
      case Row::kCenter:
      case Row::kRight:
      case Row::kJustify:
        // Render the columns
        renderColumns(row, i, xPosition, yPosition, newTab, gap);
        break;

      default:
        break;
    }
  }

  m_display->render();
}