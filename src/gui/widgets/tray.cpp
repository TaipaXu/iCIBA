/*
 * GNU General Public License, Version 3.0
 *
 * Copyright (c) 2019 Taipa Xu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "tray.h"
#include <QApplication>
#include <QMenu>

Tray::Tray(QObject *parent) : QObject(parent), menu(new QMenu()), tray(new QSystemTrayIcon(this))
{
    QAction *showAction = new QAction(tr("Show"), this);
    menu->addAction(showAction);
    connect(showAction, &QAction::triggered, this, &Tray::actionShow);
    menu->addSeparator();

    QAction *settingsAction = new QAction(tr("Settings"), this);
    menu->addAction(settingsAction);
    connect(settingsAction, &QAction::triggered, this, &Tray::actionSettings);
    menu->addSeparator();

    QAction *quitAction = new QAction(tr("Exit"));
    menu->addAction(quitAction);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);

    connect(tray, &QSystemTrayIcon::activated, this, &Tray::onTrayActivated);

    tray->setContextMenu(menu);
    tray->setIcon(QIcon(":/images/icon"));
    tray->show();
}

Tray::~Tray()
{
    delete menu;
}

void Tray::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
        emit actionShow();
        break;

    default:
        break;
    }
}
