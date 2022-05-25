package thedrake;

import java.util.ArrayList;
import java.util.List;

public class SlideAction extends TroopAction {

    public SlideAction(Offset2D offset) {
        super(offset);
    }

    public SlideAction(int offsetX, int offsetY) {
        super(offsetX, offsetY);
    }

    @Override
    public List<Move> movesFrom(BoardPos origin, PlayingSide side, GameState state) {
        List<Move> result = new ArrayList<>();
        List<TilePos> targets = new ArrayList<>();
        TilePos target = origin.stepByPlayingSide(offset(), side);

        while (!target.equals(TilePos.OFF_BOARD)) {
            if (state.armyNotOnTurn().boardTroops().at(target).isPresent())
                break;
            targets.add(target);
            target = target.stepByPlayingSide(offset(), side);
        }
        for (TilePos target1 : targets) {
            if (state.canStep(origin, target1)) {
                result.add(new StepOnly(origin, (BoardPos) target1));
            }
        }
        if (state.canCapture(origin, target)) {
            result.add(new StepAndCapture(origin, (BoardPos) target));
        }
        return result;
    }
}
