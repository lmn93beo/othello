For the heuristic function, I decided to use 3 factors: difference in number of coins,
number of corners occupied, and number of moves available. These factors are easy to 
calculate and after reading some online sources I have found that they are generally
used in most Othello AIs. I put the most weight on number of corners and least weight
on number of coins.

I first implemented minimax, then implemented alpha-beta pruning to speed up
the computation. I tried a depth of 6 and observed a fast performance. However,
when the depth is increased to 7, there was a bad_alloc error, which was due to
a memory allocation issue. 

I fixed the issue and implemented iterative deepening. I allocated little time
at the beginning and more time in the middle game, and decreased the time again
towards the endgame when the number of moves is quite small.
