package thedrake.ui;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.layout.GridPane;
import thedrake.*;

import java.util.List;

public class StackView extends GridPane implements TroopViewContext {

    private GameState gameState;

    private TroopView selected;

    private ValidMoves validMoves;

    private StackContext stackContext;

    private PlayingSide side;

    public StackView(GameState gameState, PlayingSide side, StackContext stackContext) {
        this.gameState = gameState;
        this.validMoves = new ValidMoves(gameState);
        this.stackContext = stackContext;
        this.side = side;

        int stackLen = gameState.army(side).stack().size();
        for (int x = 0; x < stackLen; x++) {
            Troop troop = gameState.army(side).stack().get(x);
            add(new TroopView(troop, side, this), 1, x);
        }

        setHgap(5);
        setVgap(5);
        setPadding(new Insets(15));
        setAlignment(Pos.CENTER);
    }

    @Override
    public void troopViewSelected(TroopView troopView) {
        if (selected != null && selected != troopView)
            selected.unselect();

        selected = troopView;

        if (selected.troop == gameState.armyOnTurn().stack().get(0) && side == gameState.sideOnTurn()) {
            List<Move> moves = validMoves.movesFromStack();
            stackContext.movesFromStack(this, moves);
        } else
            stackContext.clearMoves(this);
    }

    public void update() {
        for (Node node : getChildren()) {
            TroopView troopView = (TroopView) node;
            troopView.update();
        }
    }
}
