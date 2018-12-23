#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<conio.h>
#include<Windows.h>

#define SCREEN_LENGTH 12
#define SNAKE_BODY "¡ö"
#define SNAKE_FOOD "¡ï"
#define SNAKE_HEAD "¡ñ"

#define MOVE_LEFT 0
#define MOVE_RIGHT 1
#define MOVE_UP 2
#define MOVE_DOWN 3

#define RESULT_OK 1
#define RESULT_EAT 2
#define RESULT_DIE -1

#define MAP_SNAKE 1
#define MAP_EMPTY 0
#define MAP_FOOD 2

typedef struct Snake {
	int x;
	int y;
	Snake* nextSnake;
} Snake;

typedef struct Food {
	int x;
	int y;
} Food;

typedef struct Point {
	int x;
	int y;
} Point;

int map[SCREEN_LENGTH][SCREEN_LENGTH];
Food food;
Snake* snakeHead = (Snake *)malloc(sizeof(Snake));
int score = 0, speed = 1000, snakeLength = 2;

int randomNumber(int max) {
	srand((int)time(0));
	return rand() % max;
}

void generateFood() {
	do {
		int x = randomNumber(SCREEN_LENGTH), y = randomNumber(SCREEN_LENGTH);
		food.x = x;
		food.y = y;
	} while (food.x == -1 || food.y == -1 || map[food.x][food.y] != MAP_EMPTY);
	map[food.x][food.y] = MAP_FOOD;
}

void render() {
	system("cls");
	//printf("yzy tql \n");
	printf("Your score : %d\nSnake Length : %d\n\n",score,snakeLength);
	for (int b = 0; b < SCREEN_LENGTH+2;b++) {
		if (b == 0) {
			printf("©°");
		}
		else if (b== SCREEN_LENGTH+1) {
			printf("©´");
		}
		else {
			printf("©¤©¤");
		}
	}
	printf("\n");
	for (int y = 0; y < SCREEN_LENGTH; y++) {
		printf("©¦");
		for (int x = 0; x < SCREEN_LENGTH; x++) {
			switch (map[x][y]) {
				case MAP_SNAKE:
					if (snakeHead->x == x && snakeHead->y ==y) {
						printf("%s", SNAKE_HEAD);
					}
					else {
						printf("%s", SNAKE_BODY);
					}
					break;
				case MAP_EMPTY: printf("%s", "  "); break;
				case MAP_FOOD: printf("%s", SNAKE_FOOD); break;
			}
		}
		printf("©¦\n");
	}
	for (int b = 0; b < SCREEN_LENGTH+2; b++) {
		if (b == 0) {
			printf("©¸");
		}
		else if (b == SCREEN_LENGTH+1) {
			printf("©¼");
		}
		else {
			printf("©¤©¤");
		}
	}
}

int nextStep(int opt) {
	int result = RESULT_OK , addLengthFlag = 0;
	Point previousOffset;
	previousOffset.x = snakeHead->x; previousOffset.y = snakeHead->y;
	switch (opt) {
		case MOVE_LEFT: snakeHead->x--; break;
		case MOVE_RIGHT: snakeHead->x++; break;
		case MOVE_UP: snakeHead->y--; break;
		case MOVE_DOWN:snakeHead->y++; break;
	}
	if (map[snakeHead->x][snakeHead->y] == MAP_SNAKE) {
		result = RESULT_DIE;
		return result;
	}
	else if (map[snakeHead->x][snakeHead->y] == MAP_FOOD) {
		result = RESULT_EAT;
		addLengthFlag = 1;
	}
	Point previousTailOffset;
	Snake* nowSnake = snakeHead->nextSnake;
	while (nowSnake != NULL) {
		if (nowSnake->nextSnake == NULL) {
			previousTailOffset.x = nowSnake->x;
			previousTailOffset.y = nowSnake->y;
		}
		int changeX = previousOffset.x, changeY = previousOffset.y;
		previousOffset.x = nowSnake->x;
		previousOffset.y = nowSnake->y;
		nowSnake->x = changeX;
		nowSnake->y = changeY;
		nowSnake = nowSnake->nextSnake;
	}
	map[snakeHead->x][snakeHead->y] = MAP_SNAKE;

	if (addLengthFlag) {
		map[previousTailOffset.x][previousTailOffset.y] =MAP_SNAKE;
		Snake* ptrSnake = snakeHead->nextSnake;
		while (ptrSnake->nextSnake != NULL) {
			ptrSnake = ptrSnake->nextSnake;
		}
		Snake* newSnake = (Snake *)malloc(sizeof(Snake));
		newSnake->x = previousTailOffset.x;
		newSnake->y = previousTailOffset.y;
		newSnake->nextSnake = NULL;
		ptrSnake->nextSnake = newSnake;
	}
	else {
		map[previousTailOffset.x][previousTailOffset.y] = MAP_EMPTY;
	}

	if (result == RESULT_EAT) {
		return result;
	}
	if (snakeHead->x >= SCREEN_LENGTH || snakeHead->y >= SCREEN_LENGTH || snakeHead->x < 0 || snakeHead->y < 0) {
		result = RESULT_DIE;
	}
	return result;
}

int main() {
	printf("Press any key to Start the Game!\n");
	getchar();
	int lastMove = MOVE_RIGHT;

	food.x = -1;
	food.y = -1;
	Snake* snakeBottom = (Snake *)malloc(sizeof(Snake));
	snakeHead->x = SCREEN_LENGTH/2;
	snakeHead->y = SCREEN_LENGTH / 2;
	snakeBottom->x = SCREEN_LENGTH / 2 - 1;
	snakeBottom->y = SCREEN_LENGTH / 2;
	snakeBottom->nextSnake = NULL;
	snakeHead->nextSnake = snakeBottom;

	map[snakeHead->x][snakeHead->x] = MAP_SNAKE;
	map[snakeBottom->x][snakeBottom->y] = MAP_SNAKE;
	generateFood();
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
			if (speed >= 300) speed = speed * 9 / 10;
			generateFood();
		}
		Sleep(speed);
	}

	Snake* nowSnake = snakeHead;
	int ptr = 0;	
	while (nowSnake!= NULL) {
		Snake* previousSnake = nowSnake;
		nowSnake = nowSnake->nextSnake;
		free(previousSnake);
	}
	getchar();
	return 0;
}