package dum;

import java.util.*;

public class QueryParses {

    static boolean isValid(String str) {
        return true;
    }

    public Queue<String> convertInfixToRPN(String[] infixNotation) {
        Stemmer stemmer = new Stemmer();
        Map<String, Integer> prededence = new HashMap<>();
        prededence.put("AND", 5);
        prededence.put("OR", 5);
        prededence.put("NOT", 6);
        prededence.put("(", 0);

        Queue<String> Q = new LinkedList<>();
        Stack<String> S = new Stack<>();

        for (String token : infixNotation) {
            if ("(".equals(token)) {
                S.push(token);
                continue;
            }

            if (")".equals(token)) {
                while (!"(".equals(S.peek())) {
                    Q.add(S.pop());
                }
                S.pop();
                continue;
            }
            // an operator
            if (prededence.containsKey(token)) {
                while (!S.empty() && prededence.get(token) <= prededence.get(S.peek())) {
                    Q.add(S.pop());
                }
                S.push(token);
                continue;
            }

            if (isValid(token)) {
                stemmer.add(token.toCharArray(), token.length());
                stemmer.stem();
                String term = stemmer.toString();
                Q.add(term);
                continue;
            }

            throw new IllegalArgumentException("Invalid input");
        }
        // at the end, pop all the elements in S to Q
        while (!S.isEmpty()) {
            Q.add(S.pop());
        }

        return Q;
    }

    public String[] Tokenizer (String query) {
        return query.split(" ");
    }

    public static void main(String[] args) {
        String[] infixNotation2 = {"(", "calculate", "AND", "similarity", "OR", "NOT", "query", "OR", "documents", ")"};
        System.out.println(new QueryParses().convertInfixToRPN(infixNotation2));


        String[] infixNotation = {"CALPURNIA", "AND", "BRUTUS"};
        System.out.println(new QueryParses().convertInfixToRPN(infixNotation));


    }
}


