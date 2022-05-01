#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "hppframe.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}
MainWindow::~MainWindow()
{
    delete ui;
}
static bool resolvetag = 0, temparg = 0;
inline void PrintAddr(QString &s, int x, int y) {
    if (resolvetag) x = 16 - x;
    s.clear();
    static char tmp[8];
    sprintf(tmp, "%d%c", x, (char)(y - 1 + 'A'));
    s.append(tmp);
}
bool resolve(const QString &s, int *x, int *y) {
    std::string t = s.toStdString();
    char p;
    if (sscanf(t.c_str(), "%d%c", x, &p) != 2) return 0;
    *y = p - 'A' + 1;
    if (*x < 1 || *x > N) return 0;
    if (*y < 1 || *y > M) {
        *y = p - 'a' + 1;
        if (*y < 1 || *y > M) return 0;
    }
    if (resolvetag) *x = 16 - *x;
    return 1;
}
void MainWindow::on_play_clicked()
{
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    int x = N/2+1, y = M/2+1;
    StartGame();
    QString tmp;
    if (bxs) { PrintAddr(tmp, x, y); ui->pme->setText(tmp); hand++, lx = x, ly = y, w[x][y] = bxs?1:2; }
    PrintGame();
    char b[8]; sprintf(b, "%d", hand);
    tmp.clear(); tmp.append("正在比赛，第 "); tmp.append(b); tmp.append(" 手");
    ui->panel1->setText(tmp);
}
void MainWindow::on_submit_clicked()
{
    int x, y;
    if (temparg) goto friendai;
    if (!resolve(ui->pfoe->text(), &x, &y)) return;
    ui->pfoe->clear();
    if (w[x][y]) return;
    w[x][y] = bxs?2:1;
    if (WinLose(x, y)) {
        ui->panel1->setText("自动判定：敌军赢");
        PrintGame(); StartGame();
        return;
    }
friendai:
    const DWORD stt = GetTickCount();
    AIPlay();
    if (WinLose(lx, ly)) {
        ui->panel1->setText("自动判定：友军赢");
        PrintGame(); StartGame();
        return;
    }
    QString tmp, tmp2; PrintAddr(tmp, lx, ly); ui->pme->setText(tmp);
    hand += 2;
    static char b[16]; sprintf(b, "%d", hand); tmp.clear(); tmp.append("正在比赛，第 "); tmp.append(b); tmp.append(" 手"); ui->panel1->setText(tmp);
    PrintGame();
    const int sum = nwin + nlose + nss + nsf;
    if (algch == 1) {
        tmp.clear(); tmp.append("当前你有 "); sprintf(b, "%d", sum); tmp.append(b); tmp.append(" 个选择：\n");
        sprintf(b, "%d", nwin); tmp.append(b); tmp.append(" 种必胜策略（"); sprintf(b, "%.2lf%%", (double)nwin*100.0/sum); tmp.append(b); tmp.append("）\n");
        sprintf(b, "%d", nlose); tmp.append(b); tmp.append(" 种必败策略（"); sprintf(b, "%.2lf%%", (double)nlose*100.0/sum); tmp.append(b); tmp.append("）\n");
        sprintf(b, "%d", nss+nsf); tmp.append(b); tmp.append(" 种存疑策略（"); sprintf(b, "%.2lf%%", (double)(nss+nsf)*100.0/sum); tmp.append(b); tmp.append("）\n");
        tmp.append("遍历了 "); sprintf(b, "%d", ndfn); tmp.append(b); tmp.append(" 个出口。");
        tmp.append("当前决策耗时 "); sprintf(b, "%.1lf", (GetTickCount() - stt) / 1000.0); tmp.append(b); tmp.append(" 秒。\n\n");
        tmp.append("次优决策："); PrintAddr(tmp2, secres.x, secres.y); tmp.append(tmp2);
        ui->panel2->setText(tmp);
    }
}
void MainWindow::on_pfoe_textChanged(const QString &arg1)
{
    tx = 0, ty = 0;
    int x, y;
    if (!resolve(arg1, &x, &y)) return;
    if (w[x][y]) return;
    tx = x, ty = y;
    PrintGame();
    tx = 0, ty = 0;
}
void MainWindow::on_tofoe_clicked()
{
    int x, y;
    if (!resolve(ui->pmod->text(), &x, &y)) return;
    w[x][y] = bxs?2:1;
    PrintGame();
}
void MainWindow::on_tome_clicked()
{
    int x, y;
    if (!resolve(ui->pmod->text(), &x, &y)) return;
    w[x][y] = bxs?1:2;
    PrintGame();
}
void MainWindow::on_tonull_clicked()
{
    int x, y;
    if (!resolve(ui->pmod->text(), &x, &y)) return;
    w[x][y] = 0;
    PrintGame();
}
void MainWindow::on_pme_textEdited(const QString &arg1)
{
    const int cx = lx, cy = ly;
    int x, y;
    if (!resolve(arg1, &x, &y)) return;
    if (w[x][y]) return;
    lx = x, ly = y;
    w[x][y] = bxs?1:2; PrintGame(); w[x][y] = 0;
    lx = cx, ly = cy;
}
void MainWindow::on_mod_me_clicked()
{
    int x, y;
    if (!resolve(ui->pme->text(), &x, &y)) return;
    w[lx][ly] = 0;
    lx = x, ly = y;
    w[x][y] = bxs?1:2;
    PrintGame();
}
void MainWindow::on_recalc_clicked()
{
    temparg = 1;
    w[lx][ly] = 0;
    on_submit_clicked();
    temparg = 0;
}
void MainWindow::on_pushButton_clicked() { PrintGame(); }
void MainWindow::on_setxian_clicked(bool checked) { bxs = checked; }
void MainWindow::on_sethou_clicked(bool checked) { bxs = !checked; }
void MainWindow::on_alg_mm_clicked(bool checked) { if (checked) algch = 1; }
void MainWindow::on_alg_rd_clicked(bool checked) { if (checked) algch = 0; }
void MainWindow::on_lim_dep_valueChanged(int arg1) { deplim = arg1; }
void MainWindow::on_resolvetag_stateChanged(int arg1) { resolvetag = arg1; }
void MainWindow::on_mm_en1_stateChanged(int arg1) { iedisabled = arg1; }
void MainWindow::on_mm_en3_stateChanged(int arg1) { entimlim = arg1; }
void MainWindow::on_lim_tim_valueChanged(int arg1) { timlim = arg1; }
