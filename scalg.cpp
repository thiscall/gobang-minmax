#include "hppframe.h"
#include <queue>
#include <algorithm>

int zw[N + 3][M + 3];
namespace SecondMinMax {
static bool tag;
static int dep, _t1;
static DWORD DDL;
inline static int wread(int x, int y, int excpt) {
    if (x < 1 || x > N || y < 1 || y > M) return excpt;
    else return zw[x][y];
}
inline static bool WinLose2(int x, int y) {
    int c = zw[x][y], i, j, cnt;
    for (i = x-1, j = y, cnt = 0; wread(i, j, 0) == c; i--, cnt++);
    for (i = x+1, j = y; wread(i, j, 0) == c; i++, cnt++);
    if (cnt >= 4) return 1;
    for (i = x, j = y-1, cnt = 0; wread(i, j, 0) == c; j--, cnt++);
    for (i = x, j = y+1; wread(i, j, 0) == c; j++, cnt++);
    if (cnt >= 4) return 1;
    for (i = x-1, j = y-1, cnt = 0; wread(i, j, 0) == c; i--, j--, cnt++);
    for (i = x+1, j = y+1; wread(i, j, 0) == c; i++, j++, cnt++);
    if (cnt >= 4) return 1;
    for (i = x-1, j = y+1, cnt = 0; wread(i, j, 0) == c; i--, j++, cnt++);
    for (i = x+1, j = y-1; wread(i, j, 0) == c; i++, j--, cnt++);
    if (cnt >= 4) return 1;
    return 0;
}
inline static void update(int &res, int lx, bool alive) {
    if (res == 100) return;
    if (alive) {
        switch (lx) {
        case 4: res = 100; break;
        case 3: res = std::min(res+60, 99); break;
        case 2: res = std::min(res+6, 99); break;
        }
    } else {
        switch(lx) {
        case 4: res = std::min(res+55, 99); break;
        case 3: res = std::min(res+4, 99); break;
        case 2: res = std::min(res+1, 99); break;
        }
    }
}
inline int H(int c)
{
    int res = 0, sl, lx;
    for (int i = 1; i <= N; i++) {
        sl = lx = 0;
        for (int j = 1; j <= M; j++) {
            if (zw[i][j] == c) lx++;
            else {
                if (sl) {
                    if (!wread(i, j+1, 1)) {
                        update(res, lx, 1); //alive
                    } else {
                        update(res, lx, 0); //dead
                    }
                }
                sl = !zw[i][j], lx = 0;
                if (res >= 99) return res;
            }
        }
    }
    for (int j = 1; j <= M; j++) {
        sl = lx = 0;
        for (int i = 1; i <= N; i++) {
            if (zw[i][j] == c) lx++;
            else {
                if (sl) {
                    if (!wread(i+1, j, 1)) {
                        update(res, lx, 1); //alive
                    } else {
                        update(res, lx, 0); //dead
                    }
                }
                sl = !zw[i][j], lx = 0;
                if (res >= 99) return res;
            }
        }
    }
    for (int k = 1; k <= M; k++) {
        sl = lx = 0;
        for (int i = 1, j = k; i<=N && j>=1; i++, j--) {
            if (zw[i][j] == c) lx++;
            else {
                if (sl) {
                    if (!wread(i+1, j-1, 1)) {
                        update(res, lx, 1); //alive
                    } else {
                        update(res, lx, 0); //dead
                    }
                }
                sl = !zw[i][j], lx = 0;
                if (res >= 99) return res;
            }
        }
    }
    for (int k = 1; k <= N; k++) {
        sl = lx = 0;
        for (int i = k, j = M; i<=N && j>=1; i++, j--) {
            if (zw[i][j] == c) lx++;
            else {
                if (sl) {
                    if (!wread(i+1, j-1, 1)) {
                        update(res, lx, 1); //alive
                    } else {
                        update(res, lx, 0); //dead
                    }
                }
                sl = !zw[i][j], lx = 0;
                if (res >= 99) return res;
            }
        }
    }
    for (int k = 1; k <= M; k++) {
        sl = lx = 0;
        for (int i = 1, j = k; i<=N && j<=M; i++, j++) {
            if (zw[i][j] == c) lx++;
            else {
                if (sl) {
                    if (!wread(i+1, j+1, 1)) {
                        update(res, lx, 1); //alive
                    } else {
                        update(res, lx, 0); //dead
                    }
                }
                sl = !zw[i][j], lx = 0;
                if (res >= 99) return res;
            }
        }
    }
    for (int k = 1; k <= N; k++) {
        sl = lx = 0;
        for (int i = k, j = 1; i<=N && j<=M; i++, j++) {
            if (zw[i][j] == c) lx++;
            else {
                if (sl) {
                    if (!wread(i+1, j+1, 1)) {
                        update(res, lx, 1); //alive
                    } else {
                        update(res, lx, 0); //dead
                    }
                }
                sl = !zw[i][j], lx = 0;
                if (res >= 99) return res;
            }
        }
    }
    return res;
}
inline int IE()
{
    return std::min(100, std::max(-100, H((bxs^tag)?1:2)-H((bxs^tag)?2:1)));
}
void alphabeta(int x, int y, int &alpha, int &beta)
{
    tag = !tag, dep++;
    zw[x][y] = (bxs^tag) ? 1 : 2;
    if (WinLose2(x, y)) goto destruct;
    if (dep == deplim - 1) {
        _t1 = IE();
        if (tag) alpha = std::max(alpha, _t1);
        else beta = std::min(beta, _t1);
        goto destruct;
    }
    if (GetTickCount() >= DDL) goto destruct;
    int ca, cb;
    for (int i = 1; i <= N; i++) for (int j = 1; j <= M; j++) if (!zw[i][j] && (zw[i-1][j-1] || zw[i-1][j] || zw[i-1][j+1] || zw[i][j-1] || zw[i][j+1] || zw[i+1][j-1] || zw[i+1][j] || zw[i+1][j+1]))
    {
        ca = alpha, cb = beta;
        alphabeta(i, j, ca, cb);
        if (tag) alpha = std::max(alpha, std::max(ca, cb));
        else beta = std::min(beta, std::min(ca, cb));
        if (alpha >= beta) goto destruct;
    }
destruct:
    tag = !tag, dep--;
    zw[x][y] = 0;
}
typedef struct _cont1 {
    int x, y, val;
}cont1;
int cmp(const cont1 &a, const cont1 &b) {
    return a.val > b.val;
}
POINT AlgMmWithAB() {
    POINT res = {1, 1};
    int alpha = -100, beta = 100, ca, cb;
    DDL = GetTickCount() - 100 + timlim*1000;
    std::deque<cont1> q;
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            if (!zw[i][j] && (zw[i-1][j-1] || zw[i-1][j] || zw[i-1][j+1] || zw[i][j-1] || zw[i][j+1] || zw[i+1][j-1] || zw[i+1][j] || zw[i+1][j+1])) {
                zw[i][j] = bxs?1:2;
                q.push_back((cont1){i, j, IE()});
                zw[i][j] = 0;
            }
        }
    }
    sort(q.begin(), q.end(), cmp);
    for (auto it : q) {
        tag = 1, dep = 1;
        ca = alpha, cb = beta;
        alphabeta(it.x, it.y, ca, cb);
        ca = std::max(ca, cb);
        if (ca > alpha) alpha = ca, res.x = it.x, res.y = it.y;
        if (alpha >= beta) goto destruct;
    }
destruct:
    return res;
}
}

DWORD WINAPI SecCH(LPVOID arg) {
    secres = SecondMinMax::AlgMmWithAB();
    return 0;
}
