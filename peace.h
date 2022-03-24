#pragma once
#include "framework.h"

using namespace std;
class peace {
protected:
	pair<int, int> prev = { 10000,10000 };
	pair<int, int> blockA = { 10000,10000 };
	pair<int, int> blockB = { 10000,10000 };
	pair<int, int> blockC = { 10000,10000 };
	pair<int, int> blockD = { 10000,10000 };
	pair<int, int>* list[4];
	pair<int, int> movedA, movedB, movedC, movedD;
	pair<int, int>* movedList[4];
	int width = 0;
	int height = 0;
	int swt = 0;
	bool rted = false;

public:
	peace() : list{ &blockA,&blockB,&blockC,&blockD }, movedList{ &movedA, &movedB, &movedC, &movedD } {}

	virtual bool DropDown(vector<vector<int>>& board, int moveSp, int size) {
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
	virtual void DownToBottom(vector<vector<int>>& board, int size) {
		int heightCount = 10000;
		int PosV;
		for (int i = 0; i < 4; i++) {
			int count = 0;
			PosV = (list[i]->second - 1) / size;
			for (int j = PosV + 2; j < board.size(); j++) {
				if (board.at(j).at(list[i]->first) != 0){
					break;
				}
				count++;
			}
			if (count < heightCount) heightCount = count;
		}
		for (int i = 0; i < 4; i++) {
			if (prev.first > list[i]->first)
				prev.first = list[i]->first;
			if (prev.second > list[i]->second)
				prev.second = list[i]->second;
			PosV = (list[i]->second - 1) / size;
			list[i]->second = (heightCount + PosV + 1) * size;
		}
	}
 	virtual void Thift(vector<vector<int>>& board, int size, int lr) {
		for (int i = 0; i < 4; i++) {
			unsigned _int64 verPos = list[i]->second / size;
			int movedPos = list[i]->first + lr;
			if (0 > movedPos ||
				movedPos >= board.at(0).size() ||
				((list[i]->second % size > size * 4 / 5) ?
					board.at(verPos + (int)1).at(movedPos) != 0 :
					board.at(verPos).at(movedPos) != 0) ||
				((list[i]->second % size > size * 1 / 5) ?
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
			}
			else if (list[i]->second % size <= size * 1 / 5) {
				list[i]->second = (list[i]->second / size) * size;
			}
		}
	}
	virtual bool Rotation(vector<vector<int>>& board, int size) {
		unsigned _int64 movedV;

		for (int i = 0; i < 4; i++) {
			movedV = movedList[i]->second / size;

			if (0 > (_int64)movedList[i]->second ||
				board.size() <= ((((unsigned _int64)movedList[i]->second - 1) / size) + 1) ||
				0 > movedList[i]->first ||
				movedList[i]->first >= board.at(0).size()) {
				return false;
			}

			if ((movedList[i]->second % size < size * 1 / 5 ?
				board.at(movedV).at(movedList[i]->first) != 0 :
				board.at(movedV + 1).at(movedList[i]->first) != 0) ||
				(movedList[i]->second % size < size * 4 / 5 ?
					board.at(movedV).at(movedList[i]->first) != 0 :
					board.at(movedV + 1).at(movedList[i]->first) != 0)
				) {
				return false;
			}
		}

		for (int i = 0; i < 4; i++) {
			if (prev.first > list[i]->first)
				prev.first = list[i]->first;
			if (prev.second > list[i]->second)
				prev.second = list[i]->second;

			if (movedList[i]->second % size > size * 4 / 5) {
				movedList[i]->second = ((movedList[i]->second / size) + 1) * size;
			}
			else if (movedList[i]->second % size <= size * 1 / 5) {
				movedList[i]->second = (movedList[i]->second / size) * size;
			}
		}

		rted = true;
		blockA = movedA;
		blockB = movedB;
		blockC = movedC;
		blockD = movedD;
		int tmp = height;
		height = width;
		width = tmp;
		return true;
	}
	virtual void Fix(vector<vector<int>>& board, int size, int psnum) {
		if (blockA.second / size < board.size()) {
			for (int i = 0; i < 4; i++) {
				board.at(list[i]->second / size).at(list[i]->first) = psnum + 1;
			}
		}
	}
	virtual void Draw(HWND hWnd, HDC hdc, int size, int margin, int mvsp, int color[]) {
		HPEN hOldPen;
		HBRUSH hOldBrush;
		HPEN hPen = CreatePen(PS_SOLID, 1, color[1]);
		HBRUSH hBrush = CreateSolidBrush(color[0]);
		int PosH, PosV, PosLine;

		if (prev.first != 10000) {
			HPEN hThiftPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
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
			}
			else {
				fillW = width;
				fillH = height;
			}
			

			Rectangle(hdc, prevPH, prevPV, prevPH + (size * fillW), prevPV + (size * fillH));

			DeleteObject(hThiftPen);
			hThiftPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
			SelectObject(hdc, hThiftPen);

			for (int i = 0; i < fillW + 1; i++) {
				MoveToEx(hdc, prevPH + (size * i), prevPV, NULL);
				LineTo(hdc, prevPH + (size * i), prevPV + (size * fillH));
			}
			for (int i = linePos; i <= prevPV + (size * fillH); i += size) {
				MoveToEx(hdc, prevPH + (size * fillW), i, NULL);
				LineTo(hdc, prevPH, i);
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


		for (int i = 0; i < 4; i++) {
			PosH = margin + (list[i]->first * size);
			PosV = margin + list[i]->second;
			Rectangle(hdc, PosH, PosV, PosH + size, PosV + size);

		}

		DeleteObject(hPen);
		DeleteObject(hBrush);
		hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		SelectObject(hdc, hPen);
		SelectObject(hdc, hBrush);

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
				bool crct = !(list[i]->second - mvsp == 0); // ïâÇÃç¿ïWÇ…Ç»ÇÁÇ»Ç¢ÇΩÇﬂ
				PosLine = (((list[i]->second - mvsp - crct) / size) + crct) * size + margin; // à⁄ìÆëOíºâ∫ÇÃâ°ê¸
				if (PosLine <= PosV) { // çƒï`é îÕàÕì‡Ç…ï`é Ç™ïKóvÇ»â°ê¸Ç™Ç†ÇÈÇ©
					for (int j = PosLine; j <= PosV; j += size) {
						MoveToEx(hdc, PosH, j, NULL);
						LineTo(hdc, PosH + size, j);
					}
				}
			}
		}

		SelectObject(hdc, hOldPen);
		SelectObject(hdc, hOldBrush);
		DeleteObject(hPen);
		DeleteObject(hBrush);
	}
};