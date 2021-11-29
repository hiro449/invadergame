#include<conio.h>
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

#define SCREEN_WIDTH (32)
#define SCREEN_HEIGHT (24)

#define INVADER_COLUMN (11)
#define INVADER_ROW (5)

#define FPS (1*10)
#define INTERVAL (1000/FPS)

#define BULLET_CHARGE 5


int screen[SCREEN_HEIGHT][SCREEN_WIDTH];

enum {
	TILE_NONE,
	TILE_INVADER,
	TILE_PLAYER,
	TILE_BULLET,
	TILE_ENEMY_BULLET,
	TILE_CHARGE,
	TILE_PERFORATE_BULLET,
	TILE_MAX
};

enum {
	RIGHT,
	DOWN,
	LEFT,
	DIRECTION_MAX
};

typedef struct {
	int  x, y;
}VEC2;

typedef struct {
	int  x, y;
	bool isdead;
}INVADER;

typedef struct {
	int  x, y;
}PLAYER;

typedef struct {
	int  x, y;
	bool isFired;
	bool charge;
	bool perforate;
}PLAYERBULLET;

typedef struct {
	int  x, y;
	bool isFired;
}ENEMYBULLET;

PLAYERBULLET playerBullet;

ENEMYBULLET enemyBullet[INVADER_COLUMN];


INVADER invaders[INVADER_ROW][INVADER_COLUMN];

PLAYER player;


VEC2 directions[] = {
	{1,0},//RIGHT,
	{0,1},//DOWN,
	{-1,0},//LEFT,
};

int invaderDirection;


const char* tileAA[TILE_MAX] = {
	"Å@", //TILE_NONE
	"Åö", //TILE_INVADER
	"Å¢",   //TILE_PLAYER
	"Å™",//TILE_BULLET
	"Å´",//TILE_ENEMY_BULLET
	"Åî",//TILE_CHARGE
	"Åù",//TILE_PERFORATE_BULLET
};

int invaderCount;

int chargeBulletCount;

bool playerBulletIntersectInvaders() {
	for (int y = 0; y < INVADER_ROW; y++)
		for (int x = 0; x < INVADER_COLUMN; x++)
			if ((invaders[y][x].x == playerBullet.x)
				&& (invaders[y][x].y == playerBullet.y)
				&& (!invaders[y][x].isdead)) {
				playerBullet.isFired = false;
				invaders[y][x].isdead = true;
				return true;


			}
	return false;
}

bool chargeBulletCountBiem() {
	for (int y = 0; y < INVADER_ROW; y++)
		for (int x = 0; x < INVADER_COLUMN; x++)
			if ((invaders[y][x].x == playerBullet.x)
				&& (invaders[y][x].y == playerBullet.y)
				&& (!invaders[y][x].isdead)) {
				//playerBullet.isFired = false;
				invaders[y][x].isdead = true;
				return true;
			}
	return false;
}

bool enemyBulletIntersect() {
	for (int x = 0; x < INVADER_COLUMN; x++)
		if ((enemyBullet[x].isFired) && (enemyBullet[x].x == player.x) && (enemyBullet[x].y == player.y)) {
			return true;
		}
	return false;
}

void Draw() {
	memset(screen, 0, sizeof screen);

	for (int y = 0; y < INVADER_ROW; y++)
		for (int x = 0; x < INVADER_COLUMN; x++)
			if (!invaders[y][x].isdead)
				screen[invaders[y][x].y][invaders[y][x].x] = TILE_INVADER;

	screen[player.y][player.x] = TILE_PLAYER;

	if (playerBullet.isFired)
		screen[playerBullet.y][playerBullet.x] = TILE_BULLET;

	if (playerBullet.charge)
		screen[player.y - 1][player.x] = TILE_CHARGE;



	for (int x = 0; x < INVADER_COLUMN; x++)
		if (enemyBullet[x].isFired)
			screen[enemyBullet[x].y][enemyBullet[x].x] = TILE_ENEMY_BULLET;

	system("cls");
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++)
			printf("%s", tileAA[screen[y][x]]);
		printf("\n");
	}


}

