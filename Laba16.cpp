// Laba16.cpp : Определяет точку входа для приложения.
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "Laba16.h"
#include <stdio.h>
#include <Windows.h>

#define MAX_LOADSTRING 100

#define N 10
#define M 15
// Коды ячеек:
// 0 - свободна
// 1 - игрок
// 2 - препятствие
// 3 - золото
int map[N][M] = {
    {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0,   0, 0, 0, 0, 3,   0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0,   0, 0, 0, 0, 3,   0, 0, 0, 0, 0},
    {0, 0, 0, 3, 3,   3, 3, 0, 0, 0,   3, 3, 0, 0, 0},
    {0, 0, 0, 0, 0,   0, 3, 0, 0, 0,   3, 3, 0, 0, 0},

    {0, 0, 0, 0, 0,   0, 3, 3, 3, 0,   0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0,   0, 0, 0, 3, 0,   2, 0, 0, 2, 0},
    {0, 0, 0, 0, 0,   0, 0, 2, 0, 0,   2, 0, 0, 2, 0},
    {0, 0, 0, 0, 0,   0, 2, 2, 2, 2,   2, 2, 2, 2, 0},
    {0, 0, 0, 0, 0,   0, 0, 0, 0, 0,   0, 0, 0, 0, 0}
};

int steps = 0;
int gold = 0;

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
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

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LABA16, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LABA16));

    MSG msg;

    // Цикл основного сообщения:
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
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LABA16));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LABA16);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

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
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
#define WIDTH 30
#define HEIGHT 20

void drawMap(HDC hdc) {
    // кисть для пустого поля
    HBRUSH hBrushEmptyCell = CreateSolidBrush(RGB(200, 200, 200)); // серый
    // кисть для поля с золотом
    HBRUSH hBrushGold = CreateSolidBrush(RGB(255, 255, 0)); // желтый
    // кисть для стены
    HBRUSH hBrushWall = CreateSolidBrush(RGB(0, 0, 0)); // черный
    // кисть для игрока
    HBRUSH hBrushMan = CreateSolidBrush(RGB(0, 0, 255)); // синий

    // Коды ячеек:
    // 0 - свободна
    // 1 - игрок
    // 2 - препятствие
    // 3 - золото
    HBRUSH brush[4] = { hBrushEmptyCell, hBrushMan, hBrushWall, hBrushGold };


    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {

            int x1 = j * WIDTH;
            int x2 = (j + 1) * WIDTH;
            int y1 = i * HEIGHT;
            int y2 = (i + 1) * HEIGHT;

            RECT r = { x1, y1, x2, y2 };
            FillRect(hdc, &r, brush[map[i][j]]);
        }
    }


    // Все кисти удаляем!!!
    for (i = 0; i < 4; i++)
        DeleteObject(brush[i]);
}

    void Right() {
        int i, j;

        for (i = 0; i < N; i++) {

            for (j = M - 2; j >= 0; j--) {

                if (map[i][j] == 1) { // если в map[i][j]  игрок
                    if (map[i][j + 1] == 0) {
                        map[i][j + 1] = 1;
                        map[i][j] = 0;
                        steps++;
                    }
                    else if (map[i][j + 1] == 3) {
                        map[i][j + 1] = 1;
                        map[i][j] = 0;
                        steps++;
                        gold++;
                    }
                }
            }
        }
    }

void Left() {
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 1; j < M; j++) {
            if (map[i][j] == 1) { // если в map[i][j]  игрок
                if (map[i][j - 1] == 0) { // если слева от игрока - пустая клетка
                    map[i][j - 1] = 1;
                    map[i][j] = 0;
                    steps++;
                }
                else if (map[i][j - 1] == 3) {// если слева от игрока - золото
                    map[i][j - 1] = 1;
                    map[i][j] = 0;
                    steps++;
                    gold++;
                }
            }
        }
    }
}

void Up() {
    int i, j;

    for (i = 1; i < N; i++) {

        for (j = 0; j < M; j++) {

            if (map[i][j] == 1) { // если в map[i][j]  игрок
                if (map[i - 1][j] == 0) {
                    map[i - 1][j] = 1;
                    map[i][j] = 0;
                    steps++;
                }
                else if (map[i - 1][j] == 3) {
                    map[i - 1][j] = 1;
                    map[i][j] = 0;
                    steps++;
                    gold++;
                }
            }
        }
    }
}

void Down() {
    int i, j;

    for (i = N - 2; i >= 0; i--) { // Начинаем с предпоследней строки и двигаемся вверх

        for (j = 0; j < M; j++) {

            if (map[i][j] == 1) { // если в map[i][j] игрок
                if (map[i + 1][j] == 0) {
                    map[i + 1][j] = 1;
                    map[i][j] = 0;
                    steps++;
                }
                else if (map[i + 1][j] == 3) {
                    map[i + 1][j] = 1;
                    map[i][j] = 0;
                    steps++;
                    gold++;
                }
            }
        }
    }
}


void Save();

void Load();

void Save() {
    // Выходной файл
    FILE* fout;
    fout = fopen("c:\\Temp\\Lection12\\game.txt", "wt");
    if (fout == NULL) {
        printf("Выходной файл не создался");
        return;
    }

    fprintf(fout, "%d ", N);
    fprintf(fout, "%d\n", M);
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            fprintf(fout, "%d ", map[i][j]);
        }
        fprintf(fout, "\n");
    }

    fprintf(fout, "%d ", steps);
    fprintf(fout, "%d", gold);

    fclose(fout);
}

