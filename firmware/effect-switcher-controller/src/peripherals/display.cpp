#include "display.h"

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
