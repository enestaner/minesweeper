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

//fills matrix(table) '0' and '1' or special chars depends on the table type
void tableFiller(int *table, int length, char type, int bombAmount);

//prints matrix on screen
void tableGetter(int *table, char type, int length);

//calculate bomb amount depends on difficulty
int bombCounter(char difficulty, int length);

//take coordinates then controls if there is a bomb or not
int bombController(int row, int col, int *ptrGame, int length);

//take coordinates, controls the box then put or remove flag
void flagController(int row, int col, int *ptrFake, int length);

//change all mine-boxes to 'x' and printing 'GAME OVER' part
void gameFinisher(int *ptrGame, int *ptrFake, int length);

//take coordinates and find surrounding coordinates
void pointFinder(int row, int col, int length, int *list);

//fills every box in 'bombTable' with surrounding mine amounts
void bombScanner(int *ptrGame, int length, int row, int col, int *ptrFake, int newRow, int newCol, int a, int b);

//take coordinates and prints surrounding mine amount (if amount == 0 then recalls itself)
void bombAmountWriter(int *ptrGame, int length, int row, int col, int *ptrFake, int *list);

//changing surrounding mine amounts '0' to '-' 
void tableFixer(int *ptrFake, int length);

//controls every turn if all safe-boxes opened or not
int gameController(int *ptrFake, int *ptrGame, int length, int bomb);