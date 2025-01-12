#ifndef TIMELINE_H
#define TIMELINE_H

#include <list>
#include <QtCore/QMutex>
#include <QObject>

namespace Sww::Core{

enum TimelineChangeReasonEnum
{
    eTimelineChangeReasonUserSeek = 0,
    eTimelineChangeReasonPlaybackSeek = 1,
    eTimelineChangeReasonCurveEditorSeek = 2,
    eTimelineChangeReasonDopeSheetEditorSeek = 3,
    eTimelineChangeReasonOtherSeek
};

typedef int SequenceTime;

class TimeLine : public QObject
{
    Q_OBJECT
public:

    TimeLine();

    virtual ~TimeLine()
    {
    }

    SequenceTime currentFrame() const;

//    seekFrame(time, false, 0, eTimelineChangeReasonPlaybackSeek);
    void seekFrame(SequenceTime frame,
//                   bool updateLastCaller,
//                   OutputEffectInstance* caller,
                   TimelineChangeReasonEnum reason);

    void incrementCurrentFrame();
    void decrementCurrentFrame();

public Q_SLOTS:
    void onFrameChanged(SequenceTime frame);
Q_SIGNALS:
    void frameAboutToChange();
    void frameChanged(SequenceTime, int reason);

private:

    mutable QMutex _lock; // protects the following SequenceTime members
    SequenceTime _currentFrame{15};

};
}//ns
#endif // TIMELINE_H
