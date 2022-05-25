package thedrake;

import java.io.PrintWriter;
import java.io.Serializable;
import java.util.Collections;
import java.util.List;

public interface BoardTile extends Tile, JSONSerializable {
    public static BoardTile EMPTY = new BoardTile() {
        @Override
        public boolean canStepOn() {
            return true;
        }

        @Override
        public boolean hasTroop() {
            return false;
        }

        @Override
        public List<Move> movesFrom(BoardPos pos, GameState state) {
            return Collections.emptyList();
        }

        public void toJSON(PrintWriter writer) {
            writer.write("\"empty\"");
        }
    };

    public static final BoardTile MOUNTAIN = new BoardTile() {
        @Override
        public boolean canStepOn() {
            return false;
        }

        @Override
        public boolean hasTroop() {
            return false;
        }

        @Override
        public List<Move> movesFrom(BoardPos pos, GameState state) {
            return Collections.emptyList();
        }

        public void toJSON(PrintWriter writer) {
            writer.write("\"mountain\"");
        }
    };
}
