#include "TimeLine.h"


namespace Sww::Core{
TimeLine::TimeLine()
{
}

SequenceTime
TimeLine::currentFrame() const
{
    QMutexLocker l(&_lock);

    return _currentFrame;
}

void
TimeLine::incrementCurrentFrame()
{
    SequenceTime frame;
    {
        QMutexLocker l(&_lock);
        ++_currentFrame;
        frame = _currentFrame;
    }
    Q_EMIT frameChanged(frame, (int)eTimelineChangeReasonPlaybackSeek);
}

void
TimeLine::decrementCurrentFrame()
{
    SequenceTime frame;
    {
        QMutexLocker l(&_lock);
        --_currentFrame;
        frame = _currentFrame;
    }
    Q_EMIT frameChanged(frame, (int)eTimelineChangeReasonPlaybackSeek);
}

void
TimeLine::seekFrame(SequenceTime frame,
                    TimelineChangeReasonEnum reason)
{
    if (reason != eTimelineChangeReasonPlaybackSeek) {
        Q_EMIT frameAboutToChange();
    }
    bool changed = false;
    {
        QMutexLocker l(&_lock);
        if (_currentFrame != frame) {
            _currentFrame = frame;
            changed = true;
        }
    }

    if (changed) {
        Q_EMIT frameChanged(frame, (int)reason);
    }
}

void
TimeLine::onFrameChanged(SequenceTime frame)
{
    Q_EMIT frameAboutToChange();
    bool changed = false;
    {
        QMutexLocker l(&_lock);
        if (_currentFrame != frame) {
            _currentFrame = frame;
            changed = true;
        }
    }

    if (changed) {
        /*This function is called in response to a signal emitted by a single timeline gui, but we also
           need to sync all the other timelines potentially existing.*/
        Q_EMIT frameChanged(frame, (int)eTimelineChangeReasonUserSeek);
    }
}

}//ns
