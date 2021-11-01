// Tetris2D.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "Tetris2D.h"
#include "peace.h"
#include "Zpeace.h"

#define MAX_LOADSTRING 100
#define MARGIN 10
#define BOARD_LINE_WIDTH 1
#define BOARD_HEIGHT 20
#define BOARD_WIDTH 10
#define BLOCK_SIZE 30
#define CLIENT_WIDTH (MARGIN*2 + (BOARD_WIDTH * BLOCK_SIZE))
#define CLIENT_HEIGHT (MARGIN*2 + (BOARD_HEIGHT * BLOCK_SIZE))
#define TIMER_SPAN 10

HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名
bool activatePeace = true;
bool init = true;
bool clearStack = false;
int MoveSp = 2;
int countTime = 0;
std::vector<std::vector<int>> board(20, std::vector<int>(10, 0));
std::vector<int> clearLine(20 , 0);
int colors[][2] = {
    {RGB(255,100,0),RGB(255,0,0)},
    {} };
int peaceNum = 0;
peace *ps = new peace;


ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void PaintAll(HWND);
void DrawLine(HWND,HDC);
void DrawBlock(HWND, HDC);
void DrawBlock(HWND,HDC);
void OperateBlock(HWND);
void GeneratePeace();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
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



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
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
            //MoveSp = 20;
            break;
        case VK_SPACE:
            ps->Rotation(board, BLOCK_SIZE);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    case WM_KEYUP:
        switch (wParam) {
        case VK_DOWN:
            MoveSp = 2;
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

void DrawBlock(HWND hWnd, HDC hdc) {
    HPEN hPen,hOldPen;
    HBRUSH hBrush,hOldBrush;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board.at(0).size(); j++) {
            if (board.at(i).at(j) != 0) {
                hPen = CreatePen(PS_SOLID, 1, colors[peaceNum][1]);
                hOldPen = (HPEN)SelectObject(hdc, hPen);
                hBrush = CreateSolidBrush(colors[peaceNum][0]);
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
            for (int i = 0; i < board.size(); i++) {
                lineFill = true;
                for (int j = 0; j < board.at(i).size(); j++) {
                    if (board.at(i).at(j) == 0) {
                        lineFill = false; break;
                    }
                }
                if (lineFill) {
                    clearStack = true;
                    for (int j = 0; j < board.at(i).size(); j++) {
                        board.at(i).at(j) = 0;
                    }
                    clearLine.at(i) = 1;
                }
            }
            if (clearStack) {
                ps = new peace();
                init = true;
                InvalidateRect(hWnd, NULL, TRUE);
                return;
            }
        } else {
            clearStack = false;
            for (int i = 0; i < clearLine.size(); i++) {
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
    ps = new Zpeace(BLOCK_SIZE);
}
