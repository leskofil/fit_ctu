package thedrake.ui;

import javafx.scene.image.ImageView;
import javafx.scene.layout.*;
import javafx.scene.paint.Color;
import thedrake.*;

public class TroopView extends Pane {

    private Border selectBorder = new Border(
            new BorderStroke(Color.BLACK, BorderStrokeStyle.SOLID, CornerRadii.EMPTY, new BorderWidths(3)));

    private TroopViewContext troopViewContext;

    private Move move;

    private final ImageView moveImage;

    Troop troop;

    PlayingSide side;

    private TileBackgrounds backgrounds = new TileBackgrounds();

    public TroopView(Troop troop, PlayingSide side, TroopViewContext context) {
        this.troop = troop;
        this.side = side;
        this.troopViewContext = context;

        setPrefSize(100, 100);
        update();

        setOnMouseClicked(e -> onClick());

        moveImage = new ImageView(getClass().getResource("/assets/move.png").toString());
        moveImage.setVisible(false);
        getChildren().add(moveImage);
    }

    public void update() {
        setBackground(backgrounds.getTroop(troop, side, TroopFace.AVERS));
    }

    private void onClick() {
        select();
    }

    public void select() {
        setBorder(selectBorder);
        troopViewContext.troopViewSelected(this);
    }

    public void unselect() {
        setBorder(null);
    }

    public void setMove(Move move) {
        this.move = move;
        moveImage.setVisible(true);
    }

    public void setTroop(Troop troop) {
        this.troop = troop;
    }
}
