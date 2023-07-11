#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <string.h>

int r,c,hs,i=0;
void menu();
void printboard();
void startGame();
void computer();
int checkFour(char x);
int emptyCell(int col);
char board[500][500];
int stack_undo[500*500];
int stack_redo[500*500];
void push_undo(int col);
int pop_undo();
void undo();
void push_redo(int x);
int pop_redo();
void redo(char symbol);
void load();
void leaderboard_view();
void save();

int top_undo=-1,top_redo=-1;

typedef struct
{
    int moves;
    int score;
    char name[256]

} player;

 player player1,player2,leader[10000],temp;
void endGame(player player1,player player2,int);
void Xml();
int find_tag (char s1[],char s2[]);

int main()
{

     if(i==0) Xml(); // function to get parameters from xml file before starting game
       i++;

    for(int i=0; i<r; i++)
    {
        for(int j=0; j<c; j++)
            board[i][j]=' ';
    }

    top_undo=-1,top_redo=-1;
    stack_undo[r*c]=0;
    stack_redo[r*c]=0;
    player1.moves=0;
    player1.score=0;
    player2.score=0;
    player2.moves=0;

    menu();
        // function to display main menu


    return 0;
}

void menu()
{   printf("\033[0m");
    system("cls");
    printf("       \n       welcome to connect 4 game\n\n");
    printf("\t1-start a new game\n");
    printf("\t2-load game\n");
    printf("\t3-top players\n");
    printf("\t4-Quit\n\n");

    printf("\tchoose from the previous menu:");

    int choice;
    scanf("%d",&choice);

    switch(choice)
    {
     case 1:startGame();break;               // function to start the game
     case 2:load();break;
     case 3:leaderboard_view();break;
     case 4 :exit(0);break;
    default:
        printf("\n\tchoose a valid choice\n\n");
        getchar();
        menu();
        break;

    }
}

void printboard()
{
    system("cls");
    printf("\n");

    for(int i=0; i<r; i++)
    {
        for(int j =0; j<c; j++)
        {
            printf("\033[0;34m");
            printf("| ");

            if(board[i][j]=='o')
            {
                printf("\033[0;31m");
                printf("%c",board[i][j]);
            }
            else
            {

                printf("\033[0;33m");
                printf("%c",board[i][j]);
            }
            printf(" ");
        }
        printf("\033[0;34m");
        printf("| \n");
        for(int i=0; i<c; i++) printf("----");
        printf("\n");
    }

    for(int i=1; i<=c; i++)
    {
        if(i<10)   printf("  %d ",i);
        else printf(" %d ",i);
    }
    printf("\n");
}

int emptyCell(int col)
{
    int row=-1;
    for(int i=0; i<r; i++)
    {
        if(board[i][col-1]==' ') row++;
    }
    return row;
}

