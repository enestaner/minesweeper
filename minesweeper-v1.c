#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define colorRed     "\e[1;31m"
#define colorGreen   "\e[1;32m"
#define colorYellow  "\e[1;33m"
#define colorBlue    "\e[1;34m"
#define colorMagenta "\e[1;35m"
#define colorCyan    "\e[1;36m"
#define colorReset   "\e[1;0m"

void tableFiller(int *table, int length, char type, int bombAmount);
void tableGetter(int *table, char type, int length);
int bombCounter(char difficulty, int length);
int bombController(int row, int col, int *ptrGame, int length);
void flagController(int row, int col, int *ptrFake, int length);
void gameFinisher(int *ptrGame, int *ptrFake, int length);
void pointFinder(int row, int col, int length, int *list);
void bombScanner(int *ptrGame, int length, int row, int col, int *ptrFake, int newRow, int newCol, int a, int b);
void bombAmountWriter(int *ptrGame, int length, int row, int col, int *ptrFake);
void tableFixer(int *ptrFake, int length);
int gameController(int *ptrFake, int *ptrGame, int length, int bomb);

int list[4];

int main(){

    int length, r, c, i, bombAmount, bomb = 0, *gameTable, *fakeTable, *bombTable;
    char difficulty, flag;

    printf("\t\tRules\n\n");
    printf( "-Side length should be between 4 and 50\n" );
    printf( "-If you want to open flag points you should remove flag first\n" );    
    printf( "-To win this game you should find and open all non-mine boxes\n" );  
    printf( "-If you entered wrong difficulty it would be assigned Normal\n" );      
    
    while(1){
        printf( "\nEnter the side length: " ); scanf("%d", &length);
        getchar();
        printf( "Choose the difficulty E-M-H: " ); scanf("%c", &difficulty);
        
        if(length >= 4 && length <= 50){
            break;
        }
        else{
            printf(colorBlue "\nSide length is not proper\n" colorReset);
        }
    }
    system("cls");

    gameTable = (int*) malloc(sizeof(int) * (length * length));
    bombTable = (int*) malloc(sizeof(int) * (length * length));
    fakeTable = (int*) malloc(sizeof(int) * (length * length));
    bombAmount = bombCounter(difficulty, length);

    tableFiller(gameTable, length, 'g', bombAmount);
    tableFiller(fakeTable, length, 'f', bombAmount);

    for(i = 0; i < (length * length); i++){
            *(bombTable + i) = 9;
    }
    
    for(r = 1; r <= length; r++){
        for(c = 1; c <= length; c++){
            pointFinder(r, c, length, list);
            bombScanner(gameTable, length, r, c, bombTable, list[0], list[1], list[2], list[3]);
        }
    }

    while(1){
        tableGetter(fakeTable, 'f', length);
        printf(" Enter the coordinates (row column) \texample >>> 1 1\n To change flag (row column flag) \texample >>> 2 2 f\n\n");
        scanf("%d %d", &r, &c);

        if(getchar() == ' '){
            flag = getc(stdin);
            getchar();
        }
        else{
            flag = 'b';
        }
        system("cls");

        if(r <= 0 || r > length || c <= 0 || c > length){
            printf(colorGreen "Wrong coordinates, try again\n\n" colorReset);
            continue;
        }

        if(flag == 'F' || flag == 'f'){
            flagController(r, c, fakeTable, length);
            continue;
        }
        else if(*(fakeTable + ((r - 1) * length) + c - 1) == 'F'){
            printf(colorGreen "Flag point couldnt be open\n\n" colorReset);
            continue;
        }
        else if(*(fakeTable + ((r - 1) * length) + c - 1) != (char)248){
            printf(colorGreen "This point already opened\n\n" colorReset);
            continue;
        }
        else{
            bomb = bombController(r, c, gameTable, length);
        }

        if(bomb == 1){
            gameFinisher(gameTable, fakeTable, length);
            
            printf(colorRed "\n\t");
            for(i = 0; i < length; i++){
                printf("-");
            }
            printf(" GAME OVER ");
            for(i = 0; i < length; i++){
                printf("-");
            }
            printf("\n\n" colorReset);

            tableGetter(fakeTable, 'f', length);
            getchar();
            getchar();
            break;
        }
        else if(bomb == 0){
            bombAmountWriter(bombTable, length, r, c, fakeTable);
            tableFixer(fakeTable, length);
        }

        if(gameController(fakeTable, gameTable, length, bombAmount) == 1){
            printf( colorCyan "\n\t\tCONGRATULATIONS YOU WIN!\n\n" colorReset);
            tableGetter(fakeTable, 'f', length);
            getchar();
            getchar();
            break;
        }
    }
    free(bombTable);
    free(gameTable);
    free(fakeTable);
    
    return 0;
}

