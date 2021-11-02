#pragma once
#include "peace.h"
class Lpeace : public peace
{
private:
	int rt = 0;
public:
	Lpeace(int size) {
		blockA = { 0 , 0 };
		blockB = { 0 , size };
		blockC = { 0 , size * 2 };
		blockD = { 1 , size * 2 };
		width = 2;
		height = 3;
		swt = 1;
	}
	bool Rotation(vector<vector<int>>& board, int size) override {
		bool x = rt & (1 << 1); // x*y(...) AND		x(...) IF(x)     (1-x)(...) !IF(x)		(x+y)(...) OR but (xy = 11 -> 2)
		bool y = rt & 1;

		movedA = { blockA.first + ((2*x*y) - (2*x) + y), blockA.second + (size * ((2*x*y) - (3*x) - (2*y) + 2)) };
		movedB = { blockB.first + ((2*x*y) - (2*x) - y + 1), blockB.second + (size * ((2*x*y) - x - (2*y) + 1)) };
		movedC = { blockC.first + ((2*x*y) - (2*x) - (3*y) + 2), blockC.second + (size * ((2*x*y) + x - (2*y))) };
		movedD = { blockD.first + ((2*x*y) - (3*y) + 1), blockD.second + (size * ((2*x*y) + x - 1)) };

		if (peace::Rotation(board, size)) {
			rt ^= swt; // 00 01 11 10
			swt = ~swt;
			return true;
		}return false;
	}
};