void startGame()                         // function  to start the game and print the board
{

    char symbol='o';
    int steps=player1.moves+player2.moves;
    if (steps%2==0){
        symbol='o';
    }
    else{
        symbol='x';
    }

    printf("\nchoose game mode: 1-two player mode  2-one player mode ");

    int mode;
    scanf("%d",&mode);
    printboard();
    time_t start, end;
    time(&start);

    if(mode==1||mode==2)
    {
        while(1)
        {

            if(symbol=='o')
            {
                time(&end);
                printf("\033[0;31m");
                printf("\ntime passed since start: %.0f:%d\n",floor(difftime(end,start)/60), (int)difftime(end,start)%60);
                printf("player '%c' score : %d\n",symbol, player1.score);
                printf("number of moves of player '%c' : %d\n",symbol,player1.moves);
                printf("press u for undo,r for redo, e for exit,s for save\n");
                printf("player '%c' your turn, choose a column:",symbol);

            }

            if(symbol=='x')
            {
                time(&end);
                printf("\033[0;33m");
                if(mode==1)
                {
                    printf("\ntime passed since start: %.0f:%d\n",floor(difftime(end,start)/60),(int)difftime(end,start)%60);
                    printf("player '%c' score : %d\n",symbol,player2.score);
                    printf("number of moves of player '%c' : %d\n",symbol,player2.moves);
                    printf("press u for undo,r for redo, e for exit,s for save\n");
                    printf("player '%c' your turn, choose a column:",symbol);
                }
                if(mode==2)
                {
                    printf("\033[0;31m");
                    printf("press u for undo,r for redo, e for exit\n");
                    printf("player '%c' your turn, choose a column:",'o');
                }
            }

            char coll[5];
            scanf("%s",&coll);
            int col=atoi(coll);
            getchar();

            if(coll[0]=='u'||coll[0]=='U')                    // undo case
            {
                if(top_undo==-1)
                {
                    printf("\nyou can't undo more the board is empty\n");     // check if undo stack is empty
                    sleep(1);
                    continue;
                }
                undo();
                steps--;

                if(symbol=='x')
                {
                    player1.score= checkFour('o');
                    player1.moves--;
                }
                else
                {
                    player2.score= checkFour('x');
                    player2.moves--;
                }

                if(mode==2)
                {
                    if(symbol=='o')   symbol='x';
                    else  symbol='o';
                    continue;
                }

            }

            else if(coll[0]=='r'||coll[0]=='R')                  // redo case
            {

                if(top_redo==-1)
                {
                    printf("\nyou can't redo \n");     // check if redo stack is empty
                    sleep(1);
                    continue;
                }
                if(mode==1) redo(symbol);
                steps++;

                if(symbol=='o')
                {
                    player1.score= checkFour('o');
                    player1.moves++;
                }
                else
                {
                    player2.score= checkFour('x');
                    player2.moves++;
                }

                if(mode==2)
                {
                    redo(symbol);
                    if(symbol=='o')   symbol='x';
                    else  symbol='o';
                    continue;
                }
            }
            else if(coll[0]=='e'||coll[0]=='E') exit(0);              // exit case

            else if(coll[0]=='s'||coll[0]=='s') save();
            else if( col<1 || col>c)
            {
                printf("\ninvalid choice ,out of range! \n\n");
                continue;
            }

            else
            {
                int row = emptyCell(col);
                if(row == -1 )
                {
                    printf("\ninvalid move , column%d is full!\n\n",col);
                    continue;
                }
                else
                {
                    board[row][col-1]=symbol;
                    push_undo(col-1);
                    stack_redo[r*c]=0;
                    top_redo=-1;
                    steps++;
                    if(symbol=='o')
                    {
                        player1.moves++;
                        player1.score = checkFour(symbol);            // check score
                    }
                    else
                    {
                        player2.moves++;
                        player2.score = checkFour(symbol);
                    }
                }

                printboard();

            }


            if(mode==1)                              // if 2 player mode switch symbols
            {
                if(symbol=='o')   symbol='x';
                else  symbol='o';
            }

            else
            {
                printf("computer turn\n");   // if 1 player mode computer will make move
                sleep(1);
                computer();
                printf("computer score:%d\n", player2.score = checkFour('x'));
                player2.moves++;
                steps++;
            }

            if(steps==r*c)   break;
        }
    }

    else
    {
        getchar();
        startGame();
    }

    endGame(player1,player2,mode);
}

void computer()
{
    //srand(time(NULL));
    int col = (rand()%(c-1+1)) + 1;                       // function to get random column in range of board
    int row = emptyCell(col);
    push_undo(col-1);
    if (row==-1) computer();                             //get another value of col if it is full
    else board[row][col-1]='x';
    printboard();

}

int checkFour(char x)                   //function to update score and check connect 4
{
    int e=0;
    for(int i=0; i<r; i++)
    {
        for(int k=0; k<c; k++)
        {
            if(i+3<r)
            {
                if(board[i][k]== x && board[i+1][k]== x &&board[i+2][k]== x &&board[i+3][k]== x )      // vertical check
                    e++;
            }

            if(k+3<c)
            {
                if(board[i][k]== x &&board[i][k+1]== x &&board[i][k+2]== x &&board[i][k+3]== x )    // horizontal check
                    e++;
            }

            if(k>=3&&i+3<r)
            {
                if(board[i][k]== x &&board[i+1][k-1]== x &&board[i+2][k-2]==x &&board[i+3][k-3]== x)      // diagonal left
                    e++;
            }
            if(i+3<r&&k+3<c)
            {
                if(board[i][k]== x && board[i+1][k+1]== x && board[i+2][k+2]== x &&board[i+3][k+3]== x) // diagonal right
                    e++;
            }
        }

    }

    return e;
}
void undo()
{
    int removed=pop_undo();
    int row=emptyCell(removed+1);
    board[row+1][removed]=' ';
    printboard();

}

