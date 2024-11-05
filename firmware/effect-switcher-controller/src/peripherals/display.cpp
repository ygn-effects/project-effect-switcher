#include "display.h"

void Display::resetCursor() {
  m_ssd1306.setCursor(0, 0);  // Reset the cursor to the top-left corner
}

void Display::newLine() {
  uint16_t currentY = m_ssd1306.getCursorY();
  m_ssd1306.setCursor(0, currentY + m_newLine);  // Move the cursor to the next line
}

uint16_t Display::calcTextWidth(const char* text) {
  int16_t x1, y1;
  uint16_t textWidth, height;
  m_ssd1306.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &height);

  return textWidth;
}

uint16_t Display::calcTextHeight(const char* text) {
  int16_t x1, y1;
  uint16_t textWidth, height;
  m_ssd1306.getTextBounds(text, 0, 0, &x1, &y1, &textWidth, &height);

  return height;
}

void Display::renderScrollIndicator(bool t_up) {
  if (t_up) {
    m_ssd1306.setCursor(m_width - 8, m_newLine); // Top right corner
    m_ssd1306.write(c_scrollUpArrow); // Up arrow character
  } else {
    m_ssd1306.setCursor(m_width - 8, m_height - m_newLine); // Bottom right corner
    m_ssd1306.write(c_scrollDownArrow); // Down arrow character
  }
}

void Display::setup() {
  m_ssd1306.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize SSD1306 with I2C address 0x3C
  clear();
  render();
}

void Display::clear() {
  m_ssd1306.clearDisplay();  // Clear the display buffer
}

void Display::render() {
  m_ssd1306.display();  // Push the buffer to the OLED display
}

uint8_t Display::getHeight() {
  return m_height;
}

uint8_t Display::getLineHeight() {
  return m_newLine;
}

void Display::renderHeader(const char* text) {
  clear();
  resetCursor();

  m_ssd1306.fillRect(0, 0, m_width, m_newLine, WHITE); // White background
  m_ssd1306.setTextColor(SSD1306_BLACK);  // Invert colors for the header
  m_ssd1306.setFont(); // Reset to default font
  m_ssd1306.setCursor((m_width - calcTextWidth(text)) / 2, m_headerOffset);  // Center text
  m_ssd1306.print(text);
  m_ssd1306.setTextColor(SSD1306_WHITE);  // Reset text color after the header

  newLine();  // Move to the next line after the header
}

void Display::renderBankAndPreset(char bank, uint8_t preset) {
  m_ssd1306.setFont(&SourceCodePro_Bold32pt7b);

  // Center the bank and preset on the screen
  uint16_t textWidth = calcTextWidth("A-1");  // Assume max width for "A | 1"
  m_ssd1306.setCursor((m_width - textWidth) / 2, m_height - m_newLine / 2);  // Center the text

  m_ssd1306.print(bank);      // Print the bank character
  m_ssd1306.print("-");     // Separator
  m_ssd1306.print(preset);    // Print the preset number

  render();  // Push the buffer to the display
}

void Display::renderListMenu(const char* items[], uint8_t itemCount, uint8_t startIndex, uint8_t selectedIndex) {
  resetCursor();

  // Calculate how many items fit on the screen
  uint8_t maxVisibleItems = (m_height - m_newLine) / m_newLine;

  // Adjust startIndex to ensure selectedIndex is in view
  if (selectedIndex >= startIndex + maxVisibleItems) {
    startIndex = selectedIndex - maxVisibleItems + 1;
  } else if (selectedIndex < startIndex) {
    startIndex = selectedIndex;
  }

  // Render each item
  for (uint8_t i = 0; i < maxVisibleItems && (startIndex + i) < itemCount; i++) {
    const char* item = items[startIndex + i];

    // Highlight the selected item
    if (startIndex + i == selectedIndex) {
      m_ssd1306.setCursor(0, i * m_newLine + m_headerOffset + m_newLine);
      m_ssd1306.write(c_menuCursor);
    }

    m_ssd1306.setCursor(c_newTab, i * m_newLine + m_headerOffset + m_newLine);
    m_ssd1306.print(item);
  }

  // Render scroll indicators if needed
  if (startIndex > 0) {
    renderScrollIndicator(true); // Up indicator
  }
  if (startIndex + maxVisibleItems < itemCount) {
    renderScrollIndicator(false); // Down indicator
  }

  m_ssd1306.setTextColor(SSD1306_WHITE); // Reset text color after rendering the list
  render(); // Push the buffer to the display
}

