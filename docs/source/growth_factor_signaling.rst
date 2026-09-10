.. _growth_factor_signaling:

##########################################################
Modeling Growth Factor Signaling: EGFR Adapter Recruitment
##########################################################

This tutorial walks through ``Models2/egfr_net.bngl``, BioNetGen's
model of early EGF receptor (EGFR) signaling [Blinov2006]_ -- one of
the two founding examples (alongside FcεRI, see
:ref:`immune_receptor_signaling`) used to demonstrate that rule-based
modeling can tame combinatorial complexity in a real signaling
network.

The biology
===========

EGF binds monomeric EGFR and drives it to dimerize with another
ligand-bound receptor. Dimerization activates each receptor's kinase
domain in trans, leading to phosphorylation of tyrosines on the
partner receptor's cytoplasmic tail. This model tracks two of those
phosphotyrosines, Y1068 and Y1148, which recruit the adapter Grb2 (and
through it, the Ras-GEF Sos) by two different routes: Y1068 binds
Grb2 directly, while Y1148 binds the adapter Shc, which must itself be
phosphorylated before it can recruit Grb2. Grb2 and Sos can also
pre-associate in the cytosol before either binds the receptor, and Shc
can pick up Grb2(-Sos) either at the receptor or free in the cytosol.
The result is several molecularly distinct routes to the same
biological outcome -- an active, Sos-loaded receptor complex -- each
with its own kinetics.

Same-molecule dimerization and multiplicity
=============================================

.. code-block:: bngl

   # Note changed multiplicity
   # Receptor-aggregation
   egfr(l!+,r) + egfr(l!+,r) <-> egfr(l!+,r!3).egfr(l!+,r!3)  kp2, km2

Both reactants here are the *same* pattern, ``egfr(l!+,r)`` -- unlike
FcεRI's ligand-bridged aggregation, EGFR dimerizes directly,
receptor-to-receptor. BioNetGen accounts for the resulting statistical
factors automatically (two identical, unbound copies of a receptor can
combine in only one way, not two), but it's worth being deliberate
about this, as the comment here flags: rules with two copies of an
identical reactant pattern are exactly the case where automatically-
computed multiplicities are easy to get wrong by hand, and easy to
mis-read when written out.

Two routes, one destination
============================

.. code-block:: bngl

   # Y1068 activity: Grb2 binds phosphorylated Y1068 directly, then recruits Sos
   egfr(Y1068~pY) + Grb2(SH2,SH3)   <-> egfr(Y1068~pY!1).Grb2(SH2!1,SH3)    kp9, km9
   egfr(Y1068~pY!1).Grb2(SH2!1,SH3) + Sos(dom) <-> egfr(Y1068~pY!1).Grb2(SH2!1,SH3!2).Sos(dom!2)  kp10, km10

   # Y1148 activity: Shc must dock and be phosphorylated before it can recruit Grb2(-Sos)
   egfr(Y1148~pY) + Shc(PTB,Y317~Y)  <-> egfr(Y1148~pY!1).Shc(PTB!1,Y317~Y)   kp13, km13
   # ... (Shc PTB-domain binding, then Y317 transphosphorylation, then Grb2/Sos recruitment)

The corresponding observable makes the "two routes" structure explicit
by combining both final complexes into a single count:

.. code-block:: bngl

   Molecules  Sos_act  Shc(PTB!+,Y317~pY!2).Grb2(SH2!2,SH3!3).Sos(dom!3),
                        egfr(Y1068~pY!1).Grb2(SH2!1,SH3!2).Sos(dom!2)

An observable can list several comma-separated patterns; ``Sos_act``
totals molecules matching *either* the Shc-mediated or the direct
Y1068 route to a receptor-associated Grb2-Sos complex, since both
represent the same downstream activity (active, membrane-recruited
Sos) despite being structurally distinct species.

Detailed balance, done by hand
================================

.. code-block:: bngl

   # check detailed balanced
   loop1 = (kp9/km9)*(kp10/km10)/((kp11/km11)*(kp12/km12))
   loop2 = (kp15/km15)*(kp17/km17)/((kp21/km21)*(kp18/km18))
   ...

Because Grb2, Sos, and Shc can associate in more than one order,
several *thermodynamic cycles* exist in this network -- closed loops
of reversible binding steps that must satisfy detailed balance (the
product of forward rate constants around a loop must equal the product
of reverse ones). This model checks that manually, with these
``loop*`` expressions, each expected to equal 1 (one existing parameter
comment even notes a value that had to be corrected because a typo
broke this constraint). Manually verifying loop constraints like this
across an entire model, as its rules grow, is exactly the bookkeeping
problem that motivated energy-based BNGL, which computes rates for a
set of interacting binding events from a single free-energy function
and satisfies detailed balance by construction -- see a future
tutorial on energy-based modeling for that approach applied to
cooperative complex formation.

Why this needed rule-based modeling
=====================================

The rule set here is 23 rules. Generating its network produces
**356 species and 3749 reactions** -- every one of them a distinct way
of assembling EGFR, Grb2, Shc, and Sos, tracked automatically from
rules that each describe a single binding or catalytic event.
Enumerating that network directly, or worse, re-deriving it by hand
after changing one rate constant, is exactly what rule-based modeling
was built to avoid [Blinov2006]_.

Running it
==========

``Models2/egfr_net.bngl`` ships with BioNetGen and has a validated
regression test (``Validate/egfr_net.bngl``). Generate its network and
simulate with:

.. code-block:: bngl

   generate_network({overwrite=>1})
   simulate({method=>"ode",t_end=>40,n_steps=>50,atol=>1e-8,rtol=>1e-8,sparse=>1})

Going further
=============

``Models2/egfr_net_red.bngl`` is a reduced, lumped-state version of
this same model with equivalent ODE dynamics but a smaller reaction
network -- a good next read for model-reduction technique.
``Models2/egfr_path.bngl`` implements an earlier, simpler EGFR
pathway model due to Kholodenko et al. [Kholodenko1999]_, useful as a
point of comparison for how much detail a rule-based description adds
over a traditional, hand-enumerated ODE model of the same pathway.
``Models2/SHP2_base_model.bngl`` [Barua2006]_ extends this general
receptor-adapter-recruitment style of model to a phosphatase acting on
a related growth factor receptor pathway.

References
==========

.. [Blinov2006] Blinov ML, Faeder JR, Goldstein B, Hlavacek WS. A
   network model of early events in epidermal growth factor receptor
   signaling that accounts for combinatorial complexity. *Biosystems*
   83(2-3):136-151, 2006. https://doi.org/10.1016/j.biosystems.2005.06.014

.. [Kholodenko1999] Kholodenko BN, Demin OV, Moehren G, Hoek JB.
   Quantification of short term signaling by the epidermal growth
   factor receptor. *Journal of Biological Chemistry*
   274(42):30169-30181, 1999. https://doi.org/10.1074/jbc.274.42.30169

.. [Barua2006] Barua D, Faeder JR, Haugh JM. Structure-based kinetic
   models of modular signaling protein function: focus on Shp2.
   *Biophysical Journal* 92(7):2290-2300, 2007.
   https://doi.org/10.1529/biophysj.106.093484
