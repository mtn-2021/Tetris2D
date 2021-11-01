#pragma once
#include "peace.h"
class Zpeace : public peace
{
private:
	pair<int, int> prev = { 10000, 10000 };
	pair<int, int> blockA = { 0,0 };
	pair<int, int> blockB = { 0,0 };
	pair<int, int> blockC = { 0,0 };
	pair<int, int> blockD = { 0,0 };
	pair<int, int>* list[4];
	int width = 0;
	int height = 0;
	int swt = 0;
	// int rt = 0;
	bool rted = false;

public:
	Zpeace(int size) : list{ &blockA,&blockB,&blockC,&blockD }{
		blockB.first = 1;
		blockC = { 1 , size };
		blockD = { 2, size };
		width = 3;
		height = 2;
	}

	bool DropDown(vector<vector<int>>& board,int moveSp,int size) override {
		for (int i = 0; i < 4; i++) {
			unsigned _int64 bottomPos = (list[i]->second / size);
			if (board.size() <= bottomPos + 1 ||
				board.at(bottomPos + 1).at(list[i]->first) != 0)
				return false;
		}
		for (int i = 0; i < 4; i++) {
			list[i]->second += moveSp;
		}
		return true;
		
	}
	void Thift(vector<vector<int>>& board,int size,int lr) override {
		for (int i = 0; i < 4; i++) {
			 unsigned _int64 verPos = list[i]->second / size;
			int movedPos = list[i]->first + lr;
			if (0 > movedPos ||
				movedPos >= board.at(0).size() ||
				((list[i]->second % size > size * 4 / 5) ?
					board.at(verPos + (int)1).at(movedPos) != 0 :
					board.at(verPos).at(movedPos) != 0) ||
				((list[i]->second % size > size * 1 / 5 ) ?
					board.at(verPos + (int)1).at(movedPos) != 0 :
					board.at(verPos).at(movedPos) != 0) 
				)
				return;
		}
		
		for (int i = 0; i < 4; i++) {
			if (prev.first > list[i]->first)
				prev.first = list[i]->first;
			if (prev.second > list[i]->second)
				prev.second = list[i]->second;
			list[i]->first += lr;
			if (list[i]->second % size > size * 4 / 5) {
				list[i]->second = ((list[i]->second / size) + 1) * size;
			}else if (list[i]->second % size <= size * 1 / 5) {
				list[i]->second = (list[i]->second / size) * size;
			}	
		}	
	}
		
	void Rotation(vector<vector<int>>& board, int size) override {
		int swip = swt & 1 ? 1 : -1;
		unsigned _int64 movedV;
		pair<int, int> movedA, movedB, movedC, movedD;
		pair<int, int>* movedList[] = { &movedA, &movedB, &movedC, &movedD };
		pair<int, int> semiPrev = { 10000,10000 };

		movedA = { blockA.first - swip, blockA.second + (size * swip) };
		movedB = { blockB.first, blockB.second };
		movedC = { blockC.first + swip, blockC.second + (size * swip) };
		movedD = { blockD.first + (swip * 2), blockD.second };
		
		/*
		int x = rt & (1 << 1) ? 1 : -1;
		int y = rt & 1 ? 1 : -1;
		movedA = { blockA.first - x, blockA.second + (size * y) }; //–{“–‚É‰ñ“]‚·‚éê‡
		movedB = { blockB.first, blockB.second };
		movedC = { blockC.first + y, blockC.second + (size * x) };
		movedD = { blockD.first + x + y, blockD.second + (size * (x - y)) };
		rt ^= swt; // 00 01 11 10
		swt = ~swt; 
		*/

		for (int i = 0; i < 4; i++) {

			if (semiPrev.first > list[i]->first)
				semiPrev.first = list[i]->first;
			if (semiPrev.second > list[i]->second)
				semiPrev.second = list[i]->second;

			movedV = movedList[i]->second / size;

			if (0 > (_int64)movedList[i]->second ||
				board.size() <= ((((_int64)movedList[i]->second - 1) / size) + 1) ||
				0 > movedList[i]->first ||
				movedList[i]->first >= board.at(0).size()) {
				return;
			}

			if ((movedList[i]->second % size < size * 1 / 5 ?
				board.at(movedV).at(movedList[i]->first) != 0 :
				board.at(movedV + 1).at(movedList[i]->first) != 0) ||
				(movedList[i]->second % size < size * 4 / 5 ?
					board.at(movedV).at(movedList[i]->first) != 0 :
					board.at(movedV + 1).at(movedList[i]->first) != 0)
				) {
				return;
			}
		}
		for (int i = 0; i < 4; i++) {
			if (list[i]->second % size > size * 4 / 5) {
				list[i]->second = ((list[i]->second / size) + 1) * size;
			}
			else if (list[i]->second % size <= size * 1 / 5) {
				list[i]->second = (list[i]->second / size) * size;
			}
		}

		rted = true;
		int tmp = height;
		height = width;
		width = tmp;
		blockA = movedA;
		blockB = movedB;
		blockC = movedC; 
		blockD = movedD;
		prev = semiPrev;
		
		swt = ~swt;
	}
	
