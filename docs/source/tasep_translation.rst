.. _tasep_translation:

###########################################################
A Simple Model of Translation, Inspired by TASEP
###########################################################

This tutorial builds a small model of mRNA translation as a totally
asymmetric simple exclusion process (TASEP): ribosomes hop along an
mRNA one codon at a time, always in the same direction, and can never
occupy or pass through a codon that is already occupied
[Zia2011]_. Unlike the other tutorials in this section, it isn't based
on a specific published BNGL model -- it's here to show that a
lattice-exclusion process, usually treated with its own specialized
mathematics, is also directly expressible as an ordinary BNGL rule set.

The idea
========

Model the mRNA as a chain of ``Site`` molecules (codons) bonded in a
line, and ribosomes as separate molecules that bind to one ``Site`` at
a time. "Only move forward if the next codon is free" becomes a
pattern-matching condition -- a rule that requires the next ``Site``
in the chain to have its ribosome-binding component unbound.

.. code-block:: bngl

   begin molecule types
     Site(prev,next,occ)   # one codon: prev/next link to neighboring
                            # codons, occ links to a ribosome if present
     Ribosome(site)
     Protein()
   end molecule types

   begin seed species
     # A 6-codon mRNA: site 1 has no "prev" neighbor, site 6 has no
     # "next" neighbor -- their free bond identifies the chain's ends.
     Site(prev,next!1,occ).Site(prev!1,next!2,occ).Site(prev!2,next!3,occ)
       .Site(prev!3,next!4,occ).Site(prev!4,next!5,occ).Site(prev!5,next,occ)   mRNA_0
     Ribosome(site)                                                            Ribosome_0
   end seed species

The chain's ends are identified implicitly, by which bonds are
missing, rather than by any special label: site 1 is the only ``Site``
whose ``prev`` is free, and site 6 is the only one whose ``next`` is
free. Patterns can pick them out directly.

Three rules
===========

.. code-block:: bngl

   # Initiation: a free ribosome binds the first codon (the only Site
   # with prev unbound).
   Ribosome(site) + Site(prev,occ) -> Ribosome(site!1).Site(prev,occ!1)  k_init

   # Elongation: a ribosome hops forward one codon, but only if that
   # codon is unoccupied -- simple exclusion, TASEP's namesake rule.
   Ribosome(site!1).Site(occ!1,next!2).Site(prev!2,occ) ->
   Ribosome(site!3).Site(occ,next!2).Site(prev!2,occ!3)  k_hop

   # Termination: a ribosome that reaches the last codon (the only
   # Site with next unbound) completes translation, releasing a
   # protein and freeing that codon for the next ribosome.
   Ribosome(site!1).Site(next,occ!1) -> Ribosome(site) + Site(next,occ) + Protein()  k_term

The elongation rule is the heart of the model: it matches a ribosome
on some codon, bonded (``next!2``) to the next codon in the chain, and
requires that next codon's ``occ`` to be free. If another ribosome is
already there, the pattern simply doesn't match -- exclusion is
enforced by graph matching, with no counter, flag, or conditional rate
needed.

A pitfall worth knowing about
================================

An earlier version of the termination rule was written as:

.. code-block:: bngl

   Ribosome(site!1).Site(next,occ!1) -> Ribosome(site) + Protein()   # WRONG

This looks reasonable -- "the ribosome lets go and a protein appears"
-- but it silently *deletes the codon*. BNGL maps reactant molecules to
product molecules positionally by type: the reactant side has one
``Ribosome`` and one ``Site``; the product side named above has one
``Ribosome`` and one ``Protein``. Since no ``Site`` appears in the
product list, BNGL deletes it -- exactly as if a rule had been written
to explicitly destroy that molecule. Running network generation with
this version produces a network with zero termination reactions ever
firing, because deleting the last codon changes the topology in a way
nothing else in the model expects. The fix is to include the ``Site``
on both sides, so it's preserved rather than deleted:

.. code-block:: bngl

   Ribosome(site!1).Site(next,occ!1) -> Ribosome(site) + Site(next,occ) + Protein()

Every molecule that should survive a rule unmodified still needs to
appear, explicitly, on the product side.

What it produces
=================

Simulating the corrected model (a single 6-codon mRNA, a pool of 10
free ribosomes) for 50 time units shows the expected TASEP signature:
after an initial transient, the number of ribosomes simultaneously on
the mRNA settles to a steady value (here, around 2.7 out of 6 codons),
and protein production becomes linear in time -- a constant
steady-state translation rate, exactly as expected once the ribosome
traffic pattern along the transcript has equilibrated.

Running it
==========

``Models2/tasep_translation.bngl`` ships with BioNetGen and has a
validated regression test (``Validate/tasep_translation.bngl``).

.. code-block:: bngl

   generate_network({overwrite=>1})
   simulate({method=>"ode",t_end=>50,n_steps=>50})

Going further
=============

Real ribosome traffic depends on codon-specific hop rates (rare codons
hop more slowly), which is a one-line change here -- give some
``Site`` instances their own state (e.g. ``Site(...,speed~fast~slow)``)
and a rate law that depends on it. Larger mRNAs, multiple transcripts,
or ribosome drop-off are all extensions of the same three-rule
structure.

References
==========

.. [Zia2011] Zia RKP, Dong JJ, Schmittmann B. Modeling translation in
   protein synthesis with TASEP: a tutorial and recent developments.
   *Journal of Statistical Physics* 144:405-428, 2011.
   https://doi.org/10.1007/s10955-011-0183-1
