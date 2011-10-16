#include <curses.h>

#include "movement.h"

enum movement movement_direction(int key) {
  switch (key) {
  case 'h':
  case KEY_LEFT:
    return(LEFT);
  case 'j':
  case KEY_DOWN:
    return(DOWN);
  case 'k':
  case KEY_UP:
    return(UP);
  case 'l':
  case KEY_RIGHT:
    return(RIGHT);
  }
}
