/*  $Id: BoolExprParser.h,v 1.14 2008/10/07 02:29:03 sarrazip Exp $
    BoolExprParser.h - Boolean expression parser and syntax tree builder

    boolstuff - Disjunctive Normal Form boolean expression library
    Copyright (C) 2002-2005 Pierre Sarrazin <http://sarrazip.com/>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
    02111-1307, USA.
*/

#ifndef _H_BoolExprParser
#define _H_BoolExprParser

#include "BoolExpr.h"

#include <string>


namespace boolstuff {


/**
    Parser for a language of boolean expressions.
    The parse() method dynamically allocates a binary tree of nodes that
    represents the syntactic structure of a textual boolean expression.
*/
class BoolExprParser
{
public:

    /**  Error descriptor. */
    class Error
    {
    public:
	/** Possible error codes returned by the parser. */
	enum Code
	{
	    /** Unexpected characters follow the position that the parser
		views as the end of the expression.
	    */
	    GARBAGE_AT_END,

	    /** An opening parenthesis has no corresponding closing
		parenthesis.
	    */
	    RUNAWAY_PARENTHESIS,

	    /** A variable identifier was expected.
		Until version 0.1.11 of this library, this enumerated name
		was STRING_EXPECTED, but the term "string" was too general.
	    */
	    IDENTIFIER_EXPECTED,

	    /** Deprecated older enumerated name for IDENTIFIER_EXPECTED.
	    */
	    STRING_EXPECTED = IDENTIFIER_EXPECTED
	};

	/** Index (>=0) in the input string where the error was detected. */
	size_t index;

	/** Code that gives the type of the error */
	Code code;

	/**
	    Initializes an error object with the given index and error.
	*/
	Error(size_t i, Code c) : index(i), code(c) {}
    };


    /**
	Initializes the parser.
    */
    BoolExprParser();

    /**
	Destroys the parser and frees the associated resources.
    */
    ~BoolExprParser();

    /**
	Parses a textual boolean expression and creates a binary syntax tree.
	Dynamically allocates a tree of nodes that represents the
	syntactic structure of 'expr'.
	The returned tree must eventually be destroyed with operator delete.

	@param	expr	text of the boolean expression to parse
	@returns	the root of the created tree
	@throws	Error	describes a parsing error
    */
    BoolExpr<std::string> *parse(const std::string &expr) throw(Error);

private:

    std::string curInput;
    size_t curIndex;

    // Implementation methods:
    BoolExpr<std::string> *parseExpr() throw(Error);
    BoolExpr<std::string> *parseTerm() throw(Error);
    BoolExpr<std::string> *parseFactor() throw(Error);
    BoolExpr<std::string> *parseAtom() throw(Error);
    BoolExpr<std::string> *parseIdentifier() throw(Error);

    bool atEnd();
    bool tokenSeen(const char *s);
    void skipToken(const char *s);
    void skipSpaces();
    bool isIdentifierChar(char c) const;

    // Forbidden operations:
    BoolExprParser(const BoolExprParser &);
    BoolExprParser &operator = (const BoolExprParser &);
};


}  // namespace boolstuff


#endif  /* _H_BoolExprParser */
