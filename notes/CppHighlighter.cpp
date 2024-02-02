#include "CppHighlighter.h"
#include <iostream>
#include "mainwindow.h"
#include <QXmlStreamReader>

namespace Syntax
{
    void CppHighlighter::defaultRuleSet()
    {
        //rules for keywords

        //keywords
        classRule.pattern.setPattern("class");
        classRule.format.setFontWeight(QFont::Bold);
        classRule.format.setForeground(Qt::blue);

        namespaceRule.pattern.setPattern("namespace");
        namespaceRule.format.setFontWeight(QFont::Bold);
        namespaceRule.format.setForeground(Qt::blue);

        std::cout<<"namespace rule format: ";
        Ui::qStringOut(namespaceRule.format.font().toString());

        //comments
        commentRule.pattern.setPattern("//.*+");
        commentRule.format.setFontItalic(true);
        commentRule.format.setForeground(Qt::darkGreen);

        //debug
        std::cout<<"comment rule format: ";
        Ui::qStringOut(commentRule.format.font().toString());

        multiLineCommentRuleStart.pattern.setPattern("\(/\\*\)");
        multiLineCommentRuleStart.format.setFontItalic(true);
        multiLineCommentRuleStart.format.setForeground(Qt::darkGreen);

        multiLineCommentRuleEnd.pattern.setPattern("\(\\*/\)");
        multiLineCommentRuleEnd.format.setFontItalic(true);
        multiLineCommentRuleEnd.format.setForeground(Qt::darkGreen);

        functionRule.pattern.setPattern("[\\w\\*\\&\\:]+\\s[\\w\\:\\>\\<\\+\\-\\=\\*]+\\([\\w\\s\\*\\&,\\:\\=]+\\)");
        functionRule.format.setFontWeight(QFont::Bold);
        functionRule.format.setForeground(Qt::red);

        emptyFunctionRule.pattern.setPattern("[\\w\\*\\&\\:]+\\s[\\w\\:\\>\\<\\+\\-\\=\\*]+\\(\\)");
        emptyFunctionRule.format.setFontWeight(QFont::Bold);
        emptyFunctionRule.format.setForeground(Qt::red);

        constructorRule.pattern.setPattern("[\\w\\:]+\\([\\w\\s\\*\\&,\\:\\=]+\\)");
        constructorRule.format.setFontWeight(QFont::Bold);
        constructorRule.format.setForeground(Qt::red);

        defaultConstructorRule.pattern.setPattern("[\\w\\:]+\\(\\)");
        defaultConstructorRule.format.setFontWeight(QFont::Bold);
        defaultConstructorRule.format.setForeground(Qt::red);

        destructorRule.pattern.setPattern("[\\w\\:\\~]+\\(\\)");
        destructorRule.format.setFontWeight(QFont::Bold);
        destructorRule.format.setForeground(Qt::red);
        std::cout<<"destructor rule format: ";
        Ui::qStringOut(destructorRule.format.font().toString());

    }


    void CppHighlighter::loadFromFile(const QString& filename)
    {
        std::cout<<"reading from: ";
        Ui::qStringOut(filename);
        File::File configFile{filename};
        QString content=configFile.read();
        //std::cout<<"content: "<<std::endl;
        //Ui::qStringOut(content);
        QXmlStreamReader xml{content};

        while(!xml.atEnd())
        {
            xml.readNext();
            if(xml.isStartDocument()) continue;
            if(xml.isStartElement())
            {
                if(xml.name().toString()=="rule")
                {
                    //Rule rule;
                    Rule* rule=stringToRule(xml.attributes().value("name").toString());
                    rule->name=xml.attributes().value("name").toString();
                    Ui::qStringOut(xml.attributes().value("name").toString());
                    xml.readNext();
                    bool patternRead=false;
                    bool formatRead=false;
                    while(!patternRead || !formatRead)
                    {
                        //bool oldPattern=patternRead;
                        //bool oldFormat=formatRead;
                        if(xml.name().toString()=="pattern" && !patternRead)
                        {
                            QString* pattern=new QString();
                            QString xmlPatternData=xml.attributes().value("regexp").toString();
                            for(QChar* qch=xmlPatternData.data(); qch-xmlPatternData.data()<xmlPatternData.length(); ++qch)
                            {
                                pattern->append(qch->toLatin1());
                            }

                            rule->pattern.setPattern(*pattern);
                            Ui::qStringOut(xml.attributes().value("regexp").toString());
                            patternRead=true;
                        }
                        if(xml.name().toString()=="format" && !formatRead)
                        {
                            rule->format.setFont(stringToFont(xml.attributes().value("fontStyle").toString()));
                            rule->format.setFontWeight(xml.attributes().value("weight").toString().toInt());
                            rule->format.setForeground(QColor::fromString(xml.attributes().value("foreground")));
                            //rule.format.setFont(QFont::)
                            Ui::qStringOut(xml.attributes().value("weight").toString());
                            Ui::qStringOut(xml.attributes().value("foreground").toString());
                            Ui::qStringOut(xml.attributes().value("fontStyle").toString());
                            formatRead=true;
                        }
                        xml.readNext();
                    }
                    ruleSet.push_back(*rule);
                    std::cout<<"debug rule: "<<std::endl;
                    rule->debugRule();
                    //Ui::qStringOut(multiLineCommentRuleStart.pattern.pattern());
                }
            }
        }
    }

