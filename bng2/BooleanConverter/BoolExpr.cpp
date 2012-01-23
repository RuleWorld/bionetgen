/*  $Id: BoolExpr.cpp,v 1.24 2010/02/14 19:28:30 sarrazip Exp $
    BoolExpr.cpp - Boolean expression binary tree node

    boolstuff - Disjunctive Normal Form boolean expression library
    Copyright (C) 2002-2010 Pierre Sarrazin <http://sarrazip.com/>

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
#error "This file is only meant to be included by BoolExpr.h"
#endif

#include <set>
#include <algorithm>
#include <stdlib.h>


// Define this to enable traces that help in leak detection
// when used with script leak-detector.pl.
//
//#define BOOLSTUFF_LEAK_DETECTION 1


template <class T>
BoolExpr<T>::BoolExpr(const T &initValue /*= T()*/)
  : type(VALUE),
    value(initValue),
    left(NULL),
    right(NULL)
{
    #ifdef BOOLSTUFF_LEAK_DETECTION
    std::cerr << "CONS " << (void *) this << " " << type << " " << value << std::endl;
    #endif
}


template <class T>
BoolExpr<T>::BoolExpr(Type t, BoolExpr *l, BoolExpr *r)
  : type(t),
    value(),
    left(l),
    right(r)
{
    #ifdef BOOLSTUFF_LEAK_DETECTION
    std::cerr << "CONS " << (void *) this << " " << type << " " << value << std::endl;
    #endif
    assert(type == AND || type == OR || type == NOT);
}


template <class T>
BoolExpr<T>::~BoolExpr()
{
    #ifdef BOOLSTUFF_LEAK_DETECTION
    std::cerr << "DEST " << (void *) this << " " << type << " " << value << std::endl;
    #endif
    delete left;
    delete right;
}


template <class T>
inline
typename BoolExpr<T>::Type
BoolExpr<T>::getType() const
{
    return type;
}


template <class T>
inline
const BoolExpr<T> *
BoolExpr<T>::getLeft() const
{
    return left;
}


template <class T>
inline
BoolExpr<T> *
BoolExpr<T>::getLeft()
{
    return left;
}


template <class T>
inline
const BoolExpr<T> *
BoolExpr<T>::getRight() const
{
    return right;
}


template <class T>
inline
BoolExpr<T> *
BoolExpr<T>::getRight()
{
    return right;
}


template <class T>
inline
const T &
BoolExpr<T>::getValue() const
{
    return value;
}


template <class T>
inline
void
BoolExpr<T>::setType(Type t)
{
    type = t;
}


template <class T>
inline
void
BoolExpr<T>::setLeft(BoolExpr *subtree)
{
    left = subtree;
}


template <class T>
inline
void
BoolExpr<T>::setRight(BoolExpr *subtree)
{
    right = subtree;
}


template <class T>
inline
void
BoolExpr<T>::setValue(const T &v)
{
    value = v;
}


template <class T>
inline
T &
BoolExpr<T>::getValue()
{
    return value;
}


template <class T>
bool
BoolExpr<T>::isDisjunctiveNormalForm() const
{
    if (type == VALUE)
	return (left == NULL && right == NULL);

    if (type == NOT)
    {
	if (left != NULL || right == NULL || !right->isDisjunctiveNormalForm())
	    return false;

	return (right->type == VALUE || right->type == NOT);
    }

    if (type == AND)
    {
	if (left == NULL || right == NULL
			|| !left->isDisjunctiveNormalForm()
			|| !right->isDisjunctiveNormalForm())
	    return false;

	return (left->type != OR && right->type != OR);
    }

    if (type == OR)
    {
	if (left == NULL || right == NULL
			|| !left->isDisjunctiveNormalForm()
			|| !right->isDisjunctiveNormalForm())
	    return false;

	return true;
    }

    return false;
}


template <class T>
template <class OutputIter>
OutputIter
BoolExpr<T>::getDNFTermRoots(OutputIter dest) const
{
    switch (type)
    {
	case OR:
	    assert(left != NULL);
	    assert(right != NULL);
	    dest = left->getDNFTermRoots(dest);
	    return right->getDNFTermRoots(dest);

	case AND:
	case NOT:
	    assert(right != NULL);
	    assert((left == NULL) == (type == NOT));
	case VALUE:
	    *dest++ = const_cast<BoolExpr<T> *>(this);
	    return dest;

	default:
	    assert(false);
	    return dest;
    }
}


