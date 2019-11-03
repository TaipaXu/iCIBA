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

#include "htmlprint.h"
#include <QObject>
#include <sstream>
#include <string>

const std::map<HtmlPrint::Color, std::string> HtmlPrint::colors{
    {Color::Black, "#000"},
    {Color::Red, "#f00"},
    {Color::Green, "#0f0"},
    {Color::Blue, "#00f"},
    {Color::White, "#fff"},
};

const std::string HtmlPrint::INDENT{"&nbsp;&nbsp;"};

HtmlPrint::HtmlPrint(const Model::WordResult &wordResult) : BasePrint(wordResult)
{
}

HtmlPrint::HtmlPrint(const Model::SentenceResult &sentenceResult) : BasePrint(sentenceResult)
{
}

std::string HtmlPrint::getNetworkErrorStr()
{
    return getErrorStr(BasePrint::getNetworkErrorStr());
}

std::string HtmlPrint::getNetworkParseErrorStr()
{
    return getErrorStr(BasePrint::getNetworkParseErrorStr());
}

QString HtmlPrint::color(const Color &color)
{
    const std::string str = colors.at(color);
    return QString::fromStdString(str);
}

QString HtmlPrint::getFgColorAttribute(const Color &color)
{
    return QString("color: %1").arg(HtmlPrint::color(color));
}

QString HtmlPrint::getBgColorAttribute(const Color &color)
{
    return QString("background-color: %1").arg(HtmlPrint::color(color));
}

std::string HtmlPrint::getWordStr()
{

    std::ostringstream stringStream;
    stringStream << wordResult.word.toStdString()
                 << getPronunciation()
                 << getMeanings()
                 << getExchange();
    return stringStream.str();
}

std::string HtmlPrint::getSentenceStr()
{
    std::ostringstream stringStream;
    stringStream << getElement("p", sentenceResult.sentence).toStdString()
                 << getElement("p",
                               sentenceResult.translation,
                               {
                                   {"style", getFgColorAttribute(Color::Green)},
                               })
                        .toStdString();
    return stringStream.str();
}

std::string HtmlPrint::getPronunciation()
{
    std::ostringstream stringStream;
    if (!wordResult.pronunciation.en.isEmpty())
    {
        stringStream << QObject::tr("en").toStdString()
                     << "["
                     << getElement("a",
                                   wordResult.pronunciation.en,
                                   {
                                       {"style", getFgColorAttribute(Color::Green)},
                                       {"href", "audio:" + wordResult.pronunciation.enVoice},
                                   })
                            .toStdString()
                     << "]";
    }
    if (!wordResult.pronunciation.en.isEmpty() && !wordResult.pronunciation.am.isEmpty())
    {
        stringStream << INDENT;
    }

    if (!wordResult.pronunciation.am.isEmpty())
    {
        stringStream << QObject::tr("us").toStdString()
                     << "["
                     << getElement("a",
                                   wordResult.pronunciation.am,
                                   {
                                       {"style", getFgColorAttribute(Color::Green)},
                                       {"href", "audio:" + wordResult.pronunciation.amVoice},
                                   })
                            .toStdString()
                     << "]";
    }
    std::string str = stringStream.str();
    if (str.length() == 0)
    {
        return "";
    }
    else
    {
        return "<p>" + str + "</p>";
    }
}

std::string HtmlPrint::getMeanings()
{
    QStringList list;
    std::transform(wordResult.meanings.begin(), wordResult.meanings.end(), std::back_inserter(list), [this](const Model::WordResult::Meaning &meaning) {
        return QString{"%1 %2"}
            .arg(getElement("span", meaning.part, {{"style", getFgColorAttribute(Color::Green)}}))
            .arg(meaning.means.join("; "));
    });
    if (list.isEmpty())
    {
        return "";
    }
    else
    {
        return QString{"<p>%1</p>"}.arg(list.join("<br>")).toStdString();
    }
}

std::string HtmlPrint::getExchange()
{
    QString exchange{};
    {
        if (!wordResult.exchange.pl.isEmpty())
        {
            exchange += QObject::tr("pl") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.pl.begin(), wordResult.exchange.pl.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.third.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("third") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.third.begin(), wordResult.exchange.third.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.past.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("past") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.past.begin(), wordResult.exchange.past.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.done.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("done") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.past.begin(), wordResult.exchange.past.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.ing.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("ing") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.ing.begin(), wordResult.exchange.ing.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.er.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("er") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.er.begin(), wordResult.exchange.er.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.est.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("est") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.est.begin(), wordResult.exchange.est.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.prep.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("prep") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.prep.begin(), wordResult.exchange.prep.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.adv.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("adv") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.adv.begin(), wordResult.exchange.adv.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.verv.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("verv") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.verv.begin(), wordResult.exchange.verv.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.noun.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("noun") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.noun.begin(), wordResult.exchange.noun.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.adj.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("adj") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.adj.begin(), wordResult.exchange.adj.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
        if (!wordResult.exchange.conn.isEmpty())
        {
            if (!exchange.isEmpty())
            {
                exchange += INDENT.data();
            }

            exchange += QObject::tr("conn") + ": ";
            QStringList list;
            std::transform(wordResult.exchange.conn.begin(), wordResult.exchange.conn.end(), std::back_inserter(list), [this](const QString str) {
                return getElement(
                    "a",
                    str,
                    {
                        {"style", getFgColorAttribute(Color::Green)},
                        {"href", "word:" + str},
                    });
            });
            exchange += list.join(", ");
        }
    }
    if (exchange.isEmpty())
    {
        return "";
    }
    else
    {
        return QString{"<p>%1</p>"}.arg(exchange).toStdString();
    }
}

std::string HtmlPrint::getErrorStr(const std::string &str)
{
    return getElement("p",
                      QString::fromStdString(str),
                      {
                          {"style", getFgColorAttribute(Color::Red)},
                      })
        .toStdString();
}

std::string HtmlPrint::getErrorStr(const QString &str)
{
    return getErrorStr(str.toStdString());
}

QString HtmlPrint::getElement(const QString &element, const QString &content, const std::map<const QString, const QString> &attributes)
{
    QString attributesStr{""};
    for (auto &&attribute : attributes)
    {
        attributesStr += QString{"%1=\"%2\" "}.arg(attribute.first).arg(attribute.second);
    }

    return QString{"<%1 %3>%2</%1>"}
        .arg(element)
        .arg(content)
        .arg(attributesStr);
}
