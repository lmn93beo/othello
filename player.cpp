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
    
    board = new Board();
    

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
    //cerr << "Black: " << board->countBlack() << endl;
    //cerr << "White: " << board->countWhite() << endl;
    
    //cerr << "Opp move: " << opponentsMove->x << " " << opponentsMove->y << endl;
    
    // Make the opponent's move 
    
    board->doMove(opponentsMove, other_side);
    
    
    // Get the list of available moves
    vector<Move*> possible = board->legalMoves(own_side);
    //cerr << "Size is " << possible.size() << endl;
    
    //cerr << "Black: " << board->countBlack() << endl;
    //cerr << "White: " << board->countWhite() << endl;
    
    if (possible.size() == 0) {
        return NULL;
    }
    
    int max_val = -1000000;
    int best_move = -1;
	int value;
    
    // Go through the moves...
    for (unsigned int i = 0; i < possible.size(); i++) {
        Board *board_0 = board->copy();
        
        // Make the move
        board_0->doMove(possible[i], own_side);
        
        // Find the opponent's options
        vector<Move*> op_choices = board_0->legalMoves(other_side);
        
        // Find the min value of the choices
        int min_val = 10000000;
        for (unsigned int j = 0; j < op_choices.size(); j++) {
            Board *board_1 = board_0->copy();
            board_1->doMove(op_choices[j], other_side);
			
			
			if (testingMinimax) {
				value = board_1->count(own_side) - board_1->count(other_side);
			}
			else {
				value = board_1->heuristic(own_side);
			}
			
			if (value < min_val) {
				min_val = value;
			}
            
        }
        
        // Update max_val if necessary
        if (min_val > max_val) {
            max_val = min_val;
            best_move = i;
        }
        
        for (unsigned int i = 0; i < op_choices.size(); i++) {
            delete op_choices[i];
        }
    
    }
    
    // Make the move and return
    board->doMove(possible[best_move], own_side);
    //sleep(2);
      
    // Clean up
    for (unsigned int i = 0; i < possible.size(); i++) {
        if ((int)i != best_move) {
            delete possible[i];
        }
    }
    
    
    
    return possible[best_move];
}

// Set the board for the player
void Player::setBoard(char data[]) {
    board->setBoard(data);
}

