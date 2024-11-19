#include "display_manager.h"

void DisplayManager::setup() {
  m_ssd1306.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}

void DisplayManager::reset() {
  m_ssd1306.clearDisplay();
  m_ssd1306.setFont();
  m_ssd1306.setTextColor(SSD1306_WHITE);
}

void DisplayManager::render() {
  m_ssd1306.display();
}

uint16_t DisplayManager::calcTextWidth(const char* t_text) {
  int16_t x1, y1;
  uint16_t textWidth, height;
  m_ssd1306.getTextBounds(t_text, 0, 0, &x1, &y1, &textWidth, &height);

  return textWidth;
}

uint16_t DisplayManager::calcTextHeight(const char* t_text) {
  int16_t x1, y1;
  uint16_t textWidth, height;
  m_ssd1306.getTextBounds(t_text, 0, 0, &x1, &y1, &textWidth, &height);

  return height;
}

uint8_t DisplayManager::getWidth() {
  return m_width;
}

uint8_t DisplayManager::getHeight() {
  return m_height;
}

uint8_t DisplayManager::getNewLine() {
  return m_newLine;
}

uint8_t DisplayManager::getNewTab() {
  return m_newTab;
}

uint8_t DisplayManager::getMaxVisibleLines() {
  return m_height / m_newLine;
}

void DisplayManager::printItem(const char* t_text, uint8_t t_x, uint8_t t_y) {
  m_ssd1306.setCursor(t_x, t_y);
  m_ssd1306.write(t_text);
}

void DisplayManager::drawInvertedLine(uint8_t t_y) {
  m_ssd1306.fillRect(0, 0, m_width, m_newLine, SSD1306_WHITE);
  m_ssd1306.setTextColor(SSD1306_WHITE);
}
