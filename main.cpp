#include "mainwindow.h"
#include "hppframe.h"
#include <QApplication>
#include <stdio.h>

#define FOREWHT FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY

int w[N + 3][M + 3];
HANDLE hStdout = NULL;
bool bxs = 1;
int nwin = 0, nlose = 0, nss = 0, nsf = 0, dI = 0, ndfn = 0;
int hand = 0, lx = 0, ly = 0, tx = 0, ty = 0;
int algch = 1, deplim = 5, iedisabled = 0, entimlim = 1, timlim = 15;
POINT secres;

int main(int argc, char *argv[])
{
    CloseHandle(hStdout);
    QApplication a(argc, argv);
    AllocConsole();
    freopen("con", "w", stdout);
    SetConsoleTitleW(L"五子棋战况界面");
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleMode(hStdout, ENABLE_PROCESSED_OUTPUT);
    StartGame();
    MainWindow w;
    w.show();
    a.exec();
    FreeConsole();
    return 0;
}

void Asserter(bool b) {
    if (!b) MessageBoxW(NULL, L"Assertion Failed", L"错误", MB_ICONERROR | MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST);
}

void StartGame()
{
    memset(w, 0, sizeof(w));
    nwin = nlose = nss = nsf = dI = 0;
    hand = 1, lx = ly = 0;
}

void ClearScr()
{
    COORD coordScreen = { 0, 0 };
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;
    GetConsoleScreenBufferInfo( hStdout, &csbi );
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter( hStdout, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );
    GetConsoleScreenBufferInfo( hStdout, &csbi );
    FillConsoleOutputAttribute( hStdout, csbi.wAttributes,dwConSize, coordScreen, &cCharsWritten );
    SetConsoleCursorPosition( hStdout, coordScreen );
}

inline void SetCl(WORD cl) { SetConsoleTextAttribute(hStdout, cl); }
inline void flputc(int ch) { putchar(ch); fflush(stdout); }

void PrintGame()
{
    ClearScr();
    printf("   ABCDEFGHIJKLMNO\n");
    for (int i = 1; i <= N; i++) {
        printf("%-2d ", i);
        for (int j = 1; j <= M; j++) {
            if (i == tx && j == ty) {
                SetCl(BACKGROUND_GREEN | FOREWHT); flputc('?'); continue;
            }
            if (i == lx && j == ly) {
                SetCl(BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY | FOREWHT); flputc(w[i][j]==1?'X':'O'); continue;
            }
            switch (w[i][j]) {
            case 0: SetCl(FOREWHT); flputc(' '); break; //null
            case 1: SetCl(BACKGROUND_RED | FOREWHT); flputc('X'); break; //先手
            case 2: SetCl(BACKGROUND_BLUE | FOREWHT); flputc('O'); break; //后手
            }
        }
        SetCl(FOREWHT); flputc(' ');
        flputc('\n');
    }
}
