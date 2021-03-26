# Research Survey for the Surface Reconstruction Using Delaunay Triangulation

#### Paper Title and link

**Surface reconstruction by Voronoi filtering**, https://dl.acm.org/doi/10.1145/276884.276889

#### Retell the paper

##### Background

Under the large amount of requirements of point cloud reconstruction in computer graphics, medical imaging and cartography, previously there are only heuristic solutions on reconstruction, at this time, there are few papers demonstrating algorithms for the two-dimensional problem with firm guaranteed properties.

There are some other algorithms

The algorithm relies on an older algorithm, Boissonnat's algorithm, and correct its mistake in even dense sample sets can give Delaunay tetrahedra with circumspheres that are arbitrarily far from the medial axis, which becomes the second observation which motivates the definition of poles.

##### Method description

##### The results based on my own understanding for the paper

#### My own analysis

##### Pros and cons

###### Pros

###### Cons

- Although Delaunay algorithm works fine with most of the point clouds, when facing point clouds with large quantity, the algorithm can be very slow or even fails to handle data.

##### The comparison with other related methods

α-shapes: Like this algorithm, it is a subcomplex  of the Delaunay, a Delaunay simplex belongs to the α-shape of S if its circumsphere has radius at most α. The major flaw of using α-shapes for surface reconstruction is that the optimal value of α depends on the sampling density, which often varies over different parts of the surface.

Euclidean minimum spanning tree: can be used to reconstruct uniformly sampled curves in the plane. (Figueiredo and Miranda Gomes)

crust: noneven

β-skeleton: noneven

##### The future work

