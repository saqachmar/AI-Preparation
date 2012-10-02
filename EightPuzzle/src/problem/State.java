/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package problem;

import java.util.ArrayList;
import java.util.List;

/**
 * The class State ; a state has a configuration, a parent ( with the exception of the root),and g_score and f_score ( to make calculation easy after) 
 * @author Soufiane
 */
public class State {
    private String configuration;
    private State parent;
    private int g_score=0;
    private int f_score=0;
    
    /**
     * first constructor of State; it only takes the configuration ( could be good for the root)
     * @param conf  the configuration of the state
     */
    public State(String conf){
        setConfiguration(conf);
    }
    
    /**
     * the second constructor of the state
     * @param conf  the configuration of the state
     * @param parent the parent of the state
     * @param g_score the corresponding g_score of this state
     * @param f_score  the corresponding f_score of this state
     */
    public State (String conf, State parent,int g_score,int f_score){
        setConfiguration(conf);
        this.parent=parent;
        this.g_score=g_score;
        this.f_score=f_score;
    }
    /**
     * @return the configuration
     */
    public String getConfiguration() {
        return configuration;
    }

    /**
     * @param configuration the configuration to set
     */
    public void setConfiguration(String configuration) {
        if(configuration.length()==9 )
            this.configuration = configuration;
        else
            System.out.println("error in the input");
    }
    
   public List<State> getneighbors(){
        List<State> neighbors=new ArrayList<>();
        int blank_index=this.getConfiguration().indexOf("b");
        
       // System.out.println("Index blank at "+blank_index);
        
        if(blank_index>=3)   // up
        {  char temp=this.getConfiguration().charAt(blank_index-3);
           StringBuilder builder=new StringBuilder(configuration);
           builder.setCharAt(blank_index, temp);        //swap blank with array[indexOfBlank-3]   
           builder.setCharAt(blank_index-3, 'b');
           String up_neighbor_string=builder.toString();
           State up_neighbor=new State(up_neighbor_string);
           neighbors.add(up_neighbor);
        }
        
         if(blank_index!=2 && blank_index!=5&& blank_index!=8)   // right
        {  char temp=this.getConfiguration().charAt(blank_index+1);
           StringBuilder builder=new StringBuilder(configuration);
           builder.setCharAt(blank_index, temp);        //swap blank with array[indexOfBlank+1]   
           builder.setCharAt(blank_index+1, 'b');
           String right_neighbor_string=builder.toString();//swap blank with array[indexOfBlank+1]
           State right_neighbor=new State(right_neighbor_string);
           neighbors.add(right_neighbor);
        }
         
        if(blank_index!=0 && blank_index!=3&& blank_index!=6)   // left
        {  
           char temp=this.getConfiguration().charAt(blank_index-1);
           StringBuilder builder=new StringBuilder(configuration);
           builder.setCharAt(blank_index, temp);        //swap blank with array[indexOfBlank-1]   
           builder.setCharAt(blank_index-1, 'b');
           String left_neighbor_string=builder.toString();//swap blank with array[indexOfBlank-1]
           State left_neighbor=new State(left_neighbor_string);
           neighbors.add(left_neighbor);
        } 
       if(blank_index>=0 && blank_index<=5)   // down
        {  
           char temp=this.getConfiguration().charAt(blank_index+3);
           StringBuilder builder=new StringBuilder(configuration);
           builder.setCharAt(blank_index, temp);        //swap blank with array[indexOfBlank+3]   
           builder.setCharAt(blank_index+3, 'b');
           String down_neighbor_string=builder.toString();//swap blank with array[indexOfBlank+3]
           State down_neighbor=new State(down_neighbor_string);
           neighbors.add(down_neighbor);
        } 
     //  System.out.println("Neighbors are :\n"+neighbors);
       return neighbors;
    }
    
    @Override
    public String toString(){
        return String.format("%s",this.configuration);
    }
    
    @Override
    public boolean equals(Object o){    //might needs modifications to include F and parent testing
        if(o==null) return false;
        if(o==this) return true;
        if (!(o instanceof State))return false;
         State oState = (State)o;
        if(oState.configuration.equals(this.configuration))
            return true;
        else return false;
    }
   
   public boolean isGoal(){
       
       return (this.configuration.equals("12345678b"));
   } 
   
   public void displayState(){
       for(int i=0;i<9;i++)
           System.out.print(this.configuration.charAt(i)+" ");
       System.out.print("\n");     
   }

    /**
     * @return the parent
     */
    public State getParent() {
        return parent;
    }

    /**
     * @param parent the parent to set
     */
    public void setParent(State parent) {
        this.parent = parent;
    }

    /**
     * @return the g_score
     */
    public int getG_score() {
        return g_score;
    }

    /**
     * @param g_score the g_score to set
     */
    public void setG_score(int g_score) {
        this.g_score = g_score;
    }

    /**
     * @return the f_score
     */
    public int getF_score() {
        return f_score;
    }

    /**
     * @param f_score the f_score to set
     */
    public void setF_score(int f_score) {
        this.f_score = f_score;
    }
   
  
   
       
  
}
