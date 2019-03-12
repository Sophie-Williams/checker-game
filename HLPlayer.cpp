//
//  HLPlayer.cpp
//
//  Created by Jason Hou on 3/10/19.
//  Copyright © 2019 Jason Hou. All rights reserved.
//

#include "HLPlayer.hpp"
#include "Game.hpp"
#include <iostream>
#include <vector>
#include <algorithm>

namespace ECE141 {
    
    struct best_move{
        best_move(const Piece *aPiece, std::vector<Location> aLocation): thePiece(aPiece), theLocation(aLocation){}
        const Piece *thePiece;
        std::vector<Location> theLocation;
    };
    
    
    bool HLPlayer::takeTurn(Game &aGame) {
        size_t theCount=aGame.countAvailablePieces(color);
        
        std::vector<best_move> Moves;
        std::vector<int> Move_score;
        int score = 0;
        
        for(int pos=0;pos<theCount;pos++) {
            
            if(const Piece *thePiece = aGame.getAvailablePiece(this->color, pos)){
                std::vector<std::vector<Location>> all_loc = possible_Move(thePiece, aGame);
                
                if(all_loc.empty()) continue;
                for (auto vec_loc:all_loc){
                    score = 0;
                    auto prevLoc = thePiece->location;
                    for(auto loc:vec_loc){
                        score += 10 * Capture(thePiece, prevLoc, loc)                 +
                                 5  * BecomeKing(thePiece, prevLoc, loc)              +
                                (-8)* WillBeCaptured(thePiece, prevLoc, loc, aGame)   +
                                 8  * AvoidBeCaptured(thePiece, prevLoc, loc, aGame)  +
                                 8  * HelpNotBeCaptured(thePiece, prevLoc, loc, aGame)+
                                (-8)* CauseCaptured(thePiece, prevLoc, loc, aGame)    +
                                 2  * CanCapture(thePiece, prevLoc, loc, aGame);
                        prevLoc = loc;
                    }
                    best_move cur_move(thePiece,vec_loc);
                    Moves.push_back(cur_move);
                    Move_score.push_back(score);
                }
            }
        }
        if(!Move_score.empty()){
            auto best_index = std::max_element(Move_score.begin(), Move_score.end()) - Move_score.begin();
            std::cout<<Moves[best_index].theLocation.size()<<std::endl;
            for (auto locations:Moves[best_index].theLocation){
                aGame.movePieceTo(*Moves[best_index].thePiece, locations);
            }
            return true;
        }
        
        return false; 
    }
    
    
    //Objective functions
    bool HLPlayer::Capture(const Piece* piece, Location cur_loc,Location next_loc){
        return abs(next_loc.row-cur_loc.row)>1;
    }
    
    bool HLPlayer::BecomeKing(const Piece* piece, Location cur_loc,Location next_loc){
        return (piece->color==PieceColor::gold && next_loc.row==7 && piece->kind==PieceKind::pawn) ||
        (piece->color==PieceColor::blue && next_loc.row==0 && piece->kind==PieceKind::pawn);
    }
    
    bool HLPlayer::WillBeCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame){
        int row_step = next_loc.row - cur_loc.row;
        int col_step = next_loc.col - cur_loc.col;
        
        Location test1(next_loc.row+row_step, next_loc.col+col_step);
        Location test2(next_loc.row+row_step, cur_loc.col);
        Location temp(cur_loc.row,test1.col);
        
        if(aGame.validLocation(test1)){
            Piece* piece1 = aGame.getTileAt(test1)->piece;
            if(piece1 && piece1->color!=piece->color)
                return true;
        }
        
        if(aGame.validLocation(test2) && aGame.validLocation(temp)){
            Piece* piece2 = aGame.getTileAt(test2)->piece;
            Piece* temp1 = aGame.getTileAt(temp)->piece;
            if(piece2 && piece2->color!=piece->color && !temp1)
                return true;
        }
        return false;
    }
    
    bool HLPlayer::CauseCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame){
        int row_step = next_loc.row - cur_loc.row;
        int col_step = next_loc.col - cur_loc.col;
        
        Location test1(next_loc.row,cur_loc.col-col_step);
        Location test2(test1.row+row_step,test1.col-col_step);
        
        if(!aGame.validLocation(test1) || !aGame.validLocation(test2)) return false;
        Piece* piece1 = aGame.getTileAt(test1)->piece;
        Piece* piece2 = aGame.getTileAt(test2)->piece;
        return piece1 && piece2 && piece1->color==piece->color && piece2->color!=piece->color;
    }
    
    bool HLPlayer::AvoidBeCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame){
        int row_step = next_loc.row - cur_loc.row;
        int col_step = next_loc.col - cur_loc.col;
        Location test1(next_loc.row,cur_loc.col-col_step);
        Location test2(cur_loc.row-row_step,next_loc.col);
        
        if(!aGame.validLocation(test1) || !aGame.validLocation(test2))  return false;
        Piece* piece1 = aGame.getTileAt(test1)->piece;
        Piece* piece2 = aGame.getTileAt(test2)->piece;
        
        return piece1&&piece1->color!=piece->color&&!piece2;
    }
    
    int HLPlayer::HelpNotBeCaptured(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame){
        int help = 0;
        int row_step = next_loc.row - cur_loc.row;
        int col_step = next_loc.col - cur_loc.col;
        
        Location test1(next_loc.row+row_step,next_loc.col+col_step);
        Location test2(test1.row+row_step,test1.col+col_step);
        
        if(aGame.validLocation(test1) && aGame.validLocation(test2)){
            Piece* piece1 = aGame.getTileAt(test1)->piece;
            Piece* piece2 = aGame.getTileAt(test2)->piece;
            if(piece1 && piece2 && piece1->color==piece->color && piece2->color!=piece->color)
                help++;
        }
        
        Location test3(next_loc.row+row_step,next_loc.col-col_step);
        Location test4(test3.row+row_step,test3.col-col_step);
        
        if(aGame.validLocation(test3) && aGame.validLocation(test4)){
            Piece* piece3 = aGame.getTileAt(test3)->piece;
            Piece* piece4 = aGame.getTileAt(test4)->piece;
            if(piece3 && piece4 && piece3->color==piece->color && piece4->color!=piece->color)
                help++;
        }
        return help;
    }
    
    bool HLPlayer::CanCapture(const Piece* piece,Location cur_loc, Location next_loc, Game &aGame){
        int row_step = next_loc.row - cur_loc.row;
        Location test1(next_loc.row+row_step,next_loc.col+1);
        Location test2(test1.row+row_step,test1.col+1);
        
        if(aGame.validLocation(test1) && aGame.validLocation(test2)){
            Piece* piece1 = aGame.getTileAt(test1)->piece;
            Piece* piece2 = aGame.getTileAt(test2)->piece;
            
            if(piece1 && !piece2 && piece1->color!=piece->color)  return true;
        }
        
        Location test3(next_loc.row+row_step,next_loc.col-1);
        Location test4(test3.row+row_step,test1.col-1);
        if(aGame.validLocation(test3) && aGame.validLocation(test4)){
            Piece* piece3 = aGame.getTileAt(test3)->piece;
            Piece* piece4 = aGame.getTileAt(test4)->piece;
            
            if(piece3 && !piece4 && piece3->color!=piece->color)  return true;
        }
        
        return  false;
    }
    
    //Neiborhood function:
    std::vector<std::vector<Location>> HLPlayer::possible_Move(const Piece *piece,Game &aGame){
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
