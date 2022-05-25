package thedrake;

import java.io.PrintWriter;
import java.util.List;
import java.util.Optional;

public class GameState implements JSONSerializable {
    private final Board board;
    private final PlayingSide sideOnTurn;
    private final Army blueArmy;
    private final Army orangeArmy;
    private final GameResult result;

    public GameState(
            Board board,
            Army blueArmy,
            Army orangeArmy) {
        this(board, blueArmy, orangeArmy, PlayingSide.BLUE, GameResult.IN_PLAY);
    }

    public GameState(
            Board board,
            Army blueArmy,
            Army orangeArmy,
            PlayingSide sideOnTurn,
            GameResult result) {
        this.board = board;
        this.sideOnTurn = sideOnTurn;
        this.blueArmy = blueArmy;
        this.orangeArmy = orangeArmy;
        this.result = result;
    }

    public Board board() {
        return board;
    }

    public PlayingSide sideOnTurn() {
        return sideOnTurn;
    }

    public GameResult result() {
        return result;
    }

    public Army army(PlayingSide side) {
        if (side == PlayingSide.BLUE) {
            return blueArmy;
        }

        return orangeArmy;
    }

    public Army armyOnTurn() {
        return army(sideOnTurn);
    }

    public Army armyNotOnTurn() {
        if (sideOnTurn == PlayingSide.BLUE)
            return orangeArmy;

        return blueArmy;
    }

    public Tile tileAt(TilePos pos) {
        // Místo pro váš kód
        if (blueArmy.boardTroops().at(pos).isPresent()) {
            return blueArmy.boardTroops().at(pos).get();
        }
        if (orangeArmy.boardTroops().at(pos).isPresent()) {
            return orangeArmy.boardTroops().at(pos).get();
        }
        return board.at(pos);
    }

    private boolean canStepFrom(TilePos origin) {
        // Místo pro váš kód
        if (origin.equals(TilePos.OFF_BOARD))
            return false;

        if (!result.equals(GameResult.IN_PLAY)) {
            return false;
        }
        if (armyOnTurn().boardTroops().at(origin).isEmpty()) {
            return false;
        }
        if (armyOnTurn().boardTroops().isPlacingGuards()) {
            return false;
        }
        return true;
    }

    private boolean canStepTo(TilePos target) {
        // Místo pro váš kód
        if (target.equals(TilePos.OFF_BOARD))
            return false;

        if (!result.equals(GameResult.IN_PLAY)) {
            return false;
        }
        if (!board.at(target).canStepOn()) {
            return false;
        }
        if (blueArmy.boardTroops().at(target).isPresent() || //metohod canStepOnTroop
                orangeArmy.boardTroops().at(target).isPresent()) {
            return false;
        }

        return true;
    }

    private boolean canCaptureOn(TilePos target) {
        // Místo pro váš kód
        if (target.equals(TilePos.OFF_BOARD))
            return false;

        if (!result.equals(GameResult.IN_PLAY)) {
            return false;
        }
        if (sideOnTurn.equals(PlayingSide.BLUE) &&
                orangeArmy.boardTroops().at(target).isEmpty())
            return false;

        if (sideOnTurn.equals(PlayingSide.ORANGE) &&
                blueArmy.boardTroops().at(target).isEmpty())
            return false;

        return true;
    }

    public boolean canStep(TilePos origin, TilePos target) {
        return canStepFrom(origin) && canStepTo(target);
    }

    public boolean canCapture(TilePos origin, TilePos target) {
        return canStepFrom(origin) && canCaptureOn(target);
    }

