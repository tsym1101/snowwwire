#ifndef SWW_ROOTNODE_H
#define SWW_ROOTNODE_H

#include "Core/Node.h"
#include "Core/SwwTypeDef.h"
#include "CoreAPI.h"

SWW_CORE_NAMESPACE_BEGIN

class IntPlug;
class RootNode;

struct SWWCORESHARED_EXPORT InfoToSubThread{
    int start;
    int end;
    int current;
    RootNode* node;
    PlugBase* plug;
    bool* stopped;
};

class SWWCORESHARED_EXPORT RootNode :public Node
{
    Q_OBJECT

public:
    friend class Node;

    RootNode();
    virtual bool initialize();
    bool execute(const PlugBase* plug){return true;}

    void setGlobalFrame(int current);
    void adaptFrameRange(bool adaptToStart);
    void stepForward(){setGlobalFrame(_currentFrame + 1);}

    static bool run(QObject *reciever,InfoToSubThread info);//BGスレッドで動作する
    bool run();//メインスレッドで動作する。

    void emitframeRangeChanged(){Q_EMIT frameRangeChanged();}

    bool save(const std::string& filename);
    bool load(const std::string& filename);

    //bool saveAsXml(const std::string& filename);
    bool saveAsJson(const std::string& filename);

    //bool loadFromXml(const std::string& filename);
    bool loadFromJson(const std::string& filename);

    void clearGraph();


public Q_SLOTS:

Q_SIGNALS:
    void runFinished();
    void frameRangeChanged();

private:
    IntPlug* _framePlug;

    SWW_SIMPLE_MEMBER_READ_ONLY(int,currentFrame)
    SWW_SIMPLE_MEMBER(int,startFrame)
    SWW_SIMPLE_MEMBER(int,endFrame)
};

SWW_CORE_NAMESPACE_END

#endif // SWW_ROOTNODE_H
