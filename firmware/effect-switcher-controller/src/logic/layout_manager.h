#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include "utils/logging.h"

constexpr uint8_t c_maxRowsPerLayout = 20;
constexpr uint8_t c_maxColumnsPerLine = 4;

struct Column {
  enum ColumnType {
    kLabel,
    kLabelAndValue
  };

  enum Style {
    kNormal,
    kBold,
    kHighlighted,
    kValueHighLighted
  };

  ColumnType type;
  Style style;
  const char* text;
  int value;
  uint8_t width;
};

struct Row {
  enum Alignment {
    kRight,
    kCenter,
    kLeft,
    kJustify
  };

  Column columns[c_maxColumnsPerLine];
  uint8_t columnsCount;
  Alignment alignment;
};

class DisplayManager {
  private:
    Adafruit_SSD1306 m_ssd1306;
    uint8_t m_width;
    uint8_t m_height;
    uint8_t m_newLine;
    uint8_t m_newTab;

  public:
    DisplayManager(uint8_t t_width, uint8_t t_height) :
      m_ssd1306(t_width, t_height, &Wire, -1),
      m_width(t_width),
      m_height(t_height) {
        m_newTab = calcTextWidth("A");
        m_newLine = calcTextHeight("A");
      }

    void setup() {
      m_ssd1306.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    }

    void reset() {
      m_ssd1306.clearDisplay();
      m_ssd1306.setFont();
      m_ssd1306.setTextColor(SSD1306_WHITE);
    }

    void render() {
      m_ssd1306.display();
    }

    uint16_t calcTextWidth(const char* t_text) {
      int16_t x1, y1;
      uint16_t textWidth, height;
      m_ssd1306.getTextBounds(t_text, 0, 0, &x1, &y1, &textWidth, &height);

      return textWidth;
    }

    uint16_t calcTextHeight(const char* t_text) {
      int16_t x1, y1;
      uint16_t textWidth, height;
      m_ssd1306.getTextBounds(t_text, 0, 0, &x1, &y1, &textWidth, &height);

      return height;
    }

    uint8_t getWidth() {
      return m_width;
    }

    uint8_t getNewLine() {
      return m_newLine;
    }

    uint8_t getNewTab() {
      return m_newTab;
    }

    uint8_t getMaxVisibleLines() {
      return m_height / m_newLine;
    }

    void printItem(const char* t_text, uint8_t t_x, uint8_t t_y) {
      m_ssd1306.setCursor(t_x, t_y);
      m_ssd1306.write(t_text);
    }

    void drawHeaderBackground() {
      m_ssd1306.fillRect(0, 0, m_width, m_newLine, SSD1306_WHITE);
      m_ssd1306.setTextColor(SSD1306_WHITE);
    }
};

class LayoutManager {
  private:
    DisplayManager* m_display;
    uint8_t m_screenWidth;
    uint8_t m_rowsCount;
    uint8_t m_visibleRowsCount;
    uint8_t m_contentStartIndex;

    uint8_t m_activeRow;
    uint8_t m_activeColumn;

    Row m_rows[c_maxRowsPerLayout];
    Row m_header;
    Row m_footer;

    void clearRows() {
      // Reset the current row count
      m_rowsCount = 0;

      // Optionally, clear the row array (not necessary if overwritten later)
      for (uint8_t i = 0; i < c_maxRowsPerLayout; i++) {
        m_rows[i] = {};  // Reset to default values
      }
    }

    void renderHeader() {
      if (m_header.columnsCount > 0) {
        m_display->drawHeaderBackground();
        uint8_t xPosition = (m_screenWidth - m_display->calcTextWidth(m_header.columns[0].text)) / 2;
        m_display->printItem(m_header.columns[0].text, xPosition, 0);
      }
    }

    void renderFooter() {

    }

  public:
    LayoutManager(DisplayManager* t_displayManager) :
      m_display(t_displayManager),
      m_rowsCount(0),
      m_contentStartIndex(0) {
        m_screenWidth = m_display->getWidth();
        m_visibleRowsCount = m_display->getMaxVisibleLines();
      };

    uint8_t getVisibleRowsCount() {
      return m_visibleRowsCount;
    }

    void clear() {
      m_display->reset();
      clearRows();
    }

    void setActiveRow(uint8_t t_row) {
      m_activeRow = t_row;
    }

    void setActiveColumn(uint8_t t_column) {
      m_activeColumn = t_column;
    }

    void setHeader(const char* t_title) {
      m_header.alignment = Row::kCenter;
      m_header.columnsCount = 1;
      m_header.columns[0] = { Column::kLabel, Column::kNormal, t_title, 0, 0 };
    }

    void setFooter(const char* t_items[], uint8_t t_itemsCount) {
      m_footer.alignment = Row::kCenter;
      m_footer.columnsCount = t_itemsCount;

      for (uint8_t i = 0; i < t_itemsCount; i++) {
        m_footer.columns[i] = { Column::kLabel, Column::kNormal, t_items[i], 0, 0 };
      }
    }

    void addRow(Row& t_row) {
      m_rows[m_rowsCount] = t_row;
      m_rowsCount++;
    }

    void render() {
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
};
