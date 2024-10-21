#include "preset.h"

void Preset::setup(uint8_t t_bank, uint8_t t_preset, Loops* t_loops, uint8_t t_loopsCount) {
    m_bank = t_bank;
    m_preset = t_preset;
    m_presetLoops = t_loops;

    LOG_DEBUG("Setting up preset: Bank %d, Preset %d", t_bank, t_preset);

    m_presetLoops->setLoopsCount(t_loopsCount);
    m_presetLoops->loopsSetup();
}

uint8_t Preset::getBank() const {
    return m_bank;
}

void Preset::setBank(uint8_t t_bank) {
    m_bank = t_bank;
    LOG_DEBUG("Bank set to %d", t_bank);
}

uint8_t* Preset::getPreset() {
    return &m_preset;
}

void Preset::setPreset(uint8_t t_preset) {
    m_preset = t_preset;
    LOG_DEBUG("Preset set to %d", t_preset);
}

uint8_t* Preset::getPresetLoopsId() {
    return m_presetLoops->getLoopsId();
}

uint8_t* Preset::getPresetLoopsStates() {
    return m_presetLoops->getLoopsState();
}

uint8_t* Preset::getPresetLoopsOrder() {
    return m_presetLoops->getLoopsOrder();
}

void Preset::swapPresetLoopsOrder(uint8_t t_id1, uint8_t t_order1, uint8_t t_id2, uint8_t t_order2) {
    LOG_DEBUG("Swapping loop order: ID1: %d, Order1: %d, ID2: %d, Order2: %d", t_id1, t_order1, t_id2, t_order2);
    m_presetLoops->setLoopOrder(t_id1, t_order2);
    m_presetLoops->setLoopOrder(t_id2, t_order1);
}

uint8_t Preset::getPresetLoopIdByOrder(uint8_t t_order) {
    return m_presetLoops->getLoopIdByOrder(t_order);
}

void Preset::setPresetLoopSendReturn(uint8_t t_loop, uint8_t t_send, uint8_t t_ret) {
    LOG_DEBUG("Setting loop send/return: Loop: %d, Send: %d, Return: %d", t_loop, t_send, t_ret);
    m_presetLoops->setLoopSend(t_loop, t_send);
    m_presetLoops->setLoopReturn(t_loop, t_ret);
}

uint8_t Preset::getPresetLoopSend(uint8_t t_loop) {
    return m_presetLoops->getLoopSend(t_loop);
}

uint8_t Preset::getPresetLoopReturn(uint8_t t_loop) {
    return m_presetLoops->getLoopReturn(t_loop);
}

void Preset::setLoopState(uint8_t t_loop, uint8_t t_state) {
    LOG_DEBUG("Setting loop state: Loop %d, State %d", t_loop, t_state);
    m_presetLoops->setLoopState(t_loop, t_state);
}

uint8_t Preset::getLoopState(uint8_t t_loop) {
    return m_presetLoops->getLoopState(t_loop);
}

void Preset::toggleLoopState(uint8_t t_loop) {
    LOG_DEBUG("Toggling loop state: Loop %d", t_loop);
    m_presetLoops->toggleLoopsState(t_loop);
}

uint8_t Preset::getActiveLoopsCount() {
    uint8_t count = 0;
    for (uint8_t i = 0; i < m_presetLoops->getLoopsCount(); i++) {
        if (m_presetLoops->getLoopState(i) == 1) {
            count++;
        }
    }

    LOG_DEBUG("Active loops count: %d", count);
    return count;
}
