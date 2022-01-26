#define colorRed     "\e[1;31m"
#define colorGreen   "\e[1;32m"
#define colorYellow  "\e[1;33m"
#define colorBlue    "\e[1;34m"
#define colorMagenta "\e[1;35m"
#define colorCyan    "\e[1;36m"
#define colorReset   "\e[1;0m"

#ifdef _WIN32
  #define SCREEN_CLEAR system("cls")
#else
  #define SCREEN_CLEAR system("clear");
#endif

void tableFiller(int *table, int length, char type, int bombAmount);
void tableGetter(int *table, char type, int length);
int bombCounter(char difficulty, int length);
int bombController(int row, int col, int *ptrGame, int length);
void flagController(int row, int col, int *ptrFake, int length);
void gameFinisher(int *ptrGame, int *ptrFake, int length);
void pointFinder(int row, int col, int length, int *list);
void bombScanner(int *ptrGame, int length, int row, int col, int *ptrFake, int newRow, int newCol, int a, int b);
void bombAmountWriter(int *ptrGame, int length, int row, int col, int *ptrFake, int *list);
void tableFixer(int *ptrFake, int length);
int gameController(int *ptrFake, int *ptrGame, int length, int bomb);