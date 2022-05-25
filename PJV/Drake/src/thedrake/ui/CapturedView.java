package thedrake.ui;

import javafx.fxml.FXML;
import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.Pane;
import javafx.stage.Stage;
import thedrake.GameState;
import thedrake.PlayingSide;
import thedrake.Troop;

public class CapturedView extends GridPane implements TroopViewContext {

    private GameState gameState;

    private TileBackgrounds backgrounds = new TileBackgrounds();


    public CapturedView(GameState gameState, PlayingSide side) {
        this.gameState = gameState;

        int capturedLen = gameState.army(side).captured().size();
        for (int x = 0; x < capturedLen; x++) {
            Troop troop = gameState.army(side).captured().get(x);
            add(new TroopView(troop, side, this), x, 1);
        }

        setHgap(5);
        setVgap(5);
        setPadding(new Insets(15));
        setAlignment(Pos.CENTER);
    }


    @Override
    public void troopViewSelected(TroopView troopView) {
    }

    public void update(GameState gameState, PlayingSide side) {

    }
}
