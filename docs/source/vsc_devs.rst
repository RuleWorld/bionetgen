.. _vsc_devs:

##############
For Developers
##############

Debug mode
----------

Please note that this section is only if you can't use the method described in :ref:`install` or are a developer working on the extension. There
are two other ways to use the extension:

* Cloning the `repository <https://github.com/RuleWorld/BNG_vscode_extension>`_ and placing it under `your VSCode extensions folder <https://code.visualstudio.com/docs/editor/extension-gallery#_where-are-extensions-installed>`_
* Cloning the `repository <https://github.com/RuleWorld/BNG_vscode_extension>`_ and using the extension in debug mode. This is only really useful for development purposes, most users won't need this. 

To use the extension in debug mode:

1. Download and install VS Code from https://code.visualstudio.com 
2. Open VS Code and open a new terminal
   
   * Terminal -> New Terminal, or
   * ``CTRL/CMD + ```

3. In the terminal, run this line: ``git clone https://github.com/RuleWorld/BNG_vscode_extension.git`` to clone the repository in the desired directory
4. File -> Open to open the repository folder (BNG_vscode_extension)
5. To open up a new window running the extension

   * Run -> Start Debugging, or
   * ``F5`` 

6. Open an existing ``.bngl`` file or create a new file with ``.bngl`` extension
