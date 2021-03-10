#pragma once
#include "Resources.h"
#include <string>	// pch?

struct GameEvent {
	std::string prologue;
	std::string consequence;
	Inventory reward; //= {0};

    /*GameEvent& operator=(const GameEvent& other)
    {
        if (this != &other) {
            this->prologue = other.prologue;
            this->consequence = other.consequence;

            for (unsigned int i = 0; i < numberOfResources; i++)
                this->reward[i] = other.reward[i];
        }
        return *this;
    }*/
};