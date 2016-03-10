#include "board.h"
#include <cstdio>
#include <cmath>

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
    //printf("Done?\n");
    
    testingMinimax = minimax;
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}


/* Returns a heuristic based on current configuration */
float Board::heuristic(Side side) {
    // Count corners
    int corners = get(side, 0, 0) + get(side, 0, 7) + get(side, 7, 0) + get(side, 7, 7);
    
    // Count next to corners
    int corner_adj = get(side, 0, 1) + get(side, 1, 0) + get(side, 1, 1) +
        get(side, 0, 6) + get(side, 1, 7) + get(side, 1, 6) +
        get(side, 6, 0) + get(side, 7, 1) + get(side, 6, 1) +
        get(side, 6, 6) + get(side, 6, 7) + get(side, 7, 6);
    
    // Count sides and next to sides
    int sides = 0;
    int intermediates = 0;
    for (int i = 1; i < 7; i++) {
        sides += get(side, 0, i) + get(side, 7, i) + get(side, i, 0) + get(side, i, 7);
        intermediates += get(side, 1, i) + get(side, 6, i) + get(side, i, 1) + get(side, i, 6);
    }
    
    
    
    int difference;
    
	if (side == WHITE) {
		difference = count(WHITE) - count(BLACK);
	} 
	else 
	{
		difference = count(BLACK) - count(WHITE);
	}
    
    if (testingMinimax) {
        return difference;
    }
    else {
        return 10 * corners - 4 * corner_adj + 5 * sides - 2 * intermediates + 0.1 * difference;
    }
}

/* Returns a vector of legal moves for side */
vector<Move*> Board::legalMoves(Side side) {
    vector<Move*> move_list;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *move = new Move(i, j);
            if (checkMove(move, side)) {
                //printf("Yup %d %d\n", i, j);
                move_list.push_back(move);
            }
            else {
                delete move;
            }
        }
    }
    //for (unsigned int i = 0; i < move_list.size(); i++) {
        //printf("Move: %d, %d\n", move_list[i]->x, move_list[i]->y);
    //}
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
        //printf("diff 0 of board = %f\n", board->heuristic(own_side));
        //value = newboard->heuristic(master);
    }

Node::~Node() {
    delete board;
    for (unsigned int i = 0; i < children.size(); i++) {
        if (children[i] != NULL) {
            delete children[i];
        }
        //delete children[i];
    }
    
}

/** @brief Insert a node with a given board configuration as a child
*/
void Node::insert(Board *board) {
    Node *newnode = new Node(other_side, master_side, board);
    children.push_back(newnode);
}

/* Expand by inserting all possible moves */
void Node::getNextLayer()
{
    // Get the list of available moves
    vector<Move*> possible = board->legalMoves(own_side);
    
    // Go through the moves...
    for (unsigned int i = 0; i < possible.size(); i++) {        
        Board *next_board = board->copy();
        next_board->doMove(possible[i], own_side);
        insert(next_board);
    }
    
    // Clean up
    for (unsigned int i = 0; i < possible.size(); i++) {
        delete possible[i];
    }
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
    //printf("Node: value = %f\n", value);
    //printf("No. of children = %i\n", (int)children.size());
    
    for (unsigned int i = 0; i < children.size(); i++) {
        children[i]->printNode();
    }
    
}

float Node::minimax(int depth, bool maximizingPlayer) {
	if (depth == 0 || board->numMoves(own_side)== 0) {
		//printf("Depth is %d, black = %d, white = %d\n", depth, board->countBlack(), board->countWhite());
		return board->heuristic(master_side);
	}
	
	if (maximizingPlayer) {
		float bestValue = -10000000;
		getNextLayer();
		for (unsigned int i = 0; i < children.size(); i++) {
			float v = children[i]->minimax(depth - 1, false);
			bestValue = max(bestValue, v);
		}
        value = bestValue;
		//printf("Depth is %d, best value is %f\n", depth, bestValue);
		return bestValue;
	} else {
		float bestValue = 10000000;
		getNextLayer();
		for (unsigned int i = 0; i < children.size(); i++) {
			float v = children[i]->minimax(depth - 1, true);
			bestValue = min(bestValue, v);
		}
        value = bestValue;
		//printf("Depth is %d, best value is %f\n", depth, bestValue);
		return bestValue;
	}
					
	return 0;
	
}

float Node::getVal() {
    return value;
}

Move* Node::best_move(int depth, float best_val) {
	// Get the list of available moves
    vector<Move*> possible = board->legalMoves(own_side);
	
	printf("%d\n", (int)possible.size());
	Move *bestMove = NULL;
    
	if (possible.size() == 0) {
		return NULL;
	} else {
		for (unsigned int i = 0; i < possible.size(); i++) {
            printf("Value of child: %f \n", children[i]->value);
			if (bestMove == NULL && abs(children[i]->value - best_val) < 0.00001) {
				bestMove = possible[i];
			}
            else {
                delete possible[i];
            }
		}
	}
	return bestMove;
			
	
}

















