#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<stdbool.h>
#include<windows.h>

static void board_print(char **board,int n,int sc) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            if (j==0) {
                printf("\t\t%c ",board[i][j]);
                fflush(stdout);
            }
            else {
                printf("%c ",board[i][j]);
                fflush(stdout);
            }
        }
        printf("\n");
    }
    printf("\n\n\t\tScore: %d",sc);
}

static void generate_fruit(char **board,int n) {
    int r= rand()%n;
    int c= rand()%n;
    if (board[r][c]=='.') {
        board[r][c]='F';
    }
    else {
        while (1) {
            r=rand()%n;
            c=rand()%n;
            if (board[r][c]=='.') {
                board[r][c]='F';
                return;
            }
        }
    }
}

static void move_snake(char **board,int n,char *snake,int len_snake) {
    snake[0]='O';
    // int r=n/2;
    // int c=n/2;
    int score=0;
    int dr=0,dc=0;
    int capacity=100;

    int *snake_r=malloc(sizeof(int)*capacity);
    if (!snake_r) {
        return;
    }
    int *snake_c=malloc(sizeof(int)*capacity);
    if (!snake_c) {
        free(snake_r);
        return;
    }

    snake_r[0]=n/2;
    snake_c[0]=n/2;
    board[n/2][n/2]=snake[0];
    generate_fruit(board,n);

    while (1) {
        bool done=false;
        if (kbhit()) {
            char ch=getch();
            if (ch=='w' || ch==72) {
                dr=-1;
                dc=0;
            }
            else if (ch=='s' || ch==80) {
                dr=1;
                dc=0;
            }
            else if (ch=='a' || ch==75) {
                dr=0;
                dc=-1;
            }
            else if (ch=='d' || ch==77) {
                dr=0;
                dc=1;
            }
            else if (ch==27) {
                done=true;
                break;
            }
        }

        int next_r=snake_r[0]+dr;
        int next_c=snake_c[0]+dc;

        if (next_r<0 || next_r>=n || next_c<0 || next_c>=n) {
            board_print(board,n,score);
            printf("\n\t\tGame Over.\n\t\tYou hit the wall!\n");
            done=true;
        }

        if (done)
            break;

        bool hit_self=false;
        for (int i=1; i<len_snake; i++) {
            if (next_r==snake_r[i] && next_c==snake_c[i]) {
                hit_self=true;
                board_print(board,n,score);
                printf("\n\t\tGame Over.\n\t\tYou hit the snake!\n");
                break;
            }
        }

        if (hit_self) {
            break;
        }

        bool eat_fruit=false;
        if (board[next_r][next_c]=='F') {
            eat_fruit=true;
            len_snake++;
            snake_r=realloc(snake_r,len_snake*sizeof(int));
            snake_c=realloc(snake_c,len_snake*sizeof(int));
            generate_fruit(board,n);
            score+=10;
        }

        if (!eat_fruit) {
            board[snake_r[len_snake-1]][snake_c[len_snake-1]]='.';
        }

        for (int i=len_snake-1; i>0; i--) {
            snake_r[i]=snake_r[i-1];
            snake_c[i]=snake_c[i-1];
        }

        snake_r[0]=next_r;
        snake_c[0]=next_c;

        for (int i=0; i<len_snake; i++) {
            if (i==0) {
                board[snake_r[i]][snake_c[i]]='O';
            }
            else {
                board[snake_r[i]][snake_c[i]]='o';
            }
        }

        if (done)
            break;
        board_print(board,n,score);
        Sleep(200);

        // HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        // COORD pos = {0, 0};
        // SetConsoleCursorPosition(hConsole, pos);

        system("cls");
    }
    free(snake_r);
    free(snake_c);
}

int main() {
    srand(time(NULL));

    int n=20;
    char **board=malloc(sizeof(char*)*n);
    if (!board) {
        return 1;
    }
    for (int i=0; i<n; i++) {
        board[i]=malloc(n);
        if (!board[i]) {
            free(board);
            return 1;
        }
    }
    for (int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            board[i][j]='.';
        }
    }
    char *snake=malloc(1);
    if (!snake) {
        free(board);
        return 1;
    }

    move_snake(board,n,snake,1);

    // freeing up
    for (int i=0; i<n; i++) {
        free(board[i]);
    }
    free(board);
    free(snake);
    getch();
}