template <class T>
void
BoolExpr<T>::getTreeVariables(
			std::set<T> &positives, std::set<T> &negatives) const
{
    if (type == VALUE)
    {
	positives.insert(value);
	return;
    }

    if (type == NOT)
    {
	assert(right != NULL);
	negatives.insert(right->value);
	return;
    }

    assert(type == OR || type == AND);
    assert(left != NULL && right != NULL);
    left->getTreeVariables(positives, negatives);
    right->getTreeVariables(positives, negatives);
}


template <class T>
bool
BoolExpr<T>::isDNFTermUseful() const
{
    std::set<T> positives, negatives;
    getTreeVariables(positives, negatives);
    return isDNFTermUseful(positives, negatives);
}


template <class T>
bool
BoolExpr<T>::isDNFTermUseful(const std::set<T> &positives,
			     const std::set<T> &negatives) const
{
    std::set<T> intersection;
    set_intersection(positives.begin(), positives.end(),
			negatives.begin(), negatives.end(),
			std::inserter(intersection, intersection.end()));
    return intersection.empty();
}


template <class T>
void
BoolExpr<T>::print(std::ostream &out) const
{
    switch (type)
    {
	case VALUE:
	    out << value;
	    break;

	case NOT:
	    {
		bool paren = (right->type == AND || right->type == OR);
		out << '!';
		if (paren)
		    out << '(';
		right->print(out);
		if (paren)
		    out << ')';
	    }
	    break;

	case OR:
	    {
		left->print(out);
		out << "|";
		right->print(out);
	    }
	    break;

	case AND:
	    {
		bool paren = (left->type == OR);
		if (paren)
		    out << '(';
		left->print(out);
		if (paren)
		    out << ')';

		out << '&';

		paren = (right->type == OR);
		if (paren)
		    out << '(';
		right->print(out);
		if (paren)
		    out << ')';
	    }
	    break;

	default:
	    assert(false);
    }
}


template <class T>
std::string
BoolExpr<T>::print() const
{
    std::ostringstream buffer;
    print(buffer);
    return buffer.str();
}


template <class T>
/*static*/
BoolExpr<T> *
BoolExpr<T>::cloneTree(const BoolExpr<T> *root)
{
    if (root == NULL)
        return NULL;

    BoolExpr<T> *leftClone = cloneTree(root->left);
    BoolExpr<T> *rightClone = cloneTree(root->right);
    BoolExpr<T> *cloneRoot = new BoolExpr<T>(root->value);
    cloneRoot->type = root->type;
    cloneRoot->left = leftClone;
    cloneRoot->right = rightClone;

    return cloneRoot;
}


// Returns -1 if term a comes before term b, +1 if term a comes after term b,
// or 0 if they are equal.
//
template <class T>
static int exprComparator(BoolExpr<T> *a, BoolExpr<T> *b)
{
    if (a == NULL && b != NULL)
	return -1;
    if (b == NULL)
	return a == NULL ? 0 : +1;

    typename BoolExpr<T>::Type aType = a->getType();
    typename BoolExpr<T>::Type bType = b->getType();
    if (aType != bType)
	return int(aType) < int(bType) ? -1 : +1;

    switch (aType)
    {
	case BoolExpr<T>::VALUE:
	{
	    const T &aValue = a->getValue();
	    const T &bValue = b->getValue();
	    if (aValue < bValue)
		return -1;
	    if (bValue < aValue)
		return +1;
	    return 0;
	}
	case BoolExpr<T>::AND:
	case BoolExpr<T>::OR:
	{
	    int leftComparison = exprComparator(a->getLeft(),  b->getLeft());
	    if (leftComparison != 0)
		return leftComparison;
	}

	/*** NO BREAK ***/

	case BoolExpr<T>::NOT:
	    return exprComparator(a->getRight(), b->getRight());
    }

    return true;
}


// Returns true iff term a comes before term b.
//
template <class T>
static bool termOrdering(BoolExpr<T> *a, BoolExpr<T> *b)
{
    return exprComparator(a, b) < 0;
}


