#ifdef __linux__

#include "states/FightState.h"

FightState::FightState() {
}

FightState::~FightState() {
}

void FightState::advanceFrame() {
}

void FightState::netPlayHandleInput() {
}

void FightState::saveState() {
}

void FightState::saveState(unsigned char** buffer, int* length, int frame) {
}
void FightState::loadState() {
}

void FightState::loadState(unsigned char* buffer, int length) {
}


void FightState::enter(){}
void FightState::exit(){}
void FightState::resume(){}
void FightState::pause(){}
void FightState::handleInput(){}
void FightState::update(){}
void FightState::draw(){}

#endif