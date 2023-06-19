/*
This program plays a game of minesweepers. For now, the only avaliable board is of 8X8.
Each turn the player chooses a digit for a row and another for a column(the program will tell the user to put a valid combination).
if in the chosen square there is a hidden mine,the program prints the fully revealed board and closes.
else it prints the board with the chosen square revealed(it will contain a ' ' if there are no mines in a radius of a square in any direction,
otherwise it will show the number of mines nearby.
if the player enters '-1' as the row chosen, then the chosen column will ber the number of squares revealed, except the ones containing mines(sort of a "cheat".
When all the non-mines squares are revealed, the player wins the game*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define ROWS 22;
#define COLS 22;
void Menu();//prints the main menu
char boardofX(char user_board[][22], int numofrows, int numofcols);//fills the userboard with 'X's
char blankboard(char board[][22], int numofrows, int numofcols); // turns all the board's squares to ' '.
void printtoplineboard(int numofcols);//prints the numbers of the columns
void printborderofboard(int numofcols);//prints a line ot the top and bottom of the board
void printboard(char board[][22], int numofrows, int numofcols); //prints the board chosen
char addrandombombs(char board[][22], int numofrows, int numofcols); //randomly adds 8 bombs to the board
void fillnearbombs(char myboard[][22], int row, int col, int numofrows, int numofcols); //fills the board with the number of nearbombs of each square
void countnearbombs(char myboard[][22], int numofrows, int numofcols); //makes the fillnearbombs function work only on squares that aren't bombs
char fillboard(char board[][22], int numofrows, int numofcols); //fiils the board with bombs, ' ',and the number of nearby bombs
void printWIN(char myboard[][22], int numofrows, int numofcols);
bool THEGAME(char user_board[][22], char myboard[][22], int numofrows, int numofcols, bool STOP_PROGRAM);
int floodFill(char myboard[][22], char user_board[][22], int numofrows, int numofcols, int rowchoice, int colchoice, int numofturns);


void main()
{
	int choice;//the option chosen from the main menu
	int numofrows = 22;
	int numofcols = 22;
	char myboard[22][22];//the hidden board with the bombs and number of nearby bombs
	char user_board[22][22];//the board the user sees and reveals a part of, each turn
	bool STOP_PROGRAM = false; //keeps the program running until a game is lost\won or the exit option in the main board has been chosen 
	while(STOP_PROGRAM != true)
	{
		Menu();
		scanf("%d", &choice);
		switch(choice)
		{
		case 0:
			printf("Goodbye, please come play again sometime. \n\n");
			STOP_PROGRAM = true;
			break;
		case 1://board of 8X8
		{
			numofrows = 8;
			numofcols = 8;
			STOP_PROGRAM = THEGAME(user_board, myboard, numofrows, numofcols, STOP_PROGRAM);
			break;
		}
		case 2: //board of 12X12
		{
			numofrows = 12;
			numofcols = 12;
			STOP_PROGRAM = THEGAME(user_board, myboard, numofrows, numofcols, STOP_PROGRAM);
			break;
		}
		case 3: //board of 15X15
		{
			numofrows = 15;
			numofcols = 15;
			STOP_PROGRAM = THEGAME(user_board, myboard, numofrows, numofcols, STOP_PROGRAM);
			break;
		}
		case 4://custom board of 1-22 rows or columns
		{
			printf("Choose a number for the rows and a number for the columns, between 1 - 22 \n");
			scanf("%d%d", &numofrows, &numofcols);
			if((numofcols == 1 && numofrows == 1) || (numofcols == 1 && numofrows == 2) || (numofcols == 2 && numofrows == 1))
			{
				printWIN(myboard, numofrows, numofcols);
				STOP_PROGRAM = true;
			}
			while(1 > numofrows || numofrows > 22 || 1 > numofcols || numofcols > 22)
			{
				printf("Please Choose Valid Number for the rows and a number for the columns!!, between 1 - 22 \n");
				scanf("%d%d", &numofrows, &numofcols);
			}
			STOP_PROGRAM = THEGAME(user_board, myboard, numofrows, numofcols, STOP_PROGRAM);
			break;
		}
		default://if the user chooses an invalid option from the main menu
		{
			printf("Please choose a Valid option for the board\n");
			break;
		}
		}
	}
}

void Menu()
{
	printf(" WELCOME TO MINESWEEPERS BY NIMROD! \n Please choose the size of the board you wish to play with: \n For a board of 8X8 type 1. \n For a board of 12X12 type 2. \n For a board of 15X15 type 3. \n For a cusmtom board (1-22 for row/column), type 4. \n To exit, type 0. \n");
}
char boardofX(char user_board[][22], int numofrows, int numofcols)
{
	int i, j;
	for(i = 0; i < numofrows; i++)
	{
		for(j = 0; j < numofcols; j++)
		{
			user_board[i][j] = 'X';
		}
	}
}
char blankboard(char myboard[][22], int numofrows, int numofcols)
{
	int i, j;
	for(i = 0; i < numofrows; i++)
	{
		for(j = 0; j < numofcols; j++)
		{
			myboard[i][j] = ' ';
		}
	}
}
void printtoplineboard(int numofcols)
{
	int i;
	printf("  ");
	for(i = 0; i < numofcols; i++)
	{
		printf("   %d", i);
	}
	printf("\n");
}
void printborderofboard(int numofcols)
{
	int i;
	printf("  __");
	for(i = 0; i < numofcols; i++)
	{
		printf("____");
	}
	printf("\n");
}
void printboard(char board[][22], int numofrows, int numofcols)
{
	int i, j;
	printtoplineboard(numofcols);
	printborderofboard(numofcols);
	for(i = 0; i < numofrows; i++)
	{
		printf("%3d", i);
		for(j = 0; j < numofcols; j++)
		{
			printf("|");
			printf(" %c ", board[i][j]);
		}
		printf("|\n");
	}
	printborderofboard(numofcols);
}
char addrandombombs(char myboard[][22], int numofrows, int numofcols)
{
	int bombs = 0;
	int irand, jrand;
	while(bombs <= (sqrt(numofcols * numofrows)))
	{
		irand = rand() % (numofrows);
		jrand = rand() % (numofcols);
		if(myboard[irand][jrand] != '*')
		{
			myboard[irand][jrand] = '*';
			bombs++;
		}
	}
}
void fillnearbombs(char myboard[][22], int row, int col, int numofrows, int numofcols)
{
	int i, j;
	int count = 0;
	for(i = row - 1; i <= row + 1; i++)
	{
		for(j = col - 1; j <= col + 1; j++)
		{
			if((0 <= i && i < numofrows) && (0 <= j && j < numofcols))
			{
				if(myboard[i][j] == '*')
				{
					count++;
				}
			}
		}
	}
	if(count == 0)
	{
		myboard[row][col] = ' ';
	}
	else
	{
		myboard[row][col] = count + '0';
	}
}
void countnearbombs(char myboard[][22], int numofrows, int numofcols)
{
	int i, j;
	for(i = 0; i < numofrows; i++)
	{
		for(j = 0; j < numofcols; j++)
		{
			if(myboard[i][j] != '*')
			{
				fillnearbombs(myboard, i, j, numofrows, numofcols);
			}
		}
	}
}
char fillboard(char myboard[][22], int numofrows, int numofcols)
{
	blankboard(myboard, numofrows, numofcols);
	addrandombombs(myboard, numofrows, numofcols);
	countnearbombs(myboard, numofrows, numofcols);
}
void printWIN(char myboard[][22], int numofrows, int numofcols)
{
	printf("\n\n\n   YOU WIN THE GAME!!!!!!\n\n\n");
	printboard(myboard, numofrows, numofcols);
}
bool THEGAME(char user_board[][22], char myboard[][22], int numofrows, int numofcols, bool STOP_PROGRAM)
{
	bool GAMEOVER = false;//keeps the turns in a loop until the game is won or lost without taking the player back to the main menu
	srand(time(NULL));//to randommally spawn bombs
	int rowchoice, colchoice;
	int shown;//number of squares revealed by the '-1' for rows option
	int numofturns = 1;//the game is won when you reveal all the squares with no bombs(calculated by number of squares - number of bombs)
	char FlagorOpen;
	boardofX(user_board, numofrows, numofcols);
	fillboard(myboard, numofrows, numofcols);
	printboard(user_board, numofrows, numofcols);
	printboard(myboard, numofrows, numofcols); //to print "cheat",the board with the answers (myboard)
	while(GAMEOVER != true)
	{
		printf("Please enter two numbers, first one for the row and the second one for the column:\n");
		scanf("%d%d", &rowchoice, &colchoice);
		if(rowchoice == -1)//function for revealing a number of squares as the number of columns recieved
		{
			int i, j;
			shown = 0;
			for(i = 0; ((i < numofrows) && (shown < colchoice)); i++)
			{
				for(j = 0; ((j < numofcols) && (shown < colchoice)); j++)
				{
					if((user_board[i][j] == 'X') && (myboard[i][j] != '*'))
					{
						user_board[i][j] = myboard[i][j];
						shown++;
					}
				}
			}
			numofturns = numofturns + colchoice;//to count it as turns for the win,VERY IMPORTANT.
			if(numofturns >= ((numofrows * numofcols) - (sqrt(numofrows * numofcols))))//This means the player won the game
			{
				printWIN(myboard, numofrows, numofcols);
				GAMEOVER = true;
			}
			printboard(user_board, numofrows, numofcols);
		}
		else if((rowchoice < -1) || (rowchoice >= numofrows) || (colchoice < 0) || (colchoice >= numofcols))
		{//if the row or column chosen is not within the board or '-1'
			printf("Please choose a valid row and column number in the board's range!\n\n");
			printboard(user_board, numofrows, numofcols);
		}
		else if(user_board[rowchoice][colchoice] != 'X' && user_board[rowchoice][colchoice] != 'F')
		{//if the square has already been revealed
			printf("Please choose a different row and column combination, this one was used before.\n\n");
			printboard(user_board, numofrows, numofcols);
		}
		else if((rowchoice >= 0) && (rowchoice < numofrows) && (colchoice >= 0) && (colchoice < numofcols))
		{//if the row and column chosen are valid 
			printf("  Please choose an action:\n  Type 'O' or 'o' to OPEN a square.\n  Type 'F' or 'f' to FLAG a suspected square.\n");
			scanf(" %c", &FlagorOpen);
			while(FlagorOpen != 'O' && FlagorOpen != 'o' && FlagorOpen != 'F' && FlagorOpen != 'f')
			{
				printf("  Invalid action! Please choose a Valid action:\n  Type 'O' or 'o' to OPEN a square.\n  Type 'F' or 'f' to FLAG a suspected square.\n");
				scanf(" %c", &FlagorOpen);
			}
			if(FlagorOpen == 'o' || FlagorOpen == 'O')
			{
				if(myboard[rowchoice][colchoice] == '*')
				{
					printf("      You hit a mine!\n\n");
					printf("        Game over!!\n\n");
					printboard(myboard, numofrows, numofcols);
					GAMEOVER = true;
				}
				else if(myboard[rowchoice][colchoice] == ' ')
				{
					numofturns = floodFill(myboard, user_board, numofrows, numofcols, rowchoice, colchoice, numofturns);
					if(numofturns >= ((numofrows * numofcols) - (sqrt(numofrows * numofcols))))//This means the player won the game
					{
						printWIN(myboard, numofrows, numofcols);
						GAMEOVER = true;//breaks the loop of turns
					}
					printboard(user_board, numofrows, numofcols);
				}
				else
				{
					user_board[rowchoice][colchoice] = myboard[rowchoice][colchoice];
					numofturns++;
					if(numofturns >= ((numofrows * numofcols) - (sqrt(numofrows * numofcols))))//This means the player won the game
					{
						printWIN(myboard, numofrows, numofcols);
						GAMEOVER = true;//breaks the loop of turns
					}
					printboard(user_board, numofrows, numofcols);
				}
			}
			else if(FlagorOpen == 'f' || FlagorOpen == 'F')
			{
				user_board[rowchoice][colchoice] = 'F';
				printboard(user_board, numofrows, numofcols);
			}
		}
	}
	STOP_PROGRAM = true;//closes the program
	return STOP_PROGRAM;
}

int floodFill(char myboard[][22], char user_board[][22], int numofrows, int numofcols, int rowchoice, int colchoice, int numofturns)
{
	if(myboard[rowchoice][colchoice] == ' ')
	{
		user_board[rowchoice][colchoice] = ' ';
		numofturns++;
		int i = 0, j = 0;
		for(i = rowchoice - 1; i <= rowchoice + 1; i++)
		{
			for(j = colchoice - 1; j <= colchoice + 1; j++)
			{
				if((0 <= i && i < numofrows) && (0 <= j && j < numofcols))
				{
					if(myboard[i][j] == ' ' && user_board[i][j] != ' ')
					{
						numofturns = floodFill(myboard, user_board, numofrows, numofcols, i, j, numofturns);
						user_board[i][j] = ' ';
					}
				}
			}
		}

	}
	return numofturns;
}
