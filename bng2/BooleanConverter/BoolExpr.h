/*  $Id: BoolExpr.h,v 1.19 2008/10/08 04:19:32 sarrazip Exp $
    BoolExpr.h - Boolean expression binary tree node

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

#ifndef _H_BoolExpr
#define _H_BoolExpr

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include <sstream>


/**
    Namespace that contains all symbols defined by this library.
*/
namespace boolstuff {

/**
    Object representing a node in a boolean expression binary tree.
    All objects of this class are expected to be allocated by operator new.

    The value type T must be a concrete type with a default constructor,
    a copy constructor and an assignment operator.
    Type T must be LessThan Comparable (i.e., it supports the operators
    < and ==).
    If one of the print() methods is called, there must be a function
    of the form operator << (std::ostream &, const T &).

    See class BoolExprParser for a way to obtain a tree from a
    textual boolean expression.
*/
template <class T>
class BoolExpr
{
public:
    /**
	Possible types for a boolean expression tree node.
    */
    enum Type
    {
	/** Node, without subtrees, that contains a value of type T.
	*/
	VALUE,

	/** Conjunction node with left and right subtrees.
	*/
	AND,

	/** Disjunction node with left and right subtrees.
	*/
	OR,

	/** Negation node with only a right subtree.
	*/
	NOT
    };

    /**
	Creates a VALUE node with the given initial value.

	This library expects all BoolExpr objects to be allocated
	by operator new.

	@param	initValue	initial value for the created node
    */
    BoolExpr(const T &initValue = T());

    /**
	Creates a node of the given type and with the given children.
	A NOT node must only have a right-hand child, while AND and OR
	nodes must have both left-hand and right-hand children.

	Example:<BR>
	    BoolExpr<string> *left = new BoolExpr<string>("left subtree");<BR>
	    BoolExpr<string> *right = new BoolExpr<string>("right subtree");<BR>
	    BoolExpr<string> *root = new BoolExpr<string>(
					BoolExpr<string>::AND, left, right);<BR>
	    delete root;<BR>

	@param	t	type of the node (must be AND, OR or NOT)
	@param	l	subtree to attach as the left-hand child (may be NULL)
	@param	r	subtree to attach as the right-hand child (may be NULL)
    */
    BoolExpr(Type t, BoolExpr *l, BoolExpr *r);

    /**
	Calls delete on the left and right subtrees of this node.

	This library expects all BoolExpr objects to be destroyed
	by operator delete.
    */
    ~BoolExpr();

    /** Returns the type of this node. */
          Type      getType() const;

    /** Returns the left-hand child of this node. */
    const BoolExpr *getLeft() const;

    /**
	Returns the left-hand child of this node.
	Operator delete should not be called on the subtrees returned
	by getLeft().  Only the root of a tree should
	be the target of a destruction.
    */
          BoolExpr *getLeft();

    /** Returns the right-hand child of this node. */
    const BoolExpr *getRight() const;

    /**
	Returns the right-hand child of this node.
	Operator delete should not be called on the subtrees returned
	by getRight().  Only the root of a tree should
	be the target of a destruction.
    */
          BoolExpr *getRight();

    /**
	Returns the value of this node.
	getValue() should only be called on a node for which getType()
	returns BoolExpr::VALUE.
    */
    const T        &getValue() const;

    /**
	Returns the value of this node.
	getValue() should only be called on a node for which getType()
	returns BoolExpr::VALUE.
    */
          T        &getValue();

    /**
	Changes the type of this node.
	@param	t	the type to this to this node
    */
    void setType(Type t);

    /**
	Attaches a subtree as this node's left-hand child.
	If this node already had a non null left-hand child, it is
	not destroyed before attaching the new child.
	@param	subtree		the subtree to attach (may be NULL)
    */
    void setLeft(BoolExpr *subtree);

    /**
	Attaches a subtree as this node's right-hand child.
	If this node already had a non null right-hand child, it is
	not destroyed before attaching the new child.
	@param	subtree		the subtree to attach (may be NULL)
    */
    void setRight(BoolExpr *subtree);

    /**
	Changes the value of this node.
	This method should only be called on a node of type VALUE.
	The given value is copied into this node's value field.
	@param	v	value to give to this node
    */
    void setValue(const T &v);

    /**
	Returns a copy of the designated tree.
	All nodes in the returned tree are independent copies of those
	in the original tree.
	All the cloned nodes are created with operator new.
	The caller must eventually destroy the cloned tree by calling
	operator delete on its root node.

	@param	root	the root of the tree to be copied
	@returns	the root of the created tree (NULL if root was NULL)
    */
    static BoolExpr *cloneTree(const BoolExpr *root);

