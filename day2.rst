Day 2: Writing OpenMP programs
==============================

:download:`Lecture slides <slides/day2.pdf>`

Using slurm
^^^^^^^^^^^

Slurm is used to submit jobs on batch systems. Below is a sample slurm script:

.. literalinclude:: code/day2/submit.sh
    :language: shell

Jobs are submitted to the batch system using sbatch:

.. code-block:: console

  $ sbatch submit.sh

You can check the status of your jobs by typing:

.. code-block:: console

  $ squeue -u <username>


Timing OpenMP programs
^^^^^^^^^^^^^^^^^^^^^^

We can use ``omp_get_wtime()`` to time OpenMP programs. Below is a simple
example:

.. code-block:: cpp

  #include <omp.h>

  double elapsed = 0.0;
  double tmp;

  tmp = omp_get_wtime();
  #pragma omp parallel for
  for (int i=0; i<n; i++) {
    /* do work */
  }
  elapsed = omp_get_wtime() - tmp;

  printf(“time spent in loop: %f\n”, elapsed);


GDB and OpenMP
^^^^^^^^^^^^^^

GDB has extensions for debugging multithreaded programs, and we will briefly
cover them here.

We will use this file for the :download:`gdb <code/day2/gdb-omp.c>` exercise.

Thread-specific Commands
""""""""""""""""""""""""

A thread is a call stack and a program counter. Most programs only have a single thread of execution, but when we start writing parallel programs with OpenMP there can be very many threads. We need a means to display threads, switch between them, and issue debugger commands on them.

* `info thread <https://sourceware.org/gdb/current/onlinedocs/gdb/Threads.html#index-info-threads-159>`_: Print out information about all current threads. If the program is in a parallel loop, then all threads may be active. If the program is in a single-threaded section, all but one of the threads will be in some idle state.

* `thread <http://sourceware.org/gdb/current/onlinedocs/gdb/Threads.html#>`_: Print the current thread, or switch to another thread. GDB numbers its threads starting at 1. So, GDB might refer to a thread as "1", while OpenMP refers to the thread as "0", and the operating system itself may have other means for referring to the thread. For example, "thread 1" will switch to thread 1, which is typically the OpenMP master thread.

* ``thread apply``: Run a command on one or more threads. For instance, "thread apply all print i" will print the value of variable i on all threads.

How Other Commands Interact With Threads
""""""""""""""""""""""""""""""""""""""""

* ``break``: By default, a breakpoint will stop all threads when any single thread meets that breakpoint. For instance, if this is the first iteration of a parallel loop, other threads may not have even reached that point yet. GDB can be configured to let the other threads run.

* `break ... thread ... <http://sourceware.org/gdb/current/onlinedocs/gdb/Thread_002dSpecific-Breakpoints.html#index-breakpoints-and-threads-345>`_: Set a breakpoint at a given line for a given thread. This can not be used if the given thread does not exist currently. For example, "break gdb-omp.c:18 thread 1" will break execution of the tutorial program at line 18 on thread 1.

* ``step`` and ``next``: The step and next commands increment execution in the current thread. The other threads will run free until execution stops in the current thread at the appropriate line as per step and next semantics. These may not work the way you expect across loop boundaries due to the stack mangling that needs to happen to support multithreaded code.


Adding OpenMP pragmas to a sequential code
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The purpose of this exercise is to gain experience in shared memory parallel
programming with OpenMP. For this project you are to write a parallel
implementation of a program to simulate the `Game of Life
<https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life>`_.
 
The game of life simulates simple cellular automata. The game is played on a
rectangular board containing cells. At the start, some of the cells are
occupied, the rest are empty. The game consists of constructing successive
generations of the board. The rules for constructing the next generation from
the previous one are:
 
* death: cells with 0,1,4,5,6,7, or 8 neighbors die (0,1 of loneliness and 4-8 of over population)
* survival: cells with 2 or 3 neighbors survive to the next generation.
* birth: an unoccupied cell with 3 neighbors becomes occupied in the next generation.
 
For this project the game board has finite size. The x-axis starts at 0 and
ends at X_limit-1 (supplied on the command line). Likewise, the y-axis start at
0 and ends at Y_limit-1 (supplied on the command line).

A sequential version of the program can be foumnd :download:`here <code/day2/life-seq.c>`.

To compile the program, you type:

.. code-block:: console

  $ gcc -O3 -o life life-seq.c 

Input
"""""
Your program should read in a file containing the coordinates of the initial cells. Sample files are located here: `life.data.1 <http://www.cs.umd.edu/class/fall2019/cmsc714/assignments/mpi/data/life.data.1>`_, `life.data.2 <http://www.cs.umd.edu/class/fall2019/cmsc714/assignments/mpi/data/life.data.2>`_, and `life.data.1000x1000 <http://www.cs.umd.edu/class/fall2019/cmsc714/assignments/mpi/data/life.data.1000x1000>`_.

The command line of the program looks like:

.. code-block:: console

  $ ./life life.data.1 100 250 250

The solution file is :download:`here <code/day2/life-par.c>`.