int bombCounter(char difficulty, int length){

    if(difficulty == 'e' || difficulty == 'E'){
        return round(length * length * 0.07);
    }
    else if(difficulty == 'm' || difficulty == 'M'){
        return round(length * length * 0.12);
    }
    else if(difficulty == 'h' || difficulty == 'H'){
        return round(length * length * 0.30);
    }
    else{
        return round(length * length * 0.12);
    }
}

void tableFiller(int *table, int length, char type, int bombAmount){

    int i, temp = 0;

    srand(time(NULL));

    if(type == 'g'){

        for(i = 0; i < (length * length); i++){
            *(table + i) = 0;
        }

        i = 0;

        while(i < bombAmount){
            temp = rand() % (length * length);
            if( *(table + temp) == 0){
                *(table + temp) = 1;
                i++;
            }
        }
    }
    else if(type == 'f'){
        
        for(i = 0; i < (length * length); i++){
            *(table + i) = (char)248;
        }
    }
}

void tableGetter(int *table, char type, int length){
    int i, j, k = 1;

    if(type == 'g'){
        for(i = 0; i < length; i++){
            for(j = 0; j < length; j++){
                printf("%d ", *(table + i * length + j));
            }
            printf("\n");
        }
    }
    else if(type == 'f'){

        printf("     ");
        for(i = 0; i < length; i++){
            printf( colorMagenta "%2d " colorReset, k);
            k++;
        }
        printf("\n");

        printf("    ");
        for(i = 0; i < length * 2 - ((length / 2) - 1); i++){
            printf( colorYellow "%c%c" colorReset, (char)194, (char)196);
        }
        printf( colorYellow "%c" colorReset, (char)194);
        printf("\n");

        k = 1;
        for(i = 0; i < length; i++){
            printf( colorMagenta " %-3d" colorReset, k);
            k++;
            printf( colorYellow "%c" colorReset, (char)195);
            for(j = 0; j < length; j++){
                if(*(table + i * length + j) == 'F'){
                    printf( colorGreen "%2c " colorReset, *(table + i * length + j));
                }
                else if(*(table + i * length + j) != (char)248 && *(table + i * length + j) != '-'){
                    printf( colorRed "%2c " colorReset, *(table + i * length + j));
                }
                else if(*(table + i * length + j) == '-'){
                    printf(colorBlue "%2c " colorReset, *(table + i * length + j));
                }
                else{
                    printf("%2c ", *(table + i * length + j));
                }
            }
            length %2 == 0 ? printf( colorYellow " %c" colorReset, (char)180) : printf( colorYellow "  %c" colorReset, (char)180);
            printf("\n");
        }

        printf("    ");
        for(i = 0; i < length * 2 - ((length / 2) - 1); i++){
            printf( colorYellow "%c%c" colorReset, (char)193, (char)196);
        }
        printf( colorYellow "%c" colorReset, (char)193);
        printf("\n");
    }
}

int bombController(int row, int col, int *ptrGame, int length){
    
    return *(ptrGame + ((row-1) * length) + col - 1);
}

void flagController(int row, int col, int *ptrFake, int length){
    
    if(*(ptrFake + ((row-1) * length) + col - 1) == (char)248){
        *(ptrFake + ((row-1) * length) + col - 1) = 'F';
    }
    else if(*(ptrFake + ((row-1) * length) + col - 1) == 'F'){
        *(ptrFake + ((row-1) * length) + col - 1) = (char)248;
    }
    else{
        printf(colorGreen "This point is not proper for flag\n\n" colorReset);
    }
}

