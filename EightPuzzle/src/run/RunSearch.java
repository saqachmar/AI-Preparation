/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package run;
import problem.State;
import problem.AStar;
import java.util.Scanner;


/**
 *  This is a testing class for our algorithm 
 * @author Soufiane Aqachmar
 */
public class RunSearch {
    public static void main(String[] args){
        final long start=System.nanoTime();
        System.out.print("Enter your input ( no spaces ): ");
        Scanner sc=new Scanner(System.in);
        
        String input=sc.next();  //get the input as a string
        int option=Character.getNumericValue(input.charAt(0)); //the first element of the array is our heuristic choice
        String state=input.substring(1);      //the remaining of the string is our initial state representation
        State initialState=new State(state);  //we create a state based on the initial state
        State goalState=new State("12345678b"); //the goal state
        AStar.Search(option, initialState, goalState); //we search for a solution using A Star given the heuristic option, the goal state, and the initial state
      
        
        // final long duration=System.nanoTime()-start;
       // System.out.println("run time is "+duration+ " nano seconds, or roughly "+ Math.floor((double)duration/1000000000.0)+" seconds");

        
    }
    
    
}
