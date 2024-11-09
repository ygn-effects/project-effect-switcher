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
const char* settingsItems[] = {"Loops Order", "MIDI", "Expression", "System", "Other1", "Othe2r", "Other3"};
ListMenu settingsMenu(&display, settingsItems, 7);
LoopOrderMenu loopsMenu(&display, nullptr);
MidiMessageMenu midiMessagesMenu(&display, nullptr);
MidiMessageEditMenu midiMessageEditMenu(&display, nullptr, 0);

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

  pollSwitch(menuEncoderSwitch, m_menuEncoderSwitchPress, m_menuEncoderSwitchLongPress);
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

void Hardware::transitionToState(SystemState t_newState) {
  m_systemState = t_newState;

  if (t_newState == SystemState::kPresetState) {
    settingsMenu.reset();
    loopsMenu.reset();
    midiMessagesMenu.reset();
    midiMessageEditMenu.reset();
    menuManager.setMenu(&homeMenu);
    menuManager.update();
  }
  else if (t_newState == SystemState::kSettingsState) {
    menuManager.reset();
    menuManager.setMenu(&settingsMenu);
    menuManager.update();
  }
  else if (t_newState == SystemState::kLoopsEditState) {
    menuManager.setMenu(&loopsMenu);
    menuManager.reset();
    loopsMenu.setCurrentPreset(presetManager.getCurrentPreset());
    menuManager.update();
  }
  else if (t_newState == SystemState::kMidiMessagesState) {
    menuManager.setMenu(&midiMessagesMenu);
    menuManager.reset();
    midiMessagesMenu.setCurrentPreset(presetManager.getCurrentPreset());
    menuManager.update();
  }
  else if (t_newState == SystemState::kMidiMessageEditState) {
    menuManager.setMenu(&midiMessageEditMenu);
    menuManager.reset();
    midiMessageEditMenu.setCurrentPreset(presetManager.getCurrentPreset());
    midiMessageEditMenu.setMidiMessageIndex(midiMessagesMenu.getStartIndex() + midiMessagesMenu.getSelectedIndex());
    midiMessageEditMenu.setMessageEditMode(true);
    menuManager.update();
  }
  else if (t_newState == SystemState::kMidiMessageAddState) {
    menuManager.setMenu(&midiMessageEditMenu);
    menuManager.reset();
    midiMessageEditMenu.setCurrentPreset(presetManager.getCurrentPreset());
    midiMessageEditMenu.setMidiMessageIndex(midiMessagesMenu.getStartIndex() + midiMessagesMenu.getSelectedIndex());
    midiMessageEditMenu.setMessageEditMode(false);
    menuManager.update();
  }
}

void Hardware::processMenuInput() {
  if (m_menuEncoderMoveLeft) {
    menuManager.handleInput(MenuInputAction::kUp);
    menuManager.update();
  }

  if (m_menuEncoderMoveRight) {
    menuManager.handleInput(MenuInputAction::kDown);
    menuManager.update();
  }

  if (m_menuEncoderSwitchPress) {
    menuManager.handleInput(MenuInputAction::kPress);
    menuManager.update();
  }

  if (m_menuEncoderSwitchLongPress) {
    menuManager.handleInput(MenuInputAction::kLongPress);
    menuManager.update();
  }
}

void Hardware::processPresetState() {
  if (m_presetBankUpFswPress) {
    presetManager.setPresetBankUp();
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    menuManager.update();
  }

  if (m_presetBankDownFswPress) {
    presetManager.setPresetBankDown();
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    menuManager.update();
  }

  if (m_preset0FswPress) {
    presetManager.setCurrentPreset(0);
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    menuManager.update();
  }

  if (m_preset1FswPress) {
    presetManager.setCurrentPreset(1);
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    menuManager.update();
  }

  if (m_preset2FswPress) {
    presetManager.setCurrentPreset(2);
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    menuManager.update();
  }

  if (m_preset3FswPress) {
    presetManager.setCurrentPreset(3);
    homeMenu.setCurrentPreset(presetManager.getCurrentPreset());
    menuManager.update();
  }

  if (m_menuEditSwitchPress) {
    transitionToState(kSettingsState);
  }
}

