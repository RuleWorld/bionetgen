package bngparser;

import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.FileWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.antlr.runtime.*;
import org.antlr.stringtemplate.*;
import org.antlr.stringtemplate.language.*;


import bngparser.dataType.ChangeableChannelTokenStream;
import bngparser.exceptions.BNGSemanticException;
import bngparser.grammars.*;
//import bngparser.netGrammar.*;

/*
 * TODO:
 * output list of operations. Missing compartment action stuff
 * labels
 * 
  */
public class Tester {
	
	public static List<String> getFileNames(String sDir){
			  File[] faFiles = new File(sDir).listFiles();
			  List<String> listOfFiles = new ArrayList<String>();
			  for(File file: faFiles){
			    if(file.getName().matches("^(.*?)bngl") && !file.isDirectory()){
			      listOfFiles.add(file.getAbsolutePath());
			    }
			    if(file.isDirectory()){
			    	listOfFiles.addAll(getFileNames(file.getAbsolutePath()));
			    }
			  }
			  return listOfFiles;
	}
	
	public static void main(String[] args) throws IOException,RecognitionException{
		
		String inputFile2 = "testModels/NFSim/actin/actin_simple.bngl";
		//String inputFile = "testModels/FullModelBurstSeconds.bngl";
		
		//List<String> inputFileList = getFileNames("testModels/NFSim");
		List<String> inputFileList = new ArrayList<String>();
		inputFileList.add("testModels/Motivating.bngl");
		String outputFile = "output.xml";
		
		if(args.length > 0){
			inputFile2 = args[0];
			outputFile = args[1];
		}
		else{
			//System.err.println("format: java -jar bngparser.jar <inputFile> <outputFile>");
			//return;
		}
		Map<String, String> map = new HashMap<String,String>();
		map.put("D_3D", "1e-8");
		map.put("location", "[0.1,0.1,0.1]");
		map.put("SITE_DIAMETER", "1.88");
		
		for (String inputFile: inputFileList){
			try{
			System.out.println("file: " + inputFile);
			CharStream input = new ANTLRFileStream(inputFile);
			BNGLexer bnglexer = new BNGLexer(input);
			 ChangeableChannelTokenStream tokens = new ChangeableChannelTokenStream(bnglexer);
			 
			//NETGrammar parser = new NETGrammar(tokens);
			 BNGGrammar parser = new BNGGrammar(tokens);
				
			
			//parser.addOptions("DNA(a)", map);
			StringTemplateGroup template = new StringTemplateGroup(new FileReader("xml.stg"),AngleBracketTemplateLexer.class);
			parser.setTemplateLib(template);
			RuleReturnScope r=null,r2=null;
			
				
			
			r = parser.prog();
			r2 = parser.actions_prog();
			
			//System.out.println(r2.getTemplate().toString());
			FileWriter writer = new FileWriter(outputFile);
			writer.write(r.getTemplate().toString());
			
			//r.g
			writer.close();
			System.out.println("pass");
			}
			catch(Exception e){
				System.err.println("fail");
				e.printStackTrace();
			}
		}
	}
}
