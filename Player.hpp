//
//  player.hpp
//
//  Created by Jason Hou on 3/10/19.
//  Copyright © 2019 Jason Hou. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include "Piece.hpp"
#include <vector>
namespace ECE141 {
  
  class Game; 
  
  class Player {
  public:
                      Player();
    virtual bool      takeTurn(Game &aGame);
    const PieceColor  color;
    static int        pcount;
    
    virtual std::vector<std::vector<Location>> possible_Move(const Piece *piece,Game &aGame);
  };
}

#endif /* Player_hpp */
