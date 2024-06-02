#include "lib.h"
#pragma comment(lib, "Winmm.lib")



int main()
{
	welcome();
	selectMode();
	return 0;
}

void selectMode()
{
	sndPlaySoundW(L".\\sounds\\mode\\chooseMode.wav", SND_FILENAME | SND_ASYNC);
	short mode = -1;
	while (mode > 2 || mode < 1)
	{
		printf("Choose your mode:\n 1.Single play\n 2.Versus play\n 3.Exit (why)\n");
		cin >> mode;
	}

	switch (mode)
	{
		case 1:
		{
			sndPlaySoundW(L".\\sounds\\mode\\singlePlay.wav", SND_FILENAME | SND_ASYNC);
			gameLogic();
			return;
		}
		case 2:
		{
			sndPlaySoundW(L".\\sounds\\mode\\vsPlay.wav", SND_FILENAME | SND_ASYNC);
			gameLogic2P();
			return;
		}
		case 3:
		{
			return;
		}
	}
};

void gameLogic()
{
	game playBoard = userMakeBoard();
	int moves = 0;
	int x1, y1, x2, y2;
	while (!isGameWon(playBoard))
	{
		clear_screen();

		coutBoard(playBoard);

		cout << "Pick the first card:\n";
		userPickSlot(playBoard, x1, y1);

		clear_screen();

		coutSecretBoard(playBoard, x1, y1);

		

		cout << "\nPick the second card:\n";
		userPickSlot(playBoard, x2, y2);
		while (x1 == x2 && y1 == y2)
		{
			cout << "You can't pick the same card twice! Pick a different one!\n";
			userPickSlot(playBoard, x2, y2);
		}
		

		coutSecretBoard(playBoard, x1, y1, x2, y2);

		if (checkForMatch(playBoard, x1, y1, x2, y2))
		{
			sndPlaySoundW(L".\\sounds\\Welcome.wav", SND_FILENAME | SND_ASYNC);
			cout << "It's a match!\n";
		}
		else
		{
			cout << "Not a match!\n";
		}

		moves++;

		this_thread::sleep_for(chrono::seconds(1));
	}
	endgameSoundLogic(playBoard, moves);
	if (perfectGame(playBoard, moves))
	{
		printf("%s!!! PERFECT GAME !!!%s\n", YELLOW_BG_COLOR ,RESET_BG_COLOR);
	}
	printf("%sCongratulations!%s You've won the game in%s %i %smoves.\n",YELLOW_COLOR,RESET_COLOR, YELLOW_COLOR, moves, RESET_COLOR );
	clearMemory(playBoard);
};

void gameLogic2P()
{
	game playBoard = userMakeBoard();
	int moves = 0;
	int player1Score = 0;
	int player2Score = 0;
	bool player1Turn = true;

	while (!isGameWon(playBoard))
	{
		int x1, y1, x2, y2;

		clear_screen();

		coutBoard(playBoard);

		if (player1Turn)
		{
			cout << "Player 1's turn.\n";
		}
		else
		{
			cout << "Player 2's turn.\n";
		}

		cout << "Pick the first card:\n";
		userPickSlot(playBoard, x1, y1);
		clear_screen();

		coutSecretBoard(playBoard, x1, y1);

		cout << "\nPick the second card:\n";
		userPickSlot(playBoard, x2, y2);
		while (x1 == x2 && y1 == y2)
		{
			cout << "You can't pick the same card twice! Pick a different one!\n";
			userPickSlot(playBoard, x2, y2);
		}

		coutSecretBoard(playBoard, x1, y1, x2, y2);

		if (checkForMatch(playBoard, x1, y1, x2, y2))
		{
			sndPlaySoundW(L".\\sounds\\match.wav", SND_FILENAME | SND_ASYNC);
			cout << "It's a match!\n";
			if (player1Turn)
			{
				player1Score++;
			}
			else
			{
				player2Score++;
			}
		}
		else
		{
			sndPlaySoundW(L".\\sounds\\nonmatch.wav", SND_FILENAME | SND_ASYNC);
			cout << "Not a match!\n";
			player1Turn = !player1Turn;
		}

		moves++;

		this_thread::sleep_for(chrono::seconds(1));
	}

	cout << "Congratulations! You've won the game in " << moves << " moves.\n";
	cout << "Player 1 Score: " << player1Score << "\n";
	cout << "Player 2 Score: " << player2Score << "\n";

	if (player1Score > player2Score)
	{

		cout << "Player 1 wins!\n";
	}
	else if (player2Score > player1Score)
	{
		cout << "Player 2 wins!\n";
	}
	else
	{
		cout << "It's a tie!\n";
	}

	clearMemory(playBoard);
}
