/*
 * ThreadObject.h
 *
 *  Created on: 17 lip 2014
 *      Author: l_jez
 */

#include <asm-generic/errno-base.h>
#include <string>

//#define PRINT_MSG
#define PRINT_ERR

#ifndef THREADOBJECT_H_
#define THREADOBJECT_H_

#include <iostream>
#include <cerrno>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <unistd.h>

using namespace std;

template<class T> class ThreadObject {

public:

    ThreadObject(
            void *(*thread_procedure)( void *arg ),
            bool thread_waitfor_join,
            T* thread_data,
            string thread_name,
            int thread_sched_policy,
            int thread_priority );
    virtual ~ThreadObject();

    bool isThreadAlive();
    bool startThread();
    bool stopThread();

    int setThreadSchedPolicy( int thread_sched_policy );
    int getThreadSchedPolicy();
    int setThreadPriority( int thread_priority );
    int getThreadPriority();

    T* getThreadData();

private:
    void initThreadName( string thread_name );
    void initThreadParams();
    void initThreadData( T* thread_data );
    void initThreadProcedure( void* (*thread_procedure)( void *arg ) );
    void initThreadAttr( bool thread_detach_state );
    void initThreadSchedPolicy( int thread_sched_policy );
    void initThreadPriority( int thread_sched_policy );

    void printMsg( string message_content );
    void printErr( string error_content );
    void printErr( string error_content, int error_no );

    bool _thread_alive;
    bool _thread_joinable;
    pthread_t _thread_id;
    pthread_attr_t _thread_attr;
    int _thread_detach_state;
    string _thread_name;
    int _thread_return_value;
    int _thread_sched_policy;
    struct sched_param _thread_sched_params;
    int _thread_priority;
    //		SCHED_OTHER		-20		99
    //		SCHED_FIFO		  1		99
    //		SCHED_RR		  1		99
    T *_p_thread_data;
    void* (*_p_thread_procedure)( void *arg );

    int result;
};

template<class T> ThreadObject< T >::ThreadObject(
        void *(*thread_procedure)( void *arg ),
        bool thread_waitfor_join,
        T* thread_data,
        string thread_name,
        int thread_sched_policy,
        int thread_priority ) {

    printMsg( "ThreadObject()" );
    initThreadParams();
    initThreadName( thread_name );
    initThreadProcedure( thread_procedure );
    initThreadData( thread_data );
    initThreadAttr( thread_waitfor_join );
    initThreadPriority( thread_priority );
    initThreadSchedPolicy( thread_sched_policy );
}

template<class T> ThreadObject< T >::~ThreadObject() {
    printMsg( "~ThreadObject()" );
    if ( _thread_alive )
        stopThread();
    pthread_attr_destroy( &_thread_attr );
    _thread_name.clear();
    //delete _p_thread_data;
}

template<class T> bool ThreadObject< T >::isThreadAlive() {
    printMsg( "isThreadAlive()" );
    return _thread_alive;
}

template<class T> bool ThreadObject< T >::startThread() {
    printMsg( "startThread()" );
    if ( !_thread_alive ) {
        _thread_alive = true;
        result = pthread_create( &_thread_id, &_thread_attr, _p_thread_procedure, _p_thread_data );
        if ( 0 != result ) {
            printErr( "startThread():\tpthread_create()", result );
            _thread_alive = false;
            return _thread_alive;
        }
        result = pthread_setname_np( _thread_id, _thread_name.c_str() );
        if ( 0 != result )
            printErr( "startThread():\tpthread_setname_np()", result );
    }
    return _thread_alive;
}

template<class T> bool ThreadObject< T >::stopThread() {
    printMsg( "stopThread()" );
    if ( _thread_alive ) {
        if ( _thread_joinable ) {
            printMsg( "stopThread()\tpthread_join() - waiting for thread to finish" );
            result = pthread_join( _thread_id, NULL );
            if ( 0 != result )
                printErr( "stopThread():\tpthread_join()", result );
        }
        else {
            printMsg( "stopThread()\tpthread_cancel() - detaching thread" );
            result = pthread_cancel( _thread_id );
            if ( 0 != result )
                printErr( "stopThread():\tpthread_cancel()", result );
        }
        _thread_alive = false;
        _thread_return_value = 0;
        printMsg( "stopThread()\tthread stopped" );
    }
    return !_thread_alive;
}

template<class T> int ThreadObject< T >::setThreadSchedPolicy( int thread_sched_policy ) {
    result = pthread_setschedparam( _thread_id, thread_sched_policy, &_thread_sched_params );
    if ( 0 != result ) {
        printErr( "setThreadSchedPolicy():\tpthread_setschedparam()", result );
        return -1;
    }
    return ( _thread_sched_policy = thread_sched_policy );
}

