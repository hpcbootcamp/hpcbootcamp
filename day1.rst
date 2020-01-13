Day 1: Introduction
===================

:download:`Lecture slides <slides/day1.pdf>`

Using GDB
^^^^^^^^^

The GNU Debugger, or GDB, is a freely available tool that you will find almost
everywhere. Using a debugger, you will be able to stop execution of your
program, step through execution, and observe the state of your program. You can
also do post-mortem analysis on core dump files, which are left by the
operating system when your program suffers a catastrophic crash.

We will use this :download:`program <code/quicksort.c>` for learning gdb:

.. literalinclude:: code/quicksort.c
    :language: c

To invoke gdb, type:

.. code-block:: console

  $ gcc -g -o quicksort quicksort.c
  $ gdb quicksort

You should investigate the following commands in gdb, and understand how and when they should be applied. Commands generally lie in two categories: controlling execution and inspecting the state of the program. First though, is the list command:

* `list <https://sourceware.org/gdb/current/onlinedocs/gdb/List.html#index-list-439>`_: List source code. You can list a particular function by using "list function-name". This is useful when using the GDB interface from the command line.

Controlling execution
"""""""""""""""""""""

**Breakpoints**

A breakpoint is a place in the code where GDB will stop execution. The most
simple type of breakpoint unconditionally stops the program every time that
point is reached during execution. There are other kinds of breakpoints, such
as conditional breakpoints and temporary breakpoints, that we will not cover
here.

* `break <http://sourceware.org/gdb/current/onlinedocs/gdb/Set-Breaks.html#index-break-211">`_: Set a break point.
* `info break <http://sourceware.org/gdb/current/onlinedocs/gdb/Set-Breaks.html#index-info-breakpoints-225>`_: List all break points.
* `delete <http://sourceware.org/gdb/current/onlinedocs/gdb/Delete-Breaks.html#index-delete-breakpoints-265>`_: Delete break points.

**Running**

* `run <http://sourceware.org/gdb/current/onlinedocs/gdb/Starting.html#index-run-103>`_: Run your program from the beginning. This will run the program until completion, the next breakpoint, or until some error occurs.
* `continue <http://sourceware.org/gdb/current/onlinedocs/gdb/Continuing-and-Stepping.html#index-continue-287>`_: Continue execution until completion, the next breakpoint, or until some error occurs.
* `step <http://sourceware.org/gdb/current/onlinedocs/gdb/Continuing-and-Stepping.html#index-step-290>`_: Execute the next line of code, including entering functions.
* `next <http://sourceware.org/gdb/current/onlinedocs/gdb/Continuing-and-Stepping.html#index-next-292>`_: Execute the next line of code, but skip over function calls.

Inspecting state
""""""""""""""""

When a function is called in a program, it pushes down the current program
address, function parameters, and local variables for the function on a
*function call stack*. Each entry on this stack is called a *stack frame*. The
stack serves as a space to allocate temporary variables and let the program
know how to return from the function. Among other things, the stack can tell us
how the program got to its current state when it crosses a breakpoint or
crashes.

**Stack**

* `backtrace <http://sourceware.org/gdb/current/onlinedocs/gdb/Backtrace.html#index-backtrace-412>`_: Print the stack trace, from the current function down to the main() function (in C).
* `up <http://sourceware.org/gdb/current/onlinedocs/gdb/Selection.html#index-up-427>`_ and `down <http://sourceware.org/gdb/current/onlinedocs/gdb/Selection.html#index-down-428>`_: Go up and down in the function call stack to examine function parameters and local variables.

**Memory**

* `print <http://sourceware.org/gdb/current/onlinedocs/gdb/Data.html#index-print-492>`_: Print the current state of a given variable or expression. The print command is smart enough to print arrays and structs properly. You can even make function calls as part of the expression.

* `display <http://sourceware.org/gdb/current/onlinedocs/gdb/Auto-Display.html#index-display-533>`_: Like print, but this produces output every time the program stops. This prints nothing if the expression is not valid.

Using Gprof
^^^^^^^^^^^

Compile the program with -pg:

.. code-block:: console

  $ gcc -pg -O3 -o pgm pgm.c

When you run the program, a gmon.out file will be created. Then, we run gprof
to obtain text output:

.. code-block:: console

  $ gprof pgm gmon.out

We will use this program to trying gprof:

.. literalinclude:: code/primes/find-primes1.c
    :language: c

You can download all the variations using these links:
:download:`primes.zip <code/primes.zip>`

Makefiles
^^^^^^^^^

``make`` makes it easy to compile large code bases. A Makefile lets us specify
what needs to be compiled and what are the dependencies.

An example of a simple Makefile is below:

.. literalinclude:: code/Makefile
    :language: make

In the :download:`make directory <code/make.zip>` for today’s lab, there are
eight  files comprising a hypothetical program. Don’t try to figure out what
the program does, as it doesn’t do anything interesting. Instead, write a
Makefile for the whole program that separately compiles each .c file and
generates a single executable file. Then add a target clean to the Makefile
that removes executable and object files, and then add another target main.tgz
that creates a tarfile with that name containing all header and source files.
The command:

.. code-block:: console

  $ tar -czvf <tarfilename> <listoffiles>

will create a compressed tar file containing those files.
