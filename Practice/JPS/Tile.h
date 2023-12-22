#pragma once

#include <windows.h>

#define GRID_SIZE 16
#define GRID_WIDTH 100
#define GRID_HEIGHT 50

// ��� ���ϱ�
enum class Mode
{
	OBSTACLE = 1,
	START,
	END,
	OPENLIST,
	CLOSELIST,
	SEARCHED,
	SEARCH
};

struct Info {
	double G;
	double H;
};

extern char gTile[GRID_HEIGHT][GRID_WIDTH];            // 0 ��ֹ� ���� / 1 ��ֹ� ����
extern Info gTileInfo[GRID_HEIGHT][GRID_WIDTH];

extern HBRUSH gOpenBrush;
extern HBRUSH gCloseBrush;
extern HBRUSH gStartBrush;
extern HBRUSH gEndBrush;
extern HBRUSH gRouteBrush;
extern HPEN gGridPen;

void RenderOpen(HDC hdc);
void RenderClose(HDC hdc);
void RenderStart(HDC hdc);
void RenderEnd(HDC hdc);

void RenderSearch(HDC hdc);
void RenderGrid(HDC hdc);
void RenderText(HDC hdc);