package dum;

import java.util.*;

public class QueryEvaluator {


    private List<TermValue> calculate(List<TermValue> left, List<TermValue> right, String operator) {
        List<TermValue> res = new ArrayList<>();
        ListIterator<TermValue> itLeft = left.listIterator();
        ListIterator<TermValue> itRight = right.listIterator();

        var v1 = itRight.hasNext() ? itRight.next() : null;
        var v2 = itLeft.hasNext() ? itLeft.next() : null;

        if (operator.equals("AND")) {
            while (v1 != null && v2 != null) {
                if (v1.getIdx() < v2.getIdx()) {
                    TermValue tv = new TermValue(v1.getIdx(), v1.getFrequency(), CalculateAND(v1.getWeight(), 0));
                    res.add(tv); //calculate
                    v1 = itRight.hasNext() ? itRight.next() : null;
                }
                else if (v1.getIdx() > v2.getIdx()) {
                    TermValue tv = new TermValue(v2.getIdx(), v2.getFrequency(), CalculateAND(v2.getWeight(), 0));
                    res.add(tv); //calculate
                    v2 = itLeft.hasNext() ? itLeft.next() : null;
                }
                else if (v1.getIdx() == v2.getIdx()) {
                    TermValue tv = new TermValue(v1.getIdx(), v1.getFrequency() + v2.getFrequency(), CalculateAND(v1.getWeight(), v2.getWeight()));
                    res.add(tv);
                    v1 = itRight.hasNext() ? itRight.next() : null;
                    v2 = itLeft.hasNext() ? itLeft.next() : null;
                }
            }
            while (v1 != null) {
                TermValue tv = new TermValue(v1.getIdx(), v1.getFrequency(), CalculateAND(v1.getWeight(), 0));
                res.add(tv); //calculate
                v1 = itRight.hasNext() ? itRight.next() : null;
            }
            while (v2 != null) {
                TermValue tv = new TermValue(v2.getIdx(), v2.getFrequency(), CalculateAND(v2.getWeight(), 0));
                res.add(tv); //calculate
                v2 = itLeft.hasNext() ? itLeft.next() : null;
            }
        }
        else if ("OR".equals(operator)) {
            while (v1 != null && v2 != null) {
                if (v1.getIdx() < v2.getIdx()) {
                    TermValue tv = new TermValue(v1.getIdx(), v1.getFrequency(), CalculateOR(v1.getWeight(), 0));
                    res.add(tv); //calculate
                    v1 = itRight.hasNext() ? itRight.next() : null;
                }
                else if (v1.getIdx() > v2.getIdx()) {
                    TermValue tv = new TermValue(v2.getIdx(), v2.getFrequency(), CalculateOR(v2.getWeight(), 0));
                    res.add(tv); //calculate
                    v2 = itLeft.hasNext() ? itLeft.next() : null;
                }
                else if (v1.getIdx() == v2.getIdx()) {
                    TermValue tv = new TermValue(v1.getIdx(), v1.getFrequency() + v2.getFrequency(), CalculateOR(v1.getWeight(), v2.getWeight()));
                    res.add(tv);
                    v1 = itRight.hasNext() ? itRight.next() : null;
                    v2 = itLeft.hasNext() ? itLeft.next() : null;
                }
            }
            while (v1 != null) {
                TermValue tv = new TermValue(v1.getIdx(), v1.getFrequency(), CalculateOR(v1.getWeight(), 0));
                res.add(tv); //calculate
                v1 = itRight.hasNext() ? itRight.next() : null;
            }
            while (v2 != null) {
                TermValue tv = new TermValue(v2.getIdx(), v2.getFrequency(), CalculateOR(v2.getWeight(), 0));
                res.add(tv); //calculate
                v2 = itLeft.hasNext() ? itLeft.next() : null;
            }

        } else throw new IllegalArgumentException("Invalid input");
        return res;
    }

    private double CalculateAND(double w1, double w2) {
        return 1 - Math.sqrt((Math.pow(1 - w1, 2) + Math.pow(1 - w2, 2)) / 2);
    }

    private double CalculateOR(double w1, double w2) {
        return Math.sqrt((Math.pow(w1, 2) + Math.pow(w2, 2)) / 2);
    }

    private List<TermValue> calculateNOT(List<TermValue> termValues, int length) {
        List<TermValue> res = new ArrayList<>();
        ListIterator<TermValue> itRight = termValues.listIterator();
        var v1 = itRight.hasNext() ? itRight.next() : null;
        for (int i = 0; i < length; ++i) {
            if (v1 != null) {
                if (v1.getIdx() == i) {
                    res.add(new TermValue(v1.getIdx(), v1.getFrequency(), 1 - v1.getWeight()));
                    v1 = itRight.hasNext() ? itRight.next() : null;
                } else {
                    res.add(new TermValue(i, 0, 1 ));
                }

            } else {
                res.add(new TermValue(i, 0, 1 ));
            }

        }
        return res;
    }

    public List<TermValue> evalQuery(Queue<String> tokens, HashMap<String, List<TermValue>> terms, int length) {
        Stack<List<TermValue>> stack = new Stack<>();
        Set<String> operators = new HashSet<>();
        operators.add("AND");
        operators.add("OR");

        String cur;

        for (String token : tokens) {
            cur = token;
            if (operators.contains(cur)) {
                // Be careful, the first pop is the right operand,
                // the second pop is the left operand
                List<TermValue> right = stack.pop();
                List<TermValue> left = stack.pop();
                stack.push(calculate(left, right, cur));

            } else if (cur.equals("NOT")) {
                List<TermValue> pop = stack.pop();
                stack.push(calculateNOT(pop, length));
            } else {
                if (terms.get(cur) == null) {
                    stack.push(new ArrayList<>());
                } else {stack.push(terms.get(cur)); }
            }
        }
        return stack.pop();
    }



}
