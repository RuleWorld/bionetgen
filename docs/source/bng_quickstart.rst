.. _bng_quickstart:

#####################
BioNetGen Quickstart
#####################

This quickstart gets you from installation to your first successful model run.
If you are brand new to BioNetGen, start here.

Prerequisites
=============

1. Complete the installation steps in :ref:`install`.
2. Confirm the CLI is available:

.. code-block:: shell

   bionetgen -h

If this command fails, revisit :ref:`install` before continuing.

Run your first model
====================

Create a file named ``SIR.bngl`` with the following content:

.. code-block:: bngl

   begin model

   begin parameters
     N  1.0e2
     I0 1
     R0 2.5
     g  0.33
     b  g*R0/N
   end parameters

   begin species
     S() N-I0
     I() I0
   end species

   begin observables
     Molecules S S()
     Molecules I I()
     Molecules R R()
   end observables

   begin reaction rules
     R1: S + I -> I + I b
     R2: I -> R g
   end reaction rules

   end model

   # Actions
   generate_network({overwrite=>1})
   simulate({method=>"ode",t_end=>30,n_steps=>200})

Run the model:

.. code-block:: shell

   bionetgen run -i SIR.bngl -o sir_output

You should now have an output folder ``sir_output`` containing simulation results.

What to check
=============

1. Verify the run completed without errors in terminal output.
2. Open files in ``sir_output`` and confirm time-course data was generated.
3. Re-run after changing parameters (for example ``R0``) and compare results.

Run from VS Code
================

If you prefer the VS Code extension workflow:

1. Open ``SIR.bngl`` in VS Code.
2. Use the extension run controls or command palette actions to execute the model.
3. Use the built-in graphing and visualization features for quick inspection.

Where to go next
================

1. For PyBioNetGen CLI details, see :ref:`cli`.
2. For Python library usage, see :ref:`library`.
3. For step-by-step tutorials, see :ref:`tutorials`.
