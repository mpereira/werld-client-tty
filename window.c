#include <curses.h>

#include "window.h"

static const int WINDOW_LINES = 21;
static const int WINDOW_COLUMNS = 80;
static const int WINDOW_STARTING_Y = 0;
static const int WINDOW_STARTING_X = 0;
static const int WERLD_ACCOUNT_INPUT_Y = 18;
static const int WERLD_ACCOUNT_INPUT_X = 53;
static const int WERLD_PASSWORD_INPUT_Y = 19;
static const int WERLD_PASSWORD_INPUT_X = 53;

const int WERLD_ACCOUNT_MAX_SIZE = 20;
const int WERLD_PASSWORD_MAX_SIZE = 20;

static const char *WERLD_LOGIN =
  "                                  /   \\\n"
  " _                        )      ((   ))     (\n"
  "(@)                      /|\\      ))_((     /|\\\n"
  "|-|                     / | \\    (/\\|/\\)   / | \\                      (@)\n"
  "| | -------------------/--|-voV---\\`|'/--Vov-|--\\---------------------|-|\n"
  "|-|                         '^`   (o o)  '^`                          | |\n"
  "| |                               `\\Y/'                               |-|\n"
  "|-|   __          __        _     _    ____        _ _                | |\n"
  "| |   \\ \\        / /       | |   | |  / __ \\      | (_)               |-|\n"
  "|-|    \\ \\  /\\  / /___ _ __| | __| | | |  | |_ __ | |_ _ __   ___     | |\n"
  "| |     \\ \\/  \\/ // _ \\ '__| |/ _` | | |  | | '_ \\| | | '_ \\ / _ \\    |-|\n"
  "|-|      \\  /\\  /|  __/ |  | | (_| | | |__| | | | | | | | | |  __/    | |\n"
  "| |       \\/  \\/  \\___|_|  |_|\\__,_|  \\____/|_| |_|_|_|_| |_|\\___|    |-|\n"
  "|_|___________________________________________________________________| |\n"
  "(@)              l   /\\ /         ( (       \\ /\\   l                `\\|-|\n"
  "                 l /   V           \\ \\       V   \\ l                  (@)\n"
  "                 l/                _) )_          \\I\n"
  "                                   `\\ /' +--------------------------------+\n"
  "                                     `   |  account: ____________________ |\n"
  "                                         | password: ____________________ |\n"
  "                                         +--------------------------------+\n";

void window_init(WINDOW **window) {
  *window = newwin(WINDOW_LINES,
                   WINDOW_COLUMNS,
                   WINDOW_STARTING_Y,
                   WINDOW_STARTING_X);
}

/* FIXME: make this resistant to terminal resizing. */
void window_get_credentials(WINDOW *window, char *account, char *password) {
  waddstr(window, WERLD_LOGIN);
  echo();
  curs_set(true);
  mvwgetnstr(window,
             WERLD_ACCOUNT_INPUT_Y,
             WERLD_ACCOUNT_INPUT_X,
             account,
             WERLD_ACCOUNT_MAX_SIZE);
  mvwgetnstr(window,
             WERLD_PASSWORD_INPUT_Y,
             WERLD_PASSWORD_INPUT_X,
             password,
             WERLD_PASSWORD_MAX_SIZE);
  curs_set(false);
  noecho();
  werase(window);
  wrefresh(window);
}
