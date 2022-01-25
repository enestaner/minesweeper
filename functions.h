#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "main.h"

int bombCounter(char difficulty, int length){           //selecting game difficulty                   

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

void tableFiller(int *table, int length, char type, int bombAmount){     //filling table with characters depends on which type of table

    int i, temp = 0;                                                     //game table just ones and zeros

    srand(time(NULL));                                                   //fake table just visual and filled with special char

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
    int i, j, k = 1;                                            //printing game screen

    if(type == 'g'){                                            
        for(i = 0; i < length; i++){
            for(j = 0; j < length; j++){                        //first part just necessary for debugging  
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

int bombController(int row, int col, int *ptrGame, int length){         //return if there is bomb or not
    
    return *(ptrGame + ((row-1) * length) + col - 1);
}

void flagController(int row, int col, int *ptrFake, int length){        //changing flag situation
    
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

void gameFinisher(int *ptrGame, int *ptrFake, int length){             //opening all mine boxes with 'x'

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

void pointFinder(int row, int col, int length, int *list){          //to find all surrounding points which given point
    
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

    int j = 0, k = 0, sum = 0, temp = 0;                                              //to find how many bomb surrounding
    
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

void bombAmountWriter(int *ptrGame, int length, int row, int col, int *ptrFake, int *list){

    int k, j, temp, newRow, newCol, a, b;                              //this func controls surrounding points then prints the screen bomb amount if amount 0 then recalls itself

    if(*(ptrGame + (row - 1) * length + col - 1) == 0){
        
        pointFinder(row, col, length, list);
        
        newRow = *list;
        newCol = *(list + 1);
        a = *(list + 2);
        b = *(list + 3);
        k = newRow + a;
        j = newCol + b;
        temp = newCol;
        
        for(; newRow < k; newRow++){
            for(; newCol < j; newCol++){
                
                if(*(ptrGame + (newRow - 1) * length + newCol - 1) == 0 && *(ptrFake + (newRow - 1) * length + newCol - 1) == (char)248){
                    *(ptrFake + (newRow - 1) * length + newCol - 1) = '-';
                    bombAmountWriter(ptrGame, length, newRow, newCol, ptrFake, list);
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

    int i, j;                                                       //one bug I cant figure it out
                                                                    //after "bombAmountWriter" call, boxes will be '0' so this func turn them to '-'
    for(i = 1; i <= length; i++){
        for(j = 1; j <= length; j++){
            if(*(ptrFake + (i - 1) * length + j - 1) == '0'){
                *(ptrFake + (i - 1) * length + j - 1) = '-';
            }
        }
    }
}

int gameController(int *ptrFake, int *ptrGame, int length, int bomb){

    int i, j, point = 0;                                    //controls every turn if all safe boxes opened or not 
    
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