package thedrake;

import java.io.PrintWriter;
import java.util.Arrays;
import java.util.StringJoiner;
import java.util.stream.Collectors;

public class Board implements JSONSerializable {

    private final BoardTile[][] board;

    private final int dimension;

    // Konstruktor. Vytvoří čtvercovou hrací desku zadaného rozměru, kde všechny dlaždice jsou prázdné, tedy BoardTile.EMPTY
    public Board(int dimension) {
        // Místo pro váš kód
        this.dimension = dimension;
        board = new BoardTile[dimension][dimension];
        for (Tile[] tiles : board) {
            Arrays.fill(tiles, BoardTile.EMPTY);
        }
    }

    // Rozměr hrací desky
    public int dimension() {
        // Místo pro váš kód
        return dimension;
    }

    // Vrací dlaždici na zvolené pozici.
    public BoardTile at(TilePos pos) {
        // Místo pro váš kód
        return board[pos.i()][pos.j()];
    }

    // Vytváří novou hrací desku s novými dlaždicemi. Všechny ostatní dlaždice zůstávají stejné
    public Board withTiles(TileAt... ats) {
        // Místo pro váš kód
        Board newBoard = new Board(dimension);

        for (int i = 0; i < dimension; i++) {
            newBoard.board[i] = board[i].clone();
        }

        for (TileAt at : ats)
            newBoard.board[at.pos.i()][at.pos.j()] = at.tile;

        return newBoard;
    }

    // Vytvoří instanci PositionFactory pro výrobu pozic na tomto hracím plánu
    public PositionFactory positionFactory() {
        // Místo pro váš kód
        return new PositionFactory(dimension);
    }

    @Override
    public void toJSON(PrintWriter writer) {
        StringJoiner joiner = new StringJoiner(",", "[", "]");
        writer.write("{\"dimension\":" + dimension() + ",");
        writer.write("\"tiles\":[");
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < dimension; j++) {
                board[j][i].toJSON(writer);
                if (j < dimension - 1 || i < dimension - 1)
                    writer.write(",");
            }
        }
        writer.write("]}");
    }

    public static class TileAt {
        public final BoardPos pos;
        public final BoardTile tile;

        public TileAt(BoardPos pos, BoardTile tile) {
            this.pos = pos;
            this.tile = tile;
        }
    }
}

