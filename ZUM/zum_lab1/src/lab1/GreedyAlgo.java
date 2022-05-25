package lab1;

import java.util.*;

public class GreedyAlgo {
    int visitedNodesCount;
    int flag = 0;
    private LinkedList<Node> open;
    private Map<Node, Node> prev;
    private List<Node> path;

    public List<Node> findPath(Maze maze, Node startNode, Node endNode) {
        ArrayList<Node> arrayList = new ArrayList<>();
        prev = new HashMap<Node, Node>();
        PriorityQueue<Node> pg = new PriorityQueue<>(200, new cmp());

        Node n1 = new Node(startNode.getRow(), startNode.getCol(), endNode);
        pg.add(n1);
        visitedNodesCount++;

        Node current;

        while (!pg.isEmpty()) {
            //maze.printMaze();
            current = pg.remove();
            if (startNode.equalNode(current, maze.end)) {
                return buildPath(n1, current);
            }
            for (Node y : current.neighbours(maze, current)) {
                if (flag == 0) {
                    Scanner in = new Scanner(System.in);
                    flag = (in.nextInt());
                    maze.printMaze();
                }
                if (maze.array[y.getRow()][y.getCol()] != 'V' && maze.array[y.getRow()][y.getCol()] != 'O') {
                    pg.add(y);
                    visitedNodesCount++;
                    maze.array[y.getRow()][y.getCol()] = 'O';
                    prev.put(y, current);
                }
            }
            maze.array[current.getRow()][current.getCol()] = 'V';
        }
        return null;
    }


    private List<Node> buildPath(Node start, Node target) {
        path = new ArrayList<Node>();
        path.add(target);
        while (start != target) {
            target = prev.get(target);
            path.add(target);
        }
        return path;
    }
}

class cmp implements Comparator<Node> {

    @Override
    public int compare(Node n1, Node n2) {
        if (n1.distance < n2.distance)
            return -1;
        if (n1.distance > n2.distance)
            return 1;
        return 0;
    }
}