	void Fix(vector<vector<int>>& board,int size, int psnum) override {
		for (int i = 0; i < 4; i++) {
			board.at(list[i]->second / size).at(list[i]->first) = psnum + 1;
		}
	}
	void Draw(HWND hWnd, HDC hdc,int size,int margin, int mvsp,int color[]) override {
		HPEN hOldPen;
		HBRUSH hOldBrush;
		HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));

		if (prev.first != 10000) {
			HPEN hThiftPen = CreatePen(PS_SOLID, 1, RGB( 0, 0, 0));
			HBRUSH hThiftBrush = CreateSolidBrush(RGB(0, 0, 0));
			hOldPen = (HPEN)SelectObject(hdc, hThiftPen);
			hOldBrush = (HBRUSH)SelectObject(hdc, hThiftBrush);
			int prevPV = margin + prev.second;
			int prevPH = margin + (prev.first * size);
			int linePos = margin + ((((prev.second - 1) / size) + 1) * size);
			int fillW, fillH;
			if (rted == true) {
				fillW = height;
				fillH = width;
				rted = false;
			}else {
				fillW = width;
				fillH = height;
			}

			Rectangle(hdc,  prevPH,prevPV,prevPH + (size*fillW),prevPV + (size*fillH));
			
			DeleteObject(hThiftPen);
			hThiftPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			SelectObject(hdc, hThiftPen);

			for (int i = 0; i < 4; i++) {
				MoveToEx(hdc, prevPH + (size * i), prevPV, NULL);
				LineTo(hdc, prevPH + (size * i), prevPV + (size * fillH));
			}
			for (int i = linePos; i <= prevPV + (size * 3); i += size) {
				MoveToEx(hdc, prevPH + (size * fillW), i, NULL);
				LineTo(hdc, prevPH, i );
			}

			DeleteObject(hThiftPen);
			DeleteObject(hThiftBrush);
			SelectObject(hdc, hPen);
			SelectObject(hdc, hBrush);
			prev = { 10000,10000 };
		}
		else {
			hOldPen = (HPEN)SelectObject(hdc, hPen);
			hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
		}

		int PosH, PosV;
		for (int i = 0; i < 4; i++) {
			PosH = margin + (list[i]->first * size);
			PosV = margin + list[i]->second;
			Rectangle(hdc, PosH, PosV - mvsp, PosH + size, PosV);
		}

		DeleteObject(hPen);
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		SelectObject(hdc, hPen);

		for (int i = 0; i < 4; i++) {
			PosH = margin + (list[i]->first * size);
			PosV = margin + list[i]->second;
			if (list[i]->second >= mvsp) {
				MoveToEx(hdc, PosH, PosV - mvsp, NULL);
				LineTo(hdc, PosH, PosV);
				MoveToEx(hdc, PosH + size, PosV - mvsp, NULL);
				LineTo(hdc, PosH + size, PosV);
				for (int i = 1; i <= mvsp; i++) {
					if ((list[i]->second - i) % size == 0) {
						MoveToEx(hdc, PosH, PosV - i, NULL);
						LineTo(hdc, PosH + size, PosV - i);
					}
				}
			}
		}

		DeleteObject(hPen);
		DeleteObject(hBrush);
		hPen = CreatePen(PS_SOLID, 1, color[1]);
		hBrush = CreateSolidBrush(color[0]);
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

		for (int i = 0; i < 4; i++) {
			PosH = margin + (list[i]->first * size);
			PosV = margin + list[i]->second;
			Rectangle(hdc, PosH, PosV, PosH + size, PosV + size);

		}

		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	}
};

