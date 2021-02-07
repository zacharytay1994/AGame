#pragma once
#include <iostream>

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