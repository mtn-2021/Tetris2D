#pragma once
#include "framework.h"
#include <bitset>

using namespace std;
class peace {
private:
	pair<int, int> prev = { -1,-1 };
	pair<int, int> blockA = { 0,0 };
	pair<int, int> blockB = { 0,0 };
	pair<int, int> blockC = { 0,0 };
	pair<int, int> blockD = { 0,0 };
	int width = 0;
	int height = 0;
	int swt = 1;
	int rt = 0;
public:
	virtual bool DropDown(vector<vector<int>>& board, int moveSp,int size) {
		return false;
	}
	virtual void Thift(vector<vector<int>>& board,int size ,int lr) {}
	virtual void Rotation(vector<vector<int>>& board, int size){}
	virtual void Fix(vector<vector<int>>& board, int size, int psnum) {}
	virtual void Draw(HWND hWnd,HDC hdc,int size,int margin,int mvsp, int color[]){}
};