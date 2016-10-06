#include <termios.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <linux/serial.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#include <iostream>
#include <regex>

#include "cserialport.h"

CSerialPortThread *CSerialPort::_serial_THR = nullptr;

CSerialPort::CSerialPort()
{
    _fd = -1;
    if (_serial_THR == nullptr)
    {
        _serial_THR = new CSerialPortThread();
    }
}

CSerialPort::~CSerialPort()
{
    if (_fd > 0)
    {
        _serial_THR->removePort(this);
        close(_fd);
    }
}

void CSerialPort::setPortName(std::string name)
{
    _port_name = name;
}

void CSerialPort::setBaudRate(BaudRate_t rate)
{
    _baud_rate = (uint32_t)rate;
}

void CSerialPort::setBaudRate(uint32_t rate)
{
    _baud_rate = rate;
}

void CSerialPort::setDataBits(DataBits_t dBits)
{
    _dBits = dBits;
}

void CSerialPort::setDataBits(uint8_t dBits)
{
    switch(dBits)
    {
    case 5:
        setDataBits(Data5);
        //_tty_config.c_cflag |= CS5;
        break;
    case 6:
        setDataBits(Data6);
        //_tty_config.c_cflag |= CS6;
        break;
    case 7:
        setDataBits(Data7);
        //_tty_config.c_cflag |= CS7;
        break;
    case 8:
        setDataBits(Data8);
        //_tty_config.c_cflag |= CS8;
        break;
    default:
        setDataBits(Data8);
        //_tty_config.c_cflag |= CS8;
        break;
    }
}

void CSerialPort::setFlowControl(FlowControl_t fControl)
{
    _fCtrl = fControl;

}

void CSerialPort::setParity(Parity_t parity)
{
    (void)parity;

    //    speed_t
    //    switch(parity)
    //    {
    //    case NoParity:
    //        _tty_config.c_cflag
    //        break;
    //    }

}

void CSerialPort::setStopBits(StopBits_t sBits)
{
    _sBits = sBits;
}

std::string CSerialPort::portName()
{
    return _port_name;
}

bool CSerialPort::openPort(OpenMode_t mode)
{
    termios tty_config;
    // if (_configured_ports_num >= MAX_CONFIGURED_PORTS-1) return false;
    if (_fd > 0) return false;
    if (_port_name.empty()) return false;

    _open_mode = mode;

    switch(mode)
    {
    case OM_READ_ONLY:
        _fd = open(_port_name.c_str(), O_RDONLY | O_NONBLOCK);
        break;
    case OM_READ_WRITE:
        _fd = open(_port_name.c_str(), O_RDWR | O_NONBLOCK);
        break;
    case OM_WRITE_ONLY:
        _fd = open(_port_name.c_str(), O_WRONLY | O_NONBLOCK);
        break;
    }
    if (_fd == (-1)) return false;

    _open_mode = mode;
    memset(&tty_config, 0, sizeof(tty_config));
    tcgetattr(_fd, &tty_config);
    cfsetspeed(&tty_config, _baud_rate);
    if (_sBits == TwoStop)
    {
        tty_config.c_cflag |= CSTOPB;
    }
    tty_config.c_cflag &= ~CSIZE;

    switch(_dBits)
    {
    case Data5:
        tty_config.c_cflag |= CS5;
        break;
    case Data6:
        tty_config.c_cflag |= CS6;
        break;
    case Data7:
        tty_config.c_cflag |= CS7;
        break;
    case Data8:
        tty_config.c_cflag |= CS8;
        break;
    }

    switch(_fCtrl)
    {
    case NoFlowControl:
        tty_config.c_cflag &= ~CRTSCTS;
        break;
    case HardwareFlowControl:
        tty_config.c_cflag |= CRTSCTS;
    default:
        break;
    }
    cfmakeraw(&tty_config);
    tcflush(_fd, TCIOFLUSH);
    tcsetattr(_fd, TCSANOW, &tty_config);


    if (_serial_THR->addPort(this))
    {
        return true;
    }
    else
    {
        close(_fd);
        return false;
    }
    return true;
}

bool CSerialPort::isOpen()
{
    return (_fd>0);
}

bool CSerialPort::isWritable()
{
    return ((_fd > 0) && ((_open_mode == OM_WRITE_ONLY)
                         || (_open_mode == OM_READ_WRITE)));

}

bool CSerialPort::closePort()
{    
    if (_serial_THR->removePort(this))
    {
        close(_fd);
        _fd = 0;
        return true;
    }
    return false;
}

bool CSerialPort::serialWrite(const vector<char>buffer)
{
    return serialWrite(buffer.data(), buffer.size());
}

bool CSerialPort::serialWrite(const char *buffer, size_t size)
{
    if (_fd > 0)
    {
        if (write(_fd, buffer, size))
        {
            return true;
        }
    }
    return false;
}

