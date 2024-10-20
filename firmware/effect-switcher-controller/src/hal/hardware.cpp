#include "hardware.h"

Memory mem(0);

Encoder selector(12, 13);
MomentarySwitch selectorSwitch(14, 1000);
MomentarySwitch editSwitch(11, 1000);
Led editSwitchLed(10);

MomentarySwitch presetUpFsw(24, 1000);
MomentarySwitch presetDownFsw(25, 1000);
MomentarySwitch preset0Fsw(26, 1000);
MomentarySwitch preset1Fsw(27, 1000);
MomentarySwitch preset2Fsw(28, 1000);
MomentarySwitch preset3Fsw(29, 1000);

LedDriver16 presetLed(1);

SwitchMatrix matrix(2);

Preset presetBank[c_maxPresets];
Loops presetLoops[c_maxPresets];

void Hardware::hardwareSetup()
{
    Serial.begin(115200);
    delay(500);
    // mem.memorySetup();
    // selector.setup();
    // selectorSwitch.setup();
    // editSwitch.setup();
    // editSwitchLed.setup();
    // presetUpFsw.setup();
    // presetDownFsw.setup();
    // preset0Fsw.setup();
    // preset1Fsw.setup();
    // preset2Fsw.setup();
    // preset3Fsw.setup();
    // presetLed.setup();
    // matrix.switchMatrixSetup();

    //mem.memoryReset();
}

void Hardware::hardwarePoll()
{
    if (m_presetMenuDisplay)
    {
        presetUpFsw.poll();
        if (presetUpFsw.isPushed())
        {
            m_currentPresetBank++;
            m_currentPreset = 0;
            m_presetUpFswPress = true;
        }
        if (presetUpFsw.isLongPress())
        {
            m_presetUpFswLongPress = true;
        }

        presetDownFsw.poll();
        if (presetDownFsw.isPushed())
        {
            m_currentPresetBank--;
            m_currentPreset = 0;
            m_presetDownFswPress = true;
        }
        if (presetDownFsw.isLongPress())
        {
            m_presetDownFswLongPress = true;
        }

        preset0Fsw.poll();
        if (preset0Fsw.isPushed())
        {
            m_currentPreset = 0;
            m_preset0FswPress = true;
        }

        preset1Fsw.poll();
        if (preset1Fsw.isPushed())
        {
            m_currentPreset = 1;
            m_preset1FswPress = true;
        }

        preset2Fsw.poll();
        if (preset2Fsw.isPushed())
        {
            m_currentPreset = 2;
            m_preset2FswPress = true;
        }

        preset3Fsw.poll();
        if (preset3Fsw.isPushed())
        {
            m_currentPreset = 3;
            m_preset2FswPress = true;
        }

        editSwitch.poll();
        if (editSwitch.isPushed())
        {
            m_editSwitchPress = true;
        }
    }
    else if (m_presetEditMenuDisplay)
    {
        if (selector.poll())
        {
            m_selectorMove = true;
        }

        selectorSwitch.poll();
        if (selectorSwitch.isReleased())
        {
            m_selectorSwitchPress = true;
        }

        if (selectorSwitch.isLongPress())
        {
            m_selectorSwitchLongPress = true;
        }

        editSwitch.poll();
        if (editSwitch.isLongPress())
        {
            m_editSwitchLongPress = true;
        }
    }
    else
    {

    }
}

void Hardware::hardwareStartup()
{
    selectorSwitch.poll();
    selector.poll();
    editSwitch.poll();
    presetUpFsw.poll();
    presetDownFsw.poll();
    preset0Fsw.poll();
    preset1Fsw.poll();
    preset2Fsw.poll();
    preset3Fsw.poll();
}

void Hardware::restoreLastState()
{
    presetLed.setLedStateByMask(0x3);
    m_currentPresetBank = mem.readCurrentPresetBank();
    m_currentPreset = mem.readCurrentPreset();
    p_currentPreset = &presetBank[m_currentPreset];
    loadPresetBank();
}

void Hardware::resetHardwareTriggers()
{
    m_selectorMove = false;
    m_selectorSwitchPress = false;
    m_selectorSwitchLongPress = false;
    m_editSwitchPress = false;
    m_editSwitchLongPress = false;
    m_presetUpFswPress = false;
    m_presetUpFswLongPress = false;
    m_presetDownFswPress = false;
    m_presetDownFswLongPress = false;
    m_preset0FswPress = false;
    m_preset1FswPress = false;
    m_preset2FswPress = false;
    m_preset3FswPress = false;
}

void Hardware::processSelector()
{

}

void Hardware::processSelectorSwitchPress()
{

}

