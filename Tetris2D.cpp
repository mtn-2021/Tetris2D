// Tetris2D.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "Tetris2D.h"
#include "peace.h"
#include "Zpeace.h"
#include "Speace.h"
#include "Opeace.h"
#include "Ipeace.h"
#include "Jpeace.h"
#include "Lpeace.h"
#include "Tpeace.h"

#include <crtdbg.h>
#ifdef _DEBUG
#define	new	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

#define MAX_LOADSTRING 100
#define MARGIN 10
#define BOARD_LINE_WIDTH 1
#define BOARD_HEIGHT 21
#define BOARD_WIDTH 10
#define BLOCK_SIZE 30
#define CLIENT_WIDTH (MARGIN*2 + (BOARD_WIDTH * BLOCK_SIZE))
#define CLIENT_HEIGHT (MARGIN*2 + (BOARD_HEIGHT * BLOCK_SIZE))
#define TIMER_SPAN 10

HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
peace *ps = new peace;
int peaceNum = 0;
int MoveSp = 2;
int countTime = 0;
int colors[][2] = {
    {RGB(255,100,0),RGB(255,0,0)},
    {RGB(100,255,0),RGB(0,255,0)},
    {RGB(255,255,0),RGB(100,100,0)},
    {RGB(170,202,255),RGB(50,50,200)},
    {RGB(0,0,255),RGB(150,150,200)},
    {RGB(240,128,128),RGB(200,50,100)},
    {RGB(167,87,168),RGB(200,0,255)}
};
bool init = true;
bool clearStack = false;
std::vector<std::vector<int>> board(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0));
std::vector<int> clearLine(BOARD_HEIGHT , 0);
std::random_device rnd;
std::mt19937 mt(rnd());
std::uniform_int_distribution<> rndP(0,6);

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void PaintAll(HWND);
void DrawLine(HWND, HDC);
void DrawDeadLine(HWND, HDC);
void DrawBlock(HWND, HDC);
void OperateBlock(HWND);
void GeneratePeace();
void GameOver(HWND);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |_CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TETRIS2D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    if (!InitInstance (hInstance, nCmdShow))
        return FALSE;

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TETRIS2D));

    MSG msg;

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


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TETRIS2D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_TETRIS2D);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowW(
       szWindowClass, szTitle, 
       WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
       0, 0, 0, 0,
       nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   RECT desktop;
   HWND hDesktop = GetDesktopWindow();
   GetWindowRect(hDesktop, &desktop);

   RECT window = { 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT};
   AdjustWindowRectEx(
       &window, GetWindowLong(hWnd,GWL_STYLE),
       true, GetWindowLong(hWnd,GWL_EXSTYLE));
   int windowWidth = window.right - window.left;
   int windowHeight = window.bottom - window.top;

   SetWindowPos(
       hWnd, NULL, 
       (desktop.right - windowWidth) / 2,
       (desktop.bottom - windowHeight) / 2,
       windowWidth, windowHeight, 
       SWP_NOZORDER);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   if (SetTimer(hWnd, IDT_BLOCK, TIMER_SPAN, NULL) == 0) {
       MessageBox(hWnd,
           (LPCWSTR)L"errer on timer",
           (LPCWSTR)L"miss timer start", MB_OK);
       DestroyWindow(hWnd);
   }

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
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
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_LEFT:
            ps->Thift(board, BLOCK_SIZE,-1);
            break;
        case VK_RIGHT:
            ps->Thift(board,BLOCK_SIZE, 1);
            break;
        case VK_DOWN:
            ps->DownToBottom(board, BLOCK_SIZE);
            break;
        case VK_SPACE:
            ps->Rotation(board, BLOCK_SIZE);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    case WM_TIMER:
        OperateBlock(hWnd);
        break;
    case WM_CREATE:
        break;
    case WM_MOVING:
        init = true;
        break;
    case WM_PAINT:
        PaintAll(hWnd);
        break;
    case WM_DESTROY:
        KillTimer(hWnd, IDT_BLOCK);
        delete ps;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

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

void PaintAll(HWND hWnd) {
    PAINTSTRUCT pst;
    HDC hdc = BeginPaint(hWnd, &pst);
    if (init == true) {
            DrawLine(hWnd, hdc);
            init = false;
        }
    DrawDeadLine(hWnd, hdc);
    ps->Draw(hWnd, hdc,BLOCK_SIZE,MARGIN, MoveSp,colors[peaceNum]);
    DrawBlock(hWnd,hdc);
    EndPaint(hWnd, &pst);
}

void DrawLine(HWND hWnd,HDC hdc) {
    HPEN pen = CreatePen(PS_SOLID, BOARD_LINE_WIDTH, RGB(255,255,255));
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    for (int i = 0; i <= BOARD_HEIGHT; i++) {
        MoveToEx(hdc, MARGIN, i * BLOCK_SIZE + MARGIN, NULL);
        LineTo(hdc,BLOCK_SIZE * BOARD_WIDTH + MARGIN, i * BLOCK_SIZE + MARGIN);
    }
    for (int i = 0; i <= BOARD_WIDTH; i++) {
        MoveToEx(hdc, i * BLOCK_SIZE + MARGIN, MARGIN, NULL);
        LineTo(hdc, i * BLOCK_SIZE + MARGIN, BLOCK_SIZE * BOARD_HEIGHT + MARGIN);
    }
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

void DrawDeadLine(HWND hWnd,HDC hdc){
    HPEN pen = CreatePen(PS_SOLID, BOARD_LINE_WIDTH, RGB(255, 0, 0));
    HPEN oldPen = (HPEN)SelectObject(hdc, pen);
    MoveToEx(hdc, MARGIN, MARGIN + BLOCK_SIZE, NULL);
    LineTo(hdc, MARGIN + (BLOCK_SIZE * BOARD_WIDTH), MARGIN + BLOCK_SIZE);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);

}

void DrawBlock(HWND hWnd, HDC hdc) {
    HPEN hPen,hOldPen;
    HBRUSH hBrush,hOldBrush;
    int colorNum;
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        for (int j = 0; j < BOARD_WIDTH; j++) {
            if (board.at(i).at(j) != 0) {
                colorNum = board.at(i).at(j) - 1;
                hPen = CreatePen(PS_SOLID, 1, colors[colorNum][1]);
                hOldPen = (HPEN)SelectObject(hdc, hPen);
                hBrush = CreateSolidBrush(colors[colorNum][0]);
                hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

                Rectangle(hdc, 
                    j * BLOCK_SIZE + MARGIN,
                    i * BLOCK_SIZE + MARGIN,
                    (j + 1) * BLOCK_SIZE + MARGIN,
                    (i + 1) * BLOCK_SIZE + MARGIN);

                SelectObject(hdc, hOldPen);
                SelectObject(hdc, hOldBrush);
                DeleteObject(hPen);
                DeleteObject(hBrush);
            }
        }
    }

}

