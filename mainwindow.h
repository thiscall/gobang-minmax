#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_play_clicked();

    void on_submit_clicked();

    void on_pfoe_textChanged(const QString &arg1);

    void on_tofoe_clicked();

    void on_tome_clicked();

    void on_tonull_clicked();

    void on_setxian_clicked(bool checked);

    void on_sethou_clicked(bool checked);

    void on_alg_mm_clicked(bool checked);

    void on_alg_rd_clicked(bool checked);

    void on_lim_dep_valueChanged(int arg1);

    void on_resolvetag_stateChanged(int arg1);

    void on_mm_en1_stateChanged(int arg1);

    void on_mm_en3_stateChanged(int arg1);

    void on_lim_tim_valueChanged(int arg1);

    void on_pme_textEdited(const QString &arg1);

    void on_mod_me_clicked();

    void on_pushButton_clicked();

    void on_recalc_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
