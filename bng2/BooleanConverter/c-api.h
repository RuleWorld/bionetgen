/*  $Id: c-api.h,v 1.16 2007/08/26 18:45:26 sarrazip Exp $
    c-api.h - C API function prototypes

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

#ifndef _H_c_api
#define _H_c_api

/*
    NOTES:

    This C API wraps the C++ API for BoolExpr<string>.
    It thus assumes that the "value" stored in a tree node is a
    character string.

    For more details on the meaning of the functions defined in this file,
    see the equivalent methods in BoolExpr.h (with T == std::string).
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>


/**
    Cookie type for an expression tree node.
    The NULL value represents the absence of a node.
*/
typedef void *boolexpr_t;

/** Possible types of a node. */
enum bool_operator_t
{
    BOOLSTUFF_VALUE,
    BOOLSTUFF_AND,
    BOOLSTUFF_OR,
    BOOLSTUFF_NOT
};

/** Possible error codes returned by the parser. */
enum bool_error_t
{
    BOOLSTUFF_OK,
    BOOLSTUFF_ERR_GARBAGE_AT_END,
    BOOLSTUFF_ERR_RUNAWAY_PARENTHESIS,
    BOOLSTUFF_ERR_IDENTIFIER_EXPECTED
};


/**
    Creates a node that contains a copy of the designated string as its value.
    @param	value	string to use as the node value
			(if NULL, an empty string is used as the node value)
    @returns		the created node
*/
boolexpr_t boolstuff_create_value_node(const char *value);

/**
    Creates a node that represents a boolean operator.
    @param	op	type of operator
    @param	left	left-hand child to attach to the created node
    			(when creating a node of type BOOLSTUFF_NOT,
			pass NULL for 'left')
    @param	right	right-hand child to attach to the created node
    @returns		the created node
*/
boolexpr_t boolstuff_create_operator_node(
		enum bool_operator_t op,
		boolexpr_t left,
		boolexpr_t right);

/**
    Parse a textual boolean expression and creates a binary tree.
    @param	expr		boolean expression to parse (must not be NULL)
    @param	error_index	pointer to a size_t that will receive
    				the index in 'expr' where the error was
				detected (ignored if NULL)
    @param	error_code	pointer to an 'enum bool_error_t' that will
    				receive the error code (ignored if NULL)
    @returns			the created tree,
				or NULL if an error occurred
*/
boolexpr_t boolstuff_parse(
		const char *expr,
		size_t *error_index,
		enum bool_error_t *error_code);

/**
    Destroys a tree and all its dynamically allocated nodes.
    @param	root	root of the tree to be destroyed
*/
void boolstuff_destroy_tree(boolexpr_t root);

/**
    Returns of type of the node (value or operator).
    @param	node	node of which to return the type
    @returns		the type of the node
*/
enum bool_operator_t boolstuff_get_node_type(boolexpr_t node);

/**
    Returns the value of node that represents a variable.
    @param	node	node that must be of type BOOLSTUFF_VALUE
    @returns		a pointer to internal storage that contains the
			string value of the node
*/
const char *boolstuff_get_node_value(boolexpr_t node);

/**
    Returns the root of the left-hand subtree of a node.
    @param	node	node of which to return the left-hand subtree
    @returns		the node at the root of the left-hand subtree
			(will be NULL for nodes of types BOOLSTUFF_VALUE
			or BOOLSTUFF_NOT)
*/
boolexpr_t boolstuff_get_left_subtree(boolexpr_t node);

/**
    Returns the root of the right-hand subtree of a node.
    @param	node	node of which to return the right-hand subtree
    @returns		the node at the root of the right-hand subtree
			(will be NULL for nodes of types BOOLSTUFF_VALUE)
*/
boolexpr_t boolstuff_get_right_subtree(boolexpr_t node);

/**
    Sets the type of a node.
    @param	node	node whose type is to be set
    @param	op	new type of this node
*/
void boolstuff_set_node_type(boolexpr_t node, enum bool_operator_t op);

