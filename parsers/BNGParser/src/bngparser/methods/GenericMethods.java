package bngparser.methods;

import java.util.regex.Pattern;

import org.antlr.stringtemplate.StringTemplate;

public class GenericMethods {
	public static StringTemplate replace(StringTemplate original,String find, String replace){
		if(original == null)
			 return null;
		String temp = original.toString();
		   
		if(!find.equals(replace)){
			temp = temp.replace(find, replace);
		}
		return new StringTemplate(temp);
	}
}
