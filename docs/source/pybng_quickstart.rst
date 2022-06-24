.. _quickstart:

##########
Quickstart
##########

Installation
============

You will need both `Python <https://www.python.org/>`_ (3.7 and above) and `Perl <http://strawberryperl.com/>`_ (for Windows users) installed.

Once both are available, you can use the following :code:`pip` command to install PyBioNetGen:

.. code-block:: shell

    pip install bionetgen

This comes with the latest version of `BioNetGen <https://bionetgen.org>`_. Please note that,
at the moment, PyBioNetGen does not support Atomizer but eventually will.

After installation is complete, you can test to see if PyBioNetGen is properly installed with

.. code-block:: shell

   bionetgen -h

If this command prints out help, the command line tool is installed.

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