/**
    Attaches a subtree as a node's left-hand subtree.
    @param	node	node that becomes the parent
    @param	subtree	root of the tree that becomes the left-hand subtree
*/
void boolstuff_set_left_subtree(boolexpr_t node, boolexpr_t subtree);

/**
    Attaches a subtree as this node's right-hand subtree.
    @param	node	node that becomes the parent
    @param	subtree	root of the tree that becomes the right-hand subtree
*/
void boolstuff_set_right_subtree(boolexpr_t node, boolexpr_t subtree);

/**
    Changes the string value of a node.
    @param	node	node whose value is to be changed
    @param	value	string that becomes the node's new value
			(must not be NULL)
*/
void boolstuff_set_node_value(boolexpr_t node, const char *value);

/**
    Prints a boolean expression tree in a file.
    @param	out	file in which to write the boolean expression
    @param	root	root node of the boolean expression tree to write
*/
void boolstuff_print_tree(FILE *out, boolexpr_t root);

/*
    Prints a boolean expression tree in a string.
    @returns		a character pointer that must not be modified and
			MUST be submitted to boolstuff_free_string() afterwards
*/
char *boolstuff_print_tree_to_string(boolexpr_t root);

/**
    Frees the memory used by a string created by this library.
    @param	s	the address of the string to be freed
*/
void boolstuff_free_string(char *s);

/**
    Creates a complete copy of a binary tree.
    @param	root	root of the tree to be cloned
    @returns		the root of the cloned tree
*/
boolexpr_t boolstuff_clone_tree(boolexpr_t root);

/**
    Transforms a tree into the Disjunctive Normal Form.
    @param	root	root of the tree to transform
    @returns		the new root of the tree
*/
boolexpr_t boolstuff_get_disjunctive_normal_form(boolexpr_t root);

/**
    Determines if a tree is in Disjunctive Normal Form.
    @param	root	root of the tree to examine
    @returns		1 if the tree is in DNF, 0 otherwise
*/
int boolstuff_is_disjunctive_normal_form(boolexpr_t root);

/**
    Returns the terms of a tree in Disjunctive Normal Form.

    The tree must be in DNF.
    The nodes of the returned trees must not be modified.
    boolstuff_free_node_array() MUST be called afterwards on the
    returned array pointer to free the allocated memory.

    @param	root	root of the DNF tree of which to return the terms
    @param	num	pointer to a size_t that receives the number of
			non-null pointers in the returned array
			(ignored if NULL)
    @returns		an array of the nodes at the root of the DNF terms
			(a NULL pointer is stored at the end of this array
			to mark its end)
*/
boolexpr_t *boolstuff_get_dnf_term_roots(boolexpr_t root, size_t *num);

/**
    Frees an array returned by boolstuff_get_dnf_term_roots().
    @param	array	array to be freed (ignored if NULL)
*/
void boolstuff_free_node_array(boolexpr_t *array);

/*
    Returns the variables that are used in the tree rooted at this node.

    The strings in the returned arrays must not be modified.
    boolstuff_free_variables_sets() MUST BE CALLED AFTERWARDS on the
    returned pointers to free the allocated memory.

    @param	tree	root of the tree of which to get the variables
    @param	positivesArray	pointer to an array of character strings
				representing the values of the variables
				that are used positively in the tree
				(the array is terminated by a NULL pointer)
    @param	negativesArray	pointer to an array of character strings
				representing the values of the variables
				that are used negatively in the tree
				(the array is terminated by a NULL pointer)
*/
void boolstuff_get_tree_variables(boolexpr_t tree,
				char ***positivesArray,
				char ***negativesArray);

/**
    Frees the arrays created by boolstuff_get_tree_variables().
    @param	positivesArray	array of positive variables values
    @param	negativesArray	array of negative variables values
*/
void boolstuff_free_variables_sets(
				char **positivesArray,
				char **negativesArray);


#ifdef __cplusplus
}
#endif

#endif  /* _H_c_api */
