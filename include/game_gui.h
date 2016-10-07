#ifndef GAME_GUI_H
#define GAME_GUI_H

#include <iostream>
#include <SFML/Window.hpp>


class gameGui
{
public:
    gameGui();
    virtual ~gameGui();
    gameGui& operator=(const gameGui& other);


    void *run_gui(void);
    static void *return_run_gui(void *arg) {return ((gameGui *)arg)->run_gui();}
    void run (void);

    const sf::Window * returnAppWindow(void) {return oknoAplikacji;}

protected:

private:



    sf::Window *oknoAplikacji;
};

#endif // GAME_GUI_H
