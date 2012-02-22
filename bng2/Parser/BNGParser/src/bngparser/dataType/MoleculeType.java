package bngparser.dataType;

import java.util.ArrayList;

public class MoleculeType {
	ArrayList<SingleMoleculeType> includedTypes;
	ArrayList<MoleculeBond> bonds;
	
	public void addMolecules(SingleMoleculeType moleculeType){
		includedTypes.add(moleculeType);
	}
	
	public void addBond(MoleculeBond bond){
		bonds.add(bond);
	}
}
