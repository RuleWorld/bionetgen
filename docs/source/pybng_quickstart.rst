.. _quickstart:

##########
Quickstart
##########

Installation
============

See :ref:`install` to learn how to get PyBioNetGen.

Basic Usage
===========

PyBioNetGen's CLI can be used to simply run a BNGL model

.. code-block:: shell

   bionetgen run -i mymodel.bngl -o output_folder

which will create :code:`output_folder` and run :code:`mymodel.bngl` inside that folder.
For other subcommands or more information on how to use PyBioNetGen's CLI, please see :ref:`cli`.

PyBioNetGen's library can also be used to run a BNGL model

.. code-block:: shell

   import bionetgen
   result = bionetgen.run("mymodel.bngl", output="myfolder")

which will create numpy record arrays.
For other methods or more information on how to use PyBioNetGen's library, please see :ref:`library`.