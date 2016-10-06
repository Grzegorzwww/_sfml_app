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
    protected:
    private:
};

#endif // GAME_GUI_H