void OperateBlock(HWND hWnd) {
    bool lineFill;
    if (!ps->DropDown(board, MoveSp, BLOCK_SIZE))
        countTime++;
    else countTime = 0;
    if (countTime >= (1000 / TIMER_SPAN)) {
        if (!clearStack) {
            ps->Fix(board, BLOCK_SIZE, peaceNum);
            for (int i = 0; i < BOARD_WIDTH; i++) {
                if (board.at(0).at(i) != 0) { GameOver(hWnd); return; }
            }
            for (int i = 1; i < BOARD_HEIGHT; i++) {
                lineFill = true;
                for (int j = 0; j < BOARD_WIDTH; j++) {
                    if (board.at(i).at(j) == 0) {
                        lineFill = false; break;
                    }
                }
                if (lineFill) {
                    clearStack = true;
                    for (int j = 0; j < BOARD_WIDTH; j++) {
                        board.at(i).at(j) = 0;
                    }
                    clearLine.at(i) = 1;
                }
            }
            if (clearStack) {
                peaceNum = 0;
                delete ps;
                ps = new peace();
                init = true;
                InvalidateRect(hWnd, NULL, TRUE);
                return;
            }
        } else {
            clearStack = false;
            for (int i = 0; i < BOARD_HEIGHT; i++) {
                if (clearLine.at(i) != 0) {
                    board.erase(board.begin() + i);
                    board.insert(board.begin(), std::vector<int>(10, 0));
                    clearLine.at(i) = 0;
                }
            }
            Sleep(500);
            init = true;
            InvalidateRect(hWnd, NULL, TRUE);
        }
        GeneratePeace();
    }
    InvalidateRect(hWnd, NULL, FALSE);
}

void GeneratePeace() {
    peaceNum = rndP(mt);
    delete ps;
    switch (peaceNum)
    {
    case 0:
        ps = new Zpeace(BLOCK_SIZE);
        break;
    case 1:
        ps = new Speace(BLOCK_SIZE);
        break;
    case 2:
        ps = new Opeace(BLOCK_SIZE);
        break;
    case 3:
        ps = new Ipeace(BLOCK_SIZE);
        break;
    case 4:
        ps = new Jpeace(BLOCK_SIZE);
        break;
    case 5:
        ps = new Lpeace(BLOCK_SIZE);
        break;
    case 6:
        ps = new Tpeace(BLOCK_SIZE);
        break;
    default:
        ps = new peace();
    }
    
}

void GameOver(HWND hWnd) {

    KillTimer(hWnd, IDT_BLOCK);
    int id = MessageBox(NULL, L"GameOver", L"もう一度プレイしますか", MB_YESNO | MB_ICONSTOP);
    switch (id) {
    case IDYES:
        board = std::vector<std::vector<int>>(BOARD_HEIGHT, std::vector<int>(BOARD_WIDTH, 0));
        if (SetTimer(hWnd, IDT_BLOCK, TIMER_SPAN, NULL) == 0) {
            MessageBox(hWnd,
                (LPCWSTR)L"errer on timer",
                (LPCWSTR)L"miss timer start", MB_OK);
        }
        GeneratePeace();
        init = true;
        InvalidateRect(hWnd, NULL, TRUE);
        break;
    case IDNO:
        DestroyWindow(hWnd);
        break;
    }
}
