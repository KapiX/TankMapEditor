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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "tankmapwidget.h"
#include "newwizard.h"
#include <QSignalMapper>
#include <QMessageBox>
#include <QTextCodec>
#include <QMouseEvent>
#ifdef Q_WS_WIN
#  include <windows.h>
#  include "qtwin.h"
#  include <QPainter>
#  include <QFont>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    newCounter(1)
{
    ui->setupUi(this);
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("System"));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    TankMapWidget::setEmptyButton(ui->btnEmpty);
    TankMapWidget::setEagleButton(ui->btnEagle);
    TankMapWidget::setP1Button(ui->btnP1);
    TankMapWidget::setP2Button(ui->btnP2);
    TankMapWidget::setEButton(ui->btnE);
#ifdef Q_WS_WIN
    HANDLE iconSm, iconBig;
    iconSm = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(10000), IMAGE_ICON, 16, 16, NULL);
    iconBig = LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(10000), IMAGE_ICON, 32, 32, NULL);
    SendMessage(winId(), WM_SETICON, ICON_SMALL, (LPARAM) iconSm);
    SendMessage(winId(), WM_SETICON, ICON_BIG, (LPARAM) iconBig);
    ui->mainToolBar->setStyleSheet("QToolBar { border:0; }");
    if (QtWin::isCompositionEnabled()) {
        QtWin::extendFrameIntoClientArea(this, 0, ui->mainToolBar->sizeHint().height(), 0, 0);
    }
#else
    QIcon icon(":/images/icon.ico");
    setWindowIcon(icon);
#endif
    ui->actionNew->setShortcuts(QKeySequence::New);
    ui->actionOpen->setShortcuts(QKeySequence::Open);
    ui->actionSave->setShortcuts(QKeySequence::Save);
    ui->actionQuit->setShortcuts(QKeySequence::Quit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    NewWizard wizard;
    if(!wizard.exec()) return;
    QString mapName;
    mapName = (!wizard.getMapName().isEmpty() ? wizard.getMapName() : tr("Map ") + QString::number(newCounter));

    int index;
    TankMapWidget *widget = new TankMapWidget();
    widget->setDrawGrid(ui->actionShowGrid->isChecked());
    widget->setDrawBonusArea(ui->actionShowBonusArea->isChecked());
    connect(ui->actionShowGrid, SIGNAL(toggled(bool)), widget, SLOT(setDrawGrid(bool)));
    connect(ui->actionShowBonusArea, SIGNAL(toggled(bool)), widget, SLOT(setDrawBonusArea(bool)));
    connect(widget, SIGNAL(mapChanged()), this, SLOT(setTabNotSaved()));

    switch(wizard.getTemplateType())
    {
    default:
    case 0:
        break;
    case 1:
        widget->load(":/templates/19x13.tlv");
        break;
    case 2:
        widget->load(":/templates/13x13.tlv");
        break;
    }

    widget->setMapName(mapName);
    index = ui->tabWidget->addTab(widget, mapName);

    ui->tabWidget->setCurrentIndex(index);
    newCounter++;

    TLV1_BonusRect br;
    br = widget->getBonusRect();
    ui->sbX->setValue(br.x);
    ui->sbY->setValue(br.y);
    ui->sbW->setValue(br.w);
    ui->sbH->setValue(br.h);

    TLV1_Point p, e1, e2, e3;
    bool cond;
    p = widget->getEagle();
    cond = (p.x == -1 && p.y == -1);
    ui->btnEagle->setEnabled(cond);
    p = widget->getSpawnP1();
    cond = (p.x == -1 && p.y == -1);
    ui->btnP1->setEnabled(cond);
    p = widget->getSpawnP2();
    cond = (p.x == -1 && p.y == -1);
    ui->btnP2->setEnabled(cond);
    e1 = widget->getSpawnE1();
    e2 = widget->getSpawnE2();
    e3 = widget->getSpawnE3();
    cond = ((e1.x == -1 && e1.y == -1) || (e2.x == -1 && e2.y == -1) || (e3.x == -1 && e3.y == -1));
    ui->btnE->setEnabled(cond);
}