template<class T> int ThreadObject< T >::getThreadSchedPolicy() {
    result = pthread_getschedparam( _thread_id, &_thread_sched_policy, &_thread_sched_params );
    if ( 0 != result ) {
        printErr( "getThreadSchedPolicy():\tpthread_getschedparam()", result );
        return -1;
    }
    return _thread_sched_policy;
}

template<class T> int ThreadObject< T >::setThreadPriority( int thread_priority ) {
    int priority_max = sched_get_priority_max( _thread_sched_policy );
    int priority_min = sched_get_priority_min( _thread_sched_policy );
    if ( thread_priority > priority_max )
        thread_priority = priority_max;
    if ( thread_priority < priority_min )
        thread_priority = priority_min;

    _thread_sched_params.__sched_priority = thread_priority;
    result = pthread_setschedparam( _thread_id, _thread_sched_policy, &_thread_sched_params );
    if ( 0 != result ) {
        printErr( "setThreadPriority():\tpthread_setschedparam()", result );
        return -1;
    }
    return ( _thread_priority = thread_priority );
}

template<class T> int ThreadObject< T >::getThreadPriority() {
    result = pthread_getschedparam( _thread_id, &_thread_sched_policy, &_thread_sched_params );
    if ( 0 != result ) {
        printErr( "setThreadPriority():\tgetThreadPriority()", result );
        return -1;
    }
    return ( _thread_priority = _thread_sched_params.__sched_priority );
}

template<class T> T* ThreadObject< T >::getThreadData() {
    printMsg( "getThreadData()" );
    return _p_thread_data;
}

template<class T> void ThreadObject< T >::initThreadName( string thread_name ) {
    printMsg( "initThreadName()" );
    _thread_name.clear();
    _thread_name.append( thread_name );
}

template<class T> void ThreadObject< T >::initThreadParams() {
    printMsg( "initThreadParams()" );

    result = 0;
    _thread_alive = false;
    _thread_joinable = false;
    _thread_id = -1;
    _thread_detach_state = 0;
    _thread_return_value = 0;
    _thread_sched_policy = SCHED_OTHER;
    _thread_priority = 0;
    _thread_name.clear();
    _p_thread_data = NULL;
    _p_thread_procedure = NULL;

}

template<class T> void ThreadObject< T >::initThreadData( T* thread_data ) {
    printMsg( "initThreadData()" );
    if ( !_thread_alive ) {
        if ( _p_thread_data != NULL )
            _p_thread_data = NULL;
        _p_thread_data = thread_data;
    }
}

template<class T> void ThreadObject< T >::initThreadProcedure( void *(*thread_procedure)( void *arg ) ) {
    printMsg( "initThreadProcedure()" );
    _p_thread_procedure = thread_procedure;
}

template<class T> void ThreadObject< T >::initThreadAttr( bool thread_waitfor_join ) {
    printMsg( "initThreadAttr()" );
    _thread_joinable = thread_waitfor_join;
    _thread_detach_state = ( thread_waitfor_join ) ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED;
    result = pthread_attr_init( &_thread_attr );
    if ( 0 != result )
        printErr( "initThreadAttr()\tpthread_attr_init()", result );
    result = pthread_attr_setdetachstate( &_thread_attr, _thread_detach_state );
    if ( 0 != result )
        printErr( "initThreadAttr()\tpthread_attr_setdetachstate()", result );
}

template<class T> void ThreadObject< T >::initThreadSchedPolicy( int thread_sched_policy ) {
    _thread_sched_policy = thread_sched_policy;
}

template<class T> void ThreadObject< T >::initThreadPriority( int thread_priority ) {
    _thread_priority = thread_priority;
}

template<class T> void ThreadObject< T >::printMsg( string message_content ) {
#ifdef PRINT_MSG
    cout << "[ MSG ]:\t";
    if( !_thread_name.empty() )
        cout << _thread_name << "\t";
    cout << message_content << endl;
    flush(cout);
#endif
    (void) message_content;
}
template<class T> void ThreadObject< T >::printErr( string error_content ) {
#ifdef PRINT_ERR
    cout << "[ ERR ]:\t";
    if ( !_thread_name.empty() )
        cout << _thread_name << "\t";
    cout << error_content << endl;
    flush( cout );
#endif
}
template<class T> void ThreadObject< T >::printErr( string error_content, int error_no ) {
#ifdef PRINT_ERR
    cout << "[ ERR ]:\t";
    if ( !_thread_name.empty() )
        cout << _thread_name << "\t";
    cout << error_content << "\t";
    cout << strerror( error_no ) << endl;
    flush( cout );
#endif
}

#endif /* THREADOBJECT_H_ */