template <class T>
/*static*/
BoolExpr<T> *
BoolExpr<T>::getDisjunctiveNormalForm(BoolExpr<T> *root)
{
    BoolExpr<T> *dnfRoot = getRawDNF(root);
    if (dnfRoot == NULL)
	return dnfRoot;

    // Get the terms of the DNF:
    std::vector<BoolExpr<T> *> termRoots;
    dnfRoot->getDNFTermRoots(std::inserter(termRoots, termRoots.end()));

    // If only one term, consider it "useful",
    // because we must return something:
    if (termRoots.size() <= 1)
    {
	// Simplify terms of the form x & x.
	simplifyXAndXTerms(termRoots);
	return termRoots.front();
    }

    // Sort terms to find repetitions:
    std::sort(termRoots.begin(), termRoots.end(), termOrdering<T>);

    // Determine which terms are useful (terms that don't always evaluate to false,
    // and are not the same as a previous term):
    std::vector<BoolExpr<T> *> usefulTerms;
    std::vector<bool> isUsefulTerm;
    typename std::vector<BoolExpr<T> *>::const_iterator it;
    typename std::vector<BoolExpr<T> *>::const_iterator previous = termRoots.end();
    for (it = termRoots.begin(); it != termRoots.end(); previous = it, it++)
    {
	assert(*it != NULL);
	bool u = (*it)->isDNFTermUseful();

	// If 'it' is not the first term in termRoots, and 'it' is equal to
	// the previous term in termRoots, then 'it' is not useful.
	//
	if (previous != termRoots.end() && exprComparator(*previous, *it) == 0)
	    u = false;

	isUsefulTerm.push_back(u);
	if (u)
	    usefulTerms.push_back(const_cast<BoolExpr<T> *>(*it));
    }
    assert(isUsefulTerm.size() == termRoots.size());

    // Nothing to do if all the terms are useful:
    if (usefulTerms.size() == termRoots.size())
	return dnfRoot;

    destroyDNFOrNodes(dnfRoot);
    // dnfRoot now invalid; termRoots holds the term trees

    // If all the terms are useless (e.g., a&!a | b&!b),
    // return the first one, because we must return something.
    if (usefulTerms.size() == 0)
    {
	// Destroy all term trees except the first one.
	for (it = termRoots.begin(), it++; it != termRoots.end(); it++)
	    delete *it;

	// Return the first term.
	return termRoots.front();
    }

    // Destroy the trees in 'termRoots' that are not useful:
    for (it = termRoots.begin(); it != termRoots.end(); it++)
	if (!isUsefulTerm[it - termRoots.begin()])
	    delete *it;

    // Simplify terms of the form x & x.
    simplifyXAndXTerms(usefulTerms);

    // Join the useful terms with OR nodes:
    return joinTreesWithOrNodes(usefulTerms);
}


template <class T>
/*static*/
void
BoolExpr<T>::simplifyXAndXTerms(std::vector<BoolExpr<T> *> &terms)
{
    typename std::vector<BoolExpr<T> *>::iterator jt;
    for (jt = terms.begin(); jt != terms.end(); jt++)
    {
	BoolExpr<T> *term = *jt;
	if (term->type == AND
		&& term->left->type == VALUE
		&& term->right->type == VALUE
		&& term->left->value == term->right->value)
	{
	    *jt = term->left;
	    term->left = NULL;
	    delete term;  // destroys AND, and VALUE at right
	}
    }
}


template <class T>
/*static*/
void
BoolExpr<T>::destroyDNFOrNodes(BoolExpr<T> *root)
{
    if (root == NULL || root->type != OR)
	return;

    // Detach the OR node's subtrees, so that 'delete' does not affect them:
    BoolExpr<T> *left = root->left, *right = root->right;
    assert(left != NULL && right != NULL);
    root->left = root->right = NULL;
    delete root;

    destroyDNFOrNodes(left);
    destroyDNFOrNodes(right);
}


