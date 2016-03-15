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
	clock_t t0;
	t0 = clock();  /* get current time */
	
    board->doMove(opponentsMove, other_side);
    
	// Create a node
	Board *original = board->copy();
	Node *myNode = new Node(own_side, own_side, original);
	
    Move *best = new Move(0,0);
	Move *to_return = new Move(0,0);
	
	
	int depth;
	int time_allowed, max_depth;
	float val;
    
    // Current move number
	int num_moves = (board->count(BLACK) + board->count(WHITE) - 4) / 2;
    
    // Allocate time and maximum depth based on number of moves	
	if (num_moves < 3) {
        depth = 6;
		time_allowed = 20000;
		max_depth = 6;
	}
	else if (num_moves < 6) {
        depth = 5;
		time_allowed = 30000;
		max_depth = 7;
	}
	else if (num_moves < 18) {
        depth = 5;
		time_allowed = 60000;
		max_depth = 7;
	} 
	else if (num_moves < 25) {
        time_allowed = msLeft / (30 - num_moves) * 2;
		cerr << "Time: " << time_allowed << endl;
        
		max_depth = 64 - (board->count(BLACK) + board->count(WHITE));
        depth = min(5, max_depth);
	}
    else {
        time_allowed = msLeft / (30 - num_moves);
        max_depth = 64 - (board->count(BLACK) + board->count(WHITE));
        depth = min(5, max_depth);
    }
    
  		
	while (true) {
		val = myNode->ab(depth, -100000, 100000, true, best, time_allowed, t0);
        cerr << "Depth completed: " << depth << endl;
		if (int(val) > -900000) {
			to_return->x = best->x;
			to_return->y = best->y;
			
				if (depth >= max_depth) {
					if (num_moves >= 16) {
						cerr << "Minimax triggered" << endl;
						testingMinimax = true;
					}
					board->doMove(to_return, own_side);
                    
					// Clean up
					delete myNode;
					delete best;

					return to_return;
				}
			depth += 1;
		}
		else {
			board->doMove(to_return, own_side);
          
            // Trigger testminimax mode towards endgame
            if (depth > max_depth && num_moves >= 16) {
                cerr << "Minimax triggered" << endl;
                testingMinimax = true;
            }
			// Clean up
			delete myNode;
			delete best;
			
			return to_return;
		}
	}
}

// Set the board for the player
void Player::setBoard(char data[]) {
    board->setBoard(data, testingMinimax);
}