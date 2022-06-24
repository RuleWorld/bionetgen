.. _lib_tut:

################
Library Tutorial
################

These tutorials shows how to use PyBioNetGen's library to run and plot a simple model,
as well as create a BNG model object.

Getting Started
===============

Make sure you have PyBioNetGen properly installed by running

.. code-block:: shell

    bionetgen -h

If this command doesn't print out help information, install PyBioNetGen with

.. code-block:: shell

    pip install bionetgen

Finally, make sure to import the PyBioNetGen library.

.. code-block:: python

    import bionetgen

Running and Plotting a Model
============================

Use the :code:`run` method to run a BNGL model. Optionally, you can save the results in a new or existing folder.

.. code-block:: python

    result = bionetgen.run("SIR.bngl")
    # OR
    result = bionetgen.run("SIR.bngl", out = "SIR_folder")

To view the resulting gdat record array, you can either use the :code:`gdats` attribute
or the index of the :code:`result` object:

.. code-block:: python

    result.gdats["SIR"][:10]
    # OR
    result[0][:10]

Similarly, to view the resulting cdat record array, use the :code:`cdats` attribute:

.. code-block:: python

    result.cdats["SIR"][:10]

To plot the gdat record array, we'll need matplotlib.

.. code-block:: python

    import matplotlib.pyplot as plt

Save the gdat record array as its own object. Then, the values can be plotted.

.. code-block:: python

    r = result[0]

    for name in r.dtype.names:
        if name != "time":
            plt.plot(r['time'], r[name], label = name)
    plt.xlabel("time")
    plt.ylabel("species (counts)")
    _ = plt.legend(frameon = False)

Using the bngmodel object
=========================

Use the :code:`bngmodel` method to create a Python representation of a BNGL model.

.. code-block:: python

    model = bionetgen.bngmodel("SIR.bngl")

To view the model, you can :code:`print()` the entire BNGL model or just certain blocks of the model.

.. code-block:: python

    print(model)

.. code-block:: python

    print(model.parameters)

Jupyter Notebooks
=================

Interactive Jupyter notebooks versions of these tutorials can be found
here: 

* :download:`Running and Plotting <../assets/library_tutorial.ipynb>`
* :download:`bngmodel <../assets/lib_bngmodel_tutorial.ipynb>`