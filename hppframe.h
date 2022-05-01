#ifndef HPPFRAME_H
#define HPPFRAME_H

#include <windows.h>

#define N 15
#define M 15

extern int w[N + 3][M + 3], zw[N + 3][M + 3];
extern HANDLE hStdout;
extern bool bxs;
extern int nwin, nlose, nss, nsf, dI, ndfn;
extern int hand, lx, ly, tx, ty;
extern int algch, deplim, iedisabled, timlim, entimlim;
extern POINT secres;

void StartGame();
void PrintGame();
void AIPlay();
bool WinLose(int x, int y);
DWORD WINAPI SecCH(LPVOID arg);

#endif // HPPFRAME_H
