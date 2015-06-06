/*
    Copyright 2011 Kacper Kasper <kacperkasper@gmail.com>

    This file is part of Tank Map Editor.

    Tank Map Editor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tank Map Editor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Tank Map Editor.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "tankmapwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSaveAll_triggered();
    void on_actionAbout_triggered();
    void on_btnEmpty_clicked();
    void on_btnBrick_clicked();
    void on_btnSteel_clicked();
    void on_btnSea_clicked();
    void on_btnJungle_clicked();
    void on_btnIce_clicked();
    void on_btnEagle_clicked();
    void on_btnEdge_clicked();
    void on_btnP1_clicked();
    void on_btnP2_clicked();
    void on_btnE_clicked();

    void on_chk00_toggled(bool checked);
    void on_chk10_toggled(bool checked);
    void on_chk01_toggled(bool checked);
    void on_chk11_toggled(bool checked);

    void on_sbX_valueChanged(int i);
    void on_sbY_valueChanged(int i);
    void on_sbW_valueChanged(int i);
    void on_sbH_valueChanged(int i);

    void on_mapName_textEdited(const QString &text);

    void on_tabWidget_currentChanged();

    void setTabNotSaved();
    void closeTab(int tab);

protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::MainWindow *ui;

    int newCounter;
    QPoint mpos;
};

#endif // MAINWINDOW_H
