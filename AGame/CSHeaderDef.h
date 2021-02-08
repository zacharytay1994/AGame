#pragma once
#include <iostream>
#include <string>

#include "zComponent.h"
#include "zSystem.h"

/*______________________________________________________________________
	Component	- Example_Position			
	System		- Example_PrintPosition		<Position>
					.. Prints out position to the console.
				- Example_PrintPositionSys	<Position>
					.. Prints out position to the console.
________________________________________________________________________*/
struct Example_Position {
	float x{ 0.0f };
	float y{ 0.0f };
};

void Example_PrintPosition(System& s) {
	std::cout << s._current_id << ": " << s.c<Example_Position>().x << "|" << s.c<Example_Position>().y << std::endl;
}

struct Example_PrintPositionSys : public System {
	std::string s = "hi ";
	void UpdateComponent() override {
		std::cout << s << c<Example_Position>().x << "|" << c<Example_Position>().y << std::endl;
	}
};

/*______________________________________________________________________
	Component	- Example_Velocity
	System		- Example_UpdatePosition <Position, Velocity>
					.. Adds velocity to the position.
________________________________________________________________________*/
struct Example_Velocity {
	float x{ 0.0f };
	float y{ 0.0f };
};

struct Example_UpdatePosition : public System {
	std::string s = "I am updating position of entity: ";
	void UpdateComponent() override {
		c<Example_Position>().x += c<Example_Velocity>().x * _dt;
		c<Example_Position>().y += c<Example_Velocity>().y * _dt;
		Example_Position& test = c<Example_Position>();
		std::cout << s << _current_id << " |" << c<Example_Position>().x << "," << c<Example_Position>().y << std::endl;
		RemoveEntity();
	}
};