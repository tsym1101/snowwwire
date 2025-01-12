#include <QCoreApplication>
#include <QHash>

#ifndef OPTIONPARSER_H
#define OPTIONPARSER_H

class OptionParser
{
private:
    QHash<QString, QVariant> optionList;
    QString trim(const QString& string);

public:
    QString OptionListFile;
    OptionParser(int argc, char *argv[], bool& isHelp, const QString& helpString = "");

    QVariant GetValue(bool& ret, const QString& name);

    QString GetValueByString(bool& ret, const QString& name);
    bool GetValueByBool(bool& ret, const QString& name);
    int GetValueByInt(bool& ret, const QString& name);
    float GetValueByFloat(bool& ret, const QString& name);

};


#endif // OPTIONPARSER_H
