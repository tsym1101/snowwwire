#include "Core/PlugType.h"
#include <QFileInfo>

SWW_CORE_NAMESPACE_BEGIN

//local function
std::string makeSeaqenceName(const std::string &fileName)
{
    QFileInfo inf(fileName.c_str());
    QString baseName = inf.completeBaseName();

    QString pattern = "\\d+$";      //末尾の数字部分を指定する正規表現　\\d→数字　+→１回以上の繰り返し　$→末尾

    QRegExp r(pattern);
    QString baseNameWithoutNumber(baseName);
    baseNameWithoutNumber.remove(r);    //数字部分削除
    QString number(baseName);
    number.remove(baseNameWithoutNumber);   //数字部分抽出

    QString sharp = "";
    sharp = sharp.rightJustified(number.length(),'#');

    QString result = baseNameWithoutNumber + sharp + "." + inf.suffix();
    result = inf.absolutePath() + "/" + result;

    return result.toStdString();
}

QString FilePlugI::defaultReplace(const QString &input)
{
    std::string str = input.toStdString();
    str = makeSeaqenceName(str);

    return str.c_str();
}

StringPlug::~StringPlug(){
    //SWW_DEBUG("delete string plug:" << getName());
}

SWW_CORE_NAMESPACE_END
