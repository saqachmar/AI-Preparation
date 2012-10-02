/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package problem;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.PriorityQueue;
//import java.util.Scanner;

/**
 *
 * @author Soufiane
 */
public class AStar {

    final public static int increment = 1;

    public static int h1(State state, State goal) {
        int counter_displaced = 0;
        for (int i = 0; i < 9; i++) {
            if (state.getConfiguration().charAt(i) != 'b' && state.getConfiguration().charAt(i) != goal.getConfiguration().charAt(i)) {
                counter_displaced++;
            }
        }
        return counter_displaced;
    }

    public static int h2(State state, State goal) {
        int distance = 0;
        int reference_values[][] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 1}, {1, 2}, {2, 0}, {2, 1}};
        for (int i = 0; i < 9; i++) {
            if (state.getConfiguration().charAt(i) != 'b') {
                if (i <= 2) {

                    distance = distance + Math.abs(0 - reference_values[Character.getNumericValue(state.getConfiguration().charAt(i)) - 1][0]) + Math.abs(i % 3 - reference_values[Character.getNumericValue(state.getConfiguration().charAt(i)) - 1][1]);
                } else if (i <= 5) {
                    distance = distance + Math.abs(1 - reference_values[Character.getNumericValue(state.getConfiguration().charAt(i)) - 1][0]) + Math.abs(i % 3 - reference_values[Character.getNumericValue(state.getConfiguration().charAt(i)) - 1][1]);

                } else {
                    distance = distance + Math.abs(2 - reference_values[Character.getNumericValue(state.getConfiguration().charAt(i)) - 1][0]) + Math.abs(i % 3 - reference_values[Character.getNumericValue(state.getConfiguration().charAt(i)) - 1][1]);
                }

            }
        }
        return distance;
    }

    public static void Search(int heuristicType, State initialState, State goalState) {
        final int heuristic = heuristicType;
        final State goal = goalState;
        List<State> closedList = new ArrayList();
        PriorityQueue<State> openList = new PriorityQueue(11,
                new Comparator<State>() {
                    @Override
                    public int compare(State S1, State S2) {
                        if (heuristic == 0) {                                       //case of heuristic a
                            int s2_value = S2.getG_score() + h1(S2, goal);
                            int s1_value = S1.getG_score() + h1(S1, goal);
                            return s1_value - s2_value;
                        } else {                                  //case of heuristic a
                            int s2_value = S2.getG_score() + h2(S2, goal);
                            int s1_value = S1.getG_score() + h2(S1, goal);
                            return s1_value - s2_value;   //inversed because our priority queue takes the element with lowest cost
                        }

                    }
                });

        openList.add(initialState);
        initialState.setG_score(0);
    //    Scanner sc = new Scanner(System.in);
        int noSolution = 0;
        if (heuristic == 0) {
            initialState.setF_score(initialState.getG_score() + h1(initialState, goal));
        } else {
            initialState.setF_score(initialState.getG_score() + h2(initialState, goal));
        }
        while (!(openList.isEmpty())) {
            State currentNode = openList.peek();
            // System.out.println("current selected state is: "+currentNode.toString()+" with f="+currentNode.getF_score());
            // sc.nextLine();
            if (currentNode.isGoal()) {
                //TODO  return build solution
                build_solution(currentNode, initialState);
                noSolution = 1;
                break;

            }
            closedList.add(openList.poll());    //add it to the closed List 
            List<State> neighbors = currentNode.getneighbors();
            for (State neighbor : neighbors) {
                if (closedList.contains(neighbor)) {
                    continue;
                }
                int g_score_try = currentNode.getG_score() + increment;
                if (!(openList.contains(neighbor)) || g_score_try < neighbor.getG_score()) {
                    neighbor.setParent(currentNode);
                    neighbor.setG_score(g_score_try);
                    if (heuristic == 0) {
                        neighbor.setF_score(h1(neighbor, goal) + neighbor.getG_score());
                    } else {
                        neighbor.setF_score(h2(neighbor, goal) + neighbor.getG_score());
                    }
                    if (!(openList.contains(neighbor))) {
                        openList.add(neighbor);
                    }
                }
            }
            //  State par=new State("1234b5786");
            //  State testState=new State("12345b786",par,1,3);
          //  System.out.println("openlist size is: " + openList.size());
            // boolean val= openList.contains(testState);
            //  System.out.println(val);
          //  System.out.println("Closed list size is: " + closedList.size());

        }
        if (noSolution == 0) {
            System.out.println("NONE");
        } //return no solution
    }

    public static void build_solution(State s, State initialState) {
        List<State> path = new ArrayList();
        path.add(s);
        while (!s.equals(initialState)) {
            path.add(s.getParent());
            s = s.getParent();
        }

        for (int i = path.size() - 1; i >= 0; i--) {
            path.get(i).displayState();
        }
    }
}
