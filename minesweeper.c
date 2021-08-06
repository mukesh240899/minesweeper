#include <stdio.h>
#include <stdlib.h>
#include <time.h>//to get the current time and other things

int **a,**b;//a stores the actual board values; b holds the opened tiles only, others are by default 'unopened'
int mine,size_x,size_y;//mine holds the number of mines. size_x and size_y holds the dimensions of the minefield
int border_v=186,border_h=205;//these are the ASCII characters for the vertical and horizontal borders, respectively
int explode='*',deactivate='~',empty=' ',unopened=177;//(176,177,178,219)
/*
explode is the mine display character.
deactivate is the mine display character if the player wins and is able to deactivate all the mines (deactivated mines)
empty is the display character when there are no numbers,i.e, the tile does not have any surrounding mines
unopened is the display character for a choose-able tile
*/
int dead; //a flag to detect the end of gameplay. dead is 1 if the player has lost; 2 if the player has won
char name[30]; //string to hold name of player

void displayRules();  //to display rules
void setup();         //to declare the 2d arrays, and set the border in second matrix
void initialize();    //to plant the mines randomly
void adjustVals();    //to set the numbers in each tile
void openUp(int,int); //to select the tiles once an empty tile is chosen
void display();       //to display the minefield
int check();          //to check if the player has won. Returns 0 if not, else 1 and sets dead to 2
void play();          //function for continuous input from the user and checking for the end of gameplay

void displayRules()
{
    system("cls");
    printf("                                        ___   _     _     ____  __ \n");
    printf("====================================== | |_) | | | | |   | |_  ( (` ======================================\n");
    printf("                                       |_| \\ \\_\\_/ |_|__ |_|__ _)_)\n\n");
    printf("\t\t\t\t      (Press enter to read new rule)\n\n");
    getch();
    printf("(1). The objective of the game is to expose all the tiles on the minefield without hitting any mines.\n");
    getch();
    printf("\n(2). Choose a tile on the minefield by entering the row and column numbers respectively.\n");
    getch();
    printf("\n(3). The numbers on the board represent how many bombs are adjacent to a square. \n  For example, if a tile has a \"3\" on it, then there are 3 bombs diagonally or orthogonally surrounding the tile.\n");
    getch();
    printf("\n(4). Avoid all the bombs and expose all the empty spaces to win Minesweeper.\n");
    getch();
    printf("\n\nYOU ARE READY!! LET'S GO!!!");
    getch();
}

void setup()
{
    dead=0;
    //declaring the array sizes
    a=(int**)calloc(size_x,sizeof(int*));
    for(int i=0;i<size_x;i++)
        a[i]=(int*)calloc(size_y,sizeof(int));
    b=(int**)calloc(size_x+2,sizeof(int*));
    for(int i=0;i<size_x+2;i++)
        b[i]=(int*)calloc(size_y+2,sizeof(int));

    //initializing default values to b
    for(int i=1;i<size_x+1;i++)
        for(int j=1;j<size_y+1;j++)
            b[i][j]=unopened;

    for(int i=0;i<size_x+2;i++)
    {
        b[i][0]=border_v;
        b[i][size_y+1]=border_v;
    }
    for(int i=0;i<size_y+2;i++)
    {
        b[0][i]=border_h;
        b[size_x+1][i]=border_h;
    }
    b[0][0]=201;
    b[0][size_y+1]=187;
    b[size_x+1][0]=200;
    b[size_x+1][size_y+1]=188;
    /*for(int i=0;i<size_x+2;i++)
    {
        for(int j=0;j<size_y+2;j++)
            printf("%c ",b[i][j]);
        printf("\n");
    }*/
    initialize();
}

void initialize()
{
    int random_x,random_y;
    int counter=0;
    srand(time(0));
    while(counter!=mine)
    {
        random_x=rand()%size_x;
        random_y=rand()%size_y;
        if(a[random_x][random_y]!=-1)
        {
            a[random_x][random_y]=-1;
            counter++;
        }
    }
    adjustVals();
}

void adjustVals()
{
    int count;
    for(int i=0;i<size_x;i++)
        for(int j=0;j<size_y;j++)
        {
            count=0;
            if(a[i][j]!=-1)
            {
                if((i-1)>=0 && (j-1)>=0 && a[i-1][j-1]==-1)
                    count++;
                if((i-1)>=0 && a[i-1][j]==-1)
                    count++;
                if((i-1)>=0 && (j+1)<size_y && a[i-1][j+1]==-1)
                    count++;
                if((j-1)>=0 && a[i][j-1]==-1)
                    count++;
                if((j+1)<size_y && a[i][j+1]==-1)
                    count++;
                if((i+1)<size_x && (j-1)>=0 && a[i+1][j-1]==-1)
                    count++;
                if((i+1)<size_x && a[i+1][j]==-1)
                    count++;
                if((i+1)<size_x && (j+1)<size_y && a[i+1][j+1]==-1)
                    count++;
                a[i][j]=count;
            }
        }
}

void openUp(int i, int j)
{
    if(i>=0 && i<size_x && j>=0 && j<size_y && b[i+1][j+1]!=empty)
    {
        if(a[i][j]==0)
        {
            b[i+1][j+1]=empty;
            openUp(i-1,j-1);
            openUp(i-1,j);
            openUp(i-1,j+1);
            openUp(i,j-1);
            openUp(i,j+1);
            openUp(i+1,j-1);
            openUp(i+1,j);
            openUp(i+1,j+1);
        }
        else
            b[i+1][j+1]=a[i][j]+48;
    }
}

