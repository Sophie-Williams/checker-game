//
//  player.cpp
//  dumb computer
//  Created by Jason Hou on 3/10/19.
//  Copyright © 2019 Jason Hou. All rights reserved.
//

#include "Player.hpp"
#include "Game.hpp"
#include <iostream>

namespace ECE141 {
  
  int Player::pcount = 0; //init our static member to track # of players...
  
  static PieceColor autoColor() { //auto assigns a color
    return (++Player::pcount % 2) ? PieceColor::blue : PieceColor::gold;
  }
  
  Player::Player() : color(autoColor()) {}
 
  bool Player::takeTurn(Game &aGame) {
    size_t theCount=aGame.countAvailablePieces(color);
    for(int pos=0;pos<theCount;pos++) {
      
      if(const Piece *thePiece = aGame.getAvailablePiece(this->color, pos)){
        std::vector<std::vector<Location>> all_loc = possible_Move(thePiece, aGame);
        if(all_loc.empty()) continue;
        aGame.movePieceTo(*thePiece, all_loc[0][0]);
        return true;
      }
    }
    return false; 
  }
  
  //Neiborhood function:
  std::vector<std::vector<Location>> Player::possible_Move(const Piece *piece,Game &aGame){
    int cur_row = piece->location.row;
    int cur_col = piece->location.col;
    
    std::vector<std::vector<Location>> total_location;
    
    int row_step = piece->color==PieceColor::gold? 1:-1;
    int col_step[4] = {-1,1,-1,1};
    
    for(int i=0; i<4; i++){
      Location location(cur_row+row_step,cur_col+col_step[i]);
      if( aGame.validLocation(location) ){
        const Tile *tile = aGame.getTileAt(location);
        //unoccupied location
        if(tile->piece==nullptr){
          total_location.push_back(std::vector<Location>{location});
        }
        
        //can jump over piece
        else if(tile->piece->color != piece->color){
          Location new_location(cur_row+2*row_step,cur_col+2*col_step[i]);
          if( aGame.validLocation(new_location) ){
            const Tile *tile = aGame.getTileAt(new_location);
            if(tile->piece==nullptr){
              std::vector<Location> loc;
              loc.push_back(new_location);
              //find multiple jumps;
              
              Location cur_location(new_location.row,new_location.col);
              Location jump_location(cur_location.row+row_step,cur_location.col+1);
              Location next_location(jump_location.row+row_step,jump_location.col+1);
              
              while(1){
                jump_location.row = cur_location.row+row_step;
                jump_location.col = cur_location.col+1;
                next_location.row = jump_location.row+row_step;
                next_location.col = jump_location.col+1;
                
                if(aGame.validLocation(jump_location) && aGame.validLocation(next_location)){
                  const Tile* next_tile = aGame.getTileAt(next_location);
                  const Tile* jump_tile = aGame.getTileAt(jump_location);
                  
                  if( !next_tile->piece&&jump_tile->piece && jump_tile->piece->color!=piece->color){
                    std::vector<Location> copy = loc;
                    total_location.push_back(copy);
                    
                    loc.push_back(next_location);
                    cur_location.row = next_location.row;
                    cur_location.col = next_location.col;
                    continue;
                  }
                }
                
                //search in another direction
                jump_location.row = cur_location.row+row_step;
                jump_location.col = cur_location.col-1;
                next_location.row = jump_location.row+row_step;
                next_location.col = jump_location.col-1;
                
                if(aGame.validLocation(jump_location) && aGame.validLocation(next_location)){
                  const Tile* next_tile = aGame.getTileAt(next_location);
                  const Tile* jump_tile = aGame.getTileAt(jump_location);
                  
                  if( !next_tile->piece&&jump_tile->piece && jump_tile->piece->color!=piece->color){
                    loc.push_back(next_location);
                    cur_location.row = next_location.row;
                    cur_location.col = next_location.col;
                    continue;
                  }
                  else{
                    total_location.push_back(loc);
                    break;
                  }
                }
                
                else{
                  total_location.push_back(loc);
                  break;
                }
              }//end While
              
            }
          }
        }//end else if
      }
      //If the piecekind is King, continue with another two directions
      if(i==1){
        if(piece->kind == PieceKind::pawn)  break;
        else row_step = -row_step;
      }
    }
    //All possible locations have been found;
    return total_location;
  }
  
}
