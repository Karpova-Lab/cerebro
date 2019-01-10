:github_url: https://github.com/Karpova-Lab/cerebro/tree/1.0
====================
Additional Resources
====================


Soldering Tips
==============

Through Hole Soldering
----------------------

.. image:: soldering101.png
  :align: center
  :width: 100 %

Surface Mount Soldering
-----------------------

.. raw:: html

  <div style="text-align:center;margin-bottom:24px">
    <iframe width="560" height="315" src="https://www.youtube.com/embed/b9FC9fAlfQE?rel=0" frameborder="0" allowfullscreen></iframe>
  </div>

Git
===

Using Git
---------

Here is an nice introduction to what Git is: `Tutorial by Atlassian <https://www.atlassian.com/git/tutorials/what-is-version-control>`_

If you don't want to use the command line for running Git commands, I recommend downloading `SourceTree <https://www.sourcetreeapp.com/>`_ to use as a GUI.


Cloning the Cerebro repository from Github
------------------------------------------

From the Command Line
`````````````````````

Create a new folder into which you want to clone the repository

.. code::

   mkdir mylocalrepo

Navigate into the folder you just created and then clone the contents of the Cerebro repository from Github

.. code::

   cd mylocalrepo/
   git clone https://github.com/Karpova-Lab/cerebro.git .

From a Mac terminal it will look like this:

.. image:: CLI_clone.png
  :align: center
  :scale: 100 %

Using SourceTree
````````````````
1. Navigate to File -> New/Clone
2. Paste ``https://github.com/Karpova-Lab/cerebro.git`` as the source/URL
3. The files now exist locally on your computer

.. image:: sourcetree_clone/cloneFromURL.png
  :align: center
  :scale: 100 %

.. image:: sourcetree_clone/local_dir_info.png
  :align: center
  :scale: 100 %

.. image:: sourcetree_clone/local_files.png
  :align: center
  :scale: 100 %

Circuit Board Design
====================

EAGLE CAD design software is used for all of the circuit boards in the Cerebro Wireless Optogenetics System. To get started with editing PCB files, check out the following tutorials:

- `Jeremy Blum's EAGLE Tutorials <http://www.jeremyblum.com/category/eagle-tutorials/>`_
- `Sparkfun's Using EAGLE Series <https://learn.sparkfun.com/tutorials/using-eagle-schematic>`_
