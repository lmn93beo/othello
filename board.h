#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.h"
#include <vector>

using namespace std;

class Board {
   
private:
    bitset<64> black;
    bitset<64> taken;    
       
    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);
    bool testingMinimax;
    
      
public:
    Board(bool minimax);
    ~Board();
    Board *copy();
        
    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();
    void setTestingMinimax(bool val);
    
    int numMoves(Side side);
    float mobility(Side side);
    float corner_advantage(Side side);
    float parity(Side side);
    
	
	float heuristic(Side side);
	vector<Move*> legalMoves(Side side);

    void setBoard(char data[], bool test);
    
};

class Node
{
private:
    
    Board *board;
	std::vector<Node*> children;
	Side own_side; // Ownside is making the next move
	Side other_side;
	Side master_side;
	
    
public:
    Node(Side side, Side master, Board *newboard);
    ~Node();
    
    void insert(Board *board);
	void deleteNode();
    void printNode();
	float ab(int depth, float alpha, float beta, bool maximizingPlayer, Move* move);
    
      
};


#endif