    /**
	Transforms the designated tree into its Disjunctive Normal Form.
	The proper way to call this method is the following:

	    root = BoolExpr<SomeType>::getDisjunctiveNormalForm(root);

	The original tree root does not necessarily remain the root
	of the transformed tree.

	A simplification is applied: when a term of the form
	a&!a&(something) is seen, it is deleted unless it is the
	root of the whole tree.

	CAUTION: this method can return a NULL pointer; such a result
	should be interpreted as a "false" boolean expression.
	Examples are when the original (or resulting) tree is
	a&!a, or a&!a|b&!b.
	This method also returns NULL if 'root' is NULL.

	@param	root	root of the tree to transform
	@returns	the root of the transformed tree (may be NULL)
    */
    static BoolExpr *getDisjunctiveNormalForm(BoolExpr *root);

    /**
	Like getDisjunctiveNormalForm(), but without simplifications.
    */
    static BoolExpr *getRawDNF(BoolExpr *root);

    /**
	Determines if the tree rooted at this node is in the DNF.
	@returns	true or false
    */
    bool isDisjunctiveNormalForm() const;

    /**
	Gets the roots of the terms of an tree in DNF form.

	The DNF is a sum of products.  Each term in this sum is
	represented by a subtree of the tree rooted at the current node.
	This method produces the BoolExpr<T> pointers that
	represent the roots of the term subtrees.

	Returns the iterator at the position past the last insertion.

	The tree must first be in DNF.  See getDisjunctiveNormalForm().

	For example, if the current node is the root a of DNF tree
	representing the expression a&b | c | d&e, then three pointers
	will be stored: one for the 'a&b' subtree, one for the 'c'
	subtree (a single node) and one for the 'd&e' subtree.

	If the tree is a single node, then 'this' designates the
	only term in the sum and it is returned as the root of the
	unique term.

	The stored pointers must not be destroyed directly.

	Example:<BR>
	    vector<const BoolExpr<string> *> termRoots;
	    dnfRoot->getDNFTermRoots(inserter(termRoots, termRoots.end()));

	@param	dest	output iterator that supports the notation *dest++,
			where the expression *dest is of type
			'const BoolExpr<T> *'.
	@returns	the output iterator that designates the end of the
			produced sequence
    */
    template <class OutputIter>
    OutputIter getDNFTermRoots(OutputIter dest) const;

    /**
	Returns the variables that are used in the tree rooted at this node.

	Example: with T == string and the expression a&b&!a&!c,
	the 'positives' set will contain "a" and "b" and the
	'negatives' set will contain "a" and "c".

	When the intersection between the two sets is not empty
	and the only binary operator used in the tree is AND, the
	tree always evaluates to false (because we have an expression
	of the form (a&!a)&(whatever)).
	If the only binary operator is OR, the tree always evaluates
	to true.

	@param	positives	set that receives the T values of the variables
				that are used positively
	@param	negatives	set that receives the T values of the variables
				that are used negatively

    */
    void getTreeVariables(std::set<T> &positives, std::set<T> &negatives) const;

    /**
	Determines if this DNF term always evaluates to false.
	Must only be called on a term of a DNF tree, which can be obtained
	with the getDNFTermRoots() method.
	(e.g., a&b&!a).
	@returns	true if and only if this term always evaluates to false
    */
    bool isDNFTermUseful() const;

    /**
	Prints the boolean expression tree rooted at this node in a stream.
	Does not print a newline afterwards.
	Uses no unnecessary parentheses.
	Uses '!', '|' and '&' as the NOT, OR and AND operator.

	If this method is called, there must be a function
	of the form operator << (ostream &, const T &).

	@param	out	stream into which the tree representation is written
    */
    void print(std::ostream &out) const;

    /**
	Prints the boolean expression tree rooted at this node in a string.

	If this method is called, there must be a method
	of the form operator << (ostream &, const T &).

	@returns	the string representation of this tree
    */
    std::string print() const;

private:
    Type type;
    T value;
    BoolExpr *left;
    BoolExpr *right;

    friend class BoolExprParser;

    static void simplifyXAndXTerms(std::vector<BoolExpr<T> *> &terms);
    static void destroyDNFOrNodes(BoolExpr<T> *root);
    static BoolExpr<T> *joinTreesWithOrNodes(
				const std::vector<BoolExpr<T> *> &trees);
    bool isDNFTermUseful(const std::set<T> &positives,
			 const std::set<T> &negatives) const;
    static BoolExpr<T> *negateDNF(BoolExpr<T> *root);
};


template <class T>
inline
std::ostream &
operator << (std::ostream &out, const BoolExpr<T> *root)
/*
    Prints nothing if 'root' is NULL.
*/
{
    if (root != NULL)
	root->print(out);
    return out;
}


#include "BoolExpr.cpp"


}  // namespace boolstuff

#endif  /* _H_BoolExpr */