void Display::renderLoopOrderList(const uint8_t* loopIndexes, const uint8_t* loopOrders, const uint8_t* loopStates, uint8_t loopCount, uint8_t selectedIndex, bool swappingMode) {
  m_ssd1306.setCursor(0, (m_height / 2));

  for (uint8_t i = 0; i < loopCount; i++) {
    // Highlight active loops
    if (loopStates[i]) {
      m_ssd1306.setTextColor(BLACK, WHITE);
      m_ssd1306.print(loopIndexes[i]);
      m_ssd1306.setTextColor(WHITE);
    }
    else {
      m_ssd1306.print(loopIndexes[i]);
    }
    if (i < loopCount - 1) {
      m_ssd1306.write(c_menuCursor);
    }
  }

  if (selectedIndex == loopCount) {
    m_ssd1306.setCursor(0, m_height - m_newLine);
    m_ssd1306.write(c_menuCursor);
  }
  else {
  // The width of a character is 6 pixels, so 12 to account for the arrows
  m_ssd1306.setCursor((selectedIndex * 12), ((m_height - m_newLine * 2) / 2));
  {
    if (swappingMode) {
      m_ssd1306.write(c_loopOrderSwap);
    }
    else {
      m_ssd1306.write(c_scrollDownArrow);
    }
  }
  }

  m_ssd1306.setCursor(c_newTab, m_height - m_newLine);
  m_ssd1306.print("Back");

  render();
}

void Display::renderMidiMessages(const uint8_t* types, const uint8_t* channels, const uint8_t* dataByte1, const uint8_t* dataByte2, uint8_t messageCount, uint8_t selectedIndex, uint8_t startIndex) {
  resetCursor();

  const uint8_t typeWidth = 46; // 7 chars width + some spacing
  const uint8_t channelWidth = 18; // 2 characters space + some spacing
  const uint8_t dataByteWidth = 24; // 3 characters + some spacing

  // Calculate how many items fit on the screen
  uint8_t maxVisibleItems = (m_height - m_newLine) / m_newLine;

  // Header
  m_ssd1306.fillRect(0, 0, m_width, m_newLine, WHITE); // White background
  m_ssd1306.setTextColor(SSD1306_BLACK);  // Invert colors for the header
  m_ssd1306.setFont(); // Reset to default font
  m_ssd1306.setCursor(c_newTab, m_headerOffset);  // Center text
  m_ssd1306.print("Type");
  m_ssd1306.setCursor(typeWidth + 2, m_headerOffset);
  m_ssd1306.print("Ch");
  m_ssd1306.setCursor(typeWidth + channelWidth + 2, m_headerOffset);
  m_ssd1306.print("B1");
    m_ssd1306.setCursor(typeWidth + channelWidth + dataByteWidth + 2, m_headerOffset);
  m_ssd1306.print("B2");
  m_ssd1306.setTextColor(SSD1306_WHITE);  // Reset text color after the header

  // Adjust startIndex to ensure selectedIndex is in view
  if (selectedIndex >= startIndex + maxVisibleItems) {
    startIndex = selectedIndex - maxVisibleItems + 1;
  } else if (selectedIndex < startIndex) {
    startIndex = selectedIndex;
  }

  for (uint8_t i = 0; i < maxVisibleItems && (startIndex + i) < messageCount; i++) {
    uint8_t cursorY = m_headerOffset + m_newLine + (i * m_newLine);
    uint8_t messageIndex = startIndex + i;

    // Print the cursor in front of the selected item
    if (messageIndex == selectedIndex) {
      m_ssd1306.setCursor(0, cursorY);
      m_ssd1306.write(c_menuCursor);
    }

    // Type
    m_ssd1306.setCursor(c_newTab, cursorY);
    switch (types[messageIndex]) {
      case 0xB0:
        m_ssd1306.print("CC");
        break;

      default:
        break;
    }

    // Channel
    m_ssd1306.setCursor(typeWidth + 2, cursorY);
    m_ssd1306.print(channels[messageIndex]);

    // Data byte 1
    m_ssd1306.setCursor(typeWidth + channelWidth + 2, cursorY);
    m_ssd1306.print(dataByte1[messageIndex]);

    // Data byte 2
    m_ssd1306.setCursor(typeWidth + channelWidth + dataByteWidth + 2, cursorY);
    m_ssd1306.print(dataByte2[messageIndex]);
  }

  // Check if there's room for "Add New Message" below the existing messages
  if (messageCount < maxVisibleItems) {
    uint8_t cursorY = m_headerOffset + m_newLine + (messageCount * m_newLine);

    // Display "Add New Message" with selection cursor if it’s selected
    if (selectedIndex == messageCount) {
      m_ssd1306.setCursor(0, cursorY);
      m_ssd1306.write(c_menuCursor);
    }
    m_ssd1306.setCursor(c_newTab, cursorY);
    m_ssd1306.print("Add");
  }

  // Check if there's room for "Back" below the existing messages
  if (messageCount + 1 < maxVisibleItems) {
    uint8_t cursorY = m_headerOffset + m_newLine + (messageCount * m_newLine + m_newLine);

    // Display "Add New Message" with selection cursor if it’s selected
    if (selectedIndex == messageCount + 1) {
      m_ssd1306.setCursor(0, cursorY);
      m_ssd1306.write(c_menuCursor);
    }
    m_ssd1306.setCursor(c_newTab, cursorY);
    m_ssd1306.print("Back");
  }

  // Render scroll indicators if needed
  if (startIndex > 0) {
    renderScrollIndicator(true); // Up indicator
  }
  if (startIndex + maxVisibleItems < messageCount) {
    renderScrollIndicator(false); // Down indicator
  }

  render();
}