#include <raylib.h>
#include <stdbool.h>

#define WIDTH 1200.0
#define HEIGHT 1200.0

#define CELL_SIZE 2
#define LAST_NUMBER 450000

typedef enum { TOP,
			   LEFT,
			   BOTTOM,
			   RIGHT } Direction;

Direction next_dir(Direction current) { return (current + 1) % 4; }

bool is_prime(int n) {
	if (n < 2) {
		return false;
	}

	for (int i = 2; i * i <= n; i++) {
		if (n % i == 0) {
			return false;
		}
	}

	return true;
}

bool prime[LAST_NUMBER];

void fill_primes(int start_idx, int end_idx) {
	for (int i = start_idx; i <= end_idx; i++) {
		prime[i] = is_prime(i + 1);
	}
}

int draw_spiral_until(int last_number) {
	ClearBackground(WHITE);

	// Start from the center.
	int x = WIDTH / 2.0;
	int y = HEIGHT / 2.0;

	// Spiral logic.
	Direction dir = RIGHT;
	int sides_completed = 0;
	int side_length = 1;
	int side_steps = 0;

	for (int idx = 0; idx < last_number; idx++) {
		if (prime[idx]) {
			DrawRectangle(x, y, CELL_SIZE, CELL_SIZE, BLACK);
		}

		switch (dir) {
		case TOP:
			y -= CELL_SIZE;
			break;
		case LEFT:
			x -= CELL_SIZE;
			break;
		case BOTTOM:
			y += CELL_SIZE;
			break;
		case RIGHT:
			x += CELL_SIZE;
			break;
		}

		if (++side_steps == side_length) {
			dir = next_dir(dir);
			side_steps = 0;
			sides_completed++;

			if (sides_completed == 2) {
				sides_completed = 0;
				side_length++;
			}
		}
	}

	return side_length;
}

int main(void) {
	SetTraceLogLevel(LOG_WARNING);
	InitWindow(WIDTH, HEIGHT, "Ulam spiral");

	Camera2D camera = {0};
	camera.target = (Vector2){WIDTH / 2, HEIGHT / 2};
	camera.offset = (Vector2){WIDTH / 2, HEIGHT / 2};
	camera.zoom = 1.0f;

	int current_last_number = 1;
	fill_primes(0, current_last_number);

	double last_time = GetTime();
	while (!WindowShouldClose()) {
		BeginDrawing();
		BeginMode2D(camera);

		int spiral_side_length = draw_spiral_until(current_last_number);

		double now = GetTime();
		if (now - last_time >= 0.1) {
			last_time = now;

			int cells_increment = spiral_side_length * 50;
			if (current_last_number + cells_increment < LAST_NUMBER) {
				fill_primes(current_last_number, current_last_number + cells_increment);
				current_last_number += cells_increment;
			}

			double spiral_size = spiral_side_length * CELL_SIZE;
			camera.zoom = WIDTH / spiral_size;
		}

		EndMode2D();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}