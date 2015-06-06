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

#ifndef NEWWIZARD_H
#define NEWWIZARD_H

#include <QWizard>

namespace Ui {
    class NewWizard;
}

class NewWizard : public QWizard
{
    Q_OBJECT

public:
    explicit NewWizard(QWidget *parent = 0);
    ~NewWizard();

    QString getMapName() const;
    int getTemplateType() const;

private:
    Ui::NewWizard *ui;
};
#endif // NEWWIZARD_H
