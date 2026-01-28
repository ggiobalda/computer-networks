#include "board.h"

int main() {

	Board* kanban = create_board(BOARD_PORT);

	// creazione di qualche card iniziale
	for (int i = 0; i < 5; i++) {
		char desc[MAX_CHARS];
		sprintf(desc, "Card%d", i);
		add_card(kanban, TODO, desc);
	}

	print_board(kanban);

	return 0;
}
