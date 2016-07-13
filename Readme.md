Transportation problem solver
=============================

This is a C implementation of the algorithm know as the "Hunagarian method" or "Munkres Algorithm" proposed in the following [paper](www.math.ucdavis.edu/~saito/data/emd/munkres.pdf):

>MUNKRES, James. Algorithms for the assignment and transportation problems. Journal of the society for industrial and applied mathematics, 1957, vol. 5, no 1, p. 32-38.

Input format:
--------------

Tags are an 64 bits unsigned integer used to identify each jobs and agent. They are used to output the cost.


```
<height of matrix = number of agents> <width of matrix = number of jobs>
<space separated list of <tag of agent type i>:<number of agents of type i>>
<space separated list of <tag of jobs type j>:<number of jobs of type j>>

<cost matrix>
```
