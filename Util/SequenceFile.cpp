#include "SequenceFile.h"
#include "Core/SwwAssert.h"

#include <QDir>
#include <QRegExp>

SWW_UTIL_NAMESPACE_BEGIN

SequenceFile::SequenceFile(const std::string &fileName, int offset):_nullStr(std::string())
{
    _isValid = initialize(fileName,offset);
}

bool SequenceFile::initialize(const std::string &fileName, int offset)
{
    _fileInfo = QFileInfo(fileName.c_str());

    _offset = offset;
    _sequenceFile.clear();
    _isSingle = false;

    QString baseName = _fileInfo.completeBaseName();
    QString ext = _fileInfo.suffix();//拡張子
    QString baseNameWithoutNumber(baseName);
    QString number(baseName);

    QRegExp r("\\d+$"); //末尾の数字部分を指定する正規表現　\\d→数字　+→１回以上の繰り返し　$→末尾
    if(baseName.contains(r))
    {
        baseNameWithoutNumber.remove(r);    //数字部分削除
        number.remove(baseNameWithoutNumber);   //数字部分抽出

        QString sharp;
        sharp.rightJustified(number.length(),'#');
    }else
    {
        r.setPattern("#+$");
        if(!baseName.contains(r))
        {
            _isSingle = true;
            _first = fileName;
            return true;
        }else
        {
            baseNameWithoutNumber.remove(r);    //数字部分削除
            number.remove(baseNameWithoutNumber);   //数字部分抽出
        }
    }

    //ディレクトリを呼び出す
    QDir dir(_fileInfo.dir());
    QString seachPatten;        //検索するルールを設定
    seachPatten +=  baseNameWithoutNumber + "*" + ext;      //ベースネームと拡張子が同じファイル名を検索対象とする
    QStringList strlFilter;
    strlFilter << seachPatten;
    QFileInfoList files = dir.entryInfoList(strlFilter,QDir::Files);      //連番ファイルを取得 ファイル名でソートする

    for ( int i = 0; i < files.size(); i++ ){
        //フレーム番号をintで取得
        QString frameNumber = files[i].completeBaseName();
        frameNumber.remove(baseNameWithoutNumber);   //数字部分抽出
        int frame = frameNumber.toInt();
        std::string fullPath = files[i].absoluteFilePath().toStdString();

        _sequenceFile.insert(frame,fullPath);

        //最初と最後のフレーム番号を取得 iの判定が冗長な書き方だが、めんどくさいので良しとする
        if(i == 0) {
            _startFrame = frame;
            _first = fullPath;
        }
        else if (i == files.size() - 1) _endFrame = frame;
    }

    if(files.size() == 1)_isSingle = true;

    QString seqInfo = baseName + ":range[" + QString::number(_startFrame) + "-" + QString::number(_endFrame) + "]";
    SWW_INF(seqInfo.toStdString());

    return true;
}

bool SequenceFile::getFileName(std::string &fileName, int frame, OutOfRangeMode mode)
{
    if(_sequenceFile.isEmpty())
    {
        SWW_ERR("seq file is empty.");
        return false;
    }

    //オフセットを追加
    frame += _offset;

    if(mode == kConstant)
    {
        //ファイルが一枚の場合
        if(_sequenceFile.count() == 1)
        {
            frame = _startFrame;
        }
        else
        {
            //フレーム番号が範囲外だった場合、先頭もしくは末尾のフレーム番号を割り当てる。
            if(frame < _startFrame)frame = _startFrame;
            else if(frame > _endFrame)frame = _endFrame;
        }
    }
    else if(mode == kNull)
    {
        if(frame < _startFrame || frame > _endFrame)
        {
            fileName = std::string();
            return true;
        }
    }

    fileName = _sequenceFile.value(frame);

    if(fileName.empty())
    {
        SWW_ERR("get file name failed.frame may drop out.");
        return false;
    }

    return true;
}

const std::string& SequenceFile::getFileName(int frame, OutOfRangeMode mode)
{
    if(_sequenceFile.isEmpty())
    {
        SWW_ERR("seq file is empty.");
        return _nullStr;
    }

    //オフセットを追加
    frame += _offset;

    if(mode == kConstant)
    {
        //ファイルが一枚の場合
        if(_sequenceFile.count() == 1)
        {
            frame = _startFrame;
        }
        else
        {
            //フレーム番号が範囲外だった場合、先頭もしくは末尾のフレーム番号を割り当てる。
            if(frame < _startFrame)frame = _startFrame;
            else if(frame > _endFrame)frame = _endFrame;
        }
    }
    else if(mode == kNull)
    {
        if(frame < _startFrame || frame > _endFrame)
        {
            return _nullStr;
        }
    }

    return _sequenceFile[frame];

}

const std::string& SequenceFile::getFileNameFirst() const{
    return _first;
}

std::string SequenceFile::makeSeaqenceName(const std::string &fileName)
{
    QFileInfo inf(fileName.c_str());
    QString baseName = inf.completeBaseName();

    QString pattern = "\\d+$";      //末尾の数字部分を指定する正規表現　\\d→数字　+→１回以上の繰り返し　$→末尾

    QRegExp r(pattern);
    QString baseNameWithoutNumber(baseName);
    baseNameWithoutNumber.remove(r);    //数字部分削除
    QString number(baseName);
    number.remove(baseNameWithoutNumber);   //数字部分抽出

    QString sharp;
    sharp.rightJustified(number.length(),'#');

    QString result = baseNameWithoutNumber + sharp + "." + inf.suffix();

    return result.toStdString();
}

const std::string& SequenceFile::getSingle(){
    if(!_isSingle)SWW_WARN("not single. " << _first);
    return _first;
}

bool SequenceFile::isSingle() const
{
//    if(_sequenceFile.count() == 1)return true;
//    return false;
    return _isSingle;
}

SWW_UTIL_NAMESPACE_END
