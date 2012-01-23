/*  $Id: c-api.cpp,v 1.19 2010/02/14 19:32:11 sarrazip Exp $
    c-api.cpp - C API function definitions

    boolstuff - Disjunctive Normal Form boolean expression library
    Copyright (C) 2002-2008 Pierre Sarrazin <http://sarrazip.com/>

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

#include "c-api.h"

#include "BoolExprParser.h"

#include <cstring>
#include <sstream>
#include <vector>
#include <string.h>

using namespace std;
using namespace boolstuff;


inline
boolexpr_t
cookie(const BoolExpr<string> *node)
{
    return reinterpret_cast<boolexpr_t>(const_cast<BoolExpr<string> *>(node));
}


inline
BoolExpr<string> *
uncookie(boolexpr_t node)
{
    return reinterpret_cast<BoolExpr<string> *>(node);
}


boolexpr_t
boolstuff_create_value_node(const char *value)
{
    if (value == NULL)
	value = "";
    return cookie(new BoolExpr<string>(value));
}


boolexpr_t
boolstuff_create_operator_node(
		enum bool_operator_t op,
		boolexpr_t left,
		boolexpr_t right)
{
    BoolExpr<string>::Type type;
    switch (op)
    {
	case BOOLSTUFF_VALUE:	type = BoolExpr<string>::VALUE; break;
	case BOOLSTUFF_AND:		type = BoolExpr<string>::AND; break;
	case BOOLSTUFF_OR:		type = BoolExpr<string>::OR; break;
	case BOOLSTUFF_NOT:		type = BoolExpr<string>::NOT; break;
	default:		assert(false); return NULL;
    }
    return cookie(new BoolExpr<string>(type, uncookie(left), uncookie(right)));
}


boolexpr_t
boolstuff_parse(const char *expr, size_t *error_index, bool_error_t *error_code)
{
    try
    {
	if (error_index != NULL)
	    *error_index = 0;
	if (error_code != NULL)
	    *error_code = BOOLSTUFF_OK;

	BoolExprParser parser;
	return cookie(parser.parse(expr));
    }
    catch (const BoolExprParser::Error &e)
    {
	if (error_index != NULL)
	    *error_index = e.index;
	if (error_code != NULL)
	{
	    switch (e.code)
	    {
		case BoolExprParser::Error::GARBAGE_AT_END:
		    *error_code = BOOLSTUFF_ERR_GARBAGE_AT_END;
		    break;
		case BoolExprParser::Error::RUNAWAY_PARENTHESIS:
		    *error_code = BOOLSTUFF_ERR_RUNAWAY_PARENTHESIS;
		    break;
		case BoolExprParser::Error::IDENTIFIER_EXPECTED:
		    *error_code = BOOLSTUFF_ERR_IDENTIFIER_EXPECTED;
		    break;
		default:
		    *error_code = BOOLSTUFF_OK;  // should not happen
	    }
	}
	return NULL;
    }
}


void
boolstuff_destroy_tree(boolexpr_t root)
{
    delete uncookie(root);
}


enum bool_operator_t
boolstuff_get_node_type(boolexpr_t node)
{
    switch (uncookie(node)->getType())
    {
	case BoolExpr<string>::VALUE:	return BOOLSTUFF_VALUE;
	case BoolExpr<string>::AND:	return BOOLSTUFF_AND;
	case BoolExpr<string>::OR:	return BOOLSTUFF_OR;
	case BoolExpr<string>::NOT:	return BOOLSTUFF_NOT;
	default:			return BOOLSTUFF_VALUE;
    }
}


const char *boolstuff_get_node_value(boolexpr_t node)
{
    return uncookie(node)->getValue().c_str();
}


boolexpr_t
boolstuff_get_left_subtree(boolexpr_t node)
{
    return cookie(uncookie(node)->getLeft());
}


boolexpr_t
boolstuff_get_right_subtree(boolexpr_t node)
{
    return cookie(uncookie(node)->getRight());
}


void
boolstuff_set_node_type(boolexpr_t node, enum bool_operator_t op)
{
    BoolExpr<string>::Type type;
    switch (op)
    {
	case BOOLSTUFF_AND:	type = BoolExpr<string>::AND; break;
	case BOOLSTUFF_OR:	type = BoolExpr<string>::OR;  break;
	case BOOLSTUFF_NOT:	type = BoolExpr<string>::NOT; break;
	default:		type = BoolExpr<string>::VALUE;
    }
    uncookie(node)->setType(type);
}


void boolstuff_set_left_subtree(boolexpr_t node, boolexpr_t subtree)
{
    uncookie(node)->setLeft(uncookie(subtree));
}


void boolstuff_set_right_subtree(boolexpr_t node, boolexpr_t subtree)
{
    uncookie(node)->setRight(uncookie(subtree));
}


void boolstuff_set_node_value(boolexpr_t node, const char *value)
{
    uncookie(node)->setValue(value);
}


boolexpr_t
boolstuff_clone_tree(boolexpr_t root)
{
    return cookie(BoolExpr<string>::cloneTree(uncookie(root)));
}


boolexpr_t
boolstuff_get_disjunctive_normal_form(boolexpr_t root)
{
    return cookie(BoolExpr<string>::getDisjunctiveNormalForm(uncookie(root)));
}


int
boolstuff_is_disjunctive_normal_form(boolexpr_t root)
{
    return uncookie(root)->isDisjunctiveNormalForm();
}


boolexpr_t *
boolstuff_get_dnf_term_roots(boolexpr_t root, size_t *num)
{
    vector<const BoolExpr<string> *> termRoots;
    uncookie(root)->getDNFTermRoots(inserter(termRoots, termRoots.end()));

    assert(termRoots.size() > 0);

    boolexpr_t *array = new boolexpr_t[termRoots.size() + 1];
    for (size_t i = 0; i < termRoots.size(); i++)
    {
	assert(termRoots[i] != NULL);
	array[i] = cookie(termRoots[i]);
    }
    array[termRoots.size()] = NULL;  // marks the end

    if (num != NULL)
	*num = termRoots.size();
    return array;
}


void
boolstuff_free_node_array(boolexpr_t *array)
{
    delete [] array;
}


void
boolstuff_print_tree(FILE *out, boolexpr_t root)
{
    string s = uncookie(root)->print();
    fprintf(out, "%s", s.c_str());
}


char *
boolstuff_print_tree_to_string(boolexpr_t root)
{
    ostringstream buffer;
    uncookie(root)->print(buffer);
    string contents = buffer.str();
    return strdup(contents.c_str());
}


void
boolstuff_free_string(char *s)
{
    free(s);
}


void
boolstuff_get_tree_variables(boolexpr_t tree,
				char ***positivesArray,
				char ***negativesArray)
{
    set<string> positives, negatives;
    uncookie(tree)->getTreeVariables(positives, negatives);

    char **posArray = (char **) malloc((positives.size() + 1) * sizeof(char *));
    char **negArray = (char **) malloc((negatives.size() + 1) * sizeof(char *));

    set<string>::const_iterator it;
    size_t i;
    for (i = 0, it = positives.begin(); it != positives.end(); it++, i++)
	posArray[i] = strdup((*it).c_str());
    posArray[i] = NULL;
    for (i = 0, it = negatives.begin(); it != negatives.end(); it++, i++)
	negArray[i] = strdup((*it).c_str());
    negArray[i] = NULL;

    *positivesArray = posArray;
    *negativesArray = negArray;
}


void
boolstuff_free_variables_sets(char **positivesArray, char **negativesArray)
{
    if (positivesArray != NULL)
    {
	for (size_t i = 0; positivesArray[i] != NULL; i++)
	    free(positivesArray[i]);
	free(positivesArray);
    }

    if (negativesArray != NULL)
    {
	for (size_t i = 0; negativesArray[i] != NULL; i++)
	    free(negativesArray[i]);
	free(negativesArray);
    }
}
