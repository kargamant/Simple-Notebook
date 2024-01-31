#ifndef CPPHIGHLIGHTER_H
#define CPPHIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QRegularExpression>

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
        void loadFromFile(const std::string& filename);
        void defaultRuleSet();
    public:
        //filename - xml settings file to load rule set from
        CppHighlighter(QObject* obj, const std::string& filename="") : QSyntaxHighlighter(obj)
        {
            if(filename=="") defaultRuleSet();
            else loadFromFile(filename);
        }
        CppHighlighter(QTextDocument* doc, const std::string& filename="") : QSyntaxHighlighter(doc)
        {
            if(filename=="") defaultRuleSet();
            else loadFromFile(filename);
        }
        void highlightBlock(const QString &text) override;
    };
}
#endif // CPPHIGHLIGHTER_H