void redo(char symbol)
{
    int add=pop_redo();
    int row=emptyCell(add+1);
    board[row][add]=symbol;
    printboard();
}


void push_undo(int col)               // function to push the index of col in the stack
{
    if(top_undo==r*c-1) return ;      //check if the stack is full
    top_undo=top_undo+1;
    stack_undo[top_undo]= col;

}

int pop_undo()
{
    int value = stack_undo[top_undo];
    top_undo=top_undo-1;
    push_redo(value);
    return value ;

}

void push_redo(int x)
{
    top_redo=top_redo+1;
    stack_redo[top_redo]=x;
}

int pop_redo()
{
    int ndx=stack_redo[top_redo];
    top_redo=top_redo-1;
    push_undo(ndx);
    return ndx;
}

void endGame(player player1,player player2,int mode)
{

    printf("\n\tthe game is over! \n");
    if(player1.score>player2.score)
    {
        printf("player'o',please enter your name:");
        gets(player1.name);
        printf("\t%s wins!, his score: %d\n",player1.name,player1.score);
        leaderboard_store(player1);
    }

    else if(player2.score>player1.score && mode==1)
    {
        printf("player'x',please enter your name:");
        gets(player2.name);
        printf("\t%s wins!, his score: %d\n",player2.name,player2.score);
        leaderboard_store(player2);
    }
    else if(player2.score>player1.score && mode==2)
        printf("\tcomputer wins!\n");
    else
    {   printf("\tit is draw!\n");
        printf("player'o',please enter your name:\n");
        gets(player1.name);
        leaderboard_store(player1);
        if(mode==1)
        {
             printf("player'x',please enter your name:\n");
              gets(player2.name);
            leaderboard_store(player2);
        }
    }

    printf("\n choose an option: 1-go to the main menu   2-show top players  3-exit\n");
    int choice;
    scanf("%d",&choice);
    switch(choice)
    {
        case 1 :system("cls");main();break;
        case 2 : leaderboard_view();break;
        case 3: exit(0);break;
        default: system("cls");main();break;
    }
}


void save(){                           // function to save the game in a file
    int fileno;
    printf("choose a slot to save game in:\n\t\t1-slot1 \n\t\t2-slot2 \n\t\t3-slot3");
    scanf("%d",&fileno);
     getchar();
    system("cls");
    if(fileno==1)
    {
        FILE*save1;
        save1=fopen("save1.bin","wb");
    for(int i=0;i<r;i++)
    {
      for(int k=0;k<c;k++)
        {
        int l=board[i][k];
        fputc(l,save1);

        }
    }

    putw(player1.moves,save1);
    putw(player2.moves,save1);
    putw(player1.score,save1);
    putw(player2.score,save1);
           fclose(save1);
           main();
    }

    if(fileno==2)
    {
         FILE*save2;
        save2=fopen("save2.bin","wb");
    for(int i=0;i<r;i++)
    {
      for(int k=0;k<c;k++)
        {
        int l=board[i][k];
        fputc(l,save2);

        }
    }
      putw(player1.moves,save2);
    putw(player2.moves,save2);
    putw(player1.score,save2);
    putw(player2.score,save2);
           fclose(save2);
          main();
    }
    if(fileno==3)
    {
        FILE*save3;
        save3=fopen("save3.bin","wb");
    for(int i=0;i<r;i++)
    {
      for(int k=0;k<c;k++)
        {
        int l=board[i][k];
        fputc(l,save3);

        }
    }
     putw(player1.moves,save3);
    putw(player2.moves,save3);
    putw(player1.score,save3);
    putw(player2.score,save3);
           fclose(save3);
           main();
    }
    else{printf("you only have three slots to save in\n");save();}
}

