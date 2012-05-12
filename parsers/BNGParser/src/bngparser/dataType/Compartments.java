package bngparser.dataType;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;



public class Compartments {
	
	Map<String,String[]> compartments;

	public Compartments() {
		compartments = new HashMap<String,String[]>();
	}
	
	public void addCompartment(String name, String dimensions, String volume, String container){
		compartments.put(name,new String[] {name,dimensions,volume,container});
	}
	
	public List<String[]> getSurfacesinVolume(String name){
		List<String[]> temp = new ArrayList<String[]>();
		for	(String key: compartments.keySet()){
			if(compartments.get(key)[3].equals(name) && compartments.get(key)[1].equals("2"))
				temp.add(compartments.get(key));
		}
		return temp;
	}
	
	public void addCompartment(String name, String dimensions, String volume){
		compartments.put(name,new String[] {name,dimensions,volume,""});
	}
	
	public void addContainer(String name,String container){
		compartments.get(name)[3] = container;
	}
	
	public boolean isSurface(String compartment){
		if(compartments.get(compartment)[1].equals("2"))
			return true;
		return false;
	}
	
	public boolean isVolume(String compartment){
		if(compartments.get(compartment)[1].equals("3"))
			return true;
		return false;
	}
	
	public String getParentCompartment(String compartment){
		return compartments.get(compartment)[3];
	}
	
	public String getDimension(String compartment){
		return compartments.get(compartment)[1];
	}
	
	public boolean isOuterCompartment(String compartment){
		return compartments.get(compartment)[3].equals("");
	}
	
	public String getVolume(String compartment){
		return compartments.get(compartment)[2];
	}

	public String getECMVolume(){
		for(String keys: compartments.keySet()){
			if(isOuterCompartment(compartments.get(keys)[0])){
				return compartments.get(keys)[2];
			}
			
		}
		return "0";
	}
	
	public String getCytoplasmicVolume(){
		for(String keys: compartments.keySet()){
			if(!isOuterCompartment(compartments.get(keys)[0]) && isVolume(compartments.get(keys)[0])){
				return compartments.get(keys)[2];
			}
			
		}
		return "0";		
		
	}
	
	public String getMembraneVolume(){
		for(String keys: compartments.keySet()){
			if(isSurface(compartments.get(keys)[0])){
				return compartments.get(keys)[2];
			}
		}
		return "0";		
		
	}
	
	public List<String[]> getVolumeCompartments(){
		List<String[]> volume = new ArrayList<String[]>();
		
		for(String keys: compartments.keySet()){
			if(compartments.get(keys)[1].equals("3")){
				volume.add(compartments.get(keys));
			}
		}
		return volume;
	}
	
	public List<String[]> getSurfaceCompartments(){
		List<String[]> surface = new ArrayList<String[]>();
		
		for(String keys: compartments.keySet()){
			if(compartments.get(keys)[1].equals("2")){
				surface.add(compartments.get(keys));
			}
		}
		return surface;
	}
	
	
}
