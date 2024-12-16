#pragma once
#include "Structs.h"

class Player
{
public:


	void MoveRight();
	void MoveLeft();

	Vector3 GetTranslate() { return translate; }

private:
	Vector3 translate = { 0.0f,0.0f,0.0f };
};