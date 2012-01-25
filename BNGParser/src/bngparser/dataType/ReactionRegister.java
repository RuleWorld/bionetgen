package bngparser.dataType;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;


public class ReactionRegister {
	public String getCompartment() {
		return compartment;
	}
	public void setCompartment(String compartment) {
		this.compartment = compartment;
	}
	public String getBonds() {
		return bonds;
	}
	public void setBonds(String bonds) {
		this.bonds = bonds;
	}
	
	public void setBondList(BondList bondList){
		this.bondList = bondList;
	}
	
	public BondList getBondList(){
		return bondList;
	}
	
	private String compartment;
	private String bonds;
	private int numBonds;
	private BondList bondList;
	
	public int getNumBonds() {
		return numBonds;
	}
	public void setNumBonds(int numBonds) {
		this.numBonds = numBonds;
	}
	
	public static void addElement(Map<String,List<ReactionRegister>> map,String key,ReactionRegister register){
		if(!map.containsKey(key))
			map.put(key, new ArrayList<ReactionRegister>());
		map.get(key).add(register);
		
	}
	
	public static void mergeMaps(Map<String,List<ReactionRegister>> sourceMap,Map<String,List<ReactionRegister>> destinyMap){
		for(String key:sourceMap.keySet()){
			if(!destinyMap.containsKey(key)){
				destinyMap.put(key, new ArrayList<ReactionRegister>());
			}
			destinyMap.get(key).addAll(sourceMap.get(key));
		}
	}
}
