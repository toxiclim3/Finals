#pragma once
#include <iostream>//стандарт
#include <ctime>//для рандома
#include <random>//для рандома тоже
#include <chrono> //для отсрочки по времени и таймера
#include <Windows.h> //для звука
#include <filesystem> //для получения файла
#include <thread> //для задержки

using namespace std;

const char* BLACK_COLOR = "\033[1;30m";
const char* RED_COLOR = "\033[1;31m";
const char* GREEN_COLOR = "\033[1;32m";
const char* YELLOW_COLOR = "\033[1;33m";
const char* BLUE_COLOR = "\033[1;34m";
const char* FUSCHIA_COLOR = "\033[1;35m";
const char* TEAL_COLOR = "\033[1;36m";
const char* WHITE_COLOR = "\033[1;37m";

const char* WHITE_BG_COLOR = "\033[1;47m";
const char* YELLOW_BG_COLOR = "\033[1;43m";

const char* RESET_COLOR = "\033[0m";
const char* RESET_BG_COLOR = "\033[49m";

const char* ENCIRCLED = "\033[52m";
const char* UNCIRCLED = "\033[54m";

void clear_screen(char fill = ' ')
{ // Я это использую вместо system("cls"); потому что https://cplusplus.com/articles/j3wTURfi/ (я без малейшего понятия как этот код снизу работает)
	COORD tl = { 0,0 };
	CONSOLE_SCREEN_BUFFER_INFO s;
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(console, &s);
	DWORD written, cells = s.dwSize.X * s.dwSize.Y;
	FillConsoleOutputCharacter(console, fill, cells, tl, &written);
	FillConsoleOutputAttribute(console, s.wAttributes, cells, tl, &written);
	SetConsoleCursorPosition(console, tl);
}

struct game
{
	int** trueBoard;
	int** board;
	int sizex;
	int sizey;
};

int randomInt(int max = 10)
{
	static mt19937 mt{ static_cast<unsigned int>(time(0)) };
	uniform_int_distribution<> random{ 0, max - 1 };
	return random(mt);
}

void playRandomSound(int type = 1)
//все решения со случайным выбором файла в директории были слишком сложные, и мне кажется что вы бы не одобрили это
{
	switch (type)
	{
		case 1:
		{
			const int soundCount = 3;
			const wchar_t* path[soundCount] =
			{
				L".\\sounds\\gameOver\\soloOver\\gameOver1.wav" ,
				L".\\sounds\\gameOver\\soloOver\\gameOver2.wav" ,
				L".\\sounds\\gameOver\\soloOver\\gameOver3.wav"
			};

			sndPlaySoundW(path[randomInt(soundCount)], SND_FILENAME | SND_ASYNC);
			return;
		}
		case 2:
		{
			const int soundCount = 4;
			const wchar_t* path[soundCount] =
			{
				L".\\sounds\\gameOver\\wayToGo\\gameOver1.wav" ,
				L".\\sounds\\gameOver\\wayToGo\\gameOver2.wav" ,
				L".\\sounds\\gameOver\\wayToGo\\gameOver3.wav" ,
				L".\\sounds\\gameOver\\wayToGo\\gameOver4.wav"
			};

			sndPlaySoundW(path[randomInt(soundCount)], SND_FILENAME | SND_ASYNC);
			return;
		}

	default:
		break;
	}

}

bool perfectGame(game& playBoard,int moves)
{
	int moveFactor = ceil((playBoard.sizex * playBoard.sizey) / 2);
	return (moves == moveFactor);
}

void endgameSoundLogic(game& playBoard, int moves)
{
	int moveFactor = ceil((playBoard.sizex * playBoard.sizey) / 2);
	float goodGameFactor = 1.4;
	if (perfectGame(playBoard,moves))
	{
		sndPlaySoundW(L".\\sounds\\gameOver\\perfectGame.wav", SND_FILENAME | SND_ASYNC);
	}
	else if (moves <= moveFactor * goodGameFactor)
	{
		playRandomSound(2);
	}
	else
	{
		playRandomSound(1);
	}
}

void welcome()
{
	sndPlaySoundW(L".\\sounds\\Welcome.wav", SND_FILENAME | SND_ASYNC);
	printf("Welcome to...\n");
	printf("%sSUPER%sPAIRS %sDELUXE%s\n", FUSCHIA_COLOR, BLUE_COLOR, YELLOW_COLOR, RESET_COLOR);

	this_thread::sleep_for(chrono::seconds(1));
}

