#include "game_gui.h"

gameGui::gameGui()
{
    //ctor
    sf::Window oknoAplikacji( sf::VideoMode( 800, 600, 32), "_sfml_app" );
    while(1){
    oknoAplikacji.display();
    }


}

gameGui::~gameGui()
{
    //dtor
}

gameGui& gameGui::operator=(const gameGui& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
