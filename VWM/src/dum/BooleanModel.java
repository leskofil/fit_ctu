package dum;//import javax.management.Query;
import java.util.HashMap;
import java.util.List;
import java.util.Queue;

public class BooleanModel {
    public static void main(String[] args) {
        TermLoad termLoad = new TermLoad();

        QueryParses qp = new QueryParses();
        String[] infixNotation2 = {"(", "calculate", "AND", "similarity", ")", "OR", "NOT", "(", "query", "OR", "documents", ")"};
        String string = new String("calculate AND similarity");

        Queue<String> query = qp.convertInfixToRPN(qp.Tokenizer(string));
        QueryEvaluator queryEvaluator = new QueryEvaluator();
        double startTime = System.nanoTime();
        List<TermValue> values = queryEvaluator.evalQuery(query, termLoad.getSeqTrav(), termLoad.getDocuments().size());
        double endTime = System.nanoTime();
        double startTime1 = System.nanoTime();
        List<TermValue> values1 = queryEvaluator.evalQuery(query, termLoad.getTerms(), termLoad.getDocuments().size());
        double endTime1 = System.nanoTime();
        values.sort((a, b) -> Double.compare(b.getWeight(), a.getWeight()));
        values1.sort((a, b) -> Double.compare(b.getWeight(), a.getWeight()));
        PrintDoc(values, termLoad.documentos);
        System.out.println((endTime - startTime) / 1000000 + "ms");
        System.out.println((endTime1 - startTime1) / 1000000 + "ms");

    }

    private static void PrintDoc(List<TermValue> values, HashMap<Integer, String> docs) {
        for (TermValue v : values) {
            System.out.println(docs.get(v.getIdx()) + "\t" + v.getWeight());
        }

    }

}
