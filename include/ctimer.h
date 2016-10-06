/*!
 * \file	ctimer.h
 * \date	16.03.2016
 * \author	Sebastian Iwaniec	< sebastian.iwaniec@zmt.tarnow.pl >
 * \brief	Deklaracje dla klasy CTimer
 */
#ifndef CTIMER_H
#define CTIMER_H

#include <vector>

#include "TimerObject.h"
#include "cdelegate.h"

/*!
 * \class CTimer
 * \brief Klasa do obsługi posixowych timerów
 * \par Opis:
 * Zasada działania jest bardzo podobna do QTimera, trzeba stworzyć obiekt:
 * "CTimer * nowyTimer = new CTimer();"
 *
 * Wykonać connect (do jednego timera można podłączyć wiele funckji obsługujących timeout):
 * "nowyTimer->connect<&funkcjaObslugujacaTimeout>();"  <-- dla podłączenia funckji globalnej
 * "nowyTimer->connect<Klasa, &Klasa::metodaObslugujacaTimeout>(wskaznikNaObiekt);" <-- dla podłączenia metody klasy
 *
 * Uruchomić timer:
 * "nowyTimer->start(100);" <-- uruchomienie z podaniem interwału w milisekundach
 * "nowyTimer->setInterval(100); nowyTimer->start();" <-- uruchamianie z wcześniejszym ustawieniem interwału
 *
 * Timer można zatrzymać w każdej chwili:
 * "nowyTimer->stop();
 *
 * Można usunąć wybranego odbiorcę timeouta:
 * "nowyTimer->disconnect<&funkcjaObslugujacaTimeout>();" <-- odłączanie funckji globalnej
 * "nowyTimer->disconnect<Klasa, &Klasa::metodaObslugujacaTimeout>(wskaznikNaObiekt);" <-- odłączanie metody klasy
 *
 * Timer można uruchomić jednorazowo:
 * "CTimer::singleShot<&funkcjaObslugujacaTimeout>(100);" <-- pojedyńczy strzał z dla funckji globalnej z podaniem interwału (obiekt jest usuwany po wykonaniu timeoutu)
 * "CTimer::singleShot<Klasa, &Klasa::metodaObslugujacaTimeout>(100);" <-- pojedyńczy strzał dla metody klasy z podaniem interwału (obiekt jest usuwany p[o wykonaniu timeoutu)
 * "nowyTimer->setSingleShot(true); [...connect funckji obsługi...] nowyTimer->start(100);" <-- pojedyńczy strzał (obiekt nie jest usuwany po wykonaniu timeoutu)
 */
class CTimer
{
    friend void timer_timeout(sigval_t);
    /*!
     * \struct TimerData_t
     * \brief Struktura z danymi dla timera
     * \par Opis:
     * Struktura przechowuje informacje potrzebne dla TimerObjectu
     */
    typedef struct
    {
        CTimer *timer;  /**< wskaźnik na obiekt timera */
    }TimerData_t;
    /*!
     * \enum SingleShotType_t
     * \brief Typ uruchomienia jako singleshot
     */
    typedef enum
    {
        SST_STATIC_RUN, /**< Timer singleShot uruchamiany przy pomocy funckji statycznej */
        SST_NORMAL_RUN, /**< Timer singleShot ustawiony na obiekcie przez użytkownika */
        SST_NONE        /**< Timer w trybie normal bez signleShot */
    }SingleShotType_t;

    typedef CDelegate<void> TimeoutRecipient_t;

public:
    CTimer();
    ~CTimer();
    /*!
    * \brief Uruchamianie timera z podaniem interwału
    *
    * \par Argumenty:
    * \param[in] msec Interwał w milisekundach
    */
    void start(int msec);
    /*!
    * \brief Uruchamianie timera z ustawionym wcześniej interwałem
    */
    void start();
    /*!
    * \brief Zatrzymywanie timera
    */
    void stop();
    /*!
    * \brief Ustawienie interwału czasowego timerowi
    *
    * \par Argumenty:
    * \param[in] msec Interwał w milisekundach
    */
    void setInterval(int msec);
    /*!
    * \brief Uruchamianie timera w trybie normal lub singleShot
    * \par Argumenty:
    * \param[in] singleShot wartość true ustawia timer na singleshot, false ustawia timer na tryb normal
    */
    void setSingleShot(bool singleShot);
    /*!
    * \brief Sprawdzanie czy timer jest uruchomiony
    *
    * \par Wartosci zwracane:
    * \return 1 timer jest uruchomiony
    * \return 0 timer nie jest uruchomiony
    */
    bool isActive() const;
    /*!
    * \brief Pobierz ustawiony interwał czasowy
    *
    * \par Wartosci zwracane:
    * \return interwał czasowy w milisekundach
    */
    int interval() const;
    /*!
    * \brief Sprawdzanie czy timer jest ustawiony na singleShot
    *
    * \par Wartosci zwracane:
    * \return 1 timer jest ustawiony na singleShot
    * \return 0 timer nie jest ustawiony na singleShot
    */
    bool isSingleShot() const;