void init() {
	for (int y = 0; y < INVADER_ROW; y++)
		for (int x = 0; x < INVADER_COLUMN; x++) {
			invaders[y][x].x = x * 2;
			invaders[y][x].y = y * 2;

			invaders[y][x].isdead = false;
		}

	playerBullet.isFired = false;

	for (int x = 0; x < INVADER_COLUMN; x++)
		enemyBullet[x].isFired = false;

	chargeBulletCount = 0;



	playerBullet.charge = false;

	invaderDirection = RIGHT;

	player.x = SCREEN_WIDTH / 2;
	player.y = SCREEN_HEIGHT - 3;

	int invaderCount = 0;

	Draw();
}





int main() {
	init();


	clock_t lasttimeclock = clock();
	while (1) {
		clock_t nowclock = clock();
		if (nowclock >= lasttimeclock + INTERVAL) {
			lasttimeclock = nowclock;

			if (playerBullet.isFired) {
				playerBullet.y--;
				if (playerBullet.y < 0)
					playerBullet.isFired = false;

				if (chargeBulletCount % BULLET_CHARGE == 0) {
					chargeBulletCountBiem();
				}
				else
					playerBulletIntersectInvaders();
			}




			invaderCount++;

			if (invaderCount > 10) {
				invaderCount = 0;

				int nextDirection = invaderDirection;
				for (int y = 0; y < INVADER_ROW; y++)
					for (int x = 0; x < INVADER_COLUMN; x++) {


						invaders[y][x].x += directions[invaderDirection].x;
						invaders[y][x].y += directions[invaderDirection].y;
						switch (invaderDirection) {
						case RIGHT:
							if (invaders[y][x].x >= SCREEN_WIDTH - 1)
								nextDirection = DOWN;
							break;
						case DOWN:
							if (invaders[y][x].x >= SCREEN_WIDTH - 1)
								nextDirection = LEFT;
							if (invaders[y][x].x <= 0)
								nextDirection = RIGHT;
							break;
						case LEFT:
							if (invaders[y][x].x <= 0)
								nextDirection = DOWN;
							break;

						}
					}

				invaderDirection = nextDirection;



				for (int x = 0; x < INVADER_COLUMN; x++) {
					if (!enemyBullet[x].isFired) {
						if (rand() % INVADER_COLUMN)
							continue;
						int invaderRow = -1;
						for (int y = 0; y < INVADER_ROW; y++) {
							if (!invaders[y][x].isdead)
								invaderRow = y;
						}
						if (invaderRow > 0) {
							enemyBullet[x].isFired = true;
							enemyBullet[x].x = invaders[invaderRow][x].x;
							enemyBullet[x].y = invaders[invaderRow][x].y + 1;
						}

					}
				}


			}

			for (int x = 0; x < INVADER_COLUMN; x++)
				if (enemyBullet[x].isFired) {
					enemyBullet[x].y++;

					if (enemyBullet[x].y >= SCREEN_HEIGHT)
						enemyBullet[x].isFired = false;


				}

			if (enemyBulletIntersect()) {
				init();
				lasttimeclock = clock();
				continue;
			}
			Draw();
		}

		if (_kbhit()) {
			switch (_getch()) {


			case 'd':
				player.x++;
				break;


			case 'a':
				player.x--;
				break;
			default:

				if (playerBullet.isFired)
					break;
			
				playerBullet.x = player.x;
				playerBullet.y = player.y - 1;
				playerBullet.isFired = true;
				chargeBulletCount++;

				if (chargeBulletCount % (BULLET_CHARGE - 1) == 0) {
					playerBullet.charge = true;
		
				}
				else
					playerBullet.charge = false;

				if (chargeBulletCount % (BULLET_CHARGE + 1) == 0) {
					chargeBulletCount = 1;
				}
	
			}
			

			if (player.x < 0)
				player.x = 0;
			if (player.x >= SCREEN_WIDTH)
				player.x = SCREEN_WIDTH - 1;

			if (enemyBulletIntersect()) {
				init();
				continue;
			}
			Draw();

		}
	}



}

