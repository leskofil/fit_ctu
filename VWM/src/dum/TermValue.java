package dum;

public class TermValue implements Comparable<TermValue>{

    private final int idx;
    private double frequency = 0;
    private double weight = 0;
    private String name = "";

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public TermValue (int idx, double frequency, double weight ) {
        this.idx = idx;
        this.frequency = frequency;
        this.weight = weight;
    }

    public int getIdx() { return idx; }

    public double getFrequency() { return frequency; }

    public void setFrequency ( double frequency ) { this.frequency = frequency; }

    public double getWeight() { return weight; }

    public void setWeight ( double weight ) { this.weight = weight; }

    @Override
    public int compareTo ( TermValue t ) { return idx - t.idx; }

    public boolean compareToWeights ( TermValue t ) { return weight < t.getWeight(); }

}
