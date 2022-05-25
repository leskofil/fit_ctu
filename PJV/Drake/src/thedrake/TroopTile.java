package thedrake;

import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

public class TroopTile implements Tile, JSONSerializable {
    private final Troop troop;

    private final PlayingSide side;

    private final TroopFace face;

    public TroopTile(Troop troop, PlayingSide side, TroopFace face) {
        this.troop = troop;
        this.side = side;
        this.face = face;
    }

    public PlayingSide side() {
        return side;
    }

    public TroopFace face() {
        return face;
    }

    public Troop troop() {
        return troop;
    }

    @Override
    public boolean canStepOn() {
        return false;
    }

    @Override
    public boolean hasTroop() {
        return true;
    }

    @Override
    public List<Move> movesFrom(BoardPos pos, GameState state) {
        List<Move> moves = new ArrayList<>();
        List<TroopAction> actions = state.armyOnTurn().boardTroops().at(pos).get().troop.actions(face);
        for (TroopAction action : actions) {
            List<Move> movesAction = action.movesFrom(pos, side, state);
            moves.addAll(movesAction);
        }
        return moves;
    }

    public TroopTile flipped() {
        if (face.equals(TroopFace.AVERS))
            return new TroopTile(troop, side, TroopFace.REVERS);
        else return new TroopTile(troop, side, TroopFace.AVERS);

    }


    @Override
    public void toJSON(PrintWriter writer) {
        writer.write("{\"troop\":");
        troop.toJSON(writer);
        writer.write(",\"side\":");
        side.toJSON(writer);
        writer.write(",\"face\":");
        face().toJSON(writer);
        writer.write("}");
    }
}
