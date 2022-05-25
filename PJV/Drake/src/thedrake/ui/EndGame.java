package thedrake.ui;

import javafx.application.Platform;
import javafx.event.EventHandler;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.effect.Shadow;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.*;
import javafx.stage.Stage;
import thedrake.Board;
import thedrake.PlayingSide;
import thedrake.StandardDrakeSetup;

import java.io.IOException;

public class EndGame extends BorderPane {

    public BorderPane getRoot() {
        return root;
    }

    private BorderPane root;

    private Button newGame;

    private Button mainMenu;

    public EndGame(PlayingSide side) {
        root = new BorderPane();
        HBox menu = new HBox();
        newGame = new Button("New Game");
        mainMenu = new Button("Main Menu");

        Label label = new Label("WINNER: " + side.name());

        newGame.setOnMouseClicked(this::newGame);
        mainMenu.setOnMouseClicked(this::goMenu);

        setCenter(menu);
        HBox.setHgrow(newGame, Priority.ALWAYS);
        HBox.setHgrow(mainMenu, Priority.ALWAYS);
        newGame.setMaxWidth(Double.MAX_VALUE);
        mainMenu.setMaxWidth(Double.MAX_VALUE);
        menu.getChildren().addAll(newGame, mainMenu);
        root.setCenter(menu);
        root.setTop(label);
    }

    private void newGame(MouseEvent e) {
        Stage primaryStage = ((Stage) (((Button) e.getSource()).getScene().getWindow()));
        GameWindow gameWindow = new GameWindow(new StandardDrakeSetup().startState(new Board(4)), primaryStage);
        primaryStage.setScene(new Scene(gameWindow.getRoot()));
        primaryStage.setTitle("The Drake");
        primaryStage.show();
    }

    private void goMenu(MouseEvent e) {
        Stage primaryStage = ((Stage) (((Button) e.getSource()).getScene().getWindow()));
        Parent root = null;
        try {
            root = FXMLLoader.load(getClass().getResource("StartingScreen.fxml"));
        } catch (IOException ioException) {
            ioException.printStackTrace();
        }
        primaryStage.setTitle("The Drake");
        primaryStage.setScene(new Scene(root));
        primaryStage.show();
    }

}

