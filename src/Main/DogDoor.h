#ifndef DOGDOOR_H
#define DOGDOOR_H

#include <Arduino.h>

void DogDoorSetup();
void DogDoorLoop();
bool IsTagAuthorized(byte* tagData, byte dataSize);
void OpenDoor();

#endif
