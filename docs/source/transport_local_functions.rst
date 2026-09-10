.. _transport_local_functions:

#########################################################################
Modeling Transport using Compartmental BNGL (cBNGL) and Local Functions
#########################################################################

This tutorial shows how to combine two BNGL features to model a common
biological situation: a molecular complex is transported between
compartments at a rate that depends on the *contents* of that complex,
not merely on the presence of the triggering molecule and not on how
many copies of it happen to be present.

* **Compartmental BNGL (cBNGL)** lets rules move molecules and complexes
  between named compartments (e.g. plasma membrane to endosome)
  [Harris2009]_.
* **Local functions** let a rate law reference properties of the
  specific reactants matched by a rule, rather than only global,
  system-wide quantities [Sekar2012]_ [Sneddon2011]_.

Used together, they let you write rate laws such as "internalize this
receptor complex faster if it is ligand-bound" -- a rate that is a
function of what a *particular* matched complex contains.

Prerequisites
=============

This tutorial assumes familiarity with basic BNGL (see
:ref:`bng_quickstart`): parameters, molecule types, seed species,
observables, reaction rules, and the ``generate_network`` action. For a
general overview of the BioNetGen software, see [Harris2016]_.

The scenario
============

Consider a receptor ``R`` on the plasma membrane (``PM``) that is
endocytosed into an endosomal membrane (``EM``) at a base rate
``k_endo``. If the complex being internalized also contains at least
one bound ligand ``L``, the rate should be boosted by a factor
``mod_endo_lig``. Two requirements make this more than a plain
compartment-transport rule:

1. The ligand effect is a 0/1 switch: "at least one ``L`` present",
   not "boosted once per bound ``L``".
2. The transport rate must not scale with how many copies of ``R`` the
   complex happens to contain -- a receptor dimer or cluster still
   moves as a single event, not once per receptor.

Compartments and transport rules
=================================

A cBNGL compartment block declares each compartment's dimensionality
(2 for a surface, 3 for a volume) and its enclosing ("outside")
compartment:

.. code-block:: bngl

   begin compartments
     EC   3   10          # extracellular volume (outermost)
     PM   2   1      EC   # plasma membrane;    outside = EC
     CP   3   1      PM   # cytoplasm;          outside = PM
     EM   2   0.1    CP   # endosomal membrane; outside = CP
     EN   3   0.05   EM   # endosome interior;  outside = EM
   end compartments

A transport rule changes a pattern's compartment tag between the
reactant and product side. For two membrane-bound compartments (like
``PM`` and ``EM``), BioNetGen requires them to be "connected by a
volume" -- concretely, the target membrane must itself have an
*inside* volume (``EN`` above), mirroring the topology of real
endocytosis/exocytosis. Omitting ``EN`` produces the error *"Reaction
Rule attempts to transport a membrane species to a membrane which is
not connected to the source compartment by a volume."*

Local functions: two kinds of tag
==================================

A local function's argument refers to whatever a rule tagged with
``%x``. There are two distinct ways to place that tag, and they behave
very differently.

**Molecule-level tag** -- ``R%x()`` tags one specific matched
molecule. Evaluating an observable as ``Obs(x)`` forces the *first*
molecule of ``Obs``'s pattern to be identified with that exact
instance, so ``Obs`` must itself start with the same molecule type as
the anchor. This is the right tool when a rate genuinely depends on a
property of *one particular* matched molecule -- e.g. "how many
ligands are bound directly to this receptor". It is also the
molecule-scoped local function mechanism originally introduced for
network-free simulation in NFsim [Sneddon2011]_, later extended
to network generation and ODE/SSA simulation.

**Species-level tag** -- ``%x@Comp:Pattern`` tags the *whole matched
reactant*, with no molecule serving as an anchor. Observables called
as ``Obs(x)`` then simply return their ordinary match count for that
species -- with no constraint on what molecule type ``Obs`` starts
with. This is the right tool for properties of the *complex as a
whole*, such as "does this complex contain a ligand" -- the question
in this tutorial's scenario.

The worked example
===================

