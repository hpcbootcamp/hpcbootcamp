Day 2: Writing OpenMP programs
==============================

:download:`Lecture slides <slides/day2.pdf>`

Using slurm
^^^^^^^^^^^

Slurm is used to submit batch jobs on batch systems. Below is a sample slurm
script:

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

.. code-block:: c 

  #include <omp.h>

  double elapsed = 0.0;
  double tmp;

  tmp = omp_get_wtime();
  for (int i=0; i<n; i++) {
    /* do work */
  }
  elapsed = omp_get_wtime() - tmp;

  printf(“time spent in loop: %f\n”, elapsed);


