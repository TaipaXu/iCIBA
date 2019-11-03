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

#include "settings.h"
#include <QSettings>
#include "../utils/autostartup.h"
#include "../utils/path.h"

Settings *Settings::instance = nullptr;
pthread_mutex_t Settings::mutex;

Settings::Settings(QObject *parent) : QObject{parent}
{
    settings = new QSettings(Path::getSettingsFilePath(), QSettings::IniFormat);
    readSettings();
}

Settings *Settings::getInstance()
{
    if (instance == nullptr)
    {
        pthread_mutex_lock(&mutex);
        instance = new Settings();
        pthread_mutex_unlock(&mutex);
    }

    return instance;
}

void Settings::destory()
{
    delete settings;
    settings = nullptr;
    delete instance;
    instance = nullptr;
}

QString Settings::getLanguage() const
{
    return language;
}

void Settings::setLanguage(const QString &language)
{
    settings->setValue("language", language);
    this->language = language;
}

bool Settings::getAutoStartup() const
{
    return autoStartup;
}

void Settings::setAutoStartup(const bool &startup)
{
    AutoStartup *autoStartup = new AutoStartup();
    autoStartup->set(startup);
    delete autoStartup;
    settings->setValue("autoStartup", startup);
    this->autoStartup = startup;
}

void Settings::readSettings()
{
    language = settings->value("language", QVariant("en")).toString();
    autoStartup = settings->value("autoStartup", QVariant(false)).toBool();
}
