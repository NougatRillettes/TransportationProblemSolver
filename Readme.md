Transportation problem solver
=============================

This is a C implementation of the algorithm know as the "Hunagarian method" or "Munkres Algorithm" proposed in the following [paper](www.math.ucdavis.edu/~saito/data/emd/munkres.pdf):

>MUNKRES, James. Algorithms for the assignment and transportation problems. Journal of the society for industrial and applied mathematics, 1957, vol. 5, no 1, p. 32-38.

Invocation
----------

`tps [-d] [-l] [input_file]`

Where:

 - `-d` activates *debug* mode
 - `-l` activates *legacy* mode
 - `input_file` is a file containing the input. If no file is provided, input is read from `stdin`.


Normal mode
------------

In normal mode, the agents and jobs are tagged with a 64 bit unsigned integer.


###Input

The input is given in the following format:

```
<height of matrix (= number of agents)>  <width of matrix (= number of jobs)>
space separated list of <tag of agent type i>:<number of agents of type i>
space separated list of <tag of job type j>:<number of jobs of type j>

<cost matrix>
```

For example, if there is:

- 2 types of agents tagged 42 and 1337, where the first type has 5 agents, and the second 3 agents.
- 3 types of jobs tagged 1, 2, 3 with the following number of jobs: 4, 2, 2

The input would be (with an arbitrary cost matrix):

```
2 3
42:5 1337:3
1:4 2:2 3:2

45 77 99
 9 37 99
```
Note that whether you use spaces, newlines, or tab doesn't matter since input is read with scanf.

###Output

For the given input, the output is:

```
Minimum cost: 416
42 1 3
42 3 2
1337 1 1
1337 2 2
```

Meaning that:

 - the less costly assignment found costs 416
 - 3 agents of type 42 are assigned to a type 1 job
 - 2 agents of type 42 are assigned to a type 3 job
 - 1 agent of type 1337 is assigned to a type 1 job
 - 2 agents of type 1337 are assigned to a type 2 job


Legacy mode
------------

Legacy mode is the same has normal mode except:

 - no tags are given in the input
 - only the minimum cost is displayed in the output

###Input

```
2 3
5 3
4 2 2

45 77 99
 9 37 99
```

###Output

```
Minimum cost: 416
```

License
--------

This work is placed under the MIT License (see attached License.txt).

Patches and bug reports are welcomed.
