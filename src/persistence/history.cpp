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

#include "history.h"
#include <QFile>
#include "../utils/path.h"

History::History()
{
    resolveDataPath();
}

History::~History()
{
}

void History::addHistory(const QString &str) const
{
    QFile file(Path::getDataFilePath());
    if (file.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        file.write(str.toStdString().data());
        file.write("\n");
    }
    file.close();
}

QStringList History::getHistories(const unsigned int &count) const
{
    const int allCounts = getDataFileLineCount();

    int startLine{0};
    if (count != 0)
    {
        startLine = std::max(allCounts - static_cast<signed>(count), {0});
    }

    QFile file(Path::getDataFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return {};
    }

    QStringList result{};
    int currentLine{0};
    while (!file.atEnd())
    {
        QString line = file.readLine();
        if (currentLine++ >= startLine)
        {
            if (!line.isNull())
            {
                result << line.remove("\n");
            }
        }
    }

    return result;
}

void History::resolveDataPath() const
{
    Path::resolveDir(Path::getDataDirPath());
}

int History::getDataFileLineCount() const
{
    return Path::getFileLineCount(Path::getDataFilePath());
}
