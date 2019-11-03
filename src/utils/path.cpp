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

#include "path.h"
#include <QCoreApplication>
#include <QStandardPaths>
#include <QDir>
#include "../errors/systemerror.h"

QString Path::getSettingsDirPath()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    return path;
}

QString Path::getSettingsFilePath()
{
    const QString dirPath = getSettingsDirPath();
    const QString fileName{"settings.ini"};
    const QString filePath = QString("%1/%2/%3")
                                 .arg(dirPath)
                                 .arg(qApp->applicationName())
                                 .arg(fileName);
    return filePath;
}

QString Path::getDataDirPath()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
    return path;
}

QString Path::getDataFilePath()
{
    const QString dirPath = getDataDirPath();
    const QString fileName{"history"};
    const QString filePath = QString("%1/%2").arg(dirPath).arg(fileName);
    return filePath;
}

QString Path::getAppPath()
{
    return QCoreApplication::applicationFilePath();
}

void Path::resolveDir(const QString &dirPath)
{
    const QDir dir(dirPath);
    if (!dir.exists() && !dir.mkpath(dirPath))
    {
        throw SystemError{""};
    }
}

int Path::getFileLineCount(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return 0;
    }
    int count{0};
    while (!file.atEnd())
    {
        file.readLine();
        count++;
    }
    file.close();
    return count;
}

void Path::removeFile(const QString &path)
{
    QFile file(path);
    if (file.exists() && !file.remove())
    {
        throw SystemError{""};
    }
}

void Path::removeDir(const QString &path)
{
}