void CSerialPort::setWorkingMode(WorkingMode_t mode)
{
    if (_serial_THR == nullptr)
    {
        _serial_THR = new CSerialPortThread();
    }
    switch(mode)
    {
    case WM_THREAD:
        _serial_THR->readInThread(true);
        break;
    case WM_GLOBAL:
        _serial_THR->readInThread(false);
        break;
    }
}

void CSerialPort::setPriority(Priority_t pri)
{
    if (_serial_THR == nullptr)
    {
        _serial_THR = new CSerialPortThread();
    }

    _serial_THR->setPriority(pri);
}

void CSerialPort::serialData(vector<char> data)
{
    for (ReadDataRecipient_t val : listeners)
    {
        val(data);
    }
}

void CSerialPort::serialRead()
{
    size_t size = 0;

    size = read(_fd, &_buffer, MAX_BUFF_LEN);

    if (size > 0)
    {
        if (_buffer_vector.empty())
        {
            _buffer_vector.insert(_buffer_vector.begin(), _buffer, _buffer+size);
        }
        else
        {
            _buffer_vector.insert(_buffer_vector.begin(), _buffer, _buffer+size);
        }

        for (ReadDataRecipient_t val : listeners)
        {
            val(_buffer_vector);
        }
        _buffer_vector.clear();
    }
}


int CSerialPort::getFD()
{
    return _fd;
}

bool CSerialPort::isSet(ReadDataRecipient_t _testListener)
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

size_t CSerialPort::getConfigPortsNum()
{
    if (_serial_THR == nullptr)
    {
        _serial_THR = new CSerialPortThread();
    }
    return _serial_THR->getConfiguredPortsNum();
}

static std::string get_driver(const std::string& tty) {
    struct stat st;
    std::string devicedir = tty;

    // Append '/device' to the tty-path
    devicedir += "/device";

    // Stat the devicedir and handle it if it is a symlink
    if (lstat(devicedir.c_str(), &st)==0 && S_ISLNK(st.st_mode)) {
        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        // Append '/driver' and return basename of the target
        devicedir += "/driver";

        if (readlink(devicedir.c_str(), buffer, sizeof(buffer)) > 0)
            return basename(buffer);
    }
    return "";
}

static void register_comport( list<std::string>& comList, list<std::string>& comList8250, const std::string& dir) {
    // Get the driver the device is using
    std::string driver = get_driver(dir);

    // Skip devices without a driver
    if (driver.size() > 0) {
        std::string devfile = std::string("/dev/") + basename(dir.c_str());

        // Put serial8250-devices in a seperate list
        if (driver == "serial8250") {
            comList8250.push_back(devfile);
        } else
            comList.push_back(devfile);
    }
}

static void probe_serial8250_comports(list<std::string>& comList, list<std::string> comList8250) {
    struct serial_struct serinfo;
    list<std::string>::iterator it = comList8250.begin();

    // Iterate over all serial8250-devices
    while (it != comList8250.end()) {

        // Try to open the device
        int fd = open((*it).c_str(), O_RDWR | O_NONBLOCK | O_NOCTTY);

        if (fd >= 0) {
            // Get serial_info
            if (ioctl(fd, TIOCGSERIAL, &serinfo)==0) {
                // If device type is no PORT_UNKNOWN we accept the port
                if (serinfo.type != PORT_UNKNOWN)
                    comList.push_back(*it);
            }
            close(fd);
        }
        it ++;
    }
}

static void register_tnt(list<std::string> &comList, const char *dir)
{
    if (std::regex_match(dir, std::regex("(tnt).(.*)")))
    {
        std::string dev = "/dev/" + std::string(dir);
        comList.push_back(dev);
    }
}

list<std::string> CSerialPort::availablePorts(bool withTNT)
{
    int n;
    struct dirent **namelist;
    list<std::string> comList, comList8250;
    const char *sysdir = "/sys/class/tty/";

    n = scandir(sysdir, &namelist, NULL, NULL);
    if (n > 0)
    {
        while(n--)
        {
            if (strcmp(namelist[n]->d_name, "..") && strcmp(namelist[n]->d_name, "."))
            {
                std::string devicedir = sysdir;
                devicedir += namelist[n]->d_name;
                register_comport(comList, comList8250, devicedir);
                if (withTNT) register_tnt(comList, namelist[n]->d_name);
            }
            free(namelist[n]);
        }
        free(namelist);
    }
    probe_serial8250_comports(comList, comList8250);

    return comList;
}

////////////////////////CSerialPortThread/////////////////////////////////////

void *thread(void *arg)
{
    char buffer[MAX_BUFF_LEN];
    vector<char>data;
    size_t size = 0;
    CSerialPortThread::ThreadData_t *p_thr_data = (CSerialPortThread::ThreadData_t *)arg;
    struct epoll_event _epoll_events[EPOLL_EVENTS_NUM];
    int n, fd_epoll;
    pthread_mutex_lock(p_thr_data->mtx);
    fd_epoll = p_thr_data->object->_fd_epoll;
    pthread_mutex_unlock(p_thr_data->mtx);
    while(1)
    {
        n = epoll_wait(fd_epoll, _epoll_events, EPOLL_EVENTS_NUM, -1);
        for (int i = 0; i < n; i++)
        {
            if ((_epoll_events[i].events & EPOLLERR) ||
                    (_epoll_events[i].events & EPOLLHUP) ||
                    (!(_epoll_events[i].events & EPOLLIN)))
            {
                continue;
            }

            if (p_thr_data->read_in_thread)
            {
                size = read(_epoll_events[i].data.fd, buffer, MAX_BUFF_LEN);
                if (size > 0)
                {
                    data.clear();
                    data.insert(data.begin(), buffer, buffer+size);

                    pthread_mutex_lock(p_thr_data->mtx);
                    if (p_thr_data->object->_config_ports.find(_epoll_events[i].data.fd) != p_thr_data->object->_config_ports.end())
                    {
                        p_thr_data->object->_config_ports[_epoll_events[i].data.fd]->serialData(data);
                    }
                    pthread_mutex_unlock(p_thr_data->mtx);
                }

            }
            else
            {
                pthread_mutex_lock(p_thr_data->mtx);
                if (p_thr_data->object->_config_ports.find(_epoll_events[i].data.fd) != p_thr_data->object->_config_ports.end())
                {
                    p_thr_data->object->_config_ports[_epoll_events[i].data.fd]->serialRead();
                }
                pthread_mutex_unlock(p_thr_data->mtx);
            }
        }
    }
    return NULL;
}


CSerialPortThread::CSerialPortThread()
{
    _config_ports.clear();
    _serial_thread_data.alive = false;
    _serial_thread_data.mtx = &_mtx;
    _serial_thread_data.object = this;
    if (THR == nullptr)
    {
        THR = new ThreadObject<ThreadData_t> (
                    &thread,
                    true,
                    &_serial_thread_data,
                    "SERIAL",
                    SCHED_FIFO,
                    90
                    );
    }

    if ((_fd_epoll = epoll_create1(0)) == -1)
    {
        std::cout << "CSerialPortThread()\tepoll_create1" << endl;
        flush(std::cout);
        exit(1);
    }
}

CSerialPortThread::~CSerialPortThread()
{
    if (THR->isThreadAlive())
    {
        THR->stopThread();
    }
    delete THR;
    _config_ports.clear();
}

bool CSerialPortThread::addPort(CSerialPort *port)
{
    map<int, CSerialPort*>::iterator it;
    struct epoll_event event;

    pthread_mutex_lock(_serial_thread_data.mtx);
    it = _config_ports.find(port->getFD());
    if (it != _config_ports.end())
    {
        pthread_mutex_unlock(_serial_thread_data.mtx);
        std::cout << "CSerialPortThread::addPort()\tPort exists" << endl;
        flush(std::cout);
        return false;
    }
    pthread_mutex_unlock(_serial_thread_data.mtx);

    event.data.fd = port->getFD();
    event.events = EPOLLIN | EPOLLET;
    if (epoll_ctl(_fd_epoll, EPOLL_CTL_ADD, port->getFD(), &event))
    {
        std::cout << "CSerialPortThread::addPort()\tepoll_ctl()" << endl;
        return false;
    }
    pthread_mutex_lock(_serial_thread_data.mtx);
    _config_ports[port->getFD()] = port;
    pthread_mutex_unlock(_serial_thread_data.mtx);

    if (!THR->isThreadAlive())
    {
        //std::cout << "Start thread" << endl;
        //flush(std::cout);
        THR->startThread();
    }

    return true;
}

bool CSerialPortThread::removePort(CSerialPort *port)
{
    map<int, CSerialPort*>::iterator it;
    struct epoll_event event;
    pthread_mutex_lock(_serial_thread_data.mtx);
    it = _config_ports.find(port->getFD());
    if (it == _config_ports.end())
    {
        pthread_mutex_unlock(_serial_thread_data.mtx);
        std::cout << "CSerialPortThread::removePort()\tPort doesn't exists" << endl;
        flush(std::cout);
        return false;
    }

    if (epoll_ctl(_fd_epoll, EPOLL_CTL_DEL, port->getFD(), &event))
    {
        pthread_mutex_unlock(_serial_thread_data.mtx);
        std::cout << "CSerialPortThread::removePort()\tepoll_ctl()" << endl;
        flush(std::cout);
        return false;
    }
    _config_ports.erase(port->getFD());

    if (_config_ports.size() == 0 && THR->isThreadAlive())
    {
        THR->stopThread();
    }
    pthread_mutex_unlock(_serial_thread_data.mtx);

    return true;
}

size_t CSerialPortThread::getConfiguredPortsNum()
{
    size_t num;
    pthread_mutex_lock(_serial_thread_data.mtx);
    num = _config_ports.size();
    pthread_mutex_unlock(_serial_thread_data.mtx);
    return num;
}

void CSerialPortThread::readInThread(bool state)
{
    _serial_thread_data.read_in_thread = state;
}

void CSerialPortThread::setPriority(int pri)
{
    THR->setThreadPriority(pri);
}
