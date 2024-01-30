#include "CppHighlighter.h"
#include <iostream>

namespace Syntax
{
    void CppHighlighter::defaultRuleSet()
    {
        //rules for keywords

        //example for class keyword
        Rule classRule;
        classRule.pattern.setPattern("class");
        classRule.format.setFontWeight(QFont::Bold);
        classRule.format.setForeground(Qt::blue);

        ruleSet.push_back(std::move(classRule));
    }
    void CppHighlighter::highlightBlock(const QString& text)
    {
        for(Rule& rule: ruleSet)
        {
            QRegularExpressionMatchIterator itr=rule.pattern.globalMatch(text);
            while(itr.hasNext())
            {
                QRegularExpressionMatch match=itr.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
    }
}
