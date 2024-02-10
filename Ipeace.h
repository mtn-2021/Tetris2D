#pragma once
#include "peace.h"
class Ipeace : public peace 
{
public:
	Ipeace(int size) {
		blockA = { 0 , 0 };
		blockB = { 0 , size };
		blockC = { 0 , size * 2 };
		blockD = { 0 , size * 3 };
		width = 1;
		height = 4;
	}
	bool Rotation(vector<vector<int>>& board, int size) override {
		bool x = swt & 1;

		movedA = { blockA.first + (2*x - 1), blockA.second - (size * (2*x - 1)) };
		movedB = { blockB.first , blockB.second };
		movedC = { blockC.first - (2*x - 1) , blockC.second + (size * (2*x - 1)) };
		movedD = { blockD.first - ((2*x - 1) * 2), blockD.second + (size * (4*x - 2)) };

		if (peace::Rotation(board, size)) {
			swt = ~swt;
			return true;
		}return false;
	}
};


