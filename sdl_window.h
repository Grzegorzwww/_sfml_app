#ifndef SDL_WINDOW_H
#define SDL_WINDOW_H


#include "ui_mainwindow.h"
#include "SDL/SDL.h"

class sdl_window
{
public:
    sdl_window(Ui::MainWindow *_ui);


      Ui::MainWindow *ui;

private:
    int _image_w;
    int _image_h;

    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;

};

#endif // SDL_WINDOW_H
