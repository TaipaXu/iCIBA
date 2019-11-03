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

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QShortcut>
#include <QUrl>
#include "../persistence/settings.h"
#include "settingsdialog.h"
#include "historydialog.h"
#include "../../../config.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow{parent},
                                          ui{new Ui::MainWindow},
                                          settings{Settings::getInstance()},
                                          settingsDialog{nullptr}
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/images/icon"));

    createActions();

    QShortcut *showDictionaryShortcutCtrl = new QShortcut(this);
    showDictionaryShortcutCtrl->setKey(Qt::CTRL + Qt::Key_1);
    showDictionaryShortcutCtrl->setAutoRepeat(false);
    connect(showDictionaryShortcutCtrl, &QShortcut::activated, this, &MainWindow::showDictionary);
    QShortcut *showDictionaryShortcutAlt = new QShortcut(this);
    showDictionaryShortcutAlt->setKey(Qt::ALT + Qt::Key_1);
    showDictionaryShortcutAlt->setAutoRepeat(false);
    connect(showDictionaryShortcutAlt, &QShortcut::activated, this, &MainWindow::showDictionary);

    QShortcut *showTranslateShortcutCtrl = new QShortcut(this);
    showTranslateShortcutCtrl->setKey(Qt::CTRL + Qt::Key_2);
    showTranslateShortcutCtrl->setAutoRepeat(false);
    connect(showTranslateShortcutCtrl, &QShortcut::activated, this, &MainWindow::showTranslate);
    QShortcut *showTranslateShortcutAlt = new QShortcut(this);
    showTranslateShortcutAlt->setKey(Qt::ALT + Qt::Key_2);
    showTranslateShortcutAlt->setAutoRepeat(false);
    connect(showTranslateShortcutAlt, &QShortcut::activated, this, &MainWindow::showTranslate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::forceShow()
{
#ifdef Q_OS_LINUX
    this->hide();
#endif
    this->show();
}

void MainWindow::showInCenter()
{
    this->show();
    QRect desktopRect = QApplication::desktop()->availableGeometry(this);
    QPoint center = desktopRect.center();
    move(center.x() - width() / 2, center.y() - height() / 2);
}

void MainWindow::handleAboutActionTriggered()
{
    QDesktopServices::openUrl(QUrl(PROJECT_DOMAIN));
}

void MainWindow::showDictionary()
{
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::showTranslate()
{
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::handleSettingsActionTriggered()
{
    SettingsDialog *settingsDialog = new SettingsDialog(settings, this);
    settingsDialog->exec();
    settingsDialog->deleteLater();
    settingsDialog = nullptr;
}

void MainWindow::handleHistoryActionTriggered()
{
    HistoryDialog *historyDialog = new HistoryDialog(this);
    historyDialog->exec();
    historyDialog->deleteLater();
}

void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    const QIcon settingsIcon = QIcon::fromTheme("emblem-system");
    QAction *settingsAction = new QAction{settingsIcon, tr("&Settings"), fileMenu};
    settingsAction->setStatusTip(tr("Settings"));
    connect(settingsAction, &QAction::triggered, this, &MainWindow::handleSettingsActionTriggered);
    fileMenu->addAction(settingsAction);
    fileMenu->addSeparator();

    QAction *historyAction = new QAction(tr("&History"), fileMenu);
    historyAction->setStatusTip(tr("History"));
    connect(historyAction, &QAction::triggered, this, &MainWindow::handleHistoryActionTriggered);
    fileMenu->addAction(historyAction);
    fileMenu->addSeparator();

    const QIcon exitIcon = QIcon::fromTheme("application-exit");
    QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), fileMenu, &QApplication::quit);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));

    QMenu *aboutMenu = menuBar()->addMenu(tr("&About"));
    QAction *aboutAction = new QAction(tr("About"), aboutMenu);
    aboutAction->setStatusTip(tr("Open the page of this project."));
    QAction *aboutQtAction = new QAction(tr("About Qt"), aboutMenu);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::handleAboutActionTriggered);
    connect(aboutQtAction, &QAction::triggered, qApp, &QApplication::aboutQt);
    aboutMenu->addAction(aboutAction);
    aboutMenu->addAction(aboutQtAction);
}
