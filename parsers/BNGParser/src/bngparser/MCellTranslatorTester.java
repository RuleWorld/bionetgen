package bngparser;

import java.io.FileReader;
import java.io.IOException;
import java.io.FileWriter;
import java.util.HashMap;
import java.util.Map;

import org.antlr.runtime.*;
import org.antlr.stringtemplate.*;
import org.antlr.stringtemplate.language.*;


import bngparser.dataType.ChangeableChannelTokenStream;
import bngparser.grammars.*;
//import bngparser.netGrammar.*;

/*
 * TODO:
 * output list of operations. Missing compartment action stuff
 * labels
 * 
  */
public class MCellTranslatorTester {
	
	public static String[] processParameters(String[] args){
		String[] options = new String[3];
		options[2] = "";
		options[1] = "";
		options[0] = "";
		for(int i =0; i<args.length;i++){
			if(args[i].equals("-i")){
				options[0] = args[++i];
			}
			else if(args[i].equals("-o")){
				options[1] = args[++i];
			}
			else if(args[i].equals("-c")){
				options[2] = "c";
			}
			
			
		}
		return options;
	}
	public static void main(String[] args) throws IOException,RecognitionException{
		
		
		String inputFile = "RecTest.net";
		//String inputFile = "testModels/egfr_compartments.net";
		String outputFile = "output.mdl";
		
		boolean compartments = true;
		/*if(args.length > 0){
			String[] parameters = processParameters(args);
			inputFile = parameters[0];
			outputFile = parameters[1];
			compartments = parameters[2].equals("c");
			
		}*/
		/*else{
			System.err.println("format: java -jar bngparser.jar -i <inputFile> -o <outputFile> [-c]");
			return;
		}*/
		Map<String, String> map = new HashMap<String,String>();
		map.put("D_3D", "1e-8");
		map.put("location", "[0.1,0.1,0.1]");
		map.put("SITE_DIAMETER", "1.88");
		
		
		CharStream input = new ANTLRFileStream(inputFile);
		BNGLexer bnglexer = new BNGLexer(input);
		 ChangeableChannelTokenStream tokens = new ChangeableChannelTokenStream(bnglexer);
		 
		NETGrammar parser = new NETGrammar(tokens);
		parser.compartmentToSurface = compartments;
		parser.surfaces.add("B");	
		//System.out.println(parser.surfaces.size());
		//parser.addOptions("DNA(a)", map);
		
		StringTemplateGroup template = new StringTemplateGroup(new FileReader("mcell.stg"),AngleBracketTemplateLexer.class);
		parser.setTemplateLib(template);
		RuleReturnScope r = parser.prog();
		
		FileWriter writer = new FileWriter(outputFile);
		writer.write(r.getTemplate().toString());
		writer.close();
		
	}
}
