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

        //comments
        commentRule.pattern.setPattern("//.*+");
        commentRule.format.setFontItalic(true);
        commentRule.format.setForeground(Qt::darkGreen);


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

        ruleSet.push_back(classRule);
        ruleSet.push_back(namespaceRule);
        ruleSet.push_back(commentRule);
        ruleSet.push_back(multiLineCommentRuleStart);
        ruleSet.push_back(multiLineCommentRuleEnd);
        ruleSet.push_back(functionRule);
        ruleSet.push_back(emptyFunctionRule);
        ruleSet.push_back(constructorRule);
        ruleSet.push_back(defaultConstructorRule);
        ruleSet.push_back(destructorRule);
    }


    void CppHighlighter::loadFromFile(const QString& filename)
    {
        //std::cout<<"reading from: ";
        //Ui::qStringOut(filename);
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
                    Rule rule;
                    //Ui::qStringOut(xml.attributes().value("name").toString());
                    xml.readNext();
                    bool patternRead=false;
                    bool formatRead=false;
                    while(!patternRead || !formatRead)
                    {
                        //bool oldPattern=patternRead;
                        //bool oldFormat=formatRead;
                        if(xml.name().toString()=="pattern")
                        {
                            rule.pattern.setPattern(xml.attributes().value("regexp").toString());
                            //Ui::qStringOut(xml.attributes().value("regexp").toString());
                            patternRead=true;
                        }
                        if(xml.name().toString()=="format")
                        {
                            rule.format.setFontWeight(xml.attributes().value("weight"))
                            //Ui::qStringOut(xml.attributes().value("weight").toString());
                            //Ui::qStringOut(xml.attributes().value("foreground").toString());
                            //Ui::qStringOut(xml.attributes().value("fontStyle").toString());
                            formatRead=true;
                        }
                        xml.readNext();
                    }
                }
            }
        }
    }

    void CppHighlighter::highlightBlock(const QString& text)
    {
        std::cout<<"processed line:"<<std::endl;
        Ui::qStringOut(text);


        Rule commentRule;
        for(Rule& rule: ruleSet)
        {
            if(rule.pattern.pattern()==multiLineCommentRuleEnd.pattern.pattern() || rule.pattern.pattern()==multiLineCommentRuleStart.pattern.pattern())
            {
                commentRule=rule;
                    break;
            }
        }

        bool noMatches=true;
        for(Rule& rule: ruleSet)
        {
            bool commentEntered=false;
            bool commentExited=false;
            QRegularExpressionMatchIterator itr=rule.pattern.globalMatch(text);
            while(itr.hasNext())
            {
                noMatches=false;
                QRegularExpressionMatch match=itr.next();
                if(rule.pattern.pattern()==multiLineCommentRuleStart.pattern.pattern() || (previousBlockState()==1 && rule.pattern.pattern()!=multiLineCommentRuleEnd.pattern.pattern()))
                {
                    setCurrentBlockState(1);
                    setFormat(0, text.length(), commentRule.format);
                    commentEntered=true;
                    break;
                }
                else if(rule.pattern.pattern()==multiLineCommentRuleEnd.pattern.pattern())
                {
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
        std::cout<<"current block state: "<<currentBlockState()<<std::endl;
    }
}
