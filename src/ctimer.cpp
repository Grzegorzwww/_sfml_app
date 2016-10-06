#include "ctimer.h"

void timer_timeout(sigval_t arg)
{
    CTimer::TimerData_t *p_tim_data = (CTimer::TimerData_t *)arg.sival_ptr;

    if (p_tim_data->timer)
    {
        p_tim_data->timer->timeout();
        switch(p_tim_data->timer->_singleShot)
        {
        case CTimer::SST_NORMAL_RUN:
            p_tim_data->timer->stop();
            break;
        case CTimer::SST_STATIC_RUN:
            delete p_tim_data->timer;
            break;
        default:
            break;
        }
    }
}

CTimer::CTimer()
{
    _timer_data.timer = this;

    _timer = new TimerObject<TimerData_t> (
                CLOCK_REALTIME,
                &timer_timeout,
                &_timer_data,
                SCHED_FIFO,
                20
                );
    _timer->setTimerInterval(1000, 1000);
}

CTimer::~CTimer()
{
    if (_timer->isTimerStarted())
    {
        _timer->stopTimer();
    }
    delete _timer;
}

void CTimer::start(int msec)
{
    if (_timer->isTimerStarted())
    {
        _timer->stopTimer();
    }
    _timer->setTimerInterval(msec*1000, msec*1000);
    _timer->startTimer();
}

void CTimer::start()
{
    if (_timer->isTimerStarted())
    {
        _timer->stopTimer();
    }
    if (!_timer->isTimerIntervalSet())
    {
        _timer->setTimerInterval(1000, 1000);
    }
    _timer->startTimer();
}

void CTimer::stop()
{
    if (_timer->isTimerStarted())
    {
        _timer->stopTimer();
    }
}

void CTimer::setInterval(int msec)
{
    _timer->setTimerInterval(msec*1000, msec*1000);
}

void CTimer::setSingleShot(bool singleShot)
{
    if (singleShot)
    {
        _singleShot = SST_STATIC_RUN;
    }
    else
    {
        _singleShot = SST_NONE;
    }
}

bool CTimer::isActive() const
{
    return _timer->isTimerStarted();
}

int CTimer::interval() const
{
    return _timer->getTimerInterval()/1000;
}

bool CTimer::isSingleShot() const
{
    return (_singleShot != SST_NONE);
}


bool CTimer::isSet(TimeoutRecipient_t _testListener)
{
    for (unsigned int i = 0; i < listeners.size(); i++)
    {
        if (listeners.at(i) == _testListener)
        {
            return true;
        }
    }
    return false;
}

void CTimer::timeout()
{
    for (TimeoutRecipient_t val : listeners)
    {
        val();
    }
    //std::for_each(listeners.begin(), listeners.end(),[](TimeoutRecipient_t val){val();});
}
