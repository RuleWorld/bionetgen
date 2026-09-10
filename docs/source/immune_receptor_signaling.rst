.. _immune_receptor_signaling:

#####################################################
Modeling Immune Receptor Signaling: FcεRI Aggregation
#####################################################

This tutorial walks through ``Models2/fceri_ji.bngl``, BioNetGen's model
of early signaling events downstream of the high-affinity IgE receptor
FcεRI on mast cells and basophils [Faeder2003]_. It is one of the
original motivating examples for rule-based modeling: a handful of
biophysically well-characterized rules generate a reaction network far
too large to write down by hand.

The biology
===========

FcεRI is a multi-subunit receptor (``alpha``, ``beta``, and ``gamma``
chains) that binds monomeric IgE with high affinity through its alpha
chain. A multivalent antigen can cross-link IgE-loaded receptors into
aggregates of essentially arbitrary size. Aggregation brings the
receptors' cytoplasmic ITAM (immunoreceptor tyrosine-based activation
motif) tails on the beta and gamma chains into proximity with the Src
family kinase Lyn, which is itself weakly, constitutively associated
with unphosphorylated beta chains. Lyn transphosphorylates the ITAMs
of a *neighboring* receptor in the aggregate; once phosphorylated,
those ITAMs recruit more Lyn (via its SH2 domain, a positive feedback
loop) and recruit the kinase Syk to phosphotyrosine on the gamma
chain. Syk is then phosphorylated in turn, both by Lyn and by
transphosphorylation between neighboring Syk molecules, ultimately
triggering downstream effector functions (e.g. degranulation).

The molecules
=============

.. code-block:: bngl

   begin species
       Lig(l,l)           Lig_tot   # bivalent antigen
       Lyn(U,SH2)         Lyn_tot   # kinase: U = catalytic domain, SH2 = SH2 domain
       Syk(tSH2,l~Y,a~Y)  Syk_tot   # kinase: tSH2, linker Tyr, activation-loop Tyr
       Rec(a,b~Y,g~Y)     Rec_tot   # receptor: a = IgE/antigen site, b/g = beta/gamma ITAMs
   end species

Each phosphorylation site is represented as a component with two
states, ``~Y`` (unphosphorylated) and ``~pY`` (phosphorylated); binding
sites (``l``, ``U``, ``SH2``, ``tSH2``, ``a``) are represented as
components that are either free or bound. This is the basic BNGL
vocabulary: structured molecules built from independently-varying
components, rather than a flat list of named states.

Aggregation with two rules
===========================

.. code-block:: bngl

   # Ligand-receptor binding
   R1: Rec(a) + Lig(l,l) <-> Rec(a!1).Lig(l!1,l)  kp1, km1

   # Receptor-aggregation
   R2: Rec(a) + Lig(l,l!+) <-> Rec(a!2).Lig(l!2,l!+)  kp2, km2

``R1`` binds a free receptor to a completely free ligand. ``R2`` binds
a free receptor to a ligand that already has *at least one* other bond
(``l!+`` is a wildcard meaning "bound, don't care to what"). Because
``R2``'s pattern doesn't say anything about what is on the other end of
that existing bond, it applies equally whether the ligand is bound to
one receptor or to a whole chain of them -- these two rules alone
generate receptor aggregates of unbounded size.

Patterns as mechanism
======================

.. code-block:: bngl

   # Transphosphorylation of beta by constitutive Lyn
   R4: Lig(l!1,l!2).Lyn(U!3,SH2).Rec(a!2,b~Y!3).Rec(a!1,b~Y) ->
       Lig(l!1,l!2).Lyn(U!3,SH2).Rec(a!2,b~Y!3).Rec(a!1,b~pY)  pLb

