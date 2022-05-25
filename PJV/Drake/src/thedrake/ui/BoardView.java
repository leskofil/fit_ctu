package thedrake.ui;

import java.util.List;

import javafx.geometry.Insets;
import javafx.geometry.Pos;
import javafx.scene.Node;
import javafx.scene.layout.GridPane;
import thedrake.*;


public class BoardView extends GridPane implements TileViewContext {

    private GameState gameState;

    private ValidMoves validMoves;

    private TileView selected;

    private BoardContext boardContext;

    public BoardView(GameState gameState, BoardContext boardContext) {
        this.gameState = gameState;
        this.validMoves = new ValidMoves(gameState);
        this.boardContext = boardContext;

        PositionFactory positionFactory = gameState.board().positionFactory();
        for (int y = 0; y < 4; y++) {
            for (int x = 0; x < 4; x++) {
                BoardPos boardPos = positionFactory.pos(x, 3 - y);
                add(new TileView(boardPos, gameState.tileAt(boardPos), this), x, y);
            }
        }

        setHgap(5);
        setVgap(5);
        setPadding(new Insets(15));
        setAlignment(Pos.CENTER);
    }

    @Override
    public void tileViewSelected(TileView tileView) {
        if (selected != null && selected != tileView)
            selected.unselect();

        selected = tileView;

        clearMoves();
        showMoves(validMoves.boardMoves(tileView.position()));
    }

    @Override
    public void executeMove(Move move) {
        if (selected != null) {
            selected.unselect();
            selected = null;
        }
        clearMoves();
        gameState = move.execute(gameState);
        validMoves = new ValidMoves(gameState);
        updateTiles();
        if (gameState.result().equals(GameResult.VICTORY) ||
                gameState.result().equals(GameResult.DRAW)) {
            boardContext.endGame(this, gameState.sideOnTurn());
        }
    }

    public void updateTiles() {
        for (Node node : getChildren()) {
            TileView tileView = (TileView) node;
            tileView.setTile(gameState.tileAt(tileView.position()));
            tileView.update();
        }
        boardContext.updateStack(this, gameState);
        boardContext.updateCaptured(this, gameState);
    }

    public void clearMoves() {
        for (Node node : getChildren()) {
            TileView tileView = (TileView) node;
            tileView.clearMove();
        }
    }

    public void showMoves(List<Move> moveList) {
        for (Move move : moveList)
            tileViewAt(move.target()).setMove(move);
    }

    private TileView tileViewAt(BoardPos target) {
        int index = (3 - target.j()) * 4 + target.i();
        return (TileView) getChildren().get(index);
    }
}