void MainWindow::on_actionOpen_triggered()
{
    int index;
    TankMapWidget *widget = new TankMapWidget();
    widget->setDrawGrid(ui->actionShowGrid->isChecked());
    connect(ui->actionShowGrid, SIGNAL(toggled(bool)), widget, SLOT(setDrawGrid(bool)));
    if(!widget->load()) return;
    index = ui->tabWidget->addTab(widget, widget->getMapName());

    ui->tabWidget->setCurrentIndex(index);

    TLV1_Point p, e1, e2, e3;
    bool cond;
    p = widget->getEagle();
    cond = (p.x == -1 && p.y == -1);
    ui->btnEagle->setEnabled(cond);
    p = widget->getSpawnP1();
    cond = (p.x == -1 && p.y == -1);
    ui->btnP1->setEnabled(cond);
    p = widget->getSpawnP2();
    cond = (p.x == -1 && p.y == -1);
    ui->btnP2->setEnabled(cond);
    e1 = widget->getSpawnE1();
    e2 = widget->getSpawnE2();
    e3 = widget->getSpawnE3();
    cond = ((e1.x == -1 && e1.y == -1) || (e2.x == -1 && e2.y == -1) || (e3.x == -1 && e3.y == -1));
    ui->btnE->setEnabled(cond);
}

void MainWindow::on_actionSave_triggered()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(!widget->getChanged()) return;
    if(widget != 0)
    {
        if(widget->save())
        {
            int index = ui->tabWidget->currentIndex();
            QString text = ui->tabWidget->tabText(index);
            if(text.endsWith('*'))
            {
                text.truncate(text.length() - 1);
                ui->tabWidget->setTabText(index, text);
            }
        }
    }
}

void MainWindow::on_actionSaveAll_triggered()
{
    for(int i = 0; i < ui->tabWidget->count(); i++)
    {
        TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->widget(i);
        if(!widget->getChanged()) continue;
        ui->tabWidget->setCurrentIndex(i);
        if(widget != 0)
        {
            if(widget->save())
            {
                int index = ui->tabWidget->currentIndex();
                QString text = ui->tabWidget->tabText(index);
                if(text.endsWith('*'))
                {
                    text.truncate(text.length() - 1);
                    ui->tabWidget->setTabText(index, text);
                }
            }
        }
    }
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog *aboutdlg = new AboutDialog(this);
    aboutdlg->exec();
    delete aboutdlg;
}

void MainWindow::on_btnEmpty_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(EMPTY);
        ui->chk00->setEnabled(false);
        ui->chk10->setEnabled(false);
        ui->chk01->setEnabled(false);
        ui->chk11->setEnabled(false);

        ui->chk00->setChecked(true);
        ui->chk10->setChecked(true);
        ui->chk01->setChecked(true);
        ui->chk11->setChecked(true);
    }
}

void MainWindow::on_btnBrick_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(BRICK);
        ui->chk00->setEnabled(true);
        ui->chk10->setEnabled(true);
        ui->chk01->setEnabled(true);
        ui->chk11->setEnabled(true);
    }
}

void MainWindow::on_btnSteel_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(STEEL);
        ui->chk00->setEnabled(true);
        ui->chk10->setEnabled(true);
        ui->chk01->setEnabled(true);
        ui->chk11->setEnabled(true);
    }
}

void MainWindow::on_btnSea_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(SEA);
        ui->chk00->setEnabled(true);
        ui->chk10->setEnabled(true);
        ui->chk01->setEnabled(true);
        ui->chk11->setEnabled(true);
    }
}

void MainWindow::on_btnJungle_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(JUNGLE);
        ui->chk00->setEnabled(true);
        ui->chk10->setEnabled(true);
        ui->chk01->setEnabled(true);
        ui->chk11->setEnabled(true);
    }
}

void MainWindow::on_btnIce_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(ICE);
        ui->chk00->setEnabled(true);
        ui->chk10->setEnabled(true);
        ui->chk01->setEnabled(true);
        ui->chk11->setEnabled(true);
    }
}

void MainWindow::on_btnEagle_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(EAGLE);
        ui->chk00->setEnabled(false);
        ui->chk10->setEnabled(false);
        ui->chk01->setEnabled(false);
        ui->chk11->setEnabled(false);

        ui->chk00->setChecked(true);
        ui->chk10->setChecked(true);
        ui->chk01->setChecked(true);
        ui->chk11->setChecked(true);
    }
}

void MainWindow::on_btnEdge_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(EDGE);
        ui->chk00->setEnabled(true);
        ui->chk10->setEnabled(true);
        ui->chk01->setEnabled(true);
        ui->chk11->setEnabled(true);
    }
}

void MainWindow::on_btnP1_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(P1);
        ui->chk00->setEnabled(false);
        ui->chk10->setEnabled(false);
        ui->chk01->setEnabled(false);
        ui->chk11->setEnabled(false);

        ui->chk00->setChecked(true);
        ui->chk10->setChecked(true);
        ui->chk01->setChecked(true);
        ui->chk11->setChecked(true);
    }
}

