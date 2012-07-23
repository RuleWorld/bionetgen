package bngparser.methods;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import bngparser.dataType.BondList;

/**
 * This class analizes a given reaction and outputs the reactant-product mapping and any necessary actions
 * @author proto
 *
 */
class MoleculeBonds{
	
	String name;
	BondList bonds;
	String[] components;
	
	public String[] getComponents() {
		return components;
	}

	public String getCompartment() {
		return compartment;
	}

	String compartment;
	
	public String getName() {
		return name;
	}

	public BondList getBonds() {
		return bonds;
	}


	
	public MoleculeBonds(String name, BondList bonds, String[] components) {
		this.name = name;
		this.bonds = bonds;
		this.components = components;
	}
	
	@Override
	public String toString() {
		StringBuilder sb = new StringBuilder();
		sb.append(name + " ");
		sb.append(bonds.getLeft().size());
		return sb.toString();
	}
}


public class ReactionAction {
	private Map<String, List<MoleculeBonds>> molecules,copyMolecules;
	List<String> id;
	List<String> leftMap;
	List<String> rightMap;
	boolean flag = false;

	public List<String> getOperator1() {
		return operator1;
	}

	public List<String> getOperator2() {
		return operator2;
	}
	
	public List<String> getOperations() {
		return operations;
	}


	List<String> operations;
	List<String> operator1;
	List<String> operator2;
	
	public ReactionAction(){
		molecules = new HashMap<String,List<MoleculeBonds>>();
		copyMolecules = new HashMap<String,List<MoleculeBonds>>();
		id = new ArrayList<String>();
		leftMap = new ArrayList<String>();
		rightMap = new ArrayList<String>();
		operations = new ArrayList<String>();
		operator1 = new ArrayList<String>();
		operator2 = new ArrayList<String>();
	}
	
	public void addMolecule(String id, String molecule, BondList  bonds){
		String[] species = molecule.split("\\.");
		String moleculeWithoutSpecies,moleculeSpecies;
		
		for(int i=0; i < species.length; i++){
			moleculeWithoutSpecies = species[i].replaceAll("(.+)\\(.*\\)", "$1");
			moleculeSpecies = species[i].replaceAll("(.+)\\((.*)\\).*", "$2");
			String[] components = moleculeSpecies.split(",");
				if(!molecules.containsKey(moleculeWithoutSpecies)){
					molecules.put(moleculeWithoutSpecies, new ArrayList<MoleculeBonds>());
					copyMolecules.put(moleculeWithoutSpecies, new ArrayList<MoleculeBonds>());
				}
					MoleculeBonds temp = new MoleculeBonds(id + "_M" + (i+1),bonds,components);
					molecules.get(moleculeWithoutSpecies).add(temp);
					copyMolecules.get(moleculeWithoutSpecies).add(temp);
			
			this.id.add(id);
			
		}
		
		
	}
	
	public List<String> getLeft(){
		return leftMap;
	}
	
	public List<String> getRight(){
		return rightMap;
	}
	
	
	public void execute(){
		Map<String,List<String>> tempBonds = new HashMap<String,List<String>>();
		for(String reactant: molecules.keySet()){
			List<MoleculeBonds> molecule = molecules.get(reactant);
			
			moleculeTag:
			for(MoleculeBonds species1: molecule){
				if(species1.getName().contains("RP")){
					for(MoleculeBonds species2: molecule){
						if(!species2.getName().contains("RP")){
							leftMap.add(species1.getName());
							rightMap.add(species2.getName());
							
							//TODO: addbond and delete bond have to be done on a per component level. 
							
							//if 2 molecules map two each other we need to check how the components map too
							for(int i = 0; i< species1.getComponents().length;i++){
								for(int j = 0; j< species2.getComponents().length;j++){
									String component1 = species1.getComponents()[i].replaceAll("([A-Za-z0-9_]+).*","$1").replaceAll("\\s","");
									String component2 = species2.getComponents()[j].replaceAll("([A-Za-z0-9_]+).*","$1").replaceAll("\\s","");
									if(component1.equals(component2)){
										leftMap.add(species1.getName() + "_C" + (i+1));
										rightMap.add(species2.getName() + "_C" + (j+1));
										
										
										
										if(species1.getComponents()[i].contains("!") && 
												!species2.getComponents()[j].contains("!")){
											
											//String label = species1.getComponents()[i].replaceAll("(.+)(![A-Za-z0-9]+)", "$2");
											String[] label = species1.getComponents()[j].split("!");
											for(int counter=1;counter<label.length;counter++){
												if(tempBonds.get(label[counter]) == null){
													tempBonds.put(label[counter], new ArrayList<String>());
													tempBonds.get(label[counter]).add("DeleteBond");
												}
													tempBonds.get(label[counter]).add(species2.getName() + "_C" + (j+1));
											}
												
										}
										else if(species2.getComponents()[j].contains("!") && 
												!species1.getComponents()[i].contains("!")){
											//this step checks how many molecules a single components is bound to and adds them to the add bond operation
											String[] label = species2.getComponents()[j].split("!");
											for(int counter=1;counter<label.length;counter++){
												if(tempBonds.get(label[counter]) == null){
													tempBonds.put(label[counter], new ArrayList<String>());
													tempBonds.get(label[counter]).add("AddBond");
												}
												tempBonds.get(label[counter]).add(species1.getName() + "_C" + (i+1));
											}
										}
										species2.getComponents()[j] = "";	
											
										break;
									}
									
								}
								
								
							}
							
							
							
							/*if(species1.getBonds().getNumBonds() > species2.getBonds().getNumBonds()){
								operations.add("DeleteBond");
								operator1.add(species1.getName());
								operator2.add(species2.getName());
							}
							else if(species1.getBonds().getNumBonds() < species2.getBonds().getNumBonds()){
								operations.add("AddBond");
								operator1.add(species1.getName());
								operator2.add(species2.getName());
							}*/
							
							species2.name = "RP"; //this is to indicate that it species2 is not eligible to appear again in the map section
					
							copyMolecules.get(reactant).remove(species1);
							copyMolecules.get(reactant).remove(species2);
							
							
							continue moleculeTag; //and this is to indicate the same for species1
						}
					}
				}
			}
			if(copyMolecules.get(reactant).size()>0){
				for(MoleculeBonds species: copyMolecules.get(reactant)){
					if(species.getName().contains("RP")){
						operations.add("Delete");
						operator1.add("id=\"" + species.getName() + "\"");
						operator2.add("");
					}
					else if(species.getName().contains("PP")){
						operations.add("Add");
						operator1.add("id=\"" +species.getName()+ "\"");
						operator2.add("");
					}
						
				}
			}
		}
		//this adds the sites fields in the add/delete actions
		for(String label: tempBonds.keySet()){
			if(tempBonds.get(label).size()>2){
				operations.add(tempBonds.get(label).get(0));
				//System.out.println(tempBonds.get(label).get(1));
				operator1.add("site1=\"" + tempBonds.get(label).get(1) + "\"");
				operator2.add("site2=\"" + tempBonds.get(label).get(2) + "\"");
			}
		}
		
	}
}
