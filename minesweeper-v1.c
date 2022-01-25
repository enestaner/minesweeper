#include "functions.h"

int main(){

    int length, r, c, i, bombAmount, bomb = 0, *gameTable, *fakeTable, *bombTable, *list;
    char difficulty, flag;

    list = (int*) malloc(sizeof(int) * 4);
    
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
            printf("\nSide length is not proper\n");
        }
    }
    SCREEN_CLEAR;

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
            bombScanner(gameTable, length, r, c, bombTable, *list, *(list + 1), *(list + 2), *(list + 3));
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
        SCREEN_CLEAR;

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
            bombAmountWriter(bombTable, length, r, c, fakeTable, list);
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