void load(){
    printf("which game do you want to open \n\t\t1-game 1 \n\t\t2-game 2 \n\t\t3-game 3\n\t\t4-main menu\n");
     int gameno;
     scanf("%d",&gameno);
     getchar();
     if(gameno==1)
     {
      FILE*save1;
      save1=fopen("save1.bin","rb");
      if(save1!=NULL){

      for(int i=0;i<r;i++)
        {
            for(int k=0;k<c;k++)
            {
              int l=fgetc(save1);
              board[i][k]=l;
            }
         }

         player1.moves=getw(save1);
         player2.moves=getw(save1);
         player1.score=getw(save1);
         player2.score=getw(save1);
         fclose(save1);

         player1.score=checkFour('o');
         player2.score=checkFour('x');
          startGame();
      }
      else printf("there's no game saved here\n");
      load();
    }

    else if(gameno==2)
    {
         FILE* save2=fopen("save2.bin","rb");
         if(save2!=NULL){
            for(int i=0;i<r;i++)
        {
            for(int k=0;k<c;k++)
            {
              int l=fgetc(save2);
              board[i][k]=l;
            }
         }
           player1.moves=getw(save2);
         player2.moves=getw(save2);
         player1.score=getw(save2);
         player2.score=getw(save2);

         fclose(save2);
         startGame();
    }
    else printf("there's no saved game here\n");
       load();
    }

    else if(gameno==3)
    {
      FILE *save3=fopen("save3.bin","rb");
        if(save3!=NULL){
      for(int i=0;i<r;i++)
        {
            for(int k=0;k<c;k++)
            {
              int l=fgetc(save3);
              board[i][k]=l;
            }
         }
          player1.moves=getw(save3);
         player2.moves=getw(save3);
         player1.score=getw(save3);
         player2.score=getw(save3);

         fclose(save3);
         startGame();

        }
        else {printf("there's no saved game here\n");
             load();
        }
    }
    else if(gameno==4){
            main();
        }
    else
    {
        printf("you have only 3 games saved\n");
            load();
    }
}


