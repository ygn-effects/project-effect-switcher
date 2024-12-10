#pragma once

#include <Arduino.h>
#include "display_manager.h"
#include "utils/logging.h"

namespace LayoutConstants {
  constexpr uint8_t c_maxRowsPerLayout = 20;
  constexpr uint8_t c_maxColumnsPerRow = 4;
}

struct Column {
  enum ColumnType {
    kLabel,
    kLabelAndValue
  };

  enum Style {
    kNormal,
    kBold,
    kAnnotated,
    kHighlighted,
    kHighlightedAndAnnotated,
    kValueHighLighted
  };

  ColumnType type;
  Style style;
  const char* text;
  const char* value;
  uint8_t width;
};

struct Row {
  enum Alignment {
    kRight,
    kCenter,
    kLeft,
    kJustify,
    kFullScreen
  };

  Column columns[LayoutConstants::c_maxColumnsPerRow];
  uint8_t columnsCount;
  Alignment alignment;
};

enum Cursor {
  kArrow,
  kAsterisk
};

class LayoutManager {
  private:
    DisplayManager* m_display;
    uint16_t m_screenWidth;
    uint16_t m_screenHeight;
    uint8_t m_rowsCount;
    uint8_t m_visibleRowsCount;
    uint8_t m_contentStartIndex;

    uint8_t m_activeRow = 0;
    uint8_t m_activeColumn = 0;
    bool m_isFooterActive = false;
    Cursor m_cursor = Cursor::kArrow;

    Row m_rows[LayoutConstants::c_maxRowsPerLayout];
    Row m_header;
    Row m_footer;

    void clearRows();

    uint16_t calculateTotalRowWidth(Row& t_row);

    uint16_t calculateColumnWidth(Column& t_column);

    uint16_t calculateStartingX(Row& row, uint16_t totalRowWidth, uint8_t gap);

    void renderColumns(Row& row, uint8_t rowIndex, uint16_t xPosition, uint8_t yPosition, uint8_t newTab, uint8_t gap = 0);

    void renderHeader();

    void renderFooter();

  public:
    LayoutManager(DisplayManager* t_displayManager) :
      m_display(t_displayManager),
      m_rowsCount(0),
      m_contentStartIndex(0) {
        m_screenWidth = m_display->getWidth();
        m_screenHeight = m_display->getHeight();
        m_visibleRowsCount = m_display->getMaxVisibleLines();
      };

    uint8_t getVisibleRowsCount();

    void clear();

    void setActiveRow(uint8_t t_row);

    void setActiveColumn(uint8_t t_column);

    void setIsFooterActive(bool t_active);

    void setCursor(Cursor t_cursor);

    void setHeader(const char* t_title);

    void setFooter(const char* t_items[], uint8_t t_itemsCount);

    void addRow(Row& t_row);

    void render();
};
