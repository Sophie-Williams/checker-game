//
//  main.cpp
//  checker game
//  Created by Jason Hou on 3/10/19.
//  Copyright © 2019 Jason Hou. All rights reserved.
//

#include "Game.hpp"
#include "HLPlayer.hpp"
#include <vector>
#include <numeric>
#include "stdio.h"
#include <iostream>
#include "Piece.hpp"

int main(int argc, const char * argv[]) {
  
  ECE141::Player player1;   //play against dumb bot
  ECE141::HLPlayer player2; //my own smart bot
  std::string player11;
  std::string player22;
  
  
  player11 = player1.color==ECE141::PieceColor::blue? "blue":"gold";
  player22 = player2.color==ECE141::PieceColor::blue? "blue":"gold";
  
  std::cout<<player11 << player22<<std::endl;
  
  ECE141::Game *theGame=ECE141::Game::create();
  theGame->run(player1,player2);
  delete theGame;

  return 0;
}
