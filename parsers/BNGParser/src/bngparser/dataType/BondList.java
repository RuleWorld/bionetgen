package bngparser.dataType;

import java.util.ArrayList;
import java.util.Map;
import java.util.TreeMap;
import java.util.List;

import antlr.SemanticException;

/**
 * This class serves as a data structure listing the bonds a molecule contains
 * @author proto
 *
 */
class StringPair {
	
	
	public StringPair(){
		left = "";
		right = "";
	}
	public String getLeft() {
		return left;
	}
	public void setLeft(String left) {
		this.left = left;
	}
	public void setLeft(String left, String leftName){
		setLeft(left);
		this.leftName = left;
	}
	public String getRight() {
		return right;
	}
	public void setRight(String right) {
		this.right = right;
	}
	public void setRight(String right, String rightName){
		setRight(right);
		this.rightName = rightName;
	}
	
	public String getLeftName(){
		return leftName;
	}
	public String getRightName(){
		return rightName;
	}
	private String left;
	private String leftName;
	
	private String right;
	private String rightName;
}

public class BondList {
	
	Map<String, StringPair> map;
	public BondList() {
		map = new TreeMap<String, StringPair>();
	}
	public void addElement(String key,String element,String name){
		try{
		if(map.containsKey(key)){
			StringPair temp = map.get(key);
			if(temp.getRight() == ""){
				temp.setRight(element,name);
				map.put(key,temp);
			}
			else throw new Exception();
				
			
		}
		else{
			StringPair temp = new StringPair();
			temp.setLeft(element,name);
			map.put(key, temp);
		}
		}
		catch(Exception e){
			System.err.println("Invalid bond: Bond " + key + " is assigned to more than one binding site");
		}
	}
	
	
	
	public List<String> getLeft(){
		List<String> list = new ArrayList<String>();
		
		for(Map.Entry<String,StringPair> entry: map.entrySet()){
			list.add(entry.getValue().getLeft());
		}
		return list;
		
	}
	
	public List<String> getRight(){
		List<String> list = new ArrayList<String>();
		
		for(Map.Entry<String,StringPair> entry: map.entrySet()){
			list.add(entry.getValue().getRight());
		}
		return list;
		
	}
	
	public boolean validateBonds(int line, int column){
		for(Map.Entry<String,StringPair> entry: map.entrySet()){
			if(entry.getValue().getRight().equals(""))
				return false;
			
		}
		return true;
	}
	
	public int getNumBonds(){
		return map.size();
	}
}