void MainWindow::on_btnP2_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(P2);
        ui->chk00->setEnabled(false);
        ui->chk10->setEnabled(false);
        ui->chk01->setEnabled(false);
        ui->chk11->setEnabled(false);

        ui->chk00->setChecked(true);
        ui->chk10->setChecked(true);
        ui->chk01->setChecked(true);
        ui->chk11->setChecked(true);
    }
}

void MainWindow::on_btnE_clicked()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->changeTool(E);
        ui->chk00->setEnabled(false);
        ui->chk10->setEnabled(false);
        ui->chk01->setEnabled(false);
        ui->chk11->setEnabled(false);

        ui->chk00->setChecked(true);
        ui->chk10->setChecked(true);
        ui->chk01->setChecked(true);
        ui->chk11->setChecked(true);
    }
}

void MainWindow::on_chk00_toggled(bool checked)
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->setPart(0, 0, checked);
    }
}

void MainWindow::on_chk01_toggled(bool checked)
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->setPart(0, 1, checked);
    }
}

void MainWindow::on_chk10_toggled(bool checked)
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->setPart(1, 0, checked);
    }
}

void MainWindow::on_chk11_toggled(bool checked)
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        widget->setPart(1, 1, checked);
    }
}

void MainWindow::on_sbX_valueChanged(int i)
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        TLV1_BonusRect br;
        br = widget->getBonusRect();
        widget->setBonusRect(i, br.y, br.w, br.h);
    }
}

void MainWindow::on_sbY_valueChanged(int i)
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        TLV1_BonusRect br;
        br = widget->getBonusRect();
        widget->setBonusRect(br.x, i, br.w, br.h);
    }
}

void MainWindow::on_sbW_valueChanged(int i)
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        TLV1_BonusRect br;
        br = widget->getBonusRect();
        widget->setBonusRect(br.x, br.y, i, br.h);
    }
}

void MainWindow::on_sbH_valueChanged(int i)
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        TLV1_BonusRect br;
        br = widget->getBonusRect();
        widget->setBonusRect(br.x, br.y, br.w, i);
    }
}

void MainWindow::on_mapName_textEdited(const QString &text)
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), text + (widget->getChanged() ? "*" : ""));
        widget->setMapName(text);
    }
}

void MainWindow::on_tabWidget_currentChanged()
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->currentWidget();
    if(widget != 0)
    {
        TLV1_BonusRect br;
        br = widget->getBonusRect();
        ui->sbX->setValue(br.x);
        ui->sbY->setValue(br.y);
        ui->sbW->setValue(br.w);
        ui->sbH->setValue(br.h);

        TLV1_Point p, e1, e2, e3;
        bool cond;
        p = widget->getEagle();
        cond = (p.x == -1 && p.y == -1);
        ui->btnEagle->setEnabled(cond);
        p = widget->getSpawnP1();
        cond = (p.x == -1 && p.y == -1);
        ui->btnP1->setEnabled(cond);
        p = widget->getSpawnP2();
        cond = (p.x == -1 && p.y == -1);
        ui->btnP2->setEnabled(cond);
        e1 = widget->getSpawnE1();
        e2 = widget->getSpawnE2();
        e3 = widget->getSpawnE3();
        cond = ((e1.x == -1 && e1.y == -1) || (e2.x == -1 && e2.y == -1) || (e3.x == -1 && e3.y == -1));
        ui->btnE->setEnabled(cond);

        ui->mapName->setText(widget->getMapName());
        ui->actionShowGrid->setChecked(widget->getDrawGrid());
        switch(widget->getSelectedTool())
        {
        case EMPTY:
            ui->btnEmpty->setChecked(true);
            ui->chk00->setEnabled(false);
            ui->chk10->setEnabled(false);
            ui->chk01->setEnabled(false);
            ui->chk11->setEnabled(false);
            break;
        case BRICK:
            ui->btnBrick->setChecked(true);
            ui->chk00->setEnabled(true);
            ui->chk10->setEnabled(true);
            ui->chk01->setEnabled(true);
            ui->chk11->setEnabled(true);
            break;
        case STEEL:
            ui->btnSteel->setChecked(true);
            ui->chk00->setEnabled(true);
            ui->chk10->setEnabled(true);
            ui->chk01->setEnabled(true);
            ui->chk11->setEnabled(true);
            break;
        case SEA:
            ui->btnSea->setChecked(true);
            ui->chk00->setEnabled(true);
            ui->chk10->setEnabled(true);
            ui->chk01->setEnabled(true);
            ui->chk11->setEnabled(true);
            break;
        case JUNGLE:
            ui->btnJungle->setChecked(true);
            ui->chk00->setEnabled(true);
            ui->chk10->setEnabled(true);
            ui->chk01->setEnabled(true);
            ui->chk11->setEnabled(true);
            break;
        case ICE:
            ui->btnIce->setChecked(true);
            ui->chk00->setEnabled(true);
            ui->chk10->setEnabled(true);
            ui->chk01->setEnabled(true);
            ui->chk11->setEnabled(true);
            break;
        case EAGLE:
            ui->btnEagle->setChecked(true);
            ui->chk00->setEnabled(false);
            ui->chk10->setEnabled(false);
            ui->chk01->setEnabled(false);
            ui->chk11->setEnabled(false);
            break;
        case EDGE:
            ui->btnEdge->setChecked(true);
            ui->chk00->setEnabled(true);
            ui->chk10->setEnabled(true);
            ui->chk01->setEnabled(true);
            ui->chk11->setEnabled(true);
            break;
        case P1:
            ui->btnP1->setChecked(true);
            ui->chk00->setEnabled(false);
            ui->chk10->setEnabled(false);
            ui->chk01->setEnabled(false);
            ui->chk11->setEnabled(false);
            break;
        case P2:
            ui->btnP2->setChecked(true);
            ui->chk00->setEnabled(false);
            ui->chk10->setEnabled(false);
            ui->chk01->setEnabled(false);
            ui->chk11->setEnabled(false);
            break;
        case E:
            ui->btnE->setChecked(true);
            ui->chk00->setEnabled(false);
            ui->chk10->setEnabled(false);
            ui->chk01->setEnabled(false);
            ui->chk11->setEnabled(false);
            break;
        }

        ui->chk00->setChecked(widget->getPart(0, 0));
        ui->chk10->setChecked(widget->getPart(1, 0));
        ui->chk01->setChecked(widget->getPart(0, 1));
        ui->chk11->setChecked(widget->getPart(1, 1));
    }
}

