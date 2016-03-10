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
    
    
    // Make the opponent's move 
    
    board->doMove(opponentsMove, other_side);
    
	// Create a node
	Board *original = board->copy();
	Node *myNode = new Node(own_side, own_side, original);
	
	// Do minimax
	float val = myNode->minimax(5, true);
	cerr << val << endl;
	
	// Get a possible move
	Move *best = myNode->best_move(5, val);
	
	//sleep(1);
	
	board->doMove(best, own_side);
    
    // Clean up
    //myNode->deleteNode();
    delete myNode;
    
    
	return best;
	
}

// Set the board for the player
void Player::setBoard(char data[]) {
    board->setBoard(data, testingMinimax);
}