    public boolean canPlaceFromStack(TilePos target) {
        // Místo pro váš kód
        if (target.equals(TilePos.OFF_BOARD))
            return false;

        if (!result.equals(GameResult.IN_PLAY)) {  //method notInPlay
            return false;
        }
        if (armyOnTurn().stack().isEmpty()) {
            return false;
        }
        if (!board.at(target).canStepOn()) {
            return false;
        }

        if (blueArmy.boardTroops().at(target).isPresent() || //metohod canStepOnTroop
                orangeArmy.boardTroops().at(target).isPresent()) {
            return false;
        }

        if (!armyOnTurn().boardTroops().isLeaderPlaced()) { //new method canPlaceLeader
            if (armyOnTurn().equals(blueArmy) && target.row() == 1) {
                return true;
            }
            if (armyOnTurn().equals(orangeArmy) && target.row() == board().dimension())
                return true;
            return false;
        }
        if (armyOnTurn().boardTroops().isPlacingGuards()) { //new method canPlaceGuard
            if (armyOnTurn().boardTroops().leaderPosition().isNextTo(target) &&
                    armyOnTurn().boardTroops().at(target).isEmpty()) {
                return true;
            }
            return false;
        }

        if (!armyOnTurn().boardTroops().isPlacingGuards() && armyOnTurn().boardTroops().isLeaderPlaced()) { //mid game placing
            for (TilePos neighbour : target.neighbours()) {
                if (armyOnTurn().boardTroops().at(neighbour).isPresent())
                    return true;
            }
            return false;
        }

        return true;

    }

    public GameState stepOnly(BoardPos origin, BoardPos target) {
        if (canStep(origin, target))
            return createNewGameState(
                    armyNotOnTurn(),
                    armyOnTurn().troopStep(origin, target), GameResult.IN_PLAY);

        throw new IllegalArgumentException();
    }

    public GameState stepAndCapture(BoardPos origin, BoardPos target) {
        if (canCapture(origin, target)) {
            Troop captured = armyNotOnTurn().boardTroops().at(target).get().troop();
            GameResult newResult = GameResult.IN_PLAY;

            if (armyNotOnTurn().boardTroops().leaderPosition().equals(target))
                newResult = GameResult.VICTORY;

            return createNewGameState(
                    armyNotOnTurn().removeTroop(target),
                    armyOnTurn().troopStep(origin, target).capture(captured), newResult);
        }

        throw new IllegalArgumentException();
    }

    public GameState captureOnly(BoardPos origin, BoardPos target) {
        if (canCapture(origin, target)) {
            Troop captured = armyNotOnTurn().boardTroops().at(target).get().troop();
            GameResult newResult = GameResult.IN_PLAY;

            if (armyNotOnTurn().boardTroops().leaderPosition().equals(target))
                newResult = GameResult.VICTORY;

            return createNewGameState(
                    armyNotOnTurn().removeTroop(target),
                    armyOnTurn().troopFlip(origin).capture(captured), newResult);
        }

        throw new IllegalArgumentException();
    }

    public GameState placeFromStack(BoardPos target) {
        if (canPlaceFromStack(target)) {
            return createNewGameState(
                    armyNotOnTurn(),
                    armyOnTurn().placeFromStack(target),
                    GameResult.IN_PLAY);
        }

        throw new IllegalArgumentException();
    }

    public GameState resign() {
        return createNewGameState(
                armyNotOnTurn(),
                armyOnTurn(),
                GameResult.VICTORY);
    }

    public GameState draw() {
        return createNewGameState(
                armyOnTurn(),
                armyNotOnTurn(),
                GameResult.DRAW);
    }

    private GameState createNewGameState(Army armyOnTurn, Army armyNotOnTurn, GameResult result) {
        if (armyOnTurn.side() == PlayingSide.BLUE) {
            return new GameState(board, armyOnTurn, armyNotOnTurn, PlayingSide.BLUE, result);
        }

        return new GameState(board, armyNotOnTurn, armyOnTurn, PlayingSide.ORANGE, result);
    }

    @Override
    public void toJSON(PrintWriter writer) {
        writer.write("{\"result\":");
        result.toJSON(writer);
        writer.write(",\"board\":");
        board().toJSON(writer);
        writer.write(",\"blueArmy\":");
        blueArmy.toJSON(writer);
        writer.write(",\"orangeArmy\":");
        orangeArmy.toJSON(writer);
        writer.write("}");
    }
}
