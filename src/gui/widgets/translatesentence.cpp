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

#include "translatesentence.h"
#include "ui_translatesentence.h"
#include "../network.h"
#include "../utils/htmlprint.h"
#include "../models/sentenceresult.h"

TranslateSentence::TranslateSentence(QWidget *parent) : QWidget{parent},
                                                        ui{new Ui::TranslateSentence},
                                                        network{new Network{this}}
{
    ui->setupUi(this);

    connect(ui->translateButton, &QPushButton::clicked, this, &TranslateSentence::translateSentence);
    connect(network, &Network::translateSentenceResult, this, &TranslateSentence::handleTranslateSentenceResult);
    connect(network, &Network::networkError, this, &TranslateSentence::handleNetworkError);
    connect(network, &Network::parseError, this, &TranslateSentence::handleNetworkParseError);
}

TranslateSentence::~TranslateSentence()
{
    delete ui;
}

void TranslateSentence::translateSentence()
{
    if (!ui->sourceTextEdit->toPlainText().isEmpty())
    {
        network->translateSentence(ui->sourceTextEdit->toPlainText());
    }
}

void TranslateSentence::handleTranslateSentenceResult(const Model::SentenceResult &sentenceResult)
{
    ui->destination->setPlainText(sentenceResult.translation);
}

void TranslateSentence::handleNetworkError()
{
    ui->destination->setText(QString::fromStdString(HtmlPrint::getNetworkErrorStr()));
}

void TranslateSentence::handleNetworkParseError()
{
    ui->destination->setText(QString::fromStdString(HtmlPrint::getNetworkParseErrorStr()));
}
