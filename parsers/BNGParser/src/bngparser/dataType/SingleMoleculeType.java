package bngparser.dataType;

import java.util.ArrayList;
import java.util.Collections;

public class SingleMoleculeType {

	private String name;
	private ArrayList<MoleculeSites> sites;

	
	public String getName() {
		return name;
	}

	public ArrayList<MoleculeSites> getArrayList() {
		return sites;
	}

	public SingleMoleculeType(String name){
		this.name = name;
		sites = new ArrayList<MoleculeSites>();
	}
	
	public void addNewSiteElement(String name){
		sites.add(new MoleculeSites());
		sites.get(sites.size()-1).setSiteName(name);
	}
	
	/**
	 * Adds an state to the last site in the list
	 * @param element
	 */
	public void addElement(String element){
		sites.get(sites.size()-1).add(element);
	}
	
	public void reverse(){
		Collections.reverse(sites);
	}
	
	@Override
	public String toString() {
		StringBuilder s = new StringBuilder();
		s.append("name:" + name + "\n");
		for(MoleculeSites moleculeSite: sites){
			s.append("\tsite name: " + moleculeSite.getSiteName());
			if(moleculeSite.size() > 0)
				s.append("\n\tstates:\n\t\t");
			for(String state: moleculeSite.getStates())
				s.append(state + ", ");
			
			s.append("\n");
		}
		
		return s.toString();
	}
}
