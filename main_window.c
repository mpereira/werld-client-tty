#include <curses.h>

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

static const char *WERLD_LOGIN_SCREEN =
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

/* FIXME: make this resistant to terminal resizing. */
void main_window_get_credentials(WINDOW *main_window, char *account, char *password) {
  waddstr(main_window, WERLD_LOGIN_SCREEN);
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
  werase(main_window);
  wrefresh(main_window);
}