.. code-block:: bngl

   begin model

   begin parameters
     k_on          0.01     # ligand-receptor association
     k_off         1.0      # ligand-receptor dissociation
     kp_LL         0.01     # ligand dimerization (lets a complex carry >1 R)
     km_LL         1.0
     k_endo        0.05     # base endocytosis rate (PM -> EM)
     mod_endo_lig  5.0      # fold-increase in endocytosis rate if >=1 L is present

     R0            100
     L0            50
   end parameters

   begin compartments
     EC   3   10
     PM   2   1      EC
     CP   3   1      PM
     EM   2   0.1    CP
     EN   3   0.05   EM
   end compartments

   begin molecule types
     R(l)        # receptor; l = ligand-binding site
     L(r,d)      # ligand;   r = receptor-binding site, d = ligand dimerization site
   end molecule types

   begin seed species
     @PM:R(l)    R0
     @EC:L(r,d)  L0
   end seed species

   begin observables
     Molecules  Lig_local   L()   # no R() anchor needed -- see species-level tag, above
     Molecules  R_PM   R()@PM
     Molecules  R_EM   R()@EM
   end observables

   begin functions
     # x = the whole complex matched by %x in the rule below.
     # Lig_local(x) = number of L's anywhere in that complex; if(...) collapses
     # that to a 0/1 switch, so the modifier applies once per complex.
     rate_endo(x) = k_endo*if(Lig_local(x)>=1, mod_endo_lig, 1)
   end functions

   begin reaction rules
     L(r) + R(l)  <->  L(r!1).R(l!1)         k_on, k_off
     L(d) + L(d)  <->  L(d!1).L(d!1)         kp_LL, km_LL

     # Transport any complex containing >=1 R from PM to EM. No TotalRate --
     # BNG rejects TotalRate combined with a local function -- and none is
     # needed: see "Why this doesn't scale with receptor count", below.
     Rule_endo: %x@PM:R() -> %x@EM:R()   rate_endo(x)
   end reaction rules

   end model

   generate_network({overwrite=>1})

This model ships with BioNetGen as
``Models2/localfunc_endocytosis.bngl``, and a copy with a checked-in
reference network is used as a regression test in
``Validate/localfunc_endocytosis.bngl``.

Why this doesn't scale with receptor count
===========================================

Rule ``Rule_endo`` matches on ``R()`` alone, and a complex with *n*
receptors has *n* distinct embeddings of that pattern. Ordinarily,
BioNetGen would generate a separate reaction instance per embedding,
and duplicate reactant/product pairs are combined by summing their
statistical weight -- exactly the behavior you want for, say,
independent binding events at symmetric sites, but not for "the
complex moves once".

Because the tag here is species-level (``%x@PM:...``), it carries no
molecule identity: it resolves to a pointer to the matched *species*,
not to any specific receptor within it. All *n* embeddings therefore
resolve to the identical local reference, and hence the identical
computed rate, and BioNetGen only ever emits **one** reaction for that
transition. This was confirmed directly for this model: a complex with
2 receptors bridged by a ligand dimer, and a complex with 3 receptors
in an asymmetric chain (ligand bound at one end only), both produce
exactly one ``Rule_endo`` reaction, at rate ``k_endo*mod_endo_lig`` --
not 2x or 3x that.

This guarantee is specific to species-level tags. If you instead use a
molecule-level tag (``R%x()``) and write an observable whose value
*can* differ depending on which matched receptor is used as the
anchor, BioNetGen does not sum or warn about the discrepancy -- it
silently uses whichever match it happens to find. The ``{MatchOnce}``
pattern attribute (e.g. ``R%x(){MatchOnce}``) forces a rule to match a
given species at most once, which is good practice with molecule-level
tags for this reason, but it does not, by itself, guarantee *which*
match is kept if the tagged property genuinely varies from one
receptor to the next.

Summary
=======

* Use cBNGL compartment blocks and ``@Comp:`` tags to move complexes
  between compartments [Harris2009]_.
* Use a **species-level** tag (``%x@Comp:Pattern``) when a rate should
  depend on a property of the whole matched complex -- it needs no
  anchor molecule, and BioNetGen will not multiply the rate by however
  many copies of the trigger molecule the complex contains.
* Reserve a **molecule-level** tag (``Pattern%x()``) for rates that
  genuinely depend on one specific matched molecule
  [Sekar2012]_ [Sneddon2011]_, and pair it with ``{MatchOnce}`` if
  the complex could otherwise be matched more than once.

References
==========

.. [Harris2009] Harris LA, Hogg JS, Faeder JR. Compartmental
   rule-based modeling of biochemical systems. In *Proceedings of the
   2009 Winter Simulation Conference*, Austin, TX, pp. 908-919, 2009.
   https://www.informs-sim.org/wsc09papers/087.pdf

.. [Sekar2012] Sekar JA, Faeder JR. Rule-based modeling of signal
   transduction: a primer. *Methods in Molecular Biology*
   880:139-218, 2012. https://doi.org/10.1007/978-1-61779-833-7_9

.. [Sneddon2011] Sneddon MW, Faeder JR, Emonet T. Efficient modeling,
   simulation and coarse-graining of biological complexity with
   NFsim. *Nature Methods* 8(2):177-183, 2011.
   https://doi.org/10.1038/nmeth.1546

.. [Harris2016] Harris LA, Hogg JS, Tapia JJ, Sekar JA, Gupta S,
   Korsunsky I, Arora A, Barua D, Sheehan RP, Faeder JR. BioNetGen
   2.2: advances in rule-based modeling. *Bioinformatics*
   32(21):3366-3368, 2016. https://doi.org/10.1093/bioinformatics/btw469
