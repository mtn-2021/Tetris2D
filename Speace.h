#pragma once
#include "peace.h"
class Speace : public peace
{
public:
	Speace(int size) {
		blockA = { 0 , size };
		blockB = { 1 , size };
		blockC = { 1 , 0 };
		blockD = { 2 , 0 };
		width = 3;
		height = 2;
	}
	bool Rotation(vector<vector<int>>& board, int size) override {
		bool x = swt & 1;

		movedA = { blockA.first - (4*x - 2), blockA.second };
		movedB = { blockB.first - (2*x - 1), blockB.second + (size * (2*x - 1)) };
		movedC = { blockC.first, blockC.second };
		movedD = { blockD.first + (2*x - 1), blockD.second + (size * (2*x - 1)) };

		if (peace::Rotation(board, size)) {
			swt = ~swt;
			return true;
		}return false;
	}
};


