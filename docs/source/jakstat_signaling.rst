.. _jakstat_signaling:

###################################################
Modeling JAK-STAT Signaling with Negative Feedback
###################################################

This tutorial builds a small model of cytokine-driven JAK-STAT
signaling: a receptor phosphorylates two STAT transcription factors,
which induce a negative-feedback inhibitor that shuts the receptor
back off. It is a simplified, teaching-scale version of the modeling
approach in Cheemalavagu et al. (2024) [Cheemalavagu2024]_, which
built mechanistic IL-6 and IL-10 models sharing a common STAT1/STAT3
pool to explain why two cytokines signaling through the same STATs
produce different temporal dynamics and different downstream genes.

The biology
===========

IL-10 (and, in the fuller model, IL-6) binds its receptor and
activates receptor-associated JAK kinases, which phosphorylate STAT1
and STAT3. Phosphorylated STATs dimerize and translocate to the
nucleus to drive transcription, including transcription of SOCS
(suppressor of cytokine signaling) proteins, which bind the activated
receptor and block further STAT phosphorylation -- a classic
negative-feedback loop. A separate, constitutively active phosphatase
returns phosphorylated STATs to their unphosphorylated state
independent of the receptor. The model below lumps receptor subunits
and JAKs into one receptor species, and multiple phosphatases into one
generic ``PTP``, following the "Stage 1" simplification strategy used
to build up the full model in [Cheemalavagu2024]_. **Parameter values
here are illustrative** -- chosen for qualitatively reasonable
dynamics, not fit to data.

The model
=========

.. code-block:: bngl

   begin molecule types
     IL10(rec)
     Rec(lig,socs,state~inactive~active)
     STAT1(Y~U~P)
     STAT3(Y~U~P)
     SOCS(rec)
     PTP()
   end molecule types

   begin reaction rules
     # Cytokine binding activates the receptor.
     IL10(rec) + Rec(lig,state~inactive) <-> IL10(rec!1).Rec(lig!1,state~active)  k_bind, k_unbind

     # Active receptor phosphorylates STAT1 and STAT3 (enzymatic: the
     # receptor is not consumed, so it appears on both sides of the rule).
     Rec(state~active,socs) + STAT1(Y~U) -> Rec(state~active,socs) + STAT1(Y~P)  k_phos1
     Rec(state~active,socs) + STAT3(Y~U) -> Rec(state~active,socs) + STAT3(Y~P)  k_phos3

     # SOCS binds the active receptor and blocks it.
     Rec(state~active,socs) + SOCS(rec) <-> Rec(state~active,socs!1).SOCS(rec!1)  k_socsbind, k_socsunbind

     # Both phospho-STATs induce SOCS; SOCS is degraded at a fixed rate.
     STAT1(Y~P) -> STAT1(Y~P) + SOCS(rec)  k_ind1
     STAT3(Y~P) -> STAT3(Y~P) + SOCS(rec)  k_ind3
     SOCS(rec) -> 0  k_socsdeg

     # Constitutive phosphatase activity.
     PTP() + STAT1(Y~P) -> PTP() + STAT1(Y~U)  k_dephos1
     PTP() + STAT3(Y~P) -> PTP() + STAT3(Y~U)  k_dephos3
   end reaction rules

The full parameterized model is ``Models2/jakstat_il10_simple.bngl``.

The receptor pattern ``Rec(state~active,socs)`` in the phosphorylation
rules requires the ``socs`` component to be *free* -- an active
receptor with SOCS already bound no longer matches, so it stops
phosphorylating STATs without needing a separate rule to say so. This
is the same idiom used for context-dependent behavior throughout
BioNetGen: the blocking mechanism is a pattern match, not a flag
checked in a rate law.

Why this produces a transient response
=========================================

Running this model over 90 minutes produces a rise-and-fall in both
pSTAT1 and pSTAT3: they climb quickly as the receptor activates, peak
within the first ~10-15 minutes, and then decline as SOCS accumulates
and progressively blocks the active receptor pool -- even though
``Rec_active`` itself (bound cytokine, independent of SOCS blocking)
plateaus and stays essentially constant. Two related, but structurally
distinct, quantities -- "receptor is cytokine-bound" and "receptor is
free to signal" -- are both derivable as observables from the same
species pool, because SOCS binding and cytokine binding are tracked as
independent components on the same ``Rec`` molecule.

This transient-then-attenuated shape is the qualitative signature of
negative feedback through an inducible inhibitor, and it is the
starting point for the biological question in [Cheemalavagu2024]_:
IL-6 and IL-10 both signal through STAT1 and STAT3, but produce
different pSTAT3 dynamics (more transient for one, more sustained for
the other) and, downstream, activate substantially different gene
programs. Reproducing that comparison requires a second receptor
sharing the same STAT1/STAT3 pool and its own SOCS species -- a
natural rule-based extension of the model above, since the shared
STATs and phosphatase are already separate molecule types that either
receptor's rules can act on.

Running it
==========

``Models2/jakstat_il10_simple.bngl`` ships with BioNetGen and has a
validated regression test (``Validate/jakstat_il10_simple.bngl``).

.. code-block:: bngl

   generate_network({overwrite=>1})
   simulate({method=>"ode",t_end=>90,n_steps=>90})

References
==========

.. [Cheemalavagu2024] Cheemalavagu N, Shoger KE, Cao YM, Michalides BA,
   Botta SA, Faeder JR, Gottschalk RA. Predicting gene-level
   sensitivity to JAK-STAT signaling perturbation using a
   mechanistic-to-machine learning framework. *Cell Systems*
   15(1):37-48, 2024. https://doi.org/10.1016/j.cels.2023.12.006
