#pragma once
#include <iostream>
#include <string>

#include "zComponent.h"
#include "zSystem.h"

/*______________________________________________________________________
	Position
________________________________________________________________________*/
struct Position {
	int x{ 0 };
	int y{ 0 };
};

void PrintPosition(System& s) {
	std::cout << s.c<Position>().x << "|" << s.c<Position>().y << std::endl;
}

struct PrintPositionSys : public System {
	std::string s = "hi";
	void UpdateComponent() override {
		std::cout << s << c<Position>().x << "|" << c<Position>().y << std::endl;
	}
};

/*______________________________________________________________________
	Velocity
________________________________________________________________________*/
struct Velocity {
	int x{ 0 };
	int y{ 0 };
};