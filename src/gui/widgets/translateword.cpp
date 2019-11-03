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

#include "translateword.h"
#include "ui_translateword.h"
#include <QCompleter>
#include <QStringListModel>
#include <QMediaPlayer>
#include "../network.h"
#include "../utils/htmlprint.h"
#include "../models/wordresult.h"

TranslateWord::TranslateWord(QWidget *parent) : QWidget{parent},
                                                ui{new Ui::TranslateWord},
                                                completer{new QCompleter(this)},
                                                completeModel{new QStringListModel(this)},
                                                network{new Network{this}},
                                                player{new QMediaPlayer{this}}
{
    ui->setupUi(this);
    ui->verticalLayout->setAlignment(Qt::AlignTop);

    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setModel(completeModel);
    ui->wordInput->setCompleter(completer);

    connect(ui->wordInput, &QLineEdit::textChanged, network, &Network::suggest);
    connect(network, &Network::suggestResult, this, &TranslateWord::handleSuggestResult);
    connect(completer, static_cast<void (QCompleter::*)(const QString &)>(&QCompleter::activated), this, &TranslateWord::handleCompleterActivated);

    connect(ui->searchButton, &QPushButton::clicked, this, &TranslateWord::translate);
    connect(ui->wordInput, &QLineEdit::returnPressed, this, &TranslateWord::translate);
    connect(network, &Network::translateWordResult, this, &TranslateWord::handleTranslateWordResult);
    connect(network, &Network::translateSentenceResult, this, &TranslateWord::handleTranslateSentenceResult);
    connect(network, &Network::networkError, this, &TranslateWord::handleNetworkError);
    connect(network, &Network::parseError, this, &TranslateWord::handleNetworkParseError);

    ui->destination->setOpenLinks(false);
    connect(ui->destination, &QTextBrowser::anchorClicked, this, &TranslateWord::handleAnchorClicked);
}

TranslateWord::~TranslateWord()
{
    delete ui;
}

void TranslateWord::handleSuggestResult(const QStringList &result)
{
    completeModel->setStringList(result);
    ui->wordInput->completer()->complete();
}

void TranslateWord::handleCompleterActivated(const QString &text)
{
    this->translate();
}

void TranslateWord::translate()
{
    if (!ui->wordInput->text().isEmpty())
    {
        network->translateWord(ui->wordInput->text());
    }
}

void TranslateWord::handleTranslateWordResult(const Model::WordResult &wordResult)
{
    HtmlPrint print{wordResult};
    ui->destination->setText(QString{print.getStr().data()});
}

void TranslateWord::handleTranslateSentenceResult(const Model::SentenceResult &sentenceResult)
{
    HtmlPrint print{sentenceResult};
    ui->destination->setText(QString{print.getStr().data()});
}

void TranslateWord::handleNetworkError()
{
    ui->destination->setText(QString::fromStdString(HtmlPrint::getNetworkErrorStr()));
}

void TranslateWord::handleNetworkParseError()
{
    ui->destination->setText(QString::fromStdString(HtmlPrint::getNetworkParseErrorStr()));
}

void TranslateWord::handleAnchorClicked(const QUrl &url)
{
    const QString type = url.scheme();
    if (type == "audio")
    {
        player->setMedia(QUrl{url.toString().remove("audio:")});
        player->play();
    }
    else if (type == "word")
    {
        ui->wordInput->setText(url.toString().remove("word:"));
        translate();
    }
}