void display()
{
    system("cls");
    if(dead)
    {
        for(int i=0;i<size_x;i++)
            for(int j=0;j<size_y;j++)
            {
                if(a[i][j]==-1 && dead==1)
                    b[i+1][j+1]=explode;
                else if(a[i][j]==-1 && dead==2)
                    b[i+1][j+1]=deactivate;
            }
    }
    printf("\n");
    /*for(int i=0;i<size_x;i++)
    {
        for(int j=0;j<size_y;j++)
            printf("%d\t",a[i][j]);
        printf("\n");
    }*/
    printf("\t\t");
    for(int i=0;i<size_y;i++)
        printf("\t(%d)",(i+1));
    printf("\n");
    for(int i=0;i<size_x+2;i++)
    {
        if(i!=0 && i!=size_x+1)
            printf("\t(%d)",(i));
        else
            printf("\t ");
        for(int j=0;j<size_y+2;j++)
        {
                printf("\t%c",b[i][j]);
        }
        printf("\n\n");
    }
}

int check()
{
    for(int i=1;i<size_x+2;i++)
        for(int j=1;j<size_y+2;j++)
            if(b[i][j]==unopened && a[i-1][j-1]!=-1)
                return 0;
    dead=2;
    display();
    return 1;
}

void play()
{
    while(!check() && !dead)
    {
        int x,y;
        display();
        printf("Enter Row and Column:: (in X Y format) :: ");
        scanf("%d %d",&x,&y);
        x--;
        y--;
        if(x>=0 && x<size_x && y>=0 && y<size_y)
        {
            if(b[x+1][y+1]!=unopened)
            {
                printf("Already selected! Choose again\n");
                printf("\a");
                getch();
            }
            else
            {
                if(a[x][y]==-1)
                {
                    dead=1;
                    display();
                    printf("\nYou Are Dead!!\n");
                    float percentage=(1-(mine/(float)(size_x*size_y)))*100;
                    if(percentage<40)
                        printf("\nWell you had only %.2f chance to win!\nBetter Luck Next time",percentage);
                    else
                        printf("\nYou should have won this! :')\nYou had %.2f chance of winning.\n",percentage);
                    return;//to exit the function, but not the entire program
                }
                else if(a[x][y]==0)
                {
                    openUp(x,y);
                    display();
                }
                else
                    b[x+1][y+1]=a[x][y]+48;
            }
        }
        else
        {
            printf("Wrong selection! :( Choose again\n");
            getch();
        }
    }
    display();
    printf("\a");
    printf("\n\n\t\t\t   _, _, ,  ,  _,  ,_  _  ___,  , ,  _  ___,___,  _, ,  , _,!\n");
    printf("\t\t\t  /  / \\,|\\ | / _  |_)'|\\' | |  | | '|\\' | ' |   / \\,|\\ |(_, \n");
    printf("\t\t\t '\\_'\\_/ |'\\|'\\_|`'| \\ |-\\ |'\\__|'|__|-\\ |  _|_,'\\_/ |'\\| _) \n");
    printf("\t\t\t    `'   '  `  _|  '  `'  `'    `   ''  `' '     '   '  `'   \n");
    printf("\t\t\t              '                                              \n");
    printf("\n\t\t\t\t  %s You have won the Game!!\n",name);
    printf("\a");
}

int main()
{
    int mode;
    int goOn;
    int rules;
    printf("\n\t\t\t\t\tWELCOME TO\n");
    printf("========================================================================================\n");
    printf(" _______ _______ _______ _______ _______ ________ _______ _______ ______ _______ ______");
    printf("\n|   |   |_     _|    |  |    ___|     __|  |  |  |    ___|    ___|   __ \\    ___|   __ \\ ");
    printf("\n|       |_|   |_|       |    ___|__     |  |  |  |    ___|    ___|    __/    ___|      <");
    printf("\n|__|_|__|_______|__|____|_______|_______|________|_______|_______|___|  |_______|___|__|\n");
    printf("\n========================================================================================\n\n");
    printf("\n\t\t\t\t      LET's BEGIN!!\n");
    getchar();
    system("cls");
    printf("Do you want to get a quick view of the game rules?(1/0): ");
    scanf("%d",&rules);
    if(rules)
        displayRules();
    system("cls");
    do
    {
        system("cls");
        printf("Enter your name:");
        //scanf("%s",&name);
        getchar();
        gets(name);
        printf("\n1.  Beginner Mode ::  8x8  :: 5 mines\n");
        printf("2.  Moderate Mode :: 10x10 :: 30 mines\n");
        printf("3.  Expert Mode   :: 15x15 :: 40 mines\n");
        printf("4.  Custom Mode\n");
        printf("Enter the mode you wanna play in (1-4: anything else to exit game): ");
        scanf("%d",&mode);
        switch(mode)
        {
            case 1: mine=5;
                    size_x=8;
                    size_y=8;
                    break;
            case 2: mine=30;
                    size_x=10;
                    size_y=10;
                    break;
            case 3: mine=40;
                    size_x=15;
                    size_y=15;
                    break;
            case 4: printf("Enter board sizes::(Try to keep it within 22x23)::\nNumber of rows: ");
                    scanf("%d",&size_x);
                    printf("Number of columns: ");
                    scanf("%d",&size_y);
                    int okay=0;
                    while(!okay)
                    {
                        printf("\nEnter Number of mines you want in the field: ");
                        scanf("%d",&mine);
                        if(mine<size_x*size_y)
                            okay=1;
                        else
                            printf("Number of mines cannot exceed the size of the minefield! :(\n");
                    }
                    break;
            default: printf("Thanks for playing! :)\n");
                     exit(0);
        }
        setup();
        play();
        getch();
        system("cls");
        printf("Wanna play again?(1/0): ");
        scanf("%d",&goOn);
    }while(goOn==1);
    printf("Thanks for playing! :)\n");
    return 0;
}
