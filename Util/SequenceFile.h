#ifndef SWW_SEQUENCEFILE_H
#define SWW_SEQUENCEFILE_H

#include <QHash>
#include <QFileInfoList>
#include <QDebug>

#include "Util/UtilAPI.h"
#include <string>

SWW_UTIL_NAMESPACE_BEGIN
class SWWUTILSHARED_EXPORT SequenceFile
{
public:
    enum OutOfRangeMode{
        kConstant = 0,
        kNull = 1
    };

    SequenceFile(const std::string& fileName,int offset = 0);

    bool getFileName(std::string &fileName,int frame,OutOfRangeMode mode = kConstant);
    const std::string& getFileName(int frame,OutOfRangeMode mode = kConstant);
    const std::string& getFileNameFirst()const;

    const std::string& getSingle();

    const  QHash<int,std::string>& getAll()const{return _sequenceFile;}

    bool isEmpty()const{return _sequenceFile.isEmpty();}
    bool isSingle()const;
    static std::string makeSeaqenceName(const std::string &fileName);//aaa_####.tifみたいな形式を作る

    int startFrame()const{return _startFrame;}
    int endFrame()const{return _endFrame;}

    void setOffset(int offset){_offset = offset;}

    const std::string& operator[](int frame) {
        return _sequenceFile[frame + _offset];
    }

    int count()const{return _sequenceFile.count();}

    bool isValid()const{return _isValid;}
    bool initialize(const std::string& fileName,int offset = 0 );

private:
    QHash<int,std::string> _sequenceFile;    ///<連番ファイル名を、フレーム数をキーにして格納
    QFileInfo _fileInfo;

    std::string _first;//1枚目
    const std::string _nullStr;

    int _startFrame;
    int _endFrame;
    int _offset;

    bool _isValid;
    bool _isSingle;
};
SWW_UTIL_NAMESPACE_END

#endif // SWW_SEQUENCEFILE_H
