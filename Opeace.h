#pragma once
#include "peace.h"
class Opeace : public peace
{
public:
	Opeace(int size){
		blockA = { 0 , 0 };
		blockB = { 1 , 0 };
		blockC = { 1 , size };
		blockD = { 0 , size };
		width = 2;
		height = 2;
	}
	bool Rotation(vector<vector<int>>& board, int size) override { return true; }
};


