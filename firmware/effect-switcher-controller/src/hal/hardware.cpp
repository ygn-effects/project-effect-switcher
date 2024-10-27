#include "hardware.h"

MemoryManager memoryManager(0);
PresetManager presetManager(memoryManager);

Encoder menuEncoder(12, 13);
MomentarySwitch menuEncoderSwitch(14, 1000);

MomentarySwitch menuEditSwitch(11, 1000);
Led menuEditSwitchLed(10);

MomentarySwitch presetBankDownFsw(24, 1000);
MomentarySwitch presetBankUpFsw(25, 1000);
MomentarySwitch preset0Fsw(26);
MomentarySwitch preset1Fsw(27);
MomentarySwitch preset2Fsw(28);
MomentarySwitch preset3Fsw(29);

// LedDriver16 presetLed(1);

// SwitchMatrix matrix(2);

Preset presetBank[c_maxPresets];

Display display(128, 64, 9, 1);
MenuManager menuManager;
HomeMenu homeMenu(&display, nullptr);

void Hardware::pollSwitch(MomentarySwitch& t_switch, bool& t_pressFlag) {
  t_switch.poll();

  t_pressFlag = t_switch.isPushed();
}

void Hardware::pollSwitch(MomentarySwitch& t_switch, bool& t_pressFlag, bool& t_longPressFlag) {
  t_switch.poll();

  t_pressFlag = t_switch.isPushed();
  t_longPressFlag = t_switch.isLongPress();
}

void Hardware::pollMenuEncoder() {
  if (menuEncoder.poll()) {
    m_menuEncoderMove = true;

    if (menuEncoder.isMovedRight()) {
      m_menuEncoderMoveRight = true;
    }
    else {
      m_menuEncoderMoveLeft = true;
    }
  }

  pollSwitch(menuEncoderSwitch, m_menuEditSwitchPress, m_menuEncoderSwitchLongPress);
}

void Hardware::pollMenuEditSwitch() {
  pollSwitch(menuEditSwitch, m_menuEditSwitchPress, m_menuEditSwitchLongPress);
}

void Hardware::pollFootSwitches() {
  pollSwitch(presetBankUpFsw, m_presetBankUpFswPress, m_presetBankDownFswLongPress);
  pollSwitch(presetBankDownFsw, m_presetBankDownFswPress, m_presetBankDownFswLongPress);
  pollSwitch(preset0Fsw, m_preset0FswPress);
  pollSwitch(preset1Fsw, m_preset1FswPress);
  pollSwitch(preset2Fsw, m_preset2FswPress);
  pollSwitch(preset3Fsw, m_preset3FswPress);
}

void Hardware::setup()
{
  delay(500);
  display.setup();
  menuEncoder.setup();
  menuEncoderSwitch.setup();
  menuEditSwitch.setup();
  menuEditSwitchLed.setup();
  presetBankUpFsw.setup();
  presetBankDownFsw.setup();
  preset0Fsw.setup();
  preset1Fsw.setup();
  preset2Fsw.setup();
  preset3Fsw.setup();
  // presetLed.setup();
  // matrix.switchMatrixSetup();
  menuManager.setMenu(&homeMenu);
  // memoryManager.initializeTestData();
  delay(100);
}

void Hardware::startup()
{
  menuEncoderSwitch.poll();
  menuEncoder.poll();
  menuEditSwitch.poll();
  presetBankUpFsw.poll();
  presetBankDownFsw.poll();
  preset0Fsw.poll();
  preset1Fsw.poll();
  preset2Fsw.poll();
  preset3Fsw.poll();
  // memoryManager.readTestData();
  presetManager.initialize();
  delay(200);
  homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
  homeMenu.update();
  delay(100);
}

void Hardware::poll() {
  switch (m_systemState)
  {
    case kPresetState:
      pollMenuEditSwitch();
      pollFootSwitches();

      break;

    case kEditMenuState:
      pollMenuEditSwitch();
      pollMenuEncoder();
      pollMenuEditSwitch();

      break;

    default:
      break;
  }
}

void Hardware::process() {
  switch (m_systemState)
  {
  case kPresetState:
    processPresetState();
    break;

  case kEditMenuState:

    break;

  default:
    break;
  }
}

void Hardware::processPresetState() {
  if (m_presetBankUpFswPress) {
    presetManager.setPresetBankUp();
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    homeMenu.update();
  }

  if (m_presetBankDownFswPress) {
    presetManager.setPresetBankDown();
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    homeMenu.update();
  }

  if (m_preset0FswPress) {
    presetManager.setCurrentPreset(0);
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    homeMenu.update();
  }

  if (m_preset1FswPress) {
    presetManager.setCurrentPreset(1);
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    homeMenu.update();
  }

  if (m_preset2FswPress) {
    presetManager.setCurrentPreset(2);
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    homeMenu.update();
  }

  if (m_preset3FswPress) {
    presetManager.setCurrentPreset(3);
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    homeMenu.update();
  }
}

void Hardware::resetTriggers()
{
  m_menuEncoderMove = false;
  m_menuEncoderMoveRight = false;
  m_menuEncoderMoveLeft = false;
  m_menuEncoderSwitchPress = false;
  m_menuEncoderSwitchLongPress = false;
  m_menuEditSwitchPress = false;
  m_menuEncoderSwitchLongPress = false;
  m_presetBankUpFswPress = false;
  m_presetBankUpFswLongPress = false;
  m_presetBankDownFswPress = false;
  m_presetBankDownFswLongPress = false;
  m_preset0FswPress = false;
  m_preset1FswPress = false;
  m_preset2FswPress = false;
  m_preset3FswPress = false;

  // Reset the internal state flags of the switches
  menuEncoderSwitch.reset();
  menuEditSwitch.reset();
  presetBankUpFsw.reset();
  presetBankDownFsw.reset();
  preset0Fsw.reset();
  preset1Fsw.reset();
  preset2Fsw.reset();
  preset3Fsw.reset();
}

