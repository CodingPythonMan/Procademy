#pragma once

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
	ROUTE
};

extern char gTile[GRID_HEIGHT][GRID_WIDTH];            // 0 ��ֹ� ���� / 1 ��ֹ� ����
