#include <cstdio>
#include "common.h"
#include "player.h"
#include "board.h"

// Use this file to test your minimax implementation (2-ply depth, with a
// heuristic of the difference in number of pieces).
int main(int argc, char *argv[]) {

    // Create board with example state. You do not necessarily need to use
    // this, but it's provided for convenience.
    char boardData[64] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', 'w', 'b', ' ', ' ', ' ', ' ', 
        ' ', ' ', 'b', 'b', 'b', ' ', ' ', ' ', 
        ' ', ' ', ' ', 'b', 'w', ' ', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
    };
    /*
    char boardData[64] = {
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
        ' ', 'b', ' ', ' ', ' ', ' ', ' ', ' ', 
        'b', 'w', 'b', 'b', 'b', 'b', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', 
        ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '
    };*/ 
    
    
    
    Board *board = new Board(true);
    board->setBoard(boardData, true);
	
	printf("Test board diff = %f\n", board->heuristic(WHITE));

    // Initialize player as the white player, and set testing_minimax flag.
    Player *player = new Player(WHITE);
	
	
	
    player->testingMinimax = false;
	//player->board->setTestingMinimax(true);
	player->setBoard(boardData);
	//player->setTree(board, 3);


    /** 
     * TODO: Write code to set your player's internal board state to the 
     * example state.
     */

    // Get player's move and check if it's right.
	printf("Testing...\n");
    Move *move = player->doMove(NULL, 30000);

    if (move != NULL && move->x == 1 && move->y == 1) {
        printf("Correct move: (1, 1)\n");
    } else {
        printf("Wrong move: got ");
        if (move == NULL) {
            printf("PASS");
        } else {
            printf("(%d, %d)", move->x, move->y);
        }
        printf(", expected (1, 1)\n");
    }

    // Clean up
    delete board;
    delete player;
    delete move;
    
    return 0;
}
