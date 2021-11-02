#pragma once
#include "peace.h"
class Tpeace : public peace
{
private:
	int rt = 0;
public:
	Tpeace(int size) {
		blockA = { 0 , 0 };
		blockB = { 1 , 0 };
		blockC = { 2 , 0 };
		blockD = { 1 , size };
		width = 3;
		height = 2;
		swt = 1;
	}
	bool Rotation(vector<vector<int>>& board, int size) override {
		bool x = rt & (1 << 1); // x*y(...) AND		x(...) IF(x)     (1-x)(...) !IF(x)		(x+y)(...) OR but (xy = 11 -> 2)
		bool y = rt & 1;

		movedA = { blockA.first + (1 - (2*x)), blockA.second + (size * (1 - (2*y))) };
		movedB = { blockB.first, blockB.second };
		movedC = { blockC.first + ((2*x) - 1), blockC.second + (size * ((2*y) - 1)) };
		movedD = { blockD.first + (1 - (2*y)), blockD.second + (size * ((2*x) - 1)) };

		if (peace::Rotation(board, size)) {
			rt ^= swt; // 00 01 11 10
			swt = ~swt;
			return true;
		}return false;
	}
};