void gameFinisher(int *ptrGame, int *ptrFake, int length){

    int i, j;
    
    for(i = 0; i < length * length; i++){
        if(*(ptrGame + i) == 1){
            *(ptrFake + i) = 'x';
        }
        else{
            *(ptrFake + i) = (char)45;
        }
    }
}

void pointFinder(int row, int col, int length, int *list){
    
    int r, c, a, b;

    if(row == 1 && col == 1){
        r = row;
        c = col;
        a = 2;
        b = 2;
    }
    else if(row == 1 && col == length){
        r = row;
        c = col - 1;
        a = 2;
        b = 2;
    }
    else if(row == length && col == 1){
        r = row - 1;
        c = col;
        a = 2;
        b = 2;
    }
    else if(row == length && col == length){
        r = row - 1;
        c = col - 1;
        a = 2;
        b = 2;
    }
    else if(row == 1){
        r = row;
        c = col - 1;
        a = 2;
        b = 3;
    }
    else if(col == 1){
        r = row - 1;
        c = col;
        a = 3;
        b = 2;
    }
    else if(col == length){
        r = row - 1;
        c = col - 1;
        a = 3;
        b = 2;
    }
    else if(row == length){
        r = row - 1;
        c = col - 1;
        a = 2;
        b = 3;
    }
    else{
        r = row - 1;
        c = col - 1;
        a = 3;
        b = 3;
    }

    *list = r;
    *(list + 1) = c;
    *(list + 2) = a;
    *(list + 3) = b;

}

void bombScanner(int *ptrGame, int length, int row, int col, int *ptrFake, int newRow, int newCol, int a, int b){

    int j = 0, k = 0, sum = 0, temp = 0;
    
    if(*(ptrGame + ((row - 1) * length) + col - 1) == 0){

        k = newRow + a;
        j = newCol + b;
        temp = newCol;
        
        for(; newRow < k; newRow++){
            for(; newCol < j; newCol++){
                sum += *(ptrGame + ((newRow - 1) * length) + newCol - 1);
            }
            newCol = temp;
        }
        *(ptrFake + ((row - 1) * length) + col - 1) = sum;
    }
}

void bombAmountWriter(int *ptrGame, int length, int row, int col, int *ptrFake){

    int k, j, temp, newRow, newCol, a, b;

    if(*(ptrGame + (row - 1) * length + col - 1) == 0){
        
        pointFinder(row, col, length, list);
        
        newRow = list[0];
        newCol = list[1];
        a = list[2];
        b = list[3];
        k = newRow + a;
        j = newCol + b;
        temp = newCol;
        
        for(; newRow < k; newRow++){
            for(; newCol < j; newCol++){
                
                if(*(ptrGame + (newRow - 1) * length + newCol - 1) == 0 && *(ptrFake + (newRow - 1) * length + newCol - 1) == (char)248){
                    *(ptrFake + (newRow - 1) * length + newCol - 1) = '-';
                    bombAmountWriter(ptrGame, length, newRow, newCol, ptrFake);
                }
                else if(*(ptrFake + (newRow - 1) * length + newCol - 1) != 'F'){
                    *(ptrFake + (newRow - 1) * length + newCol - 1) = *(ptrGame + (newRow - 1) * length + newCol - 1) + 48;
                }
            }
            newCol = temp;
        }
    }
    else{
        *(ptrFake + (row - 1) * length + col - 1) = *(ptrGame + (row - 1) * length + col - 1) + 48;
    }
}

void tableFixer(int *ptrFake, int length){

    int i, j;

    for(i = 1; i <= length; i++){
        for(j = 1; j <= length; j++){
            if(*(ptrFake + (i - 1) * length + j - 1) == '0'){
                *(ptrFake + (i - 1) * length + j - 1) = '-';
            }
        }
    }
}

int gameController(int *ptrFake, int *ptrGame, int length, int bomb){

    int i, j, point = 0;
    
    for(i = 1; i <= length; i++){
        for(j = 1; j <= length; j++){
            if(*(ptrFake + (i - 1) * length + j - 1) != (char)248 && *(ptrFake + (i - 1) * length + j - 1) != 'F' && *(ptrGame + (i - 1) * length + j - 1) == 0){
                point += 1;
            }
        }
    }

    if(length * length - bomb == point){
        return 1;
    }
    else{
        return 0;
    }
}
