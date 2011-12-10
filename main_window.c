#include <curses.h>

#include "ui.h"

static const int MAIN_WINDOW_LINES = 22;
static const int MAIN_WINDOW_COLUMNS = 80;
static const int MAIN_WINDOW_STARTING_Y = 0;
static const int MAIN_WINDOW_STARTING_X = 0;
static const int WERLD_ACCOUNT_INPUT_Y = 18;
static const int WERLD_ACCOUNT_INPUT_X = 53;
static const int WERLD_PASSWORD_INPUT_Y = 19;
static const int WERLD_PASSWORD_INPUT_X = 53;

const int WERLD_ACCOUNT_MAX_SIZE = 20;
const int WERLD_PASSWORD_MAX_SIZE = 20;

static const char *WERLD_LOGIN_SCREEN_DRAGON =
  "                                  /   \\\n"
  "                          )      ((   ))     (\n"
  "                         /|\\      ))_((     /|\\\n"
  "                        / | \\    (/\\|/\\)   / | \\\n"
  "                       /  | voV   \\`|'/  Vov-|  \\\n"
  "                            '^`   (o o)  '^`| |\n"
  "                                  `\\Y/'\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "                 l   /\\ /         ( (       \\ /\\   l\n"
  "                 l /   V           \\ \\       V   \\ l\n"
  "                 l/                _) )_          \\I\n"
  "                                   `\\ /'\n"
  "                                     `\n"
  "\n"
  "\n";

static const char *WERLD_LOGIN_SCREEN_SCROLL =
  "\n"
  " _\n"
  "(@)\n"
  "|-|                                                                   (@)\n"
  "| |-------------------- -- -   ---     --   - -- ---------------------|-|\n"
  "|-|                                                                   | |\n"
  "| |                                                                   |-|\n"
  "|-|                                                                   | |\n"
  "| |                                                                   |-|\n"
  "|-|                                                                   | |\n"
  "| |                                                                   |-|\n"
  "|-|                                                                   | |\n"
  "| |                                                                   |-|\n"
  "|_|___________________________________________________________________| |\n"
  "(@)                                                                 `\\|-|\n"
  "                                                                      (@)\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n";

static const char *WERLD_LOGIN_SCREEN_TITLE =
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "      __          __        _     _    ____        _ _\n"
  "      \\ \\        / /       | |   | |  / __ \\      | (_)\n"
  "       \\ \\  /\\  / /___ _ __| | __| | | |  | |_ __ | |_ _ __   ___\n"
  "        \\ \\/  \\/ // _ \\ '__| |/ _` | | |  | | '_ \\| | | '_ \\ / _ \\\n"
  "         \\  /\\  /|  __/ |  | | (_| | | |__| | | | | | | | | |  __/\n"
  "          \\/  \\/  \\___|_|  |_|\\__,_|  \\____/|_| |_|_|_|_| |_|\\___|\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n";

static const char *WERLD_LOGIN_SCREEN_CREDENTIALS_BOX =
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "\n"
  "                                         +--------------------------------+\n"
  "                                         |  account: ____________________ |\n"
  "                                         | password: ____________________ |\n"
  "                                         +--------------------------------+\n";

void main_window_new(WINDOW **main_window) {
  *main_window = newwin(MAIN_WINDOW_LINES,
                        MAIN_WINDOW_COLUMNS,
                        MAIN_WINDOW_STARTING_Y,
                        MAIN_WINDOW_STARTING_X);
}

void main_window_init(WINDOW *main_window) {
  keypad(main_window, true);
}

void main_window_del(WINDOW *main_window) {
  delwin(main_window);
}

void main_window_refresh(WINDOW *main_window) {
  wrefresh(main_window);
}

void main_window_draw_login_screen(WINDOW *main_window) {
  WINDOW *dragon_window;
  WINDOW *scroll_window;
  WINDOW *title_window;
  WINDOW *credentials_box_window;

  dragon_window = dupwin(main_window);
  scroll_window = dupwin(main_window);
  title_window = dupwin(main_window);
  credentials_box_window = dupwin(main_window);

  wattron(dragon_window, COLOR_PAIR(GREEN_ON_BLACK) | A_DIM);
  mvwaddstr(dragon_window, 0, 0, WERLD_LOGIN_SCREEN_DRAGON);
  wattroff(dragon_window, COLOR_PAIR(GREEN_ON_BLACK) | A_DIM);
  wattron(scroll_window, COLOR_PAIR(BLACK_ON_BLACK) | A_BOLD);
  mvwaddstr(scroll_window, 0, 0, WERLD_LOGIN_SCREEN_SCROLL);
  wattroff(scroll_window, COLOR_PAIR(BLACK_ON_BLACK) | A_BOLD);
  wattron(title_window, COLOR_PAIR(RED_ON_BLACK) | A_BOLD);
  mvwaddstr(title_window, 0, 0, WERLD_LOGIN_SCREEN_TITLE);
  wattroff(title_window, COLOR_PAIR(RED_ON_BLACK) | A_BOLD);
  wattron(credentials_box_window, COLOR_PAIR(CYAN_ON_BLACK) | A_DIM);
  mvwaddstr(credentials_box_window, 0, 0, WERLD_LOGIN_SCREEN_CREDENTIALS_BOX);
  wattroff(credentials_box_window, COLOR_PAIR(CYAN_ON_BLACK) | A_DIM);

  overlay(dragon_window, main_window);
  overlay(scroll_window, main_window);
  overlay(title_window, main_window);
  overlay(credentials_box_window, main_window);

  delwin(dragon_window);
  delwin(scroll_window);
  delwin(title_window);
  delwin(credentials_box_window);
}

/* FIXME: make this resistant to terminal resizing. */
void main_window_get_credentials(WINDOW *main_window, char *account, char *password) {
  echo();
  curs_set(true);
  mvwgetnstr(main_window,
             WERLD_ACCOUNT_INPUT_Y,
             WERLD_ACCOUNT_INPUT_X,
             account,
             WERLD_ACCOUNT_MAX_SIZE);
  mvwgetnstr(main_window,
             WERLD_PASSWORD_INPUT_Y,
             WERLD_PASSWORD_INPUT_X,
             password,
             WERLD_PASSWORD_MAX_SIZE);
  curs_set(false);
  noecho();
}
