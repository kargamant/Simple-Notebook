#include "CppHighlighter.h"

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
        multiLineCommentRuleStart.pattern.setPattern("^/\\*.*+");
        multiLineCommentRuleStart.format.setFontItalic(true);
        multiLineCommentRuleStart.format.setForeground(Qt::darkGreen);

        Rule multiLineCommentRuleEnd;
        multiLineCommentRuleEnd.pattern.setPattern(".*+\\*/$");
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
