#include "game_gui.h"

gameGui::gameGui()
{
    //ctor

   //sf::Window oknoAplikacji( sf::VideoMode( 800, 600, 32), "_sfml_app" );

}

gameGui::~gameGui()
{
    //dtor
}

gameGui& gameGui::operator=(const gameGui& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    return *this;
}

void *gameGui::run_gui(void )
{
    oknoAplikacji  =  new  sf::Window( sf::VideoMode( 800, 600, 32), "_sfml_app" );

    while(oknoAplikacji->isOpen())
    {
        oknoAplikacji->display();
    }
}

void gameGui::run (void){
  pthread_t tid;
  int       result;
  result = pthread_create(&tid, 0, gameGui::return_run_gui, this);
  if (result == 0)
     pthread_detach(tid);
}



