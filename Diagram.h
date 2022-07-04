#pragma once
#include <stdlib.h>
#include <iostream>
class Diagram {
public:
	int X;
	int Y;
	int Turn;
	int BlockNum;

	Diagram() {
		X = 4;
		Y = 0;
		Turn = rand() % 4;
		BlockNum = rand()%7;
	}

	void MoveLeft() {
		X--;
	}

	void MoveRight() {
		X++;
	}

	void MoveDown() {
		Y++;
	}

	void MoveTurn() {
		Turn = (Turn + 1) % 4;
	}

	/*Diagram& operator=(Diagram& ref) {
		this->BlockNum = ref.BlockNum;
		this->Turn = ref.Turn;
		this->X = ref.X;
		this->Y = ref.Y;
		return *this;
	}*/
};