void Hardware::processSelectorSwitchLongPress()
{

}

void Hardware::setupMatrixLoops()
{
    for (uint8_t i = 0; i < c_maxPresets; i++)
    {
        presetBank[i].setPresetLoopSendReturn(0, c_loop0Send, c_loop0Return);
        presetBank[i].setPresetLoopSendReturn(1, c_loop1Send, c_loop1Return);
        presetBank[i].setPresetLoopSendReturn(2, c_loop2Send, c_loop2Return);
        presetBank[i].setPresetLoopSendReturn(3, c_loop3Send, c_loop3Return);
        presetBank[i].setPresetLoopSendReturn(4, c_loop4Send, c_loop4Return);
        presetBank[i].setPresetLoopSendReturn(5, c_loop5Send, c_loop5Return);
        presetBank[i].setPresetLoopSendReturn(6, c_buffer1LoopSend, c_buffer1LoopReturn);
        presetBank[i].setPresetLoopSendReturn(7, c_buffer2LoopSend, c_buffer2LoopReturn);
    }
}

void Hardware::connectMatrixLoops()
{
    uint8_t count = p_currentPreset->getActiveLoopsCount();
    uint8_t order[count] = {0};
    uint8_t orderIndex = 0;

    for (uint8_t i = 0; i < c_maxLoops; i++)
    {
        if (p_currentPreset->getLoopState(i) == 1)
        {
            order[orderIndex] = p_currentPreset->getPresetLoopIdByOrder(i);
            orderIndex++;
        }
    }

    for (uint8_t i = 0; i <= count; i++)
    {
        if (i == 0) // First loop send needs to be connected to the input return
        {
            matrix.setSwitchArray(c_inputLoopReturn, p_currentPreset->getPresetLoopSend(order[i]), 1);
        }
        else if (i == count) // Last loop return needs to be connected to the output send
        {
            matrix.setSwitchArray(p_currentPreset->getPresetLoopReturn(order[i - 1]), c_outputLoopSend, 1);
        }
        else
        {
            matrix.setSwitchArray(p_currentPreset->getPresetLoopReturn(order[i - 1]), p_currentPreset->getPresetLoopSend(order[i]), 1);
        }
    }
}


void Hardware::loadPresetBank()
{
    mem.writeCurrentPresetBank(m_currentPresetBank);

    if (m_currentPresetBank > (c_firstPresetBank + c_maxPresetBanks))
    {
        m_currentPresetBank = c_firstPresetBank;
    }
    if (m_currentPresetBank < c_firstPresetBank)
    {
        m_currentPresetBank = c_firstPresetBank + c_maxPresetBanks;
    }

    for (uint8_t i = 0; i < c_maxPresets; i++)
    {
        presetBank[i].presetSetup(m_currentPresetBank, i, &presetLoops[i], c_maxLoops);
        mem.readPreset(presetBank[i].getBank(), presetBank[i].getPreset(), presetBank[i].getPresetLoopsId(), presetBank[i].getPresetLoopsStates(), presetBank[i].getPresetLoopsOrder(), c_maxLoops);
    }

    setupMatrixLoops();
    loadPreset();
}

void Hardware::loadPreset()
{

}

void Hardware::savePreset()
{

}

bool Hardware::getSelectorMove()
{
    return m_selectorMove;
}

bool Hardware::getSelectorwitchPress()
{
    return m_selectorSwitchPress;
}

bool Hardware::getSelectorSwitchLongPress()
{
    return m_selectorSwitchLongPress;
}

bool Hardware::getEditSwitchPress()
{
    return m_editSwitchPress;
}

bool Hardware::getEditSwitchLongPress()
{
    return m_editSwitchLongPress;
}

bool Hardware::getPresetUpFswPress()
{
    return m_presetUpFswPress;
}

bool Hardware::getPresetDownFswPress()
{
    return m_presetDownFswPress;
}

bool Hardware::getPreset0FswPress()
{
    return m_preset0FswPress;
}

bool Hardware::getPreset1FswPress()
{
    return m_preset1FswPress;
}

bool Hardware::getPreset2FswPress()
{
    return m_preset2FswPress;
}

bool Hardware::getPreset3FswPress()
{
    return m_preset3FswPress;
}

bool Hardware::getPresetMenuDisplay()
{
    return m_presetMenuDisplay;
}

bool Hardware::getPresetEditMenuDisplay()
{
    return m_presetEditMenuDisplay;
}

bool Hardware::getPresetLoopEditMenuDisplay()
{
    return m_presetLoopEditMenuDisplay;
}

bool Hardware::getMainMenuDisplay()
{
    return m_mainMenuDisplay;
}