    /*!
    * \brief Funckja statyczna do jednokrotnego uruchomienia timera
    *
    * \par Opis:
    * Uruchamianie timera w trybie singleShot dla metody w klasie
    * \par Parametry szablonu:
    * \tparam C Klasa z której jest metoda którą można wywołać
    * \tparam Function Wskaźnik na metodę uruchamianą w klasie
    * \par Argumenty:
    * \param[in] msec interwał czasowy w miliseknudach
    * \param[in] instance wskaźnik na obiekt dla którego am być wywołana metoda
    * \par Wartosci zwracane:
    * \return wskaźnik na obiekt timera dla tego wywołania (po wykonaniu się timeoutu ten obiekt jest usuwany automatycznie)
    */
    template<class C, void (C::*Function)()>
    static CTimer * singleShot(int msec, C *instance)
    {
        CTimer *tmp = new CTimer();
        tmp->connect<C, Function>(instance);
        tmp->_singleShot = SST_STATIC_RUN;
        tmp->start(msec);
        return tmp;
    }

    /*!
    * \brief Funckja statyczna do jednokrotnego uruchomienia timera
    *
    * \par Opis:
    * Uruchamianie timera w trybie singleShot dla funkcji globalnej
    * \par Parametry szablonu:
    * \tparam Function Wskaźnik na funckję globalną
    * \par Wartosci zwracane:
    * \return wskaźnik na obiekt timera dla tego wywołania (po wykonaniu się timeoutu ten obiekt jest usuwany automatycznie)
    */
    template<void (*Function)()>
    static CTimer * singleShot(int msec)
    {
        CTimer *tmp = new CTimer();
        tmp->connect<Function>();
        tmp->_singleShot = SST_STATIC_RUN;
        tmp->start(msec);
        return tmp;
    }
    /*!
    * \brief Metoda podłączania do timera funkcji globalnej
    *
    * \par Parametry szablonu:
    * \tparam Function Wskaźnik na funckję globalną
    */
    template <void (*Function)()>
    void connect()
    {
        TimeoutRecipient_t newListener;
        newListener.Bind<Function>();
        if (!isSet(newListener)) listeners.push_back(newListener);
    }

    /*!
    * \brief Metoda odłączenia od timera funkcji globalnej
    *
    * \par Parametry szablonu:
    * \tparam Function Wskaźnik na funckję globalną
    */
    template<void (*Function)()>
    void disconnect()
    {
        TimeoutRecipient_t testListener;
        testListener.Bind<Function>();
        for (int i = 0; i < listeners.size(); i++)
        {
            if (listeners.at(i) == testListener)
            {
                listeners.erase(listeners.begin()+i);
            }
        }
    }

    /*!
    * \brief Metoda podłączenia do timera metody innej klasy
    *
    * \par Parametry szablonu:
    * \tparam C Klasa z której jest metoda którą można wywołać
    * \tparam Function Metoda uruchamiana w klasie
    * \par Argumenty:
    * \param[in] instance wskaźnik na obiekt dla którego am być wywołana metoda
    */
    template<class C, void (C::*Function)()>
    void connect(C *instance)
    {
        TimeoutRecipient_t newListener;
        newListener.Bind<C, Function>(instance);
        if (!isSet(newListener)) listeners.push_back(newListener);
    }
    /*!
    * \brief Metoda odłączenia od timera metody innej klasy
    *
    * \par Parametry szablonu:
    * \tparam C Klasa z której jest metoda którą można wywołać
    * \tparam Function Metoda uruchamiana w klasie
    * \par Argumenty:
    * \param[in] instance wskaźnik na obiekt dla którego am być wywołana metoda
    */
    template<class C, void (C::*Function)()>
    void disconnect(C *instance)
    {
        TimeoutRecipient_t testListener;
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
    std::vector<TimeoutRecipient_t> listeners;          /**< wektor odbiorców timeoutu */
    TimerObject<TimerData_t> *_timer;           /**< obiekt klasy TimerObject osbługującej posixowe timery */
    TimerData_t _timer_data;                    /**< dane dla timerobjecta */
    SingleShotType_t _singleShot = SST_NONE;    /**< Tryb uruchamiania w singleshot */

    /*!
    * \brief Sprawdzanie czy adresat jest już w wektorze odbiorców
    * \par Argumenty:
    * \param[in] _testListener odbiorca do porównanie
    * \par Wartosci zwracane:
    * \return 1 istnieje w wektorze
    * \return 0 nie istnieje w wektorze
    */
    bool isSet(TimeoutRecipient_t _testListener);
    /*!
    * \brief Metoda wykonywana w czasie timeoutu timera posixowego
    */
    void timeout();
};

#endif // CTIMER_H
