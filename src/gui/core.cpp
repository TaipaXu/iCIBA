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

#include "core.h"
#include <QApplication>
#include <QTranslator>
#include "../network.h"
#include "../persistence/settings.h"
#include "widgets/mainwindow.h"
#include "widgets/tray.h"
#include "../models/wordresult.h"
#include "../config.h"

Core::Core(QObject *parent) : QObject(parent),
                              mainWindow(nullptr),
                              tray(nullptr),
                              settings{Settings::getInstance()}
{
    installTranslator();
    qApp->setQuitOnLastWindowClosed(false);

    connect(settings, &Settings::settingsChanged, this, &Core::handleSettingsChanged);
}

Core::~Core()
{
    delete mainWindow;
    delete tray;
    settings->destory();
}

void Core::start()
{
    mainWindow = new MainWindow();
    mainWindow->showInCenter();
    tray = new Tray();

    connect(tray, &Tray::actionShow, mainWindow, &MainWindow::forceShow);
    connect(tray, &Tray::actionSettings, mainWindow, &MainWindow::handleSettingsActionTriggered);
}

void Core::showApp()
{
    mainWindow->forceShow();
    mainWindow->showInCenter();
}

void Core::handleSettingsChanged()
{
    const QString language = settings->getLanguage();
    if (currentLanguage != language)
    {
        reboot();
    }
}

void Core::installTranslator()
{
    currentLanguage = settings->getLanguage();

    if (currentLanguage == "zh_CN")
    {
        QTranslator *translator = new QTranslator(this);
        translator->load(":/i18n/zh_CN");
        qApp->installTranslator(translator);
    }
}

void Core::reboot()
{
    qApp->exit(REBOOT_CODE);
}