// Загрузка состояния
void Load() {
    // Выходной файл
    FILE* fin;
    fin = fopen("c:\\Temp\\Lection12\\game.txt", "rt");
    if (fin == NULL) {
        printf("Входной файл не создался");
        return;
    }

    int n, m;
    fscanf_s(fin, "%d", &n);
    fscanf_s(fin, "%d", &m);
    int i, j;

    for (i = 0; i < n; i++) {
        for (j = 0; j < m; j++) {
            fscanf_s(fin, "%d", &map[i][j]);
        }
    }

    fscanf_s(fin, "%d", &steps);
    fscanf_s(fin, "%d", &gold);

    fclose(fin);
}

void SaveBinary() {
    FILE* fout;
    fout = fopen("c:\\Temp\\Lection12\\game_bin.bin", "wb");
    if (fout == NULL) {
        printf("Выходной файл не создался");
        return;
    }

    int n = N, m = M;

    // Запись размеров карты
    fwrite(&n, sizeof(n), 1, fout);
    fwrite(&m, sizeof(m), 1, fout);

    // Запись самой карты
    fwrite(map, sizeof(map), 1, fout);

    // Запись шагов и золота
    fwrite(&steps, sizeof(steps), 1, fout);
    fwrite(&gold, sizeof(gold), 1, fout);

    fclose(fout);
};
void LoadBinary() {
    FILE* fin;
    fin = fopen("c:\\Temp\\Lection12\\game_bin.bin", "wb");
    if (fin == NULL) {
        printf("Выходной файл не создался");
        return;
    }

    int n, m;

    // Запись размеров карты
    fwrite(&n, sizeof(n), 1, fin);
    fwrite(&m, sizeof(m), 1, fin);

    // Запись самой карты
    fwrite(map, sizeof(map), 1, fin);

    // Запись шагов и золота
    fwrite(&steps, sizeof(steps), 1, fin);
    fwrite(&gold, sizeof(gold), 1, fin);

    fclose(fin);
};

void AddWallLeftOfPlayer() {
    for (int i = 0; i < N; i++) {
        for (int j = 1; j < M; j++) {
            if (map[i][j] == 1) {
                if (map[i][j - 1] == 0) {
                    map[i][j - 1] = 2;
                }
            }
        }
    }
}
void AddGoldRightOfPlayer() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M - 1; j++) {
            if (map[i][j] == 1) {
                if (map[i][j + 1] == 0) {
                    map[i][j + 1] = 3;
                }
            }
        }
    }
}
void HandleUpKey() {
    for (int i = 1; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (map[i][j] == 1) {
                if (map[i - 1][j] == 0) {
                    map[i - 1][j] = 2;
                }
                else if (map[i - 1][j] == 2) {
                    map[i - 1][j] = 0;
                }
                else if (map[i - 1][j] == 3) {
                    map[i - 1][j] = 2;
                }
            }
        }
    }
}
void HandleDownKey() {
    for (int i = N - 2; i >= 0; i--) {
        for (int j = 0; j < M; j++) {
            if (map[i][j] == 1) {
                if (map[i + 1][j] == 0) {
                    map[i + 1][j] = 3;
                }
                else if (map[i + 1][j] == 2) {
                    map[i + 1][j] = 3;
                }
                else if (map[i + 1][j] == 3) {
                    map[i + 1][j] = 0;
                }
            }
        }
    }
}
void DestroyWallsRightOfPlayer() {
    for (int i = 0; i < N; i++) {
        for (int j = M - 1; j >= 0; j--) {
            if (map[i][j] == 1) {
                for (int k = j + 1; k < M; k++) {
                    if (map[i][k] == 2) {
                        map[i][k] = 0;
                    }
                }
                return;
            }
        }
    }
}

void doMidasHand(int i, int j) {
    if (a[i][j] == 2) { // 2 - стена
        a[i][j] = 1; // 1 - золото
        if (i > 0) doMidasHand(i - 1, j);
        if (i < N - 1) doMidasHand(i + 1, j);
        if (j > 0) doMidasHand(i, j - 1);
        if (j < M - 1) doMidasHand(i, j + 1);
    }
}

void midasHandToRight() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M - 1; ++j) {
            if (a[i][j] == 3 && a[i][j + 1] == 2) {
                doMidasHand(i, j + 1);
            }
        }
    }
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
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
            switch (wParam)
            {
            case 0x4d: // M
                midasHandToRight();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case VK_DOWN:
                Down();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case VK_LEFT:
                Left();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case VK_UP:
                Up();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case VK_RIGHT:
                Right();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case 0x53: // S
                //Save();
                SaveBinary();
                break;
            case 0x4c: // L
                //Load();
                //LoadBinary();
                //InvalidateRect(hWnd, NULL, TRUE);
                AddWallLeftOfPlayer();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case 0x52: // R
                AddGoldRightOfPlayer();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case 0x55: // U
                HandleUpKey();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case 0x44: // D
                HandleDownKey();
                InvalidateRect(hWnd, NULL, TRUE);
                break;
            case 0x5A: // Z
                DestroyWallsRightOfPlayer();
                InvalidateRect(hWnd, NULL, TRUE);
                break;

            }
            break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...

            drawMap(hdc);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
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
