// Astar.cpp : Defines the entry point for the application.
//
#include "Astar.h"
#include "framework.h"
#include "Main.h"
#include <windowsx.h>
#include "Tile.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

bool gFirstStart = true;
bool gFirstEnd = true;

HPEN gGridPen;
HBRUSH gTileBrush;
HBRUSH gStartBrush;
HBRUSH gEndBrush;

HBITMAP gMemDCBitmap;
HBITMAP gMemDCBitmapOld;
HDC gMemDC;
RECT gMemDCRect;

// Ÿ���� �Ӽ� �Է� / ���� ��� �÷���
// ���� Ŭ�� �� �ش� Ÿ���� �Ӽ��� �ܼ� ������ �����شٸ� �巡�׸� ���ؼ� ��ֹ� �Է��� �����ϰ� �ϱⰡ ��ƴ�.
// ù Ŭ�� �� �ش� Ÿ���� �Ӽ��� ���� ��ֹ� �Է¸��/���Ÿ�带 �� �÷��׷� �����Ͽ�, ���콺 �̵��� ������ �������
// ��ֹ��� ���������� �����ϵ��� �Ѵ�.

bool gErase = false;
bool gDrag = false;

Mode gMode = Mode::OBSTACLE;
Astar _Astar;

void RenderGrid(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HPEN OldPen = (HPEN)SelectObject(hdc, gGridPen);
	// �׸����� ������ ���� �߰��� �׸��� ���� <= �� �ݺ� ����
	for (int i = 0; i <= GRID_WIDTH; i++)
	{
		MoveToEx(hdc, X, 0, NULL);
		LineTo(hdc, X, GRID_HEIGHT * GRID_SIZE);
		X += GRID_SIZE;
	}

	for (int i = 0; i <= GRID_HEIGHT; i++)
	{
		MoveToEx(hdc, 0, Y, NULL);
		LineTo(hdc, GRID_WIDTH * GRID_SIZE, Y);
		Y += GRID_SIZE;
	}
	SelectObject(hdc, OldPen);
}

void RenderObstacle(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gTileBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	// �簢���� �׵θ��� �Ⱥ��̵��� �ϱ����� Null Pen �� �����Ѵ�.
	// CreatePen ���� Null Pen�� �����ص� ������, GetStockObject �� ����Ͽ�
	// �̹� �ý��ۿ� ������� �ִ� ���� GDI Object �� ����غ���.
	// GetStockObject �� �ý����� �������� ���� GDI Object �μ� ������ �ʿ� ����.
	// �ý��� �������� GDI Object �� �� ����Ѵٴ� ����.
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (int)Mode::OBSTACLE)
			{
				X = i * GRID_SIZE;
				Y = j * GRID_SIZE;
				// �׵θ� ũ�Ⱑ �����Ƿ� + 2 �Ѵ�.
				Rectangle(hdc, X, Y, X + GRID_SIZE + 2, Y + GRID_SIZE + 2);
			}
		}
	}
	SelectObject(hdc, OldBrush);
}

void RenderStart(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gStartBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (int)Mode::START)
			{
				X = i * GRID_SIZE;
				Y = j * GRID_SIZE;
				// �׵θ� ũ�Ⱑ �����Ƿ� + 2 �Ѵ�.
				Rectangle(hdc, X, Y, X + GRID_SIZE + 2, Y + GRID_SIZE + 2);
			}
		}
	}
	SelectObject(hdc, OldBrush);
}

