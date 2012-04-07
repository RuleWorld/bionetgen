package bngparser;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.antlr.runtime.ANTLRStringStream;
import org.antlr.runtime.CharStream;
import org.antlr.runtime.RecognitionException;
import org.antlr.stringtemplate.StringTemplateGroup;
import org.antlr.stringtemplate.language.AngleBracketTemplateLexer;

import bngparser.dataType.ChangeableChannelTokenStream;
import bngparser.grammars.BNGGrammar;
import bngparser.grammars.BNGLexer;

public class BNGParserUtility 
{

	public static BNGParseData produceParserInfoForBNGLText(String text)
	{
		BNGGrammar parser = produceBNGGrammarForBNGLText(text);
		
		BNGParseData toReturn = new BNGParseData(parser);
		
		
		
		return toReturn;
				
	}
	
	public static bngparser.grammars.BNGGrammar.prog_return produceASTForBNGLText(String text) throws RecognitionException
	{
		BNGGrammar parser = produceBNGGrammarForBNGLText(text);
		
		// Parse the tokens, which produces the ast, and then return the ast.
		return parser.prog();
	}
	
	public static BNGGrammar produceBNGGrammarForBNGLText(String text)
	{
		// Create a CharStream from the text.
		CharStream input = new ANTLRStringStream(text);
		
		// Create the lexer that will tokenize the text.
		
		BNGLexer bnglexer = new BNGLexer(input);
		// Get the tokenstream from the lexer.
		
		ChangeableChannelTokenStream tokens = new ChangeableChannelTokenStream(bnglexer);

		// Create the grammar that will parse the tokens.
		BNGGrammar parser = new BNGGrammar(tokens);
				
		// Create the StringTemplateGroup that defines the xml format for the ast.
		StringTemplateGroup template;
		
		java.net.URL xmlURL = BNGParserUtility.class.getResource("/xml.stg");
		InputStream is = null;
		try {
			is = xmlURL.openStream();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		//template = new StringTemplateGroup(new FileReader("resources/xml.stg"), AngleBracketTemplateLexer.class);
		template = new StringTemplateGroup(new InputStreamReader(is), AngleBracketTemplateLexer.class);
		
		// Send the xml template to the parser.
		parser.setTemplateLib(template);
		
		return parser;
	}

}
