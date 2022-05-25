package thedrake.ui;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Scene;
import javafx.stage.Stage;
import thedrake.*;

public class Controller {

    @FXML
    private javafx.scene.control.Button newGame;

    @FXML
    public void exit(ActionEvent event) {
        Platform.exit();
        System.exit(0);
    }

    public void twoPlayerGame(ActionEvent event) {
        Stage primaryStage = (Stage) newGame.getScene().getWindow();
        GameWindow gameWindow = new GameWindow(createSampleGameState(), primaryStage);
        primaryStage.setScene(new Scene(gameWindow.getRoot()));
        primaryStage.setTitle("The Drake");
        primaryStage.show();
    }

    private static GameState createSampleGameState() {
        Board board = new Board(4);
        PositionFactory positionFactory = board.positionFactory();
        board = board.withTiles(new Board.TileAt(positionFactory.pos(1, 1), BoardTile.MOUNTAIN));
        return new StandardDrakeSetup().startState(board);
    }
}