void Xml()
{
    char y,x[100]={'0'},conf_1[]="<Configurations>",conf_2[]= "</Configurations>";
    char height_1[]="<Height>" ,height_2[]="</Height>",width_1[]="<Width>",width_2[]="</Width>",
     highscores_1[]="<Highscores>",highscores_2[]="</Highscores>" ; FILE*xml=NULL; static char filepath[100];
    int i =0,columns,rows,high; static int j=0;

    if(j==0)   xml= fopen("parameters.xml","r");
      if(j>0&&j<3) xml = fopen(filepath,"r");
      if(j>2)
      {
         printf("the parameters file is not found the height will set to default(7)\n");r=6;
                printf("the parameters file is not found the width will set to default(6)\n");c=7;
                printf("the parameters file is not found the high scores will set to default(10)\n");hs=10;
                sleep(3);
                return;
      }
    if(xml==NULL)
    {
            printf("there is no file here enter correct file path:");
             gets(filepath);
             xml = fopen(filepath,"r");
            if(xml==NULL)
            {
                printf("the parameters file is not found the height will set to default(7)\n");r=6;
                printf("the parameters file is not found the width will set to default(6)\n");c=7;
                printf("the parameters file is not found the high scores will set to default(10)\n");hs=10;
                sleep(3);

                  return;
            }
           else
           {
              while((y=fgetc(xml))!=EOF)
             { if(y!=' '&& y!='\t'&&y!='\n')
               {
                 x[i++]=y;
               }
             }
           }
    }
    else
    {
      while((y=fgetc(xml))!=EOF)
          {  if(y!=' '&& y!='\t'&&y!='\n')
             {
                 x[i++]=y;
             }
          }
    }
int start=find_tag(conf_1,x);
int end=find_tag(conf_2,x);
int size=end-start-strlen(conf_2);

if(start!=-1&&end!=-1)
{
char arr[size];
for(int i=0;i<size;i++)
arr[i]=x[start+1+i];

int start_h=find_tag(height_1,arr);        // getting no of rows from height tag
int end_h=find_tag(height_2,arr);

char height[4];
height[0]=arr[start_h+1];
height[1]=arr[start_h+2];
height[2]=arr[start_h+3];
height[3]='\0';

int start_w=find_tag(width_1,arr);           // getting no of width from column tag
int end_w=find_tag(width_2,arr);

char width[4];
width[0]=arr[start_w+1];
width[1]=arr[start_w+2];
width[2]=arr[start_w+3];
width[3]='\0';

int start_hs=find_tag(highscores_1,arr);
int end_hs=find_tag(highscores_2,arr);                   //getting length of hs from highscore tag

char highscore[4];
highscore[0]=arr[start_hs+1];
highscore[1]=arr[start_hs+2];
highscore[2]=arr[start_hs+3];
highscore[3]='\0';

if(start_h!=-1&&end_h!=-1&&end_h>start_h&&start_w!=-1&&end_w!=-1&&end_w>start_w&&start_hs!=-1&&end_hs!=-1&&end_hs>start_hs)
{
    rows=atoi(height);high=atoi(highscore);columns=atoi(width);
     c=columns;r=rows;hs=high;
}
else{
      j++;
     printf(" the file is corrupted enter the path for the correct file:");
             gets(filepath);
            Xml();
}
}
else
{
        j++;
        printf("the file is corrupted enter the path for the correct file:");
        gets(filepath);
        Xml();

}
fclose(xml);
}
int find_tag (char s1[],char s2[])
{
  int i=0,index=-1;
  while(s2[i]!='\0')
   {
        int found = 1;
    for(int j=0;s1[j]!='\0';j++)
    {
        if(s1[j]!=s2[i++])
        {
            found =0 ;
            break;
        }
    }
    if(found)
    {
        index=i-1;
        break;
    }
   }
    return index;
}

void leaderboard_store(player player1){
    FILE*score1;
    int i=0;

    score1=fopen("scores1.bin","ab");
    fseek(score1,0,SEEK_END);
    i=ftell(score1);
    if(i!=0){
        fprintf(score1,"\n");
    }

        putw(player1.score,score1);

        for(int i=0;player1.name[i]!='\0';i++){
            putc(player1.name[i],score1);


            }

        fclose(score1);
}
void leaderboard_view(){
FILE*score;
char l[250];
int e,i=0,j=0;
score=fopen("scores1.bin","rb");
system("cls");
if(score!=NULL){

while(!feof(score)){
        leader[i].score=getw(score);
    fgets(leader[i].name,256,score);
         i++;
         e=i;
}
for(int i=0;i<e+1;i++){
    for(int k=0;leader[i].name[k]!='\0';k++){
         leader[i].name[k]=tolower(leader[i].name[k]);

    }
}

for(int i=0;i<e+1;i++){
    for(int k=i+1;k<e+1;k++){
            int j=strlen(leader[k].name);
        if(strncmp(leader[i].name,leader[k].name,j)==0){
           if(leader[i].score<=leader[k].score){
                memset(leader[i].name,'\0',sizeof(leader[i].name));
                leader[i].score=0;
           }
           else{
               memset(leader[k].name,'\0',sizeof(leader[k].name));
               leader[k].score=0;

           }
        }
    }
}

for(i=0;i<e;i++){
    for( j=i+1;j<e;j++){
        if(leader[i].score<leader[j].score)
        {
            temp=leader[i];
            leader[i]=leader[j];
            leader[j]=temp;
        }
    }
}
for(i=0;i<hs;i++)
{
    printf("%d- %s \t\t\t\t ",i+1,leader[i].name);
    printf("%d\n\n",leader[i].score);
}
int m;
printf("1-go back to main menu  2-any button to exit");
scanf("%d",&m);
getchar();
if(m==1){
    system("cls");
    main();
}
fclose(score);
}
else{
    printf("leaderboard score not found\n");
    sleep(1);
    main();
}
}

//END OF PROJECT!!
