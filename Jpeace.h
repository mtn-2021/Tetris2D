#pragma once
#include "peace.h"
class Jpeace : public peace
{
private:
	int rt = 0;
public:
	Jpeace(int size) {
		blockA = { 1 , 0 };
		blockB = { 1 , size };
		blockC = { 1 , size * 2 };
		blockD = { 0 , size * 2 };
		width = 2;
		height = 3;
		swt = 1;
	}
	bool Rotation(vector<vector<int>>& board, int size) override {
		bool x = rt & (1 << 1); // x*y(...) AND		x(...) IF(x)     (1-x)(...) !IF(x)		(x+y)(...) OR but (xy = 11 -> 2)
		bool y = rt & 1;

		// •’Ê‚ÉğŒ•ªŠò( y ? 1 : 2x - 2 ‚Æ‚©)‚ÅÀ‘•‚µ‚½•û‚ª‰Â“Ç«‚ª‚‚¢‚©
		movedA = { blockA.first + ((2 * x) + (3 * y) - (2 * x * y) - 2), blockA.second + (size * ((2 * x * y) - (3 * x) + 1)) };
		movedB = { blockB.first + ((2 * x) + y - (2 * x * y) - 1), blockB.second + (size * (x * ((2 * y) - 1))) };
		movedC = { blockC.first + ((2 * x) - y - (2 * x * y)), blockC.second + (size * (x + (2 * x * y) - 1)) };
		movedD = { blockD.first + ((-1 * y) - (2 * x * y) + 1), blockD.second + (size * (x - (2 * y) + (2 * x * y))) };

		if (peace::Rotation(board, size)) {
			rt ^= swt; // 00 01 11 10
			swt = ~swt;
			return true;
		}return false;
	}
};
