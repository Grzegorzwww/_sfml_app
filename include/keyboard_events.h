#ifndef KEYBOARD_EVENTS_H
#define KEYBOARD_EVENTS_H

#include <iostream>
#include <SFML/Window.hpp>


class keyboardEvents
{
    public:
        keyboardEvents(const sf::Window *app_window_ptr);
        virtual ~keyboardEvents();


       /* void scan_keyboard_events(void);
        static void *return_run_keyboard_events(void *arg) {return (( keyboardEvents *)arg)->scan_keyboard_events();}
        void *run(void);*/

        void *scan_keyboard_events(void);
        void run(void);


    protected:
    private:

    const sf::Window *app_window;
};

#endif // KEYBOARD_EVENTS_H
