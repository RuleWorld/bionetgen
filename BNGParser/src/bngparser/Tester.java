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
import bngparser.netGrammar.*;

/*
 * TODO:
 * output list of operations. Missing compartment action stuff
 * labels
 * 
  */
public class Tester {
	
	
	public static void main(String[] args) throws IOException,RecognitionException{
		
		
		String inputFile = "compartments.net";
		String outputFile = "output.mdl";
		
		if(args.length > 0){
			inputFile = args[0];
			outputFile = args[1];
		}
		Map<String, String> map = new HashMap<String,String>();
		map.put("D_3D", "1e-8");
		map.put("location", "[0.1,0.1,0.1]");
		map.put("SITE_DIAMETER", "1.88");
		
		
		CharStream input = new ANTLRFileStream(inputFile);
		BNGLexer bnglexer = new BNGLexer(input);
		 ChangeableChannelTokenStream tokens = new ChangeableChannelTokenStream(bnglexer);
		 
		NETGrammar parser = new NETGrammar(tokens);
		// BNGGrammar parser = new BNGGrammar(tokens);
			
		
		//parser.addOptions("DNA(a)", map);
		
		StringTemplateGroup template = new StringTemplateGroup(new FileReader("mcell.stg"),AngleBracketTemplateLexer.class);
		parser.setTemplateLib(template);
		RuleReturnScope r = parser.prog();
		
		FileWriter writer = new FileWriter(outputFile);
		writer.write(r.getTemplate().toString());
		writer.close();
		
	}
}
