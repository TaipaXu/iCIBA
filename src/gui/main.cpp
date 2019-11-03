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

#include <QApplication>
#include <QProcess>
#include <QDir>
#include <csignal>
#include "singleapplication.h"
#include "core.h"
#include "../config.h"

void signalHandler(int sig)
{
    qApp->quit();
}

int main(int argc, char *argv[])
{
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    SingleApplication app(argc, argv);

    qApp->setOrganizationDomain(ORGANIZATION_DOMAIN);
    qApp->setApplicationName(PROJECT_NAME);
    qApp->setApplicationVersion(PROJECT_VERSION);
    qApp->setApplicationDisplayName(DISPLAY_NAME);

    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);

    Core core{};
    core.start();

    QObject::connect(&app, &SingleApplication::instanceStarted, &core, &Core::showApp);

    const int result = app.exec();

    if (result == REBOOT_CODE)
    {
        const QString program = QApplication::applicationFilePath();
        const QStringList arguments = QApplication::arguments();
        const QString appDirectory = QDir::currentPath();
        QProcess::startDetached(program, arguments, appDirectory);

        return 0;
    }

    return result;
}
