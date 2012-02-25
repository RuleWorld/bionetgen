package bngparser.dataType;

import java.util.ArrayList;

public class MoleculeSites{
	/**
	 * 
	 */
	private static final long serialVersionUID = -9138717443347474389L;
	private String siteName;
	private ArrayList<String> states;
	private String currentState;
	
	public String getSiteName() {
		return siteName;
	}

	public void setSiteName(String siteName) {
		this.siteName = siteName;
	}
	
	public void addState(String state){
		states.add(state);
	}
	
	public void setCurrentState(String state){
		this.currentState = state;
	}
	
	public String getCurrentState(){
		return currentState;
	}
	
	public int size(){
		return states.size();
	}
	
	public void add(String state){
		states.add(state);
	}
	
	public ArrayList<String> getStates(){
		return states;
	}
	
}
