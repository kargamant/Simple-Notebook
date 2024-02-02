#ifndef CPPHIGHLIGHTER_H
#define CPPHIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QString>

namespace Syntax
{
    struct Rule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
        Rule() : pattern(), format() {}
    };

    class CppHighlighter: public QSyntaxHighlighter
    {
    private:
        //default rules
        Rule classRule;
        Rule namespaceRule;
        Rule commentRule;
        Rule multiLineCommentRuleStart;
        Rule multiLineCommentRuleEnd;
        Rule functionRule;
        Rule emptyFunctionRule;
        Rule constructorRule;
        Rule defaultConstructorRule;
        Rule destructorRule;

        std::vector<Rule> ruleSet;
        void defaultRuleSet();
    public:
        void loadFromFile(const QString& filename);
        //filename - xml settings file to load rule set from
        CppHighlighter(QObject* obj, const QString& filename="") : QSyntaxHighlighter(obj)
        {
            if(filename=="") defaultRuleSet();
            else loadFromFile(filename);
        }
        CppHighlighter(QTextDocument* doc, const QString& filename="") : QSyntaxHighlighter(doc)
        {
            if(filename=="") defaultRuleSet();
            else loadFromFile(filename);
        }
        void highlightBlock(const QString &text) override;
    };
}
#endif // CPPHIGHLIGHTER_H
