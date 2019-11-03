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

#pragma once

#include <QObject>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

class Settings : public QObject
{
    Q_OBJECT

public:
    static Settings *getInstance();
    void destory();

public:
    QString getLanguage() const;
    void setLanguage(const QString &language);
    bool getAutoStartup() const;
    void setAutoStartup(const bool &startup);

signals:
    void settingsChanged();

private:
    explicit Settings(QObject *parent = nullptr);
    Settings(Settings &) = delete;
    Settings &operator=(const Settings &) = delete;
    ~Settings() = default;

    void readSettings();

    static Settings *instance;
    static pthread_mutex_t mutex;
    QSettings *settings;
    QString language;
    bool autoStartup;
};
