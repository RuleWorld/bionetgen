.. _energy_cooperative_binding:

##########################################################
Energy-Based Modeling: Cooperativity in Complex Formation
##########################################################

Ordinary BNGL rate laws are rate constants you supply directly. This
tutorial introduces BioNetGen's energy-based modeling extension
(eBNG) [Sekar2016]_, where you instead assign free energies to
structural motifs, and BNG derives every rate constant from them --
guaranteeing detailed balance automatically, and letting cooperative
effects fall out of the model rather than being hand-tuned into it.

The problem with hand-tuned cooperative rates
================================================

:ref:`growth_factor_signaling` showed a model (``egfr_net.bngl``)
where several binding steps form closed thermodynamic cycles, and its
authors had to manually verify (via ``loop1``...``loop5`` parameter
expressions) that the chosen rate constants satisfied detailed
balance -- an easy thing to get subtly wrong as a model grows, and
[Blinov2006]_ notes exactly such a correction to one rate constant.
Energy-based modeling solves this by construction: rate constants for
every version of a binding rule (e.g. "site free" vs. "other site
already occupied") are computed from a single free-energy function, so
they cannot help but be mutually consistent.

The model
=========

A receptor with two independent ligand-binding sites, where occupying
the second site is thermodynamically favored once the first is already
occupied:

.. code-block:: bngl

   begin molecule types
     R(s1,s2)
     L(r)
   end molecule types

   begin energy patterns
     R(s1!0).L(r!0)              Gf_bind
     R(s2!0).L(r!0)              Gf_bind
     R(s1!0,s2!1).L(r!0).L(r!1)  Gf_coop
   end energy patterns

   begin reaction rules
     R0: R(s1) + L(r) <-> R(s1!0).L(r!0)  Arrhenius(phi,Ea0)
     R1: R(s2) + L(r) <-> R(s2!0).L(r!0)  Arrhenius(phi,Ea0)
   end reaction rules

An energy pattern assigns a free-energy contribution to any complex
containing a matching motif; a complex's total free energy of
formation is the sum over every pattern match found in it. A fully
bound receptor matches the first two patterns once each (contributing
``2*Gf_bind``) *and* the third pattern once (contributing ``Gf_coop``
on top) -- so ``Gf_coop`` is the *extra* stabilization from having
both sites occupied simultaneously, over and above two independent
binding events. Making ``Gf_coop`` negative (favorable) models
positive cooperativity; ``Gf_coop = 0`` recovers two independent
sites; a positive value would model negative cooperativity.

The rules themselves say nothing about cooperativity at all -- each
just binds one site, with no mention of the other site's state. The
``Arrhenius(phi,Ea0)`` rate law tells BNG to *derive* the rate constant
from the free-energy change of the specific reaction each rule
generates, rather than take one supplied directly.

One rule, two rate constants
===============================

Because binding the second site starts from a different free-energy
context than binding the first, ``generate_network`` produces *two
different numeric rate constants* from the single rule ``R0`` (and
likewise for ``R1``) -- one for each reactant context:

.. code-block:: text

   _R0_local1  exp(-(Ea0 + phi*Gf_bind))            # first site binds
   _R0_local2  exp(-(Ea0 + phi*(Gf_bind+Gf_coop)))  # second site binds

With ``Gf_bind=-5``, ``Gf_coop=-3``, ``phi=0.5``, ``Ea0=2`` (all
dimensionless, chosen for illustration), working out the corresponding
equilibrium constants gives a clean check on the bookkeeping: the
second binding event's ``Keq`` comes out to be exactly
``exp(-Gf_coop)`` (about 20.1) times the first's -- precisely the
cooperativity factor built into the model, recovered automatically
from the free-energy accounting rather than fit or asserted.

Running it
==========

``Models2/energy_cooperative.bngl`` ships with BioNetGen and has a
validated regression test (``Validate/energy_cooperative.bngl``).

.. code-block:: bngl

   generate_network({overwrite=>1})

Going further
=============

``Validate/isingspin_energy.bngl`` applies the same free-energy
approach to a 2D Ising spin lattice -- a good second example precisely
because it has nothing to do with signaling, showing that "assign
energies to motifs, derive kinetics" is a general modeling tool, not
one tied to any particular biology.

References
==========

.. [Sekar2016] Sekar JA, Hogg JS, Faeder JR. Energy-based modeling in
   BioNetGen. *Proceedings of the 2016 IEEE International Conference
   on Bioinformatics and Biomedicine (BIBM)*, pp. 1460-1467, 2016.
   https://doi.org/10.1109/BIBM.2016.7822739