void Hardware::processSettingsState() {
  processMenuInput();

  if (m_menuEditSwitchLongPress) {
    transitionToState(kPresetState);
  }

  if (settingsMenu.isItemSelected()) {
    switch (settingsMenu.getSelectedIndex()) {
      case 0:
        transitionToState(kLoopsEditState);
        break;

      case 1:
        transitionToState(kMidiMessagesState);
        break;

      case 2:
        break;

      case 3:
        break;

      case 4:
        break;

      default:
        break;
    }
  }
}

void Hardware::processLoopsEditState() {
  processMenuInput();

  if (loopsMenu.isToggleRequested()) {
    presetManager.toggleLoopState(presetManager.getLoopByOrder(loopsMenu.getSelectedLoop()));
    presetManager.saveCurrentPreset();

    menuManager.update();
  }

  if (loopsMenu.isSwapRequested()) {
    presetManager.swapLoops(presetManager.getLoopByOrder(loopsMenu.getSourceLoop()), presetManager.getLoopByOrder(loopsMenu.getTargetLoop()));
    presetManager.saveCurrentPreset();

    menuManager.update();
  }

  if (loopsMenu.isGoBackRequested()) {
    transitionToState(kSettingsState);
  }

  if (m_menuEditSwitchLongPress) {
    transitionToState(kPresetState);
  }
}

void Hardware::processMidiMessagesState() {
  processMenuInput();

  if (midiMessagesMenu.isEditRequested()) {
    transitionToState(kMidiMessageEditState);
  }

  if (midiMessagesMenu.isAddRequested()) {
    transitionToState(kMidiMessageAddState);
  }

  if (midiMessagesMenu.isGoBackRequested()) {
    transitionToState(kSettingsState);
  }

  if (m_menuEditSwitchLongPress) {
    transitionToState(kPresetState);
  }
}

void Hardware::processMidiMessageEditState() {
  processMenuInput();

  if (midiMessageEditMenu.isCancelRequested()) {
    transitionToState(kMidiMessagesState);
  }

  if (midiMessageEditMenu.isDeleteRequested()) {
    presetManager.removeMidiMessage(midiMessageEditMenu.getMidiMessageIndex());
    presetManager.saveCurrentPreset();
    transitionToState(kMidiMessagesState);
  }

  if(midiMessageEditMenu.isSaveRequested()) {
    presetManager.setMidiMessageValues(midiMessageEditMenu.getMidiMessageIndex() ,midiMessageEditMenu.getNewMessageType(), midiMessageEditMenu.getNewMessageChannel(), midiMessageEditMenu.getNewMessageDataByte1(), midiMessageEditMenu.getNewMessageDataByte2(), midiMessageEditMenu.getNewMessageHasDataByte2());
    presetManager.saveCurrentPreset();
    transitionToState(kMidiMessagesState);
  }

  if (midiMessagesMenu.isAddRequested()) {
    presetManager.addMidiMessage(midiMessageEditMenu.getNewMessageType(), midiMessageEditMenu.getNewMessageChannel(), midiMessageEditMenu.getNewMessageDataByte1(), midiMessageEditMenu.getNewMessageDataByte2(), midiMessageEditMenu.getNewMessageHasDataByte2());
    presetManager.saveCurrentPreset();
    transitionToState(kMidiMessagesState);
  }

  if (m_menuEditSwitchLongPress) {
    transitionToState(kPresetState);
  }
}

void Hardware::setup() {
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

void Hardware::startup() {
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
  menuManager.update();
  delay(100);
}

void Hardware::poll() {
  switch (m_systemState) {
    case kPresetState:
      pollMenuEditSwitch();
      pollFootSwitches();
      break;

    case kSettingsState:
    case kLoopsEditState:
    case kMidiMessagesState:
    case kMidiMessageAddState:
    case kMidiMessageEditState:
      pollMenuEditSwitch();
      pollMenuEncoder();
      break;

    default:
      break;
  }
}

void Hardware::process() {
  switch (m_systemState) {
    case kPresetState:
      processPresetState();
      break;

    case kSettingsState:
      processSettingsState();
      break;

    case kLoopsEditState:
      processLoopsEditState();
      break;

    case kMidiMessagesState:
      processMidiMessagesState();
      break;

    case kMidiMessageAddState:
    case kMidiMessageEditState:
      processMidiMessageEditState();

    default:
      break;
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

