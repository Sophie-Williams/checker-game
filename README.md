# checker-game
A checker game and a game bot implemented in C++

## Rules of the Game

Checkers is a two-person board game played on an 8x8 game board (same as chess board). Each player places their 12 pieces (usually red vs black) on the board on alternating dark squares in the three rows nearest to themselves. (See the following picture).

![Image](checkers.png)

- At the start of the game, all pieces are "pawns", and can only move forward by 1 diagonal space at a time (on dark squares) toward the opponent's side of the board.
- If a piece player's piece is directly adjacent to an opponent's piece, it _may_ be able to "capture" the opponent's piece by jumping. 
- A pawn can only jump an opponent's piece when they are adjacent on a diagonal line, and the following space in the forward direction is unoccupied. Only one piece can be captured in a single jump; however, multiple jumps are allowed during a single turn, provided the condition of adjacency is met.
- Any time a move exists on the board where an opponent's piece can be captured, the player is required to jump and capture the piece.  
- Captured pieces must be removed from the board.
- When a piece reaches the furthest row away from the player who controls that piece, it is "crowned" and becomes a King. One of the pieces that have already been removed is stacked on the Pawn, indicating it is now a King. (Kings are twice as tall as Pawns).
- Unlike Pawns which may only move forward, Kings may move forward or backward (toward the controlling player) along the diagonal.
- Also unlike Pawns, a King may jump other players in either a forward or backward direction, in any combination.

### Game visualization in IDE:
![Image](visualize.png)



## Overall strategies     
1.	Find all possible moves for each piece, for each move there may be 1 step or 2 steps, thus returning a vector containing a vector.
```cpp
virtual std::vector<std::vector<Location>> possible_Move(const Piece *piece,Game &aGame);
```
Without being captured, we need to push our pieces to the center of the board.    

2.	Defining each move. Each move could be "capture an opponent" or avoid being captured next round. These functions help to judge if it satisfies the condition.

```cpp
virtual bool Capture(const Piece* piece, Location cur_loc,Location next_loc);
virtual bool CanCapture(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame);        
virtual bool BecomeKing(const Piece* piece, Location cur_loc,Location next_loc);
virtual bool WillBeCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame);
virtual bool AvoidBeCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame);
virtual bool CauseCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame);
virtual int HelpNotBeCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame);
```

3.	Assign a score to each move then pick the move that has the maximum score.  
```cpp
score += 10  * Capture(thePiece, prevLoc, loc)                 +
         5   * BecomeKing(thePiece, prevLoc, loc)              +
        (-8) * WillBeCaptured(thePiece, prevLoc, loc, aGame)   +
         8   * AvoidBeCaptured(thePiece, prevLoc, loc, aGame)  +
         8   * HelpNotBeCaptured(thePiece, prevLoc, loc, aGame)+
        (-8) * CauseCaptured(thePiece, prevLoc, loc, aGame)    +
         2   * CanCapture(thePiece, prevLoc, loc, aGame);
```
    

### Neighborhood function
For each Pawn and each King, we will implement different strategies.      

![](./pawn.png)     
                       
For each Pawn, we only need to consider at most two positions for a valid move.      
But for each king, we need to consider 4 positions for a valid move(including moving upward and downwards).    
      
To implement a valid move, for each piece, using the methods `Game` offers:
```cpp
virtual size_t        countAvailablePieces(PieceColor aColor)=0;
virtual const Piece*  getPlayerPiece(PieceColor aColor, int anIndex)=0;
virtual bool          validLocation(const Location &aLocation)=0;
virtual const Tile*   getTileAt(const Location &aLocation)=0;
virtual bool          movePieceTo(const Piece &aPiece, const Location &aLocation)=0;
```     
For each Piece, we first find out all the valid moves of this Piece based on these functions. Then we calculate the score of all the valid moves and store the move with the highest score for this Piece in a vector.
```cpp
std::vector<Move> all_moves;
```   
Then we iterate over all the available Pieces and pick the move with the higest score.   

## Indicating decision
Inform the `Game` of our best move via the `movePieceTo()` method by passing in the piece we want to move and the destination location.
