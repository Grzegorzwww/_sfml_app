#include "keyboard_events.h"

using namespace std;

keyboardEvents::keyboardEvents(const sf::Window * app_window_ptr) :app_window(app_window_ptr)
{
    //ctor

}

keyboardEvents::~keyboardEvents()
{
    //dtor
}

/*
void keyboardEvents::scan_keyboard_events(void){
        while(app_window->isOpen()){
            cout << "!";

        }
}



void *keyboardEvents::run(void){
  pthread_t tid;
  int result;
  result = pthread_create(&tid, 0,keyboardEvents::return_run_keyboard_events, this);
  if (result == 0)
     pthread_detach(tid);
}
*/

void *keyboardEvents::scan_keyboard_events(void){
        while(app_window->isOpen()){
            cout << "!";

        }
        return 0;
}

void  keyboardEvents::run(void) {
  pthread_t tid;
  int       result;
  result = pthread_create(&tid, 0, &keyboardEvents::scan_keyboard_events, this);
  if (result == 0)
     pthread_detach(tid);
}
