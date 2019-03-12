//
//  Piece.hpp
//
//  Created by Jason Hou on 3/10/19.
//  Copyright © 2019 Jason Hou. All rights reserved.
//
#ifndef Piece_hpp
#define Piece_hpp

namespace ECE141 {

  enum class PieceKind  {pawn, king, captured};
  enum class PieceColor {gold, blue};
  enum class TileColor  {light, dark};
  
  class Piece;
  
  struct Location {
    int  row;
    int  col;
    Location(int aRow, int aCol) : row(aRow), col(aCol) {}
    Location(const Location &aCopy) : row(aCopy.row), col(aCopy.col) {}
  };
  
  struct Tile {
    Tile(TileColor aTileColor, Location aLocation, Piece *aPiece=nullptr)
      : color(aTileColor), piece(aPiece), location(aLocation) {
    }

    Tile(const Tile &aTile) : color(aTile.color), location(aTile.location), piece(nullptr) {}

    const TileColor  color;
    const Location   location; //temporary copy of board location
    Piece            *piece;    
  };
  
  class Piece {
  public:
    
    Piece(PieceColor aColor, Location aLocation, PieceKind aKind=PieceKind::pawn) :
      kind(aKind), color(aColor), location(aLocation) {
    }
    
    const PieceColor  color;
    Location          location;
    PieceKind         kind;
  };
}
#endif /* Piece_hpp */