void RenderEnd(HDC hdc)
{
	int X = 0;
	int Y = 0;
	HBRUSH OldBrush = (HBRUSH)SelectObject(hdc, gEndBrush);
	SelectObject(hdc, GetStockObject(NULL_PEN));
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		for (int j = 0; j < GRID_HEIGHT; j++)
		{
			if (gTile[j][i] == (int)Mode::END)
			{
				X = i * GRID_SIZE;
				Y = j * GRID_SIZE;
				// �׵θ� ũ�Ⱑ �����Ƿ� + 2 �Ѵ�.
				Rectangle(hdc, X, Y, X + GRID_SIZE + 2, Y + GRID_SIZE + 2);
			}
		}
	}
	SelectObject(hdc, OldBrush);
}

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ASTAR, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ASTAR));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ASTAR));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ASTAR);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		int TileX = xPos / GRID_SIZE;
		int TileY = yPos / GRID_SIZE;

		switch (gMode)
		{
		case Mode::OBSTACLE:
			gDrag = true;
			{
				// ù ���� Ÿ���� ��ֹ��̸� ����� ��� �ƴϸ� ��ֹ� �ֱ� ���
				if (TileX >= GRID_WIDTH || TileY >= GRID_HEIGHT)
					break;

				if (gTile[TileY][TileX] == 1)
					gErase = true;
				else
					gErase = false;
			}
			break;
		case Mode::START:
		{
			if (gFirstStart)
			{
				gFirstStart = false;
			}
			else
			{
				int OldTileX = _Astar._Start->_X;
				int OldTileY = _Astar._Start->_Y;
				gTile[OldTileY][OldTileX] = 0;
			}
			
			_Astar._Start->_X = TileX;
			_Astar._Start->_Y = TileY;
			gTile[TileY][TileX] = (int)Mode::START;
			InvalidateRect(hWnd, NULL, false);
		}
		break;
		case Mode::END:
		{
			if (gFirstEnd)
			{
				gFirstEnd = false;
			}
			else
			{
				int OldTileX = _Astar._End->_X;
				int OldTileY = _Astar._End->_Y;
				gTile[OldTileY][OldTileX] = 0;
			}
			
			_Astar._End->_X = TileX;
			_Astar._End->_Y = TileY;
			gTile[TileY][TileX] = (int)Mode::END;
			InvalidateRect(hWnd, NULL, false);
		}
		break;
		default:
			break;
		}
	}
	break;
	case WM_LBUTTONUP:
		if(gMode == Mode::OBSTACLE)
			gDrag = false;
		break;
	case WM_RBUTTONDOWN:
		switch (gMode)
		{
		case Mode::OBSTACLE:
			gMode = Mode::START;
			break;
		case Mode::START:
			gMode = Mode::END;
			break;
		case Mode::END:
			gMode = Mode::OBSTACLE;
			break;
		default:
			break;
		}
		break;
	case WM_MOUSEMOVE:
	{
		if (gMode == Mode::OBSTACLE)
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);

			if (gDrag)
			{
				int xPos = GET_X_LPARAM(lParam);
				int yPos = GET_Y_LPARAM(lParam);
				int TileX = xPos / GRID_SIZE;
				int TileY = yPos / GRID_SIZE;

				if (TileX >= GRID_WIDTH || TileY >= GRID_HEIGHT)
					break;

				gTile[TileY][TileX] = !gErase;
				//InvalidateRect(hWnd, NULL, true);
				InvalidateRect(hWnd, NULL, false);
			}
		}
		// ���콺 �巡�׷� �����Ͱ� ����Ǿ� ������ ��û�� �� ������ Erase �÷��׸� false �� �Ͽ�
		// ȭ�� �������� ���ش�. WM_PAINT ������ ������ ��ü�� ����� ������ ������ �ʾƵ� �ȴ�.
	}
	break;
	case WM_KEYDOWN:
		if (wParam == 0x52)
		{
			_Astar.RoutingStart(hWnd);
		}
		break;
	case WM_CREATE:
	{
		gGridPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
		gTileBrush = CreateSolidBrush(RGB(100, 100, 100));
		gStartBrush = CreateSolidBrush(RGB(250, 0, 0));
		gEndBrush = CreateSolidBrush(RGB(0, 0, 250));
		gOpenBrush = CreateSolidBrush(RGB(0, 250, 0));
		gCloseBrush = CreateSolidBrush(RGB(0, 200, 200));
		gRouteBrush = CreateSolidBrush(RGB(200, 200, 0));

		// ������ ���� �� �� ������ ũ��� ������ �޸� DC ����
		HDC hdc = GetDC(hWnd);
		GetClientRect(hWnd, &gMemDCRect);
		gMemDCBitmap = CreateCompatibleBitmap(hdc, gMemDCRect.right, gMemDCRect.bottom);
		gMemDC = CreateCompatibleDC(hdc);
		ReleaseDC(hWnd, hdc);
		gMemDCBitmapOld = (HBITMAP)SelectObject(gMemDC, gMemDCBitmap);
	}
	break;
	case WM_PAINT:
		// �޸𸮿� DC�� Ŭ����
		PatBlt(gMemDC, 0, 0, gMemDCRect.right, gMemDCRect.bottom, WHITENESS);

		// RenderObstacle, RenderGrid �� �޸� DC�� ���
		RenderObstacle(gMemDC);
		RenderGrid(gMemDC);
		RenderStart(gMemDC);
		RenderEnd(gMemDC);
		RenderOpen(gMemDC);
		RenderClose(gMemDC);
		RenderRoute(gMemDC);

		// �޸� DC �� �������� ������, �޸� DC->������ DC ���
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdc, 0, 0, gMemDCRect.right, gMemDCRect.bottom, gMemDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
	{
		SelectObject(gMemDC, gMemDCBitmapOld);
		DeleteObject(gMemDC);
		DeleteObject(gMemDCBitmap);

		HDC hdc = GetDC(hWnd);

		GetClientRect(hWnd, &gMemDCRect);
		gMemDCBitmap = CreateCompatibleBitmap(hdc, gMemDCRect.right, gMemDCRect.bottom);
		gMemDC = CreateCompatibleDC(hdc);
		ReleaseDC(hWnd, hdc);

		gMemDCBitmapOld = (HBITMAP)SelectObject(gMemDC, gMemDCBitmap);
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_DESTROY:
		SelectObject(gMemDC, gMemDCBitmapOld);
		DeleteObject(gMemDC);
		DeleteObject(gMemDCBitmap);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}