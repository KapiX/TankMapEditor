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

#include <QtGui/QApplication>
#include <QLocale>
#include <QTranslator>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString qtFilename = QString("qt_%1.qm").arg(QLocale::system().name());
    QString appFilename = QString("tankme_%1.qm").arg(QLocale::system().name());
    QTranslator qtTranslator;
    QTranslator appTranslator;
    qtTranslator.load(qtFilename.toLower(), qApp->applicationDirPath() + QString("/translations"));
    appTranslator.load(appFilename.toLower(), qApp->applicationDirPath() + QString("/translations"));
    a.installTranslator(&qtTranslator);
    a.installTranslator(&appTranslator);

    MainWindow w;
    w.show();

    return a.exec();
}
