#include "player.h"
#include <cstdio>
#include <stdio.h>
#include <time.h>
#include <unistd.h>


/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish 
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;
    
    own_side = side;
    other_side = (side == BLACK) ? WHITE : BLACK;
    
    board = new Board(testingMinimax);
    

    /* 
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */
    
}

/*
 * Destructor for the player.
 */
Player::~Player() {
    delete board;
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be NULL.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return NULL.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {
    /* 
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */ 
    //unsigned t0 = clock(), t1;
    //cerr << msLeft << endl;
    // Make the opponent's move 
    
    board->doMove(opponentsMove, other_side);
    
	// Create a node
	Board *original = board->copy();
	Node *myNode = new Node(own_side, own_side, original);
	
    Move *best = new Move(0,0);
    // If more than 5 min left, default depth 8, else, do deepening
    float val;
	//int depth_to_search = 7;
	/*while (true) {
		// Do minimax
		float val;

		cerr << "Trying depth of " << depth_to_search << endl;

		try {
			val = myNode->ab(depth_to_search, -100000, 100000, true);
		} catch (const std::bad_alloc& e) {
			val = -100000;
			cerr << "Something wrong.. " << endl;
		}

		//t1 = clock();
		//cerr << float(t1 - t0) / CLOCKS_PER_SEC * 1000 << endl;

		if (val == -100000 || depth_to_search > 6) {//float(t1 - t0) / CLOCKS_PER_SEC * 1000 > msLeft) {
			break;
		}
		else {
			// Get a possible move
			best = myNode->best_move(val);

			depth_to_search += 1;
		}
    }*/
    
	if (board->count(own_side) < 6) {
		val = myNode->ab(6, -100000, 100000, true, best);
	}
	
	else {
		val = myNode->ab(6, -100000, 100000, true, best);
	} 
	board->doMove(best, own_side);
	
    // Clean up
    delete myNode;
	return best;
}

// Set the board for the player
void Player::setBoard(char data[]) {
    board->setBoard(data, testingMinimax);
}


