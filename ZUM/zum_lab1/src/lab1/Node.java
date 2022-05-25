package lab1;

import java.util.ArrayList;
import java.util.List;

public class Node {
    double distance;
    private int row, col;
    private Node end;

    public Node(int row, int col) {
        this.col = col;
        this.row = row;
    }

    public Node(int row, int col, Node end) {
        this.col = col;
        this.row = row;
        this.end = end;
        this.distance = calculateDistance(end);
    }

    public int getCol() {
        return col;
    }

    public int getRow() {
        return row;
    }

    @Override
    public String toString() {
        return "(x, y) = (" + row + ", " + col + ")";
    }

    public List<Node> neighbours(Maze maze, Node node) {
        List<Node> list = new ArrayList<>();
        if (maze.array[node.row + 1][node.col] == ' ' || maze.array[node.row + 1][node.col] == 'e') {
            list.add(new Node(node.row + 1, node.col, maze.end));
        }
        if (maze.array[node.row - 1][node.col] == ' ' || maze.array[node.row - 1][node.col] == 'e') {
            list.add(new Node(node.row - 1, node.col, maze.end));
        }
        if (maze.array[node.row][node.col - 1] == ' ' || maze.array[node.row][node.col - 1] == 'e') {
            list.add(new Node(node.row, node.col - 1, maze.end));
        }
        if (maze.array[node.row][node.col + 1] == ' ' || maze.array[node.row][node.col + 1] == 'e') {
            list.add(new Node(node.row, node.col + 1, maze.end));
        }
        return list;
    }

    public boolean equalNode(Node n1, Node n2) {
        return (n1.col == n2.col) && (n1.row == n2.row);
    }

    public double calculateDistance(Node b) {
        return Math.sqrt((col - b.getCol()) * (col - b.getCol()) + (row - b.getRow()) * (row - b.getRow()));
    }

}