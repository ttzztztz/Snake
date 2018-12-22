#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<Windows.h>

#define SCREEN_LENGTH 25
#define SNAKE_BODY "O"
#define SNAKE_FOOD "Y"

#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3

#define RESULT_OK 1
#define RESULT_EAT 2
#define RESULT_DIE -1

#define SCREEN_SNAKE 1
#define SCREEN_EMPTY 0
#define SCREEN_FOOD 2
typedef struct Point {
	int x;
	int y;
} Point;

int map[SCREEN_LENGTH][SCREEN_LENGTH];
Point snakeBodyBegin;
Point snakeBodyEnd;

void render() {
	system("cls");
	for (int y = 0; y < SCREEN_LENGTH; y++) {
		for (int x = 0; x < SCREEN_LENGTH; x++) {
			switch (map[x][y]) {
			case SCREEN_SNAKE: printf("%s", SNAKE_BODY); break;
			case SCREEN_EMPTY: printf("%s", " "); break;
			case SCREEN_FOOD: printf("%s", SNAKE_FOOD); break;
			}
		}
		printf("\n");
	}
}
int nextStep(int opt) {
	Point nextPoint = snakeBodyBegin, nextLastPoint = snakeBodyEnd;
	int result = RESULT_OK;
	switch (opt) {
	case MOVE_LEFT: nextPoint.x--; break;
	case MOVE_RIGHT: nextPoint.x++; break;
	case MOVE_UP: nextPoint.y++; break;
	case MOVE_DOWN:nextPoint.y--; break;
	}
	snakeBodyBegin = nextPoint;

	if (nextPoint.x >= SCREEN_LENGTH || nextPoint.y >= SCREEN_LENGTH) {
		result = RESULT_DIE;
	}
	else if (map[nextPoint.x][nextPoint.y] == 2) {
		result = RESULT_EAT;
	}
	return result;
}
int main() {
	int lastMove = MOVE_RIGHT;
	int score = 0, speed = 1000, snakeLength = 2;

	snakeBodyBegin.x = SCREEN_LENGTH / 2;
	snakeBodyBegin.y = SCREEN_LENGTH / 2;

	snakeBodyEnd.x = SCREEN_LENGTH / 2 - 1;
	snakeBodyEnd.y = SCREEN_LENGTH / 2;

	map[snakeBodyBegin.x][snakeBodyBegin.y] = SCREEN_SNAKE;
	map[snakeBodyEnd.x][snakeBodyEnd.y] = SCREEN_SNAKE;

	while (1) {
		if (GetAsyncKeyState(VK_UP)) {
			lastMove = MOVE_UP;
		}
		else if (GetAsyncKeyState(VK_DOWN)) {
			lastMove = MOVE_DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT)) {
			lastMove = MOVE_LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT)) {
			lastMove = MOVE_RIGHT;
		}
		int result = nextStep(lastMove);
		render();
		if (result == RESULT_DIE) {
			printf("\n YOU DIE!\n");
			break;
		}
		else if (result == RESULT_EAT) {
			if (!score) {
				score = 1;
			}
			else {
				score += score;
			}
			snakeLength++;
		}
		Sleep(speed);
	}
	getchar();
	return 0;
}