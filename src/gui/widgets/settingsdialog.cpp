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

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "../../persistence/settings.h"

SettingsDialog::SettingsDialog(Settings *settings, QWidget *parent) : QDialog{parent},
                                                                      ui{new Ui::SettingsDialog},
                                                                      settings{settings}

{
    ui->setupUi(this);

    readSettings();
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::handleAcceptButtonClicked);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::handleAcceptButtonClicked()
{
    writeSettings();
    accept();
}

void SettingsDialog::readSettings() const
{
    const QString language{settings->getLanguage()};
    if (language == "en")
    {
        ui->language_combo->setCurrentIndex(0);
    }
    else if (language == "zh_CN")
    {
        ui->language_combo->setCurrentIndex(1);
    }

    bool autoStartup{settings->getAutoStartup()};
    if (autoStartup)
    {
        ui->autoStartUpcheckBox->setCheckState(Qt::Checked);
    }
    else
    {
        ui->autoStartUpcheckBox->setCheckState(Qt::Unchecked);
    }
}

void SettingsDialog::writeSettings() const
{
    const int languageIndex = ui->language_combo->currentIndex();
    QString language{};
    if (languageIndex == 0)
    {
        language = "en";
    }
    else if (languageIndex == 1)
    {
        language = "zh_CN";
    }

    settings->setLanguage(language);

    Qt::CheckState autoStartUp = ui->autoStartUpcheckBox->checkState();
    if (autoStartUp == Qt::Checked)
    {
        settings->setAutoStartup(true);
    }
    else
    {
        settings->setAutoStartup(false);
    }

    emit settings->settingsChanged();
}
