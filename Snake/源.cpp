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

#define SCREEN_SNAKE 1
#define SCREEN_EMPTY 0
#define SCREEN_FOOD 2

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

int randomNumber(int max) {
	srand((int)time(0));
	return rand() % max;
}

void generateFood() {
	do {
		int x = randomNumber(SCREEN_LENGTH), y = randomNumber(SCREEN_LENGTH);
		food.x = x;
		food.y = y;
	} while (food.x == -1 || food.y == -1 || map[food.x][food.y] != SCREEN_EMPTY);
	map[food.x][food.y] = SCREEN_FOOD;
}

void render() {
	system("cls");
	printf("yzy tql \n");
	for (int b = 0; b < SCREEN_LENGTH+2;b++) {
		if (b == 0) {
			printf("©°");
		}
		else if (b== SCREEN_LENGTH+1) {
			printf("©´");
		}
		else {
			printf("¡ª");
		}
	}
	printf("\n");
	for (int y = 0; y < SCREEN_LENGTH; y++) {
		printf("©¦");
		for (int x = 0; x < SCREEN_LENGTH; x++) {
			switch (map[x][y]) {
				case SCREEN_SNAKE:
					if (snakeHead->x == x && snakeHead->y ==y) {
						printf("%s", SNAKE_HEAD);
					}
					else {
						printf("%s", SNAKE_BODY);
					}
					break;
				case SCREEN_EMPTY: printf("%s", "  "); break;
				case SCREEN_FOOD: printf("%s", SNAKE_FOOD); break;
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
			printf("¡ª");
		}
	}
}

int nextStep(int opt) {
	int result = RESULT_OK;
	Point previousOffset;
	previousOffset.x = snakeHead->x; previousOffset.y = snakeHead->y;
	switch (opt) {
		case MOVE_LEFT: snakeHead->x--; break;
		case MOVE_RIGHT: snakeHead->x++; break;
		case MOVE_UP: snakeHead->y--; break;
		case MOVE_DOWN:snakeHead->y++; break;
	}
	if (map[snakeHead->x][snakeHead->y] == SCREEN_SNAKE) {
		result = RESULT_DIE;
		return result;
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
	map[snakeHead->x][snakeHead->y] = SCREEN_SNAKE;
	map[previousTailOffset.x][previousTailOffset.y] = SCREEN_EMPTY;

	if (snakeHead->x >= SCREEN_LENGTH || snakeHead->y >= SCREEN_LENGTH) {
		result = RESULT_DIE;
	}
	else if (map[snakeHead->x][snakeHead->y] == SCREEN_FOOD) {
		result = RESULT_EAT;
	}
	return result;
}

int main() {
	printf("Press any key to Start the Game!\n");
	getchar();
	int lastMove = MOVE_RIGHT;
	int score = 0, speed = 1000, snakeLength = 2;

	food.x = -1;
	food.y = -1;
	Snake* snakeBottom = (Snake *)malloc(sizeof(Snake));
	snakeHead->x = SCREEN_LENGTH/2;
	snakeHead->y = SCREEN_LENGTH / 2;
	snakeBottom->x = SCREEN_LENGTH / 2 - 1;
	snakeBottom->y = SCREEN_LENGTH / 2;
	snakeBottom->nextSnake = NULL;
	snakeHead->nextSnake = snakeBottom;

	map[snakeHead->x][snakeHead->x] = SCREEN_SNAKE;
	map[snakeBottom->x][snakeBottom->y] = SCREEN_SNAKE;
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
			if (speed >= 300) speed *= 0.9;
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
	//todo: END SHOW RESULT
	getchar();
	return 0;
}