Day 3: Writing MPI programs
===========================

:download:`Lecture slides <slides/day3.pdf>`

Using MPI
^^^^^^^^^

To load a particular MPI, you can use the module commamd. For example,

.. code-block:: console

  $ module load openmpi/gnu

You can then use ``mpicc`` to compile your MPI program:

.. code-block:: console

  $ mpicc -O3 -o pgm pgm.c

``mpicc`` is a wrapper and the command below shows what it does:

.. code-block:: console

  $ mpicc -show

To run an MPI program, you type:

.. code-block:: console

  $ mpirun -np <#processes> ./pgm

Writing a simple MPI program
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The exercise is to write a simple ping benchmark in MPI.c. You can start with
this boilerplate code:

.. code-block:: c

  #include <mpi.h>
  int main(int argc, char ** argv) {
    int processes, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    ...

    MPI_Finalize();
    return 0;
  }


Using mpiP
^^^^^^^^^^

You can load the mpip module by typing:

.. code-block:: console

  $ module load libunwind
  $ module load mpip

To link mpiP to your program you need to add these libraries on your link line:

.. code-block:: console

  -L$(MPIP_LIBDIR) -lmpiP -lm -lbfd -L$(LIBUNWIND_LIBDIR) -lunwind

Using HPCToolkit
^^^^^^^^^^^^^^^^

