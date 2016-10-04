#include "sdl_window.h"
#include "SDL/SDL.h"



sdl_window::sdl_window(Ui::MainWindow *_ui) : ui(_ui)
{

    _image_w = 700;
    _image_h = 544;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
            fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
            exit(1);
       }

    sdlWindow = SDL_CreateWindowFrom((void*)ui->openGLWidget->winId());
    if (!sdlWindow) {
        fprintf(stderr, "SDL: failed to create window: %s\n", SDL_GetError());
    }
    sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, NULL);


    //ui->openGLWidget->resize(_image_w, _image_h);

}