game makeBoard(int sizex, int sizey)
{
	game gameBoard;
	gameBoard.sizex = sizex;
	gameBoard.sizey = sizey;
	gameBoard.trueBoard = new int* [sizey];
	gameBoard.board = new int* [sizey];

	for (int i = 0; i < sizey; i++)
	{
		gameBoard.trueBoard[i] = new int[sizex];
		gameBoard.board[i] = new int[sizex];
		for (int j = 0; j < sizex; j++)
		{
			gameBoard.trueBoard[i][j] = -1;
			gameBoard.board[i][j] = -2;
		}
	}

	for (int i = 0; i < ((sizex * sizey) / 2); i++)
	{
		for (int j = 0; j < 2; j++)
		{
			bool foundSpot = 0;
			do
			{
				int x, y;
				x = randomInt(gameBoard.sizex);
				y = randomInt(gameBoard.sizey);
				if (gameBoard.trueBoard[y][x] == -1)
				{
					foundSpot = 1;
					gameBoard.trueBoard[y][x] = i;
				}
			} while (foundSpot == 0);
		}
	}

	return gameBoard;
}

game userMakeBoard()
{
	int sizex, sizey;
	bool validStep = 1, validBoard = 1;

	cout << "Please enter the board size: \n";

	do
	{
		do
		{
			cout << "Vertical size: \n";
			cin >> sizey;
			if (sizey <= 0)
			{
				validStep = 0;
				cout << "Size can't be <= 0\n";
			}
			else
			{
				validStep = 1;
			}
		} while (validStep == 0);

		validStep = 0;

		do
		{
			cout << "Horizontal size: \n";
			cin >> sizex;
			if (sizex <= 0)
			{
				validStep = 0;
				cout << "Size can't be <= 0\n";
			}
			else
			{
				validStep = 1;
			}
		} while (validStep == 0);

		if (((sizex * sizey) % 2) == 1)
		{
			cout << "That won't work, the board has to have an even amount of tiles, try a different size please \n";
			validBoard = 0;
		}
		else
		{
			validBoard = 1;
		}
	} while (validBoard == 0);

	return makeBoard(sizex, sizey);
}

void coutBoard(game& board)
{
	for (int i = 0; i < board.sizey; i++)
	{
		for (int j = 0; j < board.sizex; j++)
		{
			if (board.board[i][j] == -2)
			{
				printf("[#]");
			}
			else
			{
				printf("[%i]", board.board[i][j]);
			}
		}
		cout << endl;
	}
}

void coutSecretBoard(game& board, int x1 = -1, int y1 = -1, int x2 = -1, int y2 = -1)
{
	for (int i = 0; i < board.sizey; i++)
	{
		for (int j = 0; j < board.sizex; j++)
		{
			if ((j == x1 && i == y1) || (j == x2 && i == y2))
			{
				printf("(%i)", board.trueBoard[i][j]);
			}
			else if (board.board[i][j] == -2)
			{
				printf("[#]");
			}
			else
			{
				printf("[%i]", board.board[i][j]);
			}
		}
		cout << endl;
	}
}

void userPickSlot(game& board, int& x, int& y)
{
	bool validStep = 1;
	cout << "Please enter the column you want to pick: \n";

	do
	{
		cin >> x;
		x--;
		if (x < 0 || x >= board.sizex)
		{
			validStep = 0;
			cout << "Invalid column! \n";
		}
		else
		{
			validStep = 1;
		}
	} while (!validStep);

	cout << "Please enter the row you want to pick: \n";
	do
	{
		cin >> y;
		y--;
		if (y < 0 || y >= board.sizey)
		{
			validStep = 0;
			cout << "Invalid row! \n";
		}
		else
		{
			validStep = 1;
		}
	} while (!validStep);
	sndPlaySoundW(L".\\sounds\\click.wav", SND_FILENAME | SND_ASYNC);
}

bool checkForMatch(game& board, int x1, int y1, int x2, int y2)
{
	if (board.trueBoard[y1][x1] == board.trueBoard[y2][x2])
	{
		board.board[y1][x1] = board.trueBoard[y1][x1];
		board.board[y2][x2] = board.trueBoard[y2][x2];
		return true;
	}
	return false;
}

bool isGameWon(game& board)
{
	for (int i = 0; i < board.sizey; i++)
	{
		for (int j = 0; j < board.sizex; j++)
		{
			if (board.board[i][j] == -2)
			{
				return false;
			}
		}
	}
	return true;
}

void clearMemory(game& playBoard)
{
	for (int i = 0; i < playBoard.sizey; i++)
	{
		delete[] playBoard.trueBoard[i];
		delete[] playBoard.board[i];
	}
	delete[] playBoard.trueBoard;
	delete[] playBoard.board;
}



void achievementsMenu()
{

};

void selectMode();
void gameLogic2P();
void gameLogic();