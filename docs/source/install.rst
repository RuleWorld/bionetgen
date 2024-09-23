.. _install:

############
Installation
############

Installing under VS Code
-------------------------

You will need to do the following to install the extension:

1. Download and install `VS Code <https://code.visualstudio.com/>`_
2. Install BioNetGen extension for VS Code
3. Download and install `Anaconda python <https://www.anaconda.com/products/individual>`_
4. (Windows users only) Install `Perl <https://www.perl.org/>`_
5. Open a ``.bngl`` file in VS Code

First, `download and install VS Code <https://code.visualstudio.com>`_. The suggested way to install this VS Code extension is
from the VS Code marketplace. Simply open the Extensions tab (or press ``CTRL/CMD + SHIFT + X``) after opening VS Code and search 
for BioNetGen. `This package <https://marketplace.visualstudio.com/items?itemName=als251.bngl>`_ will show up, click install. If 
this step doesn't work, please make sure you have the `latest version <https://vscode-docs.readthedocs.io/en/stable/supporting/howtoupdate/>`_.

Next, you will need Python installed, and you need to have your terminal environment set correctly. We recommend 
`Anaconda Python <https://www.anaconda.com/products/individual>`_. `Download and install Anaconda Python <https://docs.anaconda.com/anaconda/install/index.html>`_.
During the installation, we recommend adding Anaconda to your PATH environment variable when given the option.
In VS Code, open a new terminal (``CTRL/CMD + ``` or under ``Terminal -> New Terminal``) and test if you have access to Python package manager ``pip`` (by running ``pip -h``, for example). 
If you don't, you need to setup your environment so that the terminal has access to ``pip``; this is OS dependent and there are various 
guides you can find online.

Windows users should also install Perl at this point if not already installed. You can use ``conda install -c conda-forge perl`` to do this.
If you do not wish to use Anaconda, or if you do not wish to install Perl through Anaconda, we suggest `Strawberry Perl <https://strawberryperl.com/>`_.

Windows users may encounter the error ``spawn bionetgen ENOENT process exited with code -4058``. If this happens you should go into the VS Code Extension settings 
and enable ``General: Enable_terminal_runner`` as described `here <https://bionetgen.readthedocs.io/en/latest/vsc_options.html>`_

Finally, open a ``.bngl`` file in VS Code; this should check if PyBioNetGen is installed, and will automatically install it if not. Once complete, make sure it's installed correctly by
running ``bionetgen -h``. If this doesn't work, you can open the command palette (``CTRL/CMD + SHIFT + P``) and run the ``BNG setup`` command.
Alternatively, you can open a terminal and run the install command (``pip install bionetgen``) yourself.

Additional Setup Advice
=======================
You may need to specify which installation of Python that VS Code uses.
To do so, open the command palette (``CTRL/CMD + SHIFT + P``) and select the command ``Python: Select Interpreter``, which will allow you to make your selection.
