package bngparser;

import bngparser.grammars.BNGGrammar;

public class BNGParseData 
{
	// The abstract syntax tree
	private bngparser.grammars.BNGGrammar.prog_return m_ast;
	
	// temporary.  I am returning the parser here so I can do whatever I want with it in my code for testing. 
	private BNGGrammar m_parser;
	
	// Need to include error data in here somehow. 
	
	public BNGParseData(BNGGrammar parser) 
	{
		setParser(parser);
	}

	private void setParser(BNGGrammar parser) 
	{
		m_parser = parser;
	}

	public BNGGrammar getParser()
	{
		return m_parser;
	}
}
