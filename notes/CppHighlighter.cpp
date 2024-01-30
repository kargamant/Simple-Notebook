#include "CppHighlighter.h"
#include <iostream>
#include "mainwindow.h"

namespace Syntax
{
    void CppHighlighter::defaultRuleSet()
    {
        //rules for keywords

        //keywords
        Rule classRule;
        classRule.pattern.setPattern("class");
        classRule.format.setFontWeight(QFont::Bold);
        classRule.format.setForeground(Qt::blue);

        Rule namespaceRule;
        namespaceRule.pattern.setPattern("namespace");
        namespaceRule.format.setFontWeight(QFont::Bold);
        namespaceRule.format.setForeground(Qt::blue);

        //comments
        Rule commentRule;
        commentRule.pattern.setPattern("//.*+");
        commentRule.format.setFontItalic(true);
        commentRule.format.setForeground(Qt::darkGreen);


        Rule multiLineCommentRuleStart;
        multiLineCommentRuleStart.pattern.setPattern("\(/\\*\)");
        multiLineCommentRuleStart.format.setFontItalic(true);
        multiLineCommentRuleStart.format.setForeground(Qt::darkGreen);

        Rule multiLineCommentRuleEnd;
        multiLineCommentRuleEnd.pattern.setPattern("\(\\*/\)");
        multiLineCommentRuleEnd.format.setFontItalic(true);
        multiLineCommentRuleEnd.format.setForeground(Qt::darkGreen);

        ruleSet.push_back(std::move(classRule));
        ruleSet.push_back(std::move(namespaceRule));
        ruleSet.push_back(std::move(commentRule));
        ruleSet.push_back(std::move(multiLineCommentRuleStart));
        ruleSet.push_back(std::move(multiLineCommentRuleEnd));
    }
    void CppHighlighter::highlightBlock(const QString& text)
    {
        std::cout<<"processed line:"<<std::endl;
        Ui::qStringOut(text);


        Rule commentRule;
        for(Rule& rule: ruleSet)
        {
            if(rule.pattern.pattern()=="\(\\*/\)" || rule.pattern.pattern()=="\(/\\*\)")
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
                if(rule.pattern.pattern()=="\(/\\*\)" || (previousBlockState()==1 && rule.pattern.pattern()!="\(\\*/\)"))
                {
                    setCurrentBlockState(1);
                    setFormat(0, text.length(), commentRule.format);
                    commentEntered=true;
                    break;
                }
                else if(rule.pattern.pattern()=="\(\\*/\)")
                {
                    setCurrentBlockState(0);
                    setFormat(0, text.length(), commentRule.format);
                    commentExited=true;
                    break;
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