The rate law here is a plain rate constant (``pLb``) -- the mechanism
is entirely encoded in the *pattern*. The rule requires a ligand
bridging two receptors, with Lyn bound (through its ``U`` domain) to
one of them, and it phosphorylates the beta chain of the *other*
receptor: a graph, not a flag, expresses "this only happens in
trans, between two receptors already cross-linked by antigen." A
second copy of this idea (``R7``) covers the same reaction with Lyn
recruited through its SH2 domain instead of constitutively -- the two
routes have different rates (``pLb`` vs. ``pLbs``) precisely because
they are different bound states, distinguishable by pattern.

Context-dependent rates without local functions
=================================================

.. code-block:: bngl

   # Dephosphorylation of Syk at membrane
   R16: Syk(tSH2!+,l~pY) -> Syk(tSH2!+,l~Y)  dm

   # Dephosphorylation of Syk in cytosol
   R18: Syk(tSH2,l~pY) -> Syk(tSH2,l~Y)  dc

:ref:`transport_local_functions` showed how a local function can make
a rate depend on a matched reactant's context. Here the same kind of
idea -- "the rate depends on whether this molecule is receptor-bound or
free in the cytosol" -- is expressed even more directly, with two
separate rules distinguished by the ``tSH2`` component's bond state
(``!+`` bound vs. unbound) and two different rate constants (``dm``
vs. ``dc``). When the distinguishing context is a simple, enumerable
condition like this, writing it as separate rules is often clearer
than a single rule with a conditional rate law; local functions become
useful when the condition isn't expressible as a fixed pattern (e.g.
"how many ligands are anywhere in this complex").

Why this needed rule-based modeling
=====================================

The full rule set is 19 rules. Generating its reaction network
produces **354 species and 3680 reactions** -- and that is the
*smallest* member of this model family. Variants shipped alongside it
in ``Models2/FceriModels/`` swap in a trivalent antigen
(``fceri_trimer.bngl``: 2954 species, ~50,000 reactions) or a receptor
with two distinguishable gamma chains (``fceri_gamma2_asym.bngl``:
10734 species, ~180,000 reactions). No two of these models required
more than a few dozen rules to specify; the reaction counts are a
*consequence* of the rules, not something anyone wrote down directly.
This gap between rule count and reaction count is the combinatorial
complexity problem that motivated rule-based modeling in the first
place [Faeder2003]_ [Sekar2017]_.

Observables
===========

.. code-block:: bngl

   Molecules  RecDim     Rec.Rec

``Rec.Rec`` matches any complex containing two receptors, with no bond
specified between them -- the same "co-membership in one complex,
no direct bond required" idiom used for the species-level local
function tag in :ref:`transport_local_functions`. It counts aggregated
receptors regardless of aggregate size or which other molecules
(ligand, Lyn, Syk) happen to also be bound.

Running it
==========

``Models2/fceri_ji.bngl`` already ships with BioNetGen and has a
validated regression test (``Validate/fceri_ji.bngl``, checked against
reference ODE trajectories). Generate its network and simulate with:

.. code-block:: bngl

   generate_network({overwrite=>1})
   simulate({method=>"ode",t_end=>600,n_steps=>10,atol=>1e-8,rtol=>1e-8})

Going further
=============

``Models2/FceriModels/`` contains several variants worth reading once
the base model is familiar: an added Fyn kinase pathway
(``fceri_fyn.bngl``), a trivalent antigen (``fceri_trimer.bngl``), and
receptors with two gamma chains, symmetric and asymmetric
(``fceri_gamma2.bngl``, ``fceri_gamma2_asym.bngl``). Each changes only
a small part of the rule set, but produces a substantially different
(and larger) reaction network.

References
==========

.. [Faeder2003] Faeder JR, Hlavacek WS, Reischl I, Blinov ML, Metzger
   H, Redondo A, Wofsy C, Goldstein B. Investigation of early events
   in FcεRI-mediated signaling using a detailed mathematical model.
   *Journal of Immunology* 170(7):3769-3781, 2003.
   https://doi.org/10.4049/jimmunol.170.7.3769

.. [Sekar2017] Sekar JA, Faeder JR. An Introduction to Rule-based
   Modeling of Immune Receptor Signaling. arXiv:1709.06658, 2017.
   https://arxiv.org/abs/1709.06658
