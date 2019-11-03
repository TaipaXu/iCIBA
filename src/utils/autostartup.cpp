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

#include "autostartup.h"
#include <QCoreApplication>
#include <QSettings>
#include <QStandardPaths>
#include <QFile>
#include <sstream>
#include "../errors/systemerror.h"
#include "./path.h"
#include "../config.h"

static QString winPath{"HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"};

AutoStartup::AutoStartup()
{
}

#ifdef Q_OS_LINUX
void AutoStartup::set(bool enabled)
{
    QString autoStartPath = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/autostart/";

    QString desktopFilePath = autoStartPath + PROJECT_NAME + QLatin1String(".desktop");

    if (enabled)
    {
        Path::resolveDir(autoStartPath);

        QFile iniFile(desktopFilePath);
        if (!iniFile.open(QIODevice::WriteOnly))
        {
            //qDebug() << "Could not write auto start entry" << desktopFilePath;
            throw SystemError{""};
        }

        std::ostringstream stringStream;
        stringStream << "[Desktop Entry]" << std::endl
                     << "Name=" << PROJECT_NAME << std::endl
                     << "Exec=" << Path::getAppPath().toStdString() << std::endl
                     << "Terminal="
                     << "false" << std::endl
                     << "Categories="
                     << "Application" << std::endl
                     << "Type="
                     << "Application" << std::endl
                     << "StartupNotify="
                     << "false" << std::endl
                     << "X-GNOME-Autostart-enabled="
                     << "true" << std::endl;
        iniFile.write(stringStream.str().data());
    }
    else
    {
        Path::removeFile(desktopFilePath);
    }
}
#endif

#ifdef Q_OS_WIN
void AutoStartup::set(bool enabled)
{
    QSettings settings(winPath, QSettings::NativeFormat);

    if (enabled)
    {
        settings.setValue(PROJECT_NAME, QCoreApplication::applicationFilePath().replace('/', '\\'));
    }
    else
    {
        settings.remove(PROJECT_NAME);
    }
}
#endif
