/*
 * TimerObject.h
 *
 *  Created on: 26 sty 2015
 *      Author: l_jez
 */

#ifndef SRC_TIMEROBJECT_H_
#define SRC_TIMEROBJECT_H_

#include <iostream>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include <csignal>
#include <time.h>
#include <cerrno>
#include <sys/types.h>
#include <pthread.h>

//#include <pthread.h>
//typedef unsigned long uint64_t;
using namespace std;

typedef struct {
		pthread_mutex_t mutex;
		pthread_mutexattr_t attributes;
} timerMutex_t;

template< class T >
class TimerObject {
	public:
		TimerObject< T >(
				clockid_t clockid,
				void (*timer_handler)( sigval_t arg ),
				T* timer_handler_data,
				int thread_sched_policy,
				int thread_priority );

		virtual ~TimerObject();
	private:
		timer_t _timer_id;
		sigevent _timer_sigev;

	public:
		bool startTimer();
		bool stopTimer();
		bool isTimerStarted();
	private:
		bool _timer_started;

	public:
		bool setTimerInterval( uint64_t timer_interval_usecs, uint64_t timer_delay_usecs );
		uint64_t getTimerInterval();
		bool isTimerIntervalSet();
	private:
		bool _timer_interval_set;
		itimerspec _timer_interval;

	public:
		T* getTimerDataPtr();
		T getTimerData();
		void setTimerData( T* data );
	private:
		T* _timer_data;
		timerMutex_t _timer_data_mtx;
		pthread_attr_t timer_handler_attributes;
		sched_param timer_scheduling_parameter;
};

template< class T >
TimerObject< T >::TimerObject(
		clockid_t clockid,
		void (*timer_handler)( sigval_t arg ),
		T* timer_handler_data,
		int timer_handler_sched_policy,
		int timer_handler_priority  ) {
	_timer_id = NULL;
	_timer_started = false;
	_timer_interval_set = false;
	_timer_data = timer_handler_data;

	_timer_sigev.sigev_notify = SIGEV_THREAD;
	_timer_sigev.sigev_notify_function = timer_handler;
	timer_scheduling_parameter.__sched_priority = timer_handler_priority;
	pthread_attr_init( &timer_handler_attributes );
	pthread_attr_setschedparam( &timer_handler_attributes, &timer_scheduling_parameter );
	pthread_attr_setschedpolicy( &timer_handler_attributes, timer_handler_sched_policy );
	_timer_sigev.sigev_notify_attributes = &timer_handler_attributes;

	_timer_sigev.sigev_value.sival_ptr = ( void* ) _timer_data;

	pthread_mutexattr_init( &_timer_data_mtx.attributes );
	pthread_mutexattr_setpshared(
	        &_timer_data_mtx.attributes,
	        PTHREAD_PROCESS_SHARED );
	pthread_mutexattr_settype(
	        &_timer_data_mtx.attributes,
	        PTHREAD_MUTEX_NORMAL );
	pthread_mutex_init(
	        &_timer_data_mtx.mutex,
	        &_timer_data_mtx.attributes );

	if( 0 > timer_create( clockid, &_timer_sigev, &_timer_id ) ){
		_timer_id = NULL;
		cout << "[ TIMER ]:\tTimerObject() timer_create(): " << strerror( errno ) << endl;
		flush( cout );
	}
}

template< class T >
TimerObject< T >::~TimerObject() {

	stopTimer();

	if( _timer_id != NULL )
		timer_delete( _timer_id );
}
template< class T >
bool TimerObject< T >::startTimer() {
	if( ( !isTimerStarted() ) ){
		if( isTimerIntervalSet() ){
			_timer_started = true;
			if( 0 > timer_settime( _timer_id, 0, &_timer_interval, NULL ) ){
				cout << "[ TIMER ]:\tstartTimer() timer_settime(): " << strerror( errno ) << endl;
				flush( cout );
				_timer_started = false;
			}
		}
	}
	return _timer_started;
}
template< class T >
bool TimerObject< T >::stopTimer() {
	itimerspec timer_stop;
	timer_stop.it_value.tv_sec = 0;
	timer_stop.it_value.tv_nsec = 0;
	timer_stop.it_interval.tv_sec = 0;
	timer_stop.it_interval.tv_nsec = 0;

	if( isTimerStarted() ){
		if( 0 > timer_settime( _timer_id, 0, &timer_stop, NULL ) ){
			cout << "[ TIMER ]:\tstopTimer() timer_settime(): " << strerror( errno ) << endl;
			flush( cout );
		}
		_timer_started = false;
	}
	return !_timer_started;
}
template< class T >
bool TimerObject< T >::isTimerStarted() {
	return _timer_started;
}

template< class T >
bool TimerObject< T >::setTimerInterval( uint64_t timer_interval_usecs, uint64_t timer_delay_usecs ) {
	_timer_interval.it_value.tv_sec = timer_delay_usecs / 1000000UL;
	_timer_interval.it_value.tv_nsec = ( timer_delay_usecs - _timer_interval.it_value.tv_sec * 1000000UL ) * 1000;

	_timer_interval.it_interval.tv_sec = timer_interval_usecs / 1000000UL;
	_timer_interval.it_interval.tv_nsec = ( timer_interval_usecs - _timer_interval.it_interval.tv_sec * 1000000UL ) * 1000;

	return ( _timer_interval_set = true );
}
template< class T >
uint64_t TimerObject< T >::getTimerInterval() {
	uint64_t usecs;
	itimerspec timer_interval;
	if( isTimerStarted() ){
		if( 0 > timer_gettime( _timer_id, &timer_interval ) ){
			cout << "[ TIMER ]:\tgetTimerInterval() timer_gettime(): " << strerror( errno ) << endl;
			flush( cout );
			return -1;
		}
		usecs = (timer_interval.it_interval.tv_sec * 1000000000UL);
		usecs += timer_interval.it_interval.tv_nsec;
		return ( usecs / 1000UL );
	}
	else if( isTimerIntervalSet() ){
		usecs = (_timer_interval.it_interval.tv_sec * 1000000000UL);
		usecs += _timer_interval.it_interval.tv_nsec;
		return ( usecs / 1000UL );
	}
	else{
		cout << "[ TIMER ]:\tgetTimerInterval(): interval not set." << endl;
		flush( cout );
		return -1;
	}
}
template< class T >
bool TimerObject< T >::isTimerIntervalSet() {
	return _timer_interval_set;
}

template< class T >
T* TimerObject< T >::getTimerDataPtr() {
	return _timer_data;
}
template< class T >
T TimerObject< T >::getTimerData() {
	pthread_mutex_lock( &_timer_data_mtx.mutex );
	T data = *_timer_data;
	pthread_mutex_unlock( &_timer_data_mtx.mutex );
	return data;
}
template< class T >
void TimerObject< T >::setTimerData( T* data ) {
	pthread_mutex_lock( &_timer_data_mtx.mutex );
	_timer_data = data;
	pthread_mutex_unlock( &_timer_data_mtx.mutex );
}

#endif /* SRC_TIMEROBJECT_H_ */
