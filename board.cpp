#include "board.h"
#include <cstdio>
#include <cmath>
#include <iostream>

using namespace std;

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board(bool minimax) {
    taken.set(3 + 8 * 3);
    taken.set(3 + 8 * 4);
    taken.set(4 + 8 * 3);
    taken.set(4 + 8 * 4);
    black.set(4 + 8 * 3);
    black.set(3 + 8 * 4);
    
    testingMinimax = minimax;
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

float Board::mobility(Side side) {
    Side other = (side == BLACK) ? (WHITE) : BLACK;
    
    if (numMoves(side) + numMoves(other) != 0) {
        return float(numMoves(side) - numMoves(other)); // / (numMoves(side) + numMoves(other));
    } else {
        return 0;
    }
}
            
float Board::corner_advantage(Side side) {
    Side other = (side == BLACK) ? (WHITE) : BLACK;
    int self_corners = get(side, 0, 0) + get(side, 0, 7) + get(side, 7, 0) + get(side, 7, 7);
    int other_corners = get(other, 0, 0) + get(other, 0, 7) + get(other, 7, 0) + get(other, 7, 7);
    
    return self_corners - other_corners;
}

float Board::parity(Side side) {
    Side other = (side == BLACK) ? (WHITE) : BLACK;
	if (count(side) == 0) {
		return -1000;
	}
    return float(count(side) - count(other)); // / (count(side) + count(other));
}

       


/* Returns a heuristic based on current configuration */
float Board::heuristic(Side side) {
    // Count corners

    if (testingMinimax) {
        return parity(side);
    }
    else {
        return 10*mobility(side) + 100*corner_advantage(side) + parity(side);
		
    }
}

/* Returns a vector of legal moves for side */
vector<Move*> Board::legalMoves(Side side) {
    vector<Move*> move_list;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *move = new Move(i, j);
            if (checkMove(move, side)) {
                move_list.push_back(move);
            }
            else {
                delete move;
            }
        }
    }
    return move_list;
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board(testingMinimax);
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    return taken[x + 8*y];
}

bool Board::get(Side side, int x, int y) {
    return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
}

void Board::set(Side side, int x, int y) {
    taken.set(x + 8*y);
    black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}

int Board::numMoves(Side side) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *move = new Move(i, j);
            if (checkMove(move, side)) {
                count += 1;
            }
            delete move;
        }
    }
    
    
    return count;
}
    

 
/*
 * Returns true if the game is finished; false otherwise. The game is finished 
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == NULL) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A NULL move means pass.
    if (m == NULL) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    return black.count();
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    return taken.count() - black.count();
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[], bool test) {
    taken.reset();
    black.reset();
    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            taken.set(i);
            black.set(i);
        } if (data[i] == 'w') {
            taken.set(i);
        }
    }
    testingMinimax = test;
}

void Board::setTestingMinimax(bool val) {
    testingMinimax = val;
}


// *********** THE NODE CLASS *******************
Node::Node(Side side, Side master, Board *newboard)
    {
        this->own_side = side;
		this->other_side = (side == BLACK) ? WHITE : BLACK;
		this->master_side = master;
        board = newboard;
    }

Node::~Node() {
    delete board;
    for (unsigned int i = 0; i < children.size(); i++) {
        if (children[i] != NULL) {
            delete children[i];
        }
    }
}

/** @brief Insert a node with a given board configuration as a child
*/
void Node::insert(Board *board) {
    Node *newnode = new Node(other_side, master_side, board);
    children.push_back(newnode);
}


void Node::deleteNode() // Clears the node's children list
{
	delete board;
    for (unsigned int i = 0; i < children.size(); i++) {
        if (children[i] != NULL) {
            children[i]->deleteNode();
        }
        delete children[i];
    }
}

void Node::printNode() {
    cerr << "number of children: " << children.size() << endl;
}


float Node::ab(int depth, float alpha, float beta, bool maximizingPlayer, Move *move,
			  int msLeft, time_t t0) {
	// Time checking
	clock_t t1;
	t1 = clock();
	float seconds = float(t1 - t0) / CLOCKS_PER_SEC;
	
	if (seconds * 1000 > msLeft) {
		return -1000000;
	}
	
	
	if (depth == 0 || board->numMoves(own_side)== 0) {
		return board->heuristic(master_side);
	}
	
	// Get the list of available moves
    vector<Move*> possible = board->legalMoves(own_side);
    
    // Go through the moves...
    for (unsigned int i = 0; i < possible.size(); i++) {        
        Board *next_board = board->copy();
        next_board->doMove(possible[i], own_side);
        insert(next_board);
    }
	
	
	// Get next move
	if (maximizingPlayer) {
		for (unsigned int i = 0; i < children.size(); i++) {
			float score = children[i]->ab(depth - 1, alpha, beta, false, NULL,
										 msLeft, t0);
			if (score == -1000000) {
				return score;
			}
			
			
			if (score > alpha) {
				alpha = score;
				if (move != NULL) {
					//cerr << "Setting move to be " << possible[i]->x << " " << possible[i]->y << endl;
					move->setX(possible[i]->x);
					move->setY(possible[i]->y);
				}
			}
			
			if (score > beta) {
				break;
			}
		}
		
		for (unsigned int i = 0; i < children.size(); i++) {
			delete children[i];
		}
		
		children.clear();
		
		for (unsigned int i = 0; i < possible.size(); i++) {
        	delete possible[i];
    	}
		
		return alpha;
	} else {
		for (unsigned int i = 0; i < children.size(); i++) {
			float score = children[i]->ab(depth - 1, alpha, beta, true, NULL,
										 msLeft, t0);
			if (score == -1000000) {
				return score;
			}
			
			if (score < beta) {
				beta = score;
			}
			
			if (score < alpha) {
				break;
			}
		}
		
		for (unsigned int i = 0; i < children.size(); i++) {
			delete children[i];
		}
		children.clear();
		
		for (unsigned int i = 0; i < possible.size(); i++) {
			delete possible[i];
		}
		return beta;
	}
}
















