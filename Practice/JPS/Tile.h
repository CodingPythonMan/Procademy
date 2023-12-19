#pragma once

#include <windows.h>
#include "Pos.h"

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
	ROUTE,
	SEARCHED,
	SEARCH
};

extern char gTile[GRID_HEIGHT][GRID_WIDTH];            // 0 ��ֹ� ���� / 1 ��ֹ� ����
extern char gDesignTile[GRID_HEIGHT][GRID_WIDTH];

extern HBRUSH gOpenBrush;
extern HBRUSH gCloseBrush;
extern HBRUSH gRouteBrush;
extern HPEN gGridPen;

void RenderOpen(HDC hdc);
void RenderClose(HDC hdc);
void RenderRoute(HDC hdc);
void RenderSearch(HDC hdc);
void RenderGrid(HDC hdc);