#include <QDebug>

#include "OptionParser.h"

OptionParser::OptionParser(int argc, char *argv[], bool& isHelp, const QString& helpString)
{
    isHelp = false;

    for(int i = 0; i < argc; i++)
    {
        QString o(argv[i]);
        if(o.left(1) == "-")
        {
            QString value;
            QString name = trim(o);

            QString v(argv[i+1]);
            if(v.left(1) != "-")
            {
                value = trim(v);
            }
            else
            {
                value = "";
            }

            if(name == "h")
            {
                isHelp = true;
                qDebug() << helpString;
            }

            optionList[name] = value;

            bool ret(false);
            float floatValue = value.toFloat(&ret);
            if(ret == true)
            {
                optionList[name] = floatValue;
            }
            int intValue = value.toInt(&ret, 10);
            if(ret == true)
            {
                optionList[name] = intValue;
            }
            if(value.toLower() == "true")
            {
                optionList[name] = bool(true);
            }
            if(value.toLower() == "false")
            {
                optionList[name] = bool(false);
            }
            if(value == "")
            {
                optionList[name] = bool(true);
            }
        }
    }
}

QVariant OptionParser::GetValue(bool& ret, const QString& name)
{
    ret = false;
    if(optionList.contains(name))
    {
        ret = true;
        return optionList[name];
    }
    else
    {
        ret = false;
        return  QVariant(0);
    }
}

QString OptionParser::GetValueByString(bool& ret, const QString& name)
{
    ret = false;
    if(optionList.contains(name))
    {
        ret = true;
        return optionList[name].toString();
    }
    else
    {
        ret = false;
        return NULL;
    }
}

int OptionParser::GetValueByInt(bool& ret, const QString& name)
{
    ret = false;
    if(optionList.contains(name))
    {
        ret = true;
        return optionList[name].toInt(&ret);
    }
    else
    {
        ret = false;
        return NULL;
    }
}

float OptionParser::GetValueByFloat(bool& ret, const QString& name)
{
    ret = false;
    if(optionList.contains(name))
    {
        ret = true;
        return optionList[name].toFloat(&ret);
    }
    else
    {
        ret = false;
        return NULL;
    }
}

bool OptionParser::GetValueByBool(bool& ret, const QString& name)
{
    ret = false;
    if(optionList.contains(name))
    {
        ret = true;
        return optionList[name].toBool();
    }
    else
    {
        ret = false;
        return NULL;
    }
}

QString OptionParser::trim(const QString& string)
{
    QString temp;
    temp = string.trimmed();
    while(temp.left(1) == "-")
    {
        temp = temp.right(temp.length() - 1);
    }
    return temp;
}


