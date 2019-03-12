//
//  HLPlayer.hpp
//
//  Created by Jason Hou on 3/10/19.
//  Copyright © 2019 Jason Hou. All rights reserved.
//

#ifndef AnotherPlayer_hpp
#define AnotherPlayer_hpp

#include <stdio.h>
#include "Piece.hpp"
#include "Player.hpp"
#include <vector>
namespace ECE141 {
    
    class Game;
  
    class HLPlayer: public Player {
    public:
        virtual bool      takeTurn(Game &aGame); 
        virtual bool Capture(const Piece* piece, Location cur_loc,Location next_loc);
        virtual bool CanCapture(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame);
        
        virtual bool BecomeKing(const Piece* piece, Location cur_loc,Location next_loc);
        virtual bool WillBeCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame);
        virtual bool AvoidBeCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame);
        virtual bool CauseCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame);
        virtual int HelpNotBeCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame);
        virtual std::vector<std::vector<Location>> possible_Move(const Piece *piece,Game &aGame);
    };
    
}
#endif /* AnotherPlayer_hpp */
