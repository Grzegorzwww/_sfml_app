#ifndef CSERIALPORT_H
#define CSERIALPORT_H

#include <map>
#include <vector>
#include <list>

#include "ThreadObject.h"
#include "cdelegate.h"

#define EPOLL_EVENTS_NUM 30
#define MAX_BUFF_LEN 256

class CSerialPortThread;

class CSerialPort
{
    typedef CDelegate<void, std::vector<char>> ReadDataRecipient_t;
public:

    friend void *thread(void *arg);
    friend class CSerialPortThread;
    typedef enum
    {
        Baud1200 = 1200,
        Baud2400 = 2400,
        Baud4800 = 4800,
        Baud9600 = 9600,
        Baud19200 = 19200,
        Baud38400 = 38400,
        Baud57600 = 57600,
        Baud115200 = 115200
    } BaudRate_t;

    typedef enum
    {
        Data5 = 5,
        Data6 = 6,
        Data7 = 7,
        Data8 = 8
    } DataBits_t;

    typedef enum
    {
        OneStop,
        TwoStop
    } StopBits_t;

    typedef enum
    {
        NoFlowControl = 0,
        HardwareFlowControl = 1,
        SoftwareFlowControl = 2
    } FlowControl_t;

    typedef enum
    {
        OM_READ_ONLY,
        OM_READ_WRITE,
        OM_WRITE_ONLY
    } OpenMode_t;

    typedef enum
    {
        NoParity
    } Parity_t;

    typedef enum
    {
        WM_THREAD,
        WM_GLOBAL
    } WorkingMode_t;

    typedef enum
    {
        P_HIGH = 10,
        P_NORMAL = 20,
        P_LOW = 30
    } Priority_t;

    CSerialPort();
    ~CSerialPort();

    static size_t getConfigPortsNum();

    void setPortName(std::string name);
    void setBaudRate(BaudRate_t rate);
    void setBaudRate(uint32_t rate);
    void setDataBits(DataBits_t dBits);
    void setDataBits(uint8_t dBits);
    void setFlowControl(FlowControl_t fControl);
    void setParity(Parity_t parity);
    void setStopBits(StopBits_t sBits);

    std::string portName();

    bool openPort(OpenMode_t mode = OM_READ_WRITE);
    bool closePort();

    bool isOpen();
    bool isWritable();

    bool serialWrite(const char *buffer, size_t size);
    bool serialWrite(const vector<char>buffer);


    static void setWorkingMode(WorkingMode_t mode);
    static void setPriority(Priority_t pri);

    static list<std::string> availablePorts(bool withTNT = false);

    template <void (*Function)(vector<char>)>
    void connect()
    {
        ReadDataRecipient_t newListener;
        newListener.Bind<Function>();
        if (!isSet(newListener)) listeners.push_back(newListener);
    }

    template<void (*Function)(vector<char>)>
    void disconnect()
    {
        ReadDataRecipient_t testListener;
        testListener.Bind<Function>();
        for (int i = 0; i < listeners.size(); i++)
        {
            if (listeners.at(i) == testListener)
            {
                listeners.erase(listeners.begin()+i);
            }
        }
    }

    template<class C, void (C::*Function)(vector<char>)>
    void connect(C *instance)
    {
        ReadDataRecipient_t newListener;
        newListener.Bind<C, Function>(instance);
        if (!isSet(newListener)) listeners.push_back(newListener);
    }

    template<class C, void (C::*Function)(vector<char>)>
    void disconnect(C *instance)
    {
        ReadDataRecipient_t testListener;
        testListener.Bind<C, Function>(instance);
        for (int i = 0; i < listeners.size(); i++)
        {
            if (listeners.at(i) == testListener)
            {
                listeners.erase(listeners.begin()+i);
            }
        }
    }
private:
    static CSerialPortThread *_serial_THR;
    std::vector<ReadDataRecipient_t> listeners;
    OpenMode_t _open_mode = OM_READ_WRITE;
    std::string _port_name = "";
    uint32_t _baud_rate = 115200;
    DataBits_t _dBits = Data8;
    FlowControl_t _fCtrl = NoFlowControl;
    StopBits_t _sBits = OneStop;
    std::vector<char>_buffer_vector;
    int _fd;
    char _buffer[MAX_BUFF_LEN];

    void serialRead();
    void serialData(vector<char> data);
    int getFD();
    bool isSet(ReadDataRecipient_t _testListener);
};

class CSerialPortThread
{
public:
    typedef struct
    {
        bool alive;
        bool read_in_thread;
        pthread_mutex_t *mtx;
        CSerialPortThread *object;
    } ThreadData_t;
public:
    friend void *thread(void *args);
    CSerialPortThread();
    ~CSerialPortThread();
    bool addPort(CSerialPort *port);
    bool removePort(CSerialPort *port);
    size_t getConfiguredPortsNum();
    void readInThread(bool state);
    void setPriority(int pri);

private:
    std::map<int, CSerialPort*> _config_ports;
    ThreadObject<ThreadData_t> *THR = nullptr;
    ThreadData_t _serial_thread_data;
    pthread_mutex_t _mtx;
    int _fd_epoll;
};


#endif // CSERIALPORT_H
