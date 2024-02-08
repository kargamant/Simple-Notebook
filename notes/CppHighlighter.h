#ifndef CPPHIGHLIGHTER_H
#define CPPHIGHLIGHTER_H
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QString>
#include "mainwindow.h"

namespace Syntax
{
    //rule structure
    struct Rule
    {
        QString name;
        QRegularExpression pattern;
        QTextCharFormat format;
        Rule() : name("Rule"), pattern(), format() {}
        void debugRule(std::ostream& stream=std::cout)
        {
            Ui::qStringOut(name, stream);
            Ui::qStringOut(pattern.pattern(), stream);
            Ui::qStringOut(format.font().toString(), stream);
        }
    };

    class CppHighlighter: public QSyntaxHighlighter
    {
    private:
        //rules
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
        void loadFromFile(const QString& filename);

        //Consider that ruleName must satisfy one of member variable names associated with rules. Otherwise exception will be thrown.
        Rule* stringToRule(const QString& ruleName);
        QFont stringToFont(const QString& config);
    public:
        //filename - xml settings file to load rule set from
        CppHighlighter(QObject* obj, const QString& filename="") : QSyntaxHighlighter(obj)
        {
            if(filename=="")
            {
                defaultRuleSet();
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
            else loadFromFile(filename);
        }
        CppHighlighter(QTextDocument* doc, const QString& filename="") : QSyntaxHighlighter(doc)
        {
            if(filename=="")
            {
                defaultRuleSet();
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
            else loadFromFile(filename);
        }
        void highlightBlock(const QString &text) override;
    };
}
#endif // CPPHIGHLIGHTER_H
