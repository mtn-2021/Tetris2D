#pragma once
#include "peace.h"

class Zpeace : public peace
{
public:
	Zpeace(int size){
		blockA = { 0 , 0 };
		blockB = { 1 , 0 };
		blockC = { 1 , size };
		blockD = { 2, size };
		width = 3;
		height = 2;
	}
	bool Rotation(vector<vector<int>>& board, int size) override {
		bool x = swt & 1; // int a = (2*x - 1); || int a = x ? 1 : -1;

		movedA = { blockA.first - (2*x - 1), blockA.second + (size * (2*x - 1)) };
		movedB = { blockB.first, blockB.second };
		movedC = { blockC.first + (2*x - 1), blockC.second + (size * (2*x - 1)) };
		movedD = { blockD.first + (4*x - 2), blockD.second };
		
		if (peace::Rotation(board, size)) {
			swt = ~swt;
			return true;
		}return false;
	}
};