    QFont CppHighlighter::stringToFont(const QString& config)
    {
        QStringList params=config.split(u',');
        QFont font;
        font.setFamily(params[0]);
        font.setPointSize(params[1].toInt());
        font.setPixelSize(params[2].toInt());
        font.setStyleHint(QFont::StyleHint(params[3].toInt()));
        font.setWeight(QFont::Weight(params[4].toInt()));
        font.setStyle(QFont::Style(params[5].toInt()));
        font.setUnderline(params[6].toInt());
        font.setStrikeOut(params[7].toInt());
        font.setFixedPitch(params[8].toInt());
        font.setOverline(params[9].toInt()); // "Always 0" mapped to "Overline" as there is no direct "Always 0" property
        font.setCapitalization(QFont::Capitalization(params[10].toInt()));
        font.setLetterSpacing(QFont::AbsoluteSpacing, params[11].toDouble());
        font.setWordSpacing(params[12].toInt());
        font.setStretch(params[13].toInt());
        font.setStyleStrategy(QFont::StyleStrategy(params[14].toInt()));
        font.setStyle(QFont::Style(params[15].toInt()));
        return font;
    }

    Rule* CppHighlighter::stringToRule(const QString& ruleName)
    {
        if(ruleName=="classRule") return &classRule;
        else if(ruleName=="namespaceRule") return &namespaceRule;
        else if(ruleName=="commentRule") return &commentRule;
        else if(ruleName=="multiLineCommentRuleStart") return &multiLineCommentRuleStart;
        else if(ruleName=="multiLineCommentRuleEnd") return &multiLineCommentRuleEnd;
        else if(ruleName=="functionRule") return &functionRule;
        else if(ruleName=="emptyFunctionRule") return &emptyFunctionRule;
        else if(ruleName=="constructorRule") return &constructorRule;
        else if(ruleName=="defaultConstructorRule") return &defaultConstructorRule;
        else if(ruleName=="destructorRule") return &destructorRule;
        else throw std::invalid_argument("Error. No sush rule with given ruleName.");
    }

    void CppHighlighter::highlightBlock(const QString& text)
    {
        //std::cout<<"processed line:"<<std::endl;
        //Ui::qStringOut(text);

        /*Rule commentRule;
        for(Rule& rule: ruleSet)
        {
            if(rule.pattern.pattern()==multiLineCommentRuleEnd.pattern.pattern() || rule.pattern.pattern()==multiLineCommentRuleStart.pattern.pattern())
            {
                commentRule=rule;
                    break;
            }
        }*/
        std::cout<<"highlight block:"<<std::endl;
        bool noMatches=true;
        for(Rule& rule: ruleSet)
        {
            rule.debugRule();
            bool commentEntered=false;
            bool commentExited=false;
            QRegularExpressionMatchIterator itr=rule.pattern.globalMatch(text);
            while(itr.hasNext())
            {
                noMatches=false;
                QRegularExpressionMatch match=itr.next();
                if(rule.name==multiLineCommentRuleStart.name || (previousBlockState()==1 && rule.name!=multiLineCommentRuleEnd.name))
                {
                    std::cout<<"entered"<<std::endl;
                    setCurrentBlockState(1);
                    setFormat(0, text.length(), commentRule.format);
                    commentEntered=true;
                    break;
                }
                else if(rule.name==multiLineCommentRuleEnd.name)
                {
                    std::cout<<"exited"<<std::endl;
                    setCurrentBlockState(0);
                    setFormat(0, text.length(), commentRule.format);
                    commentExited=true;
                    break;
                }
                else if(rule.pattern.pattern()==functionRule.pattern.pattern() || rule.pattern.pattern()==emptyFunctionRule.pattern.pattern())
                {
                    if(currentBlockState()==2) continue;
                    int spacePos=-1, openBracketPos=-1;
                    for(int i=match.capturedStart(); i<text.length(); i++)
                    {
                        if(text[i]==QChar::Space) spacePos=i;
                        else if(text[i]==QChar::fromLatin1('(')) openBracketPos=i;
                        if(spacePos!=-1 && openBracketPos!=-1) break;
                    }
                    setFormat(spacePos+1, openBracketPos-spacePos-1, rule.format);
                    setCurrentBlockState(2);
                    continue;
                }
                else if(rule.pattern.pattern()==constructorRule.pattern.pattern() || rule.pattern.pattern()==defaultConstructorRule.pattern.pattern() || rule.pattern.pattern()==destructorRule.pattern.pattern())
                {
                    if(currentBlockState()==2) continue;
                    else
                    {

                        int openBracketPos=-1;
                        for(int i=match.capturedStart(); i<text.length(); i++)
                        {
                            if(text[i]==QChar::fromLatin1('('))
                                {
                                    openBracketPos=i;
                                    break;
                                }
                        }
                        setFormat(match.capturedStart(), openBracketPos-1, rule.format);
                        setCurrentBlockState(2);
                        continue;
                    }
                }
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);

            }
            if(commentEntered || commentExited) break;
        }
        if(noMatches)
        {
            if(previousBlockState()==1)
            {
                setCurrentBlockState(1);
                setFormat(0, text.length(), commentRule.format);
            }
        }
        //std::cout<<"current block state: "<<currentBlockState()<<std::endl;
    }
}