void MainWindow::setTabNotSaved()
{
    int index = ui->tabWidget->currentIndex();
    QString text = ui->tabWidget->tabText(index);
    if(!text.endsWith('*'))
        ui->tabWidget->setTabText(index, text + "*");
}

void MainWindow::closeTab(int tab)
{
    TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->widget(tab);
    if (widget)
    {
        if(widget->getChanged())
        {
            QMessageBox::StandardButton result =
                    QMessageBox::warning(
                        this,
                        tr("Unsaved changes"),
                        widget->getMapName() + tr(" contains unsaved changes. Do you want to save it now?"),
                        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                        QMessageBox::Save
                    );
            switch(result)
            {
            case QMessageBox::Save:
                widget->save();
                break;
            case QMessageBox::Discard:
                break;
            case QMessageBox::Cancel:
                return;
            default:
                return;
            }
        }
        ui->tabWidget->removeTab(ui->tabWidget->indexOf(widget));
        delete widget;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    while(ui->tabWidget->count() != 0)
    {
        TankMapWidget *widget = (TankMapWidget *) ui->tabWidget->widget(0);
        ui->tabWidget->setCurrentIndex(0);
        if (widget)
        {
            if(widget->getChanged())
            {
                QMessageBox::StandardButton result =
                        QMessageBox::warning(
                            this,
                            tr("Unsaved changes"),
                            widget->getMapName() + tr(" contains unsaved changes. Do you want to save it now?"),
                            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
                            QMessageBox::Save
                        );
                switch(result)
                {
                case QMessageBox::Save:
                    widget->save();
                    break;
                case QMessageBox::Discard:
                    break;
                case QMessageBox::Cancel:
                    event->ignore();
                    return;
                default:
                    return;
                }
            }
            ui->tabWidget->removeTab(0);
            delete widget;
        }
    }
    event->accept();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
#ifdef Q_WS_WIN
    QPainter painter(this);
    painter.setBackground(QBrush(palette().color(QPalette::Window)));
    painter.eraseRect(QRect(0, ui->mainToolBar->sizeHint().height(), width(), height()));
#endif
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mpos = event->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(mpos.y() > ui->mainToolBar->pos().y() &&
       mpos.y() < ui->mainToolBar->pos().y() + ui->mainToolBar->size().height() &&
       mpos.x() > ui->mainToolBar->childrenRect().right() &&
       mpos.x() < ui->mainToolBar->size().width())
    {
        if(event->buttons() & Qt::LeftButton)
        {
            QPoint diff = event->pos() - mpos;
            QPoint newpos = pos() + diff;

            move(newpos);
        }
    }
}
