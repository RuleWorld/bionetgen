package bngparser.dataType;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

public class Register {
	public Register(Double value, String type) {
		super();
		this.value = value;
		this.type = type;
	}
	public Double getValue() {
		return value;
	}
	public void setValue(Double value) {
		this.value = value;
	}
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	private Double value;
	private String type;
	
	public static List<String> getTypes(Map<String,Register> map){
		List<String> list = new ArrayList<String>();
		for(String key: map.keySet()){
			if(map.get(key) != null)
				list.add(map.get(key).getType());
			
		}
		
		return list;
		
	}
}