template <class T>
/*static*/
BoolExpr<T> *
BoolExpr<T>::joinTreesWithOrNodes(const std::vector<BoolExpr<T> *> &trees)
{
    if (trees.size() == 0)
	return NULL;
    if (trees.size() == 1)
	return trees.front();

    typename std::vector<BoolExpr<T> *>::const_iterator it = trees.begin();

    BoolExpr<T> *result = new BoolExpr<T>(OR, *it++, NULL);
    result->right = *it++;
	// *it++ not used twice in same statement because eval order is undef

    for ( ; it != trees.end(); it++)
	result = new BoolExpr<T>(OR, result, *it);

    return result;
}


template <class T>
/*static*/
BoolExpr<T> *
BoolExpr<T>::getRawDNF(BoolExpr<T> *root)
{
    if (root == NULL)
        return NULL;

    /*  One-level trees:
    */
    if (root->type == VALUE)
        return root;

    /*  Two levels trees:
    */
    assert(root->right != NULL);
    switch (root->type)
    {
        case NOT:
            assert(root->left == NULL);
            if (root->right->type == VALUE)
                return root;
            break;

        case OR:
        case AND:
            assert(root->left != NULL);
            if (root->left->type == VALUE && root->right->type == VALUE)
                return root;
            break;

        default:
            assert(false);
    }


    /*  Three or more levels:
    */
    root->left = getRawDNF(root->left);
    assert(root->left == NULL || root->left->isDisjunctiveNormalForm());
    root->right = getRawDNF(root->right);
    assert(root->right->isDisjunctiveNormalForm());


    /*  Here, because of some simplifications, we may now have
	a two-level tree.  For example, if the original tree was
	(!(!a)) & (!(!m)).  The double negations have been simplified
	and we now have a&m.
    */


    assert(root->right != NULL);
    if (root->type == NOT)
    {
        assert(root->left == NULL);

        if (root->right->type == NOT)
        {
            /*  Two NOTs make a positive:
            */
            assert(root->right->left == NULL);
            assert(root->right->right != NULL);

            BoolExpr<T> *newRoot = root->right->right;
	    assert(newRoot->isDisjunctiveNormalForm());
            root->right->right = NULL;
            delete root;  // deletes two nodes
            return newRoot;
        }

	BoolExpr<T> *x = root->right;
	root->right = NULL;
	delete root;
	return negateDNF(x);

        assert(false);
        return NULL;
    }


    assert(root->type != NOT);
    assert(root->left != NULL);


    /*  If one side is a value, make sure that this value is at the left:
    */
    if (root->left->type != VALUE && root->right->type == VALUE)
        swap(root->left, root->right);


    /*  Permutate the left and right subtrees if they are
	not in our "conventional order":
    */
    if (root->left->type == NOT && root->right->type == OR)
        swap(root->left, root->right);
    else if (root->left->type == NOT && root->right->type == AND)
        swap(root->left, root->right);
    else if (root->left->type == OR && root->right->type == AND)
        swap(root->left, root->right);


    /*  Conventional order:
	root->left->type and root->right->type are expected to be
	equal or to be one of (OR, NOT), (AND, NOT), (AND, OR).
    */

    if (root->type == OR)
    {
        if (root->left->type == VALUE && root->right->type == NOT)
        {
            // Expected because of recursion step:
            assert(root->right->right->type == VALUE);

            return root;
        }

        if (root->left->type == VALUE)
            return root;

        if (root->left->type != NOT && root->right->type != NOT)
            return root;

        if (root->left->type == NOT)
        {
            assert(root->left->left == NULL);
            assert(root->right->type == NOT);  // expected re: conv. order
            assert(root->right->left == NULL);

            // Expected because of recursion step:
            assert(root->left->right->type == VALUE);
            assert(root->right->right->type == VALUE);

            return root;
        }

        if (root->right->type != NOT)
            return root;

        // Expected because of recursion step:
        assert(root->right->right->type == VALUE);
        return root;
    }


    if (root->type == AND)
    {
        if (root->left->type == VALUE && root->right->type == NOT)
        {
            // Expected because of recursion step:
            assert(root->right->right->type == VALUE);

            return root;
        }

        if (root->left->type == VALUE && root->right->type == AND)
            return root;

        if (root->left->type == VALUE && root->right->type == OR)
        {
            BoolExpr<T> *andNode = root;
            BoolExpr<T> *x = root->left;
            BoolExpr<T> *orNode = root->right;
            BoolExpr<T> *a = root->right->left;
            BoolExpr<T> *b = root->right->right;

            andNode->right = a;

            BoolExpr<T> *xClone = new BoolExpr<T>(x->getValue());
            BoolExpr<T> *newAndNode = new BoolExpr<T>(AND, xClone, b);

            orNode->left = andNode;
            orNode->right = newAndNode;

            return getRawDNF(orNode);
        }

        if (root->left->type == AND && root->right->type == AND)
            return root;

        if (root->left->type == VALUE && root->right->type == VALUE)
            return root;

        if (root->left->type == NOT)
        {
            assert(root->left->left == NULL);
            assert(root->right->type == NOT);  // expected re: conv. order
            assert(root->right->left == NULL);

            // Expected because of recursion step:
            assert(root->left->right->type == VALUE);
            assert(root->right->right->type == VALUE);

            return root;
        }

        if (root->right->type == NOT)
        {
            assert(root->right->left == NULL);
            assert(root->right->right != NULL);
            assert(root->right->right->type == VALUE);

            if (root->left->type == AND)
                return root;

            BoolExpr<T> *a = root->left->left;
            BoolExpr<T> *b = root->left->right;
            BoolExpr<T> *c = root->right->right;
            BoolExpr<T> *andNode = root;
            BoolExpr<T> *orNode = root->left;
            BoolExpr<T> *notNode = root->right;

            // We have (a|b) & !c, which becomes (a&!c) | (b&!c):

            BoolExpr<T> *newCNode = new BoolExpr<T>(c->getValue());
            BoolExpr<T> *newNotNode = new BoolExpr<T>(NOT, NULL, newCNode);
            BoolExpr<T> *newAndNode = new BoolExpr<T>(AND, b, newNotNode);

            orNode->left = andNode;
            orNode->right = newAndNode;
            andNode->left = a;
            andNode->right = notNode;
            notNode->right = c;

            return getRawDNF(orNode);
        }

        assert(root->right->type == OR);

        if (root->left->type == OR)
        {
            BoolExpr<T> *a = root->left->left;
            BoolExpr<T> *b = root->left->right;
            BoolExpr<T> *c = root->right->left;
            BoolExpr<T> *d = root->right->right;
            BoolExpr<T> *andNode = root;
            BoolExpr<T> *leftOrNode = root->left;
            BoolExpr<T> *rightOrNode = root->right;

            // We have (a|b) & (c|d), which becomes a&b | a&c | b&c | b&d:

            andNode->left = a;
            andNode->right = c;

            BoolExpr<T> *aClone = cloneTree(a);
            BoolExpr<T> *firstNewAndNode = new BoolExpr<T>(AND, aClone, d);

            BoolExpr<T> *cClone = cloneTree(c);
            BoolExpr<T> *secondNewAndNode = new BoolExpr<T>(AND, b, cClone);

            BoolExpr<T> *bClone = cloneTree(b);
            BoolExpr<T> *dClone = cloneTree(d);
            BoolExpr<T> *thirdNewAndNode = new BoolExpr<T>(AND, bClone, dClone);

            leftOrNode->left = andNode;
            leftOrNode->right = firstNewAndNode;

            rightOrNode->left = secondNewAndNode;
            rightOrNode->right = thirdNewAndNode;

            BoolExpr<T> *newRoot = new BoolExpr<T>(OR, leftOrNode, rightOrNode);
            return getRawDNF(newRoot);
        }

        if (root->left->type == AND)
        {
            BoolExpr<T> *a = root->left->left;
            BoolExpr<T> *b = root->left->right;
            BoolExpr<T> *c = root->right->left;
            BoolExpr<T> *d = root->right->right;
            BoolExpr<T> *topAndNode = root;
            BoolExpr<T> *rightOrNode = root->right;

            BoolExpr<T> *aClone = cloneTree(a);
            BoolExpr<T> *bClone = cloneTree(b);
            BoolExpr<T> *newLowAndNode = new BoolExpr<T>(AND, aClone, bClone);
            BoolExpr<T> *newHighAndNode = new BoolExpr<T>(AND, newLowAndNode, d);

            topAndNode->right = c;

            rightOrNode->left = topAndNode;
            rightOrNode->right = newHighAndNode;
            return getRawDNF(rightOrNode);
        }

        assert(false);
        return NULL;
    }

    return NULL;
}


