.. _lat_polymerization:

#####################################################
A Simple Model of Polymerization: LAT-Grb2-SOS
#####################################################

This tutorial builds a minimal polymerization model in BNGL, inspired
by how the T cell/mast cell adaptor LAT is cross-linked into large
signaling clusters by the adaptor Grb2 and the Ras-GEF SOS
[Nag2009]_. Three rules are enough to generate chains and branched
aggregates of arbitrary size.

The biology
===========

LAT is a transmembrane scaffold with several phosphotyrosines, each
able to recruit the adaptor Grb2 through Grb2's SH2 domain. Grb2 also
has an SH3 domain that binds a proline-rich motif on SOS -- and SOS
has *two* such motifs, so one SOS molecule can simultaneously engage
two separate Grb2 molecules. If both of those Grb2's are themselves
bound to (different) LAT molecules, the SOS becomes a bridge between
two LAT molecules. Because each LAT has more than one phosphotyrosine,
a single LAT can be bridged to several others at once, producing
chains and branched aggregates whose size is limited only by the
supply of LAT, Grb2, and SOS -- a polymer, built from bimolecular
binding rules alone.

The model
=========

.. code-block:: bngl

   begin molecule types
     LAT(Y~P,Y~P)     # 2 identical, independently-tracked phosphotyrosines
     Grb2(SH2,SH3)
     SOS(pr,pr)       # 2 identical Grb2-binding (proline-rich) motifs
   end molecule types

   begin reaction rules
     # Grb2 binds a free phosphotyrosine on LAT.
     LAT(Y~P) + Grb2(SH2,SH3) <-> LAT(Y~P!1).Grb2(SH2!1,SH3)  kp1, km1

     # SOS binds a free Grb2 SH3 domain.
     Grb2(SH3) + SOS(pr,pr) <-> Grb2(SH3!1).SOS(pr!1,pr)  kp2, km2

     # SOS's second proline-rich motif binds another free Grb2 SH3 domain --
     # if both Grb2's happen to be LAT-bound, this bridges two LAT molecules.
     Grb2(SH3) + SOS(pr,pr!+) <-> Grb2(SH3!1).SOS(pr!1,pr!+)  kp2, km2
   end reaction rules

``LAT(Y~P,Y~P)`` declares two components with the *same* name, ``Y``
-- BNGL tracks them as independent sites (each with its own bond
state), not as a single site with a count. The last rule is the
polymerization step, and it should look familiar: it is the same
"bind one, then bind more via a wildcard (``!+``) on the already-bound
partner" idiom used for FcεRI receptor aggregation in
:ref:`immune_receptor_signaling` (``Rec(a) + Lig(l,l!+) <-> ...``).
There, a bivalent ligand aggregated receptors; here, a divalent SOS
aggregates Grb2-decorated LAT molecules -- the same rule shape solves
both problems.

Seeing the polymer form
==========================

Generating this network (with generous but finite caps on aggregate
size, since a truly unbounded polymer has infinitely many distinct
species -- see below) produces species like:

.. code-block:: text

   Grb2(SH2!1,SH3!2).Grb2(SH2!3,SH3!4).Grb2(SH2!5,SH3!6).Grb2(SH2!7,SH3!8)
     .LAT(Y~P!1,Y~P!5).LAT(Y~P!3,Y~P!7).LAT(Y~P!8,Y~P)
     .SOS(pr!4,pr!2).SOS(pr!6,pr)

Reading the bonds: this is a chain of three LAT molecules, each
connected to the next through a Grb2-SOS-Grb2 bridge -- exactly the
LAT-Grb2-SOS-Grb2-LAT unit the model was built to produce, generated
automatically from three bimolecular rules, with no rule that
mentions "trimer" or "chain" at all.

A genuine limitation, and how to work around it
==================================================

Unlike the other tutorials in this section, this model's *true*
reaction network is infinite: there is no largest aggregate. BNGL's
network generation (``generate_network``) enumerates every distinct
species explicitly, so it cannot finish unless the network is finite.
The model file caps this with ``max_stoich``:

.. code-block:: bngl

   generate_network({overwrite=>1, max_stoich=>{LAT=>3,Grb2=>4,SOS=>3}, max_iter=>50})

which stops network generation once a species would contain more than
3 LAT, 4 Grb2, or 3 SOS molecules -- enough to see chains up to a LAT
trimer, but not a faithful picture of the full, potentially much
larger aggregates that can form at higher concentrations. For that,
BioNetGen's network-free simulator, NFsim, simulates the rules
directly against a population of molecules without ever enumerating
the species list, and is the appropriate tool for genuinely unbounded
polymerization -- which is how the full LAT-Grb2-SOS aggregate size
distribution is actually studied in [Nag2009]_.

Running it
==========

``Models2/lat_polymer.bngl`` ships with BioNetGen and has a validated
regression test (``Validate/lat_polymer.bngl``).

.. code-block:: bngl

   generate_network({overwrite=>1, max_stoich=>{LAT=>3,Grb2=>4,SOS=>3}, max_iter=>50})
   simulate({method=>"ode",t_end=>500,n_steps=>50})

References
==========

.. [Nag2009] Nag A, Monine MI, Faeder JR, Goldstein B. Aggregation of
   membrane proteins by cytosolic cross-linkers: theory and simulation
   of the LAT-Grb2-SOS1 system. *Biophysical Journal*
   96(7):2604-2623, 2009. https://doi.org/10.1016/j.bpj.2009.01.019
