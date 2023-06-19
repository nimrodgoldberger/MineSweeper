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
#define ROWS 8
#define COLS 8
void printboard(char board[][COLS]); //prints the board chosen
char blankboard(char board[][COLS]); // turns all the board's squares to ' '.
char fillboard(char board[][COLS]); //fiils the board with bombs, ' ',and the number of nearby bombs
char addrandombombs(char board[][COLS]); //randomly adds 8 bombs to the board
void fillnearbombs(char myboard[][COLS], int row, int col); //fills the board with the number of nearbombs of each square
void countnearbombs(char myboard[][COLS]); //makes the fillnearbombs function work only on squares that aren't bombs
char boardofX(char user_board[][COLS]);//fills the userboard with 'X's
void Menu();//prints the main menu

void main()
{
	int choice;//the option chosen from the main menu
	bool RUNPROGRAM = false; //keeps the program running until a game is lost\won or the exit option in the main board has been chosen 
	bool GAMEOVER = false;//keeps the turns in a loop until the game is won or lost without taking the player back to the main menu
	srand(time(NULL));//to randommally spawn 8 bombs
	int rowchoice, colchoice;
	int shown;//number of squares revealed by the '-1' for rows option
	int numofturns = 0;//the game is won when you reach 56 turns
	char myboard[ROWS][COLS];//the hidden board with the bombs and number of nearby bombs
	char user_board[ROWS][COLS];//the board the user sees and reveals a part of, each turn

	while(RUNPROGRAM != true)
	{
		Menu();
		scanf("%d", &choice);
		switch(choice)
		{
		case 0:
			printf("Goodbye, please come play again sometime. \n\n");
			RUNPROGRAM = true;
			break;
		case 1://board of 8X8
		{
			boardofX(user_board);
			fillboard(myboard);
			printboard(user_board);
			//printboard(myboard); //to print "cheat",the board with the answers (myboard)
			while(GAMEOVER != true)
			{
				printf("Please enter two numbers, first one for the row and the second one for the column:\n");
				scanf("%d%d", &rowchoice, &colchoice);
				if(rowchoice == -1)//function for revealing a number of squares as the number of columns recieved
				{
					int i, j;
					shown = 0;
					for(i = 0; ((i < ROWS) && (shown < colchoice)); i++)
					{
						for(j = 0; ((j < COLS) && (shown < colchoice)); j++)
						{
							if((user_board[i][j] == 'X') && (myboard[i][j] != '*'))
							{
								user_board[i][j] = myboard[i][j];
								shown++;
							}
						}
					}
					numofturns = numofturns + colchoice;//to count it as turns for the win,VERY IMPORTANT.
					if(numofturns == 56)
					{
						GAMEOVER = true;
					}
					printboard(user_board);
				}
				else if((rowchoice < -1) || (rowchoice >= ROWS) || (colchoice < 0) || (colchoice >= COLS))
				{//if the row or column chosen is not within the board or '-1'
					printf("Please choose a valid row and column number in the board's range!\n\n");
					printboard(user_board);
				}
				else if(user_board[rowchoice][colchoice] != 'X')
				{//if the square has already been revealed
					printf("Please choose a different row and column combination, this one was used before.\n\n");
					printboard(user_board);
				}
				else if(myboard[rowchoice][colchoice] == '*')
				{//this means game over, when you hit a mine
					printf("      You hit a mine!\n\n");
					printf("        Game over!!\n\n");
					printboard(myboard);
					GAMEOVER = true;
				}
				else
				{//if the row and column chosen are valid and don't hide a bomb
					user_board[rowchoice][colchoice] = myboard[rowchoice][colchoice];
					printboard(user_board);
					numofturns++;
					if(numofturns == 56)//This means the player won the game
					{
						printf("   YOU WIN THE GAME!!!!!!\n\n\n");
						printboard(myboard);
						GAMEOVER = true;//breaks the loop of turns
					}
				}
			}
			RUNPROGRAM = true;//closes the program
			break;
		}
		case 2://not avaliable for now
		case 3:
		case 4:
			printf("This option is currently under development. ''Please wait for part 2''.\n\n");
			break;
		default://if the user chooses an invalid option from the main menu
			printf("Please choose a Valid option for the board\n");
			break;
		}
	}
}

void Menu()
{
	printf(" WELCOME TO MINESWEEPERS BY NIMROD! \n Please choose the size of the board you wish to play with: \n For a board of 8X8 type 1. \n For a board of 12X12 type 2. \n For a board of 15X15 type 3. \n For a cusmtom board (1-22 for row/column), type 4. \n To exit, type 0. \n");
}

char addrandombombs(char board[][COLS])
{
	int bombs = 0;
	int irand, jrand;
	while(bombs < COLS)
	{
		irand = rand() % (ROWS);
		jrand = rand() % (COLS);
		if(board[irand][jrand] != '*')
		{
			board[irand][jrand] = '*';
			bombs++;
		}
	}
}

char blankboard(char board[][COLS])
{
	int i, j;
	for(i = 0; i < ROWS; i++)
	{
		for(j = 0; j < COLS; j++)
		{
			board[i][j] = ' ';
		}
	}
}

char fillboard(char board[][COLS])
{
	blankboard(board);
	addrandombombs(board);
	countnearbombs(board);
}

void printboard(char board[][COLS])
{
	int i, j;
	printf("     0   1   2   3   4   5   6   7\n");
	printf("  __________________________________\n");
	for(i = 0; i < ROWS; i++)
	{
		printf("  %d", i);
		for(j = 0; j < COLS; j++)
		{
			printf("|");
			printf(" %c ", board[i][j]);
		}
		printf("|\n");
	}
	printf("  __________________________________\n\n\n\n");
}

char boardofX(char user_board[][COLS])
{
	int i, j;
	for(i = 0; i < ROWS; i++)
	{
		for(j = 0; j < COLS; j++)
		{
			user_board[i][j] = 'X';
		}
	}
}

void fillnearbombs(char myboard[][COLS], int row, int col)
{
	int i, j;
	int count = 0;
	for(i = row - 1; i <= row + 1; i++)
	{
		for(j = col - 1; j <= col + 1; j++)
		{
			if((0 <= i && i < ROWS) && (0 <= j && j < COLS))
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

void countnearbombs(char myboard[][COLS])
{
	int i, j;
	for(i = 0; i < ROWS; i++)
	{
		for(j = 0; j < COLS; j++)
		{
			if(myboard[i][j] != '*')
			{
				fillnearbombs(myboard, i, j);
			}
		}
	}
}
