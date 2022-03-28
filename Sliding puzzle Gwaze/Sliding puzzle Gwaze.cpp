#include<bangtal.h>
#include<stdlib.h>
#include<time.h>

SceneID scene;
ObjectID game_board[9], init_board[9];
ObjectID start, end;
TimerID timer1, timer2, timer3;

const char* board_image[9] = {
	"image\\1번.jpg",
	"image\\2번.jpg",
	"image\\3번.jpg",
	"image\\4번.jpg",
	"image\\5번.jpg",
	"image\\6번.jpg",
	"image\\7번.jpg",
	"image\\8번.jpg",
	"image\\9번.jpg"
};
const int board_x[9] = { 300, 527, 754, 300, 527, 754, 300, 527, 754 };
const int board_y[9] = { 360, 360, 360, 180, 180, 180, 0,0,0 };

int blank;
int index;
int mixcount;

int board_index(ObjectID object) {
	for (int i = 0; i < 9; i++) {
		if (game_board[i] == object) return i;
	}
	return -1;
}

void board_move(int index) {
	ObjectID t = game_board[blank];
	game_board[blank] = game_board[index];
	game_board[index] = t;

	locateObject(game_board[blank], scene, board_x[blank], board_y[blank]);
	locateObject(game_board[index], scene, board_x[index], board_y[index]);

	blank = index;
}

bool movable(int index) {
	if (index < 0 or index > 8) return false;

	// 좌 우 상 하
	if (blank % 3 != 0 && blank - 1 == index) return true;
	if (blank % 3 != 2 && blank + 1 == index) return true;
	if (blank / 3 != 0 && blank - 3 == index) return true;
	if (blank / 3 != 2 && blank + 3 == index) return true;

	return false;
}

void board_mix() {
	do {
		switch (rand() % 4) {
		case 0: index = blank - 1; break;
		case 1: index = blank + 1; break;
		case 2: index = blank - 3; break;
		case 3: index = blank + 3; break;
		}
	} while (movable(index) == false);
	board_move(index);
}

bool completed() {
	for (int i = 0; i < 9; i++) {
		if (game_board[i] != init_board[i]) return false;
	}

	return true;
}

void mouseCallback(ObjectID object, int x, int y, MouseAction action) {
	if (object == start) {
		hideObject(start);
		hideObject(end);
		mixcount = 2;
		setTimer(timer1, 0.1f);
		startTimer(timer1);
	}
	else if (object == end) {
		endGame();
	}
	else {
		int index = board_index(object);
		if (movable(index)) {
			board_move(index);

			if (completed()) {
				showMessage("completed!");

				setObjectImage(start, "image\\Restart.png");
				showObject(start);
				showObject(end);
			}
		}
	}
}

void timerCallback(TimerID timer) {
	mixcount--;

	if (mixcount >= 0) {
		board_mix();

		setTimer(timer1, 0.1f);
		startTimer(timer1);
	}
}

int main() {
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene = createScene("Scene", "image\\background.png");

	for (int i = 0; i < 9; i++) {
		game_board[i] = createObject(board_image[i]);
		init_board[i] = game_board[i];
		locateObject(game_board[i], scene, board_x[i], board_y[i]);
		showObject(game_board[i]);
	}

	start = createObject("image\\Start.png");
	locateObject(start, scene, 540, 100);
	showObject(start);

	end = createObject("image\\End.png");
	locateObject(end, scene, 540, 60);
	showObject(end);

	blank = 8;
	hideObject(game_board[blank]);

	timer1 = createTimer(0.1f);

	startGame(scene);
}