template <class T>
inline std::ostream &
operator << (std::ostream &out, const std::set<T> &s)
{
    out << "{ ";
    for (typename std::set<T>::const_iterator jt = s.begin();
						jt != s.end(); jt++)
	out << *jt << " ";
    out << "}";
    return out;
}


// A "literal" is a variable name, possibly accompanied by a NOT operator.
template <class T>
class Literal
{
public:
    Literal(const T &v, bool p) : value(v), pos(p) {}
    T value;
    bool pos;

    bool operator == (const Literal &lit) const
    {
	return pos == lit.pos && value == lit.value;
    }

    bool operator < (const Literal &lit) const
    {
	if (pos != lit.pos)
	    return pos < lit.pos;
	return value < lit.value;
    }
};


template <class T>
/*static*/
BoolExpr<T> *
BoolExpr<T>::negateDNF(BoolExpr<T> *root)
{
    if (root == NULL)
	return NULL;

    assert(root->isDisjunctiveNormalForm());

    std::vector<const BoolExpr<T> *> termRoots;
    root->getDNFTermRoots(std::inserter(termRoots, termRoots.end()));
    assert(termRoots.size() > 0);

    // Re-express the DNF as a list of lists of literals.

    std::vector< std::vector< Literal<T> > > terms;

    typename std::vector<const BoolExpr<T> *>::const_iterator it;
    size_t product = 1;  // will be the product of the sizes of the terms
    const size_t max = 10000;  // maximum allowed product (to avoid using excessive memory)
    for (it = termRoots.begin(); it != termRoots.end(); it++)
    {
	std::set<T> pos, neg;
	(*it)->getTreeVariables(pos, neg);
	bool useful = (*it)->isDNFTermUseful(pos, neg);
	if (useful)
	{
	    terms.push_back(std::vector< Literal<T> >());
	    std::vector< Literal<T> > &variables = terms.back();

	    typename std::set<T>::const_iterator jt;
	    for (jt = pos.begin(); jt != pos.end(); jt++)
		variables.push_back(Literal<T>(*jt, true));
	    for (jt = neg.begin(); jt != neg.end(); jt++)
		variables.push_back(Literal<T>(*jt, false));
	    assert(variables.size() > 0);

	    if (product > max / variables.size())
	    {
		delete root;
		return NULL;  // resulting expression would be too large
	    }

	    product *= variables.size();
	}
    }


    // 'root' and 'termRoots' are not useful anymore.
    // Only 'terms' is used from now on.
    termRoots.clear();
    delete root;
    root = NULL;


    std::vector<size_t> indexVec(terms.size(), 0);
    size_t numTermsCreated = 0;
    BoolExpr<T> *disjunction = NULL;
    for (;;)
    {
	BoolExpr<T> *conjunction = NULL;
	std::set< Literal<T> > usedLiterals;
	for (size_t i = 0; i < indexVec.size(); i++)
	{
	    assert(indexVec[i] < terms[i].size());
	    const Literal<T> &lit = terms[i][indexVec[i]];

	    // If 'lit' not already used in the conjunction:

	    if (usedLiterals.find(lit) == usedLiterals.end())
	    {
		// Create a node (or two) for 'lit' and add it
		// to the current conjunction.

		BoolExpr<T> *literal = new BoolExpr<T>(lit.value);
		if (lit.pos)
		    literal = new BoolExpr<T>(NOT, NULL, literal);

		if (conjunction == NULL)
		    conjunction = literal;
		else
		    conjunction = new BoolExpr<T>(AND, conjunction, literal);

		usedLiterals.insert(lit);
	    }
	}
	numTermsCreated++;

	if (disjunction == NULL)
	    disjunction = conjunction;
	else
	    disjunction = new BoolExpr<T>(OR, disjunction, conjunction);

	size_t i = indexVec.size() - 1;
	for (;;)
	{
	    indexVec[i]++;
	    if (indexVec[i] < terms[i].size())
		break;
	    // Overflow:
	    indexVec[i] = 0;
	    i--;
	    if (i == size_t(-1))
		break;
	}
	if (i == size_t(-1))
	    break;
    }

    assert(disjunction != NULL);
    return disjunction;
}
