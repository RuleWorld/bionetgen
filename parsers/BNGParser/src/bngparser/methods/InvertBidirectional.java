package bngparser.methods;

import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.antlr.runtime.ANTLRFileStream;
import org.antlr.runtime.CharStream;
import org.antlr.stringtemplate.StringTemplate;

/**
 * This class receives a reaction and inverts. It's used in for bidirectional reactions
 * Change later for a proper ANTLR tree procedure
 * @author proto
 *
 */
public class InvertBidirectional {
	public static StringTemplate invert(String original, int number){
		
		Pattern pattern = Pattern.compile("<ListOfReactantPatterns>.*</ListOfReactantPatterns>" , Pattern.DOTALL | Pattern.MULTILINE);
		Pattern pattern2 = Pattern.compile("<ListOfProductPatterns>.*</ListOfProductPatterns>" , Pattern.DOTALL | Pattern.MULTILINE);
		Pattern pattern3 = Pattern.compile("<ListOfOperations>.*</ListOfOperations>",Pattern.DOTALL|Pattern.MULTILINE);
		
		Matcher matches1 = pattern.matcher(original);
		String match1;
		if(matches1.find())
			match1 = new String(matches1.group());
		else
			return null;
		
		Matcher matches2 = pattern2.matcher(original);
		matches2.find();
		String match2 = new String(matches2.group());
		
		Matcher matches3 = pattern3.matcher(original);
		matches3.find();
		String match3 = new String(matches3.group());
		
		
		String[] splitted = pattern.split(original);
		String[] splitted2 = pattern2.split(splitted[1]);
		
		
		String[] splitted3 = pattern3.split(splitted2[1]);

		
		
		match1 = match1.replaceAll("ReactantPattern", "ProductPattern");
		match1 = match1.replaceAll("(RR[0-9]+)_RP", "$1_PP");
		
		match2 = match2.replaceAll("ProductPattern", "ReactantPattern");
		match2 = match2.replaceAll("(RR[0-9]+)_PP", "$1_RP");

		if(match3.contains("PP"))
				match3 = match3.replaceAll("(RR[0-9]+)_PP", "$1_RP");
		else
			match3 = match3.replaceAll("(RR[0-9]+)_RP", "$1_PP");
		
		if(match3.indexOf("AddBond")>0)
			match3 = match3.replaceAll("AddBond", "DeleteBond");
		else if(match3.indexOf("DeleteBond")>0)
				match3 = match3.replaceAll("DeleteBond", "AddBond");
					
			
		
				
		
		//inverts the reactants and products, making the necessary adjustments in the tags
		String finalString = splitted[0] + match2 +"\n\t"+ match1 + splitted3[0] + match3 + splitted3[1];
		
		//for the map section, it inverts the relationships between source and target
		finalString = finalString.replaceAll("(sourceID)(=\"RR.+_)(RP)(.+_M.+\") (targetID)(=\"RR.+_)(PP)(.+_M.+\")", "sourceID$6RP$8 targetID$2PP$4");
		//adds an internal reaction counter to the name
		finalString = finalString.replaceAll("(\"RR)([0-9]+)","$1" + number);
		//adds the reverse tag to the reaction name
		finalString = finalString.replaceAll("(<ReactionRule.+name=\")([A-Za-z0-9_]+)(\">)","$1$2(reverse)$3");
		
		
		
		
		return new StringTemplate(finalString);
	}
	
}
