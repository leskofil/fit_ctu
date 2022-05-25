package thedrake.ui;

import javafx.event.Event;
import javafx.fxml.FXML;
import javafx.geometry.Side;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.layout.BorderPane;
import javafx.stage.Stage;
import thedrake.GameState;
import thedrake.Move;
import thedrake.PlayingSide;

import java.util.List;

public class GameWindow extends BorderPane implements StackContext, BoardContext {

    public BorderPane getRoot() {
        return root;
    }

    private BorderPane root;

    private BoardView boardView;

    private StackView stackViewB;

    private StackView stackViewO;

    private CapturedView capturedViewB;

    private CapturedView capturedViewO;

    private Stage stage;

    public GameWindow(GameState gameState, Stage stage) {
        this.stage = stage;
        this.boardView = new BoardView(gameState, this);
        this.stackViewB = new StackView(gameState, PlayingSide.BLUE, this);
        this.stackViewO = new StackView(gameState, PlayingSide.ORANGE, this);
        this.capturedViewB = new CapturedView(gameState, PlayingSide.BLUE);
        this.capturedViewO = new CapturedView(gameState, PlayingSide.ORANGE);
        root = new BorderPane();

        root.setCenter(boardView);
        root.setLeft(stackViewB);
        root.setRight(stackViewO);
        root.setTop(capturedViewB);
        root.setBottom(capturedViewO);

    }

    @Override
    public void movesFromStack(StackView stackView, List<Move> moves) {
        boardView.clearMoves();
        boardView.showMoves(moves);
    }

    @Override
    public void clearMoves(StackView stackView) {
        boardView.clearMoves();
    }

    @Override
    public void updateStack(BoardView boardView, GameState gameState) {
        this.stackViewB = new StackView(gameState, PlayingSide.BLUE, this);
        this.stackViewO = new StackView(gameState, PlayingSide.ORANGE, this);
        root.setCenter(boardView);
        root.setLeft(stackViewB);
        root.setRight(stackViewO);
    }

    @Override
    public void updateCaptured(BoardView boardView, GameState gameState) {
        this.capturedViewB = new CapturedView(gameState, PlayingSide.BLUE);
        this.capturedViewO = new CapturedView(gameState, PlayingSide.ORANGE);
        root.setTop(capturedViewB);
        root.setBottom(capturedViewO);
        root.setCenter(boardView);
    }

    @Override
    public void endGame(BoardView boardView, PlayingSide side) {
        EndGame endGame = new EndGame(side);
        stage.close();
        Stage stage1 = new Stage();
        stage1.setScene(new Scene(endGame.getRoot()));
        stage1.setTitle("The Drake");
        stage1.show();
    }
}
