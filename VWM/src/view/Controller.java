package view;

import dum.QueryEvaluator;
import dum.QueryParses;
import dum.TermLoad;
import dum.TermValue;
import javafx.beans.property.ReadOnlyObjectWrapper;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.control.*;

import java.net.URL;
import java.util.List;
import java.util.Queue;
import java.util.ResourceBundle;

public class Controller implements Initializable {

    @FXML
    private TextField nameField;

    @FXML
    private Button SequentialTravel;

    @FXML
    private Button InvertedIndex;

    @FXML
    private TableView<TermValue> tableView;

    @FXML
    private TableColumn<TermValue, String> documentCol;

    @FXML
    private TableColumn<TermValue, Double> relevanceCol;

    @FXML
    private Label timeLabel;

    private QueryEvaluator evaluator;

    private QueryParses parses;

    private TermLoad termLoad;

    public void seqTrav (ActionEvent event) {
        if (nameField.getText().equals(""))
            return;

        Queue<String> aaa = parses.convertInfixToRPN(parses.Tokenizer(nameField.getText()));
        double startTime1 = System.nanoTime();
        List<TermValue> values = evaluator.evalQuery(aaa, termLoad.getSeqTrav(), termLoad.getDocuments().size());
        double endTime = System.nanoTime();
        timeLabel.setText(String.valueOf((endTime - startTime1) / 1000000) + "ms");
        values.sort((a, b) -> Double.compare(b.getWeight(), a.getWeight()));
        tableView.getItems().clear();
        tableView.getItems().setAll(values);
    }

    public void invInx (ActionEvent event) {
        Queue<String> aaa = parses.convertInfixToRPN(parses.Tokenizer(nameField.getText()));
        double startTime1 = System.nanoTime();
        List<TermValue> values = evaluator.evalQuery(aaa, termLoad.getTerms(), termLoad.getDocuments().size());
        double endTime = System.nanoTime();
        timeLabel.setText(String.valueOf((endTime - startTime1) / 1000000) + "ms");
        values.sort((a, b) -> Double.compare(b.getWeight(), a.getWeight()));
        tableView.getItems().clear();
        tableView.getItems().setAll(values);
    }

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        termLoad = new TermLoad();
        evaluator = new QueryEvaluator();
        parses = new QueryParses();

        documentCol.setCellValueFactory(p -> new ReadOnlyObjectWrapper<>(termLoad.getDocumentos().get(p.getValue().getIdx())));
        relevanceCol.setCellValueFactory(p -> new ReadOnlyObjectWrapper<>(p.getValue().getWeight()));


    }
}
