# Research Survey for the Surface Reconstruction Using Delaunay Triangulation

#### Paper Title and link

**Surface reconstruction by Voronoi filtering**, https://dl.acm.org/doi/10.1145/276884.276889

#### Retell the paper

##### Background

Under the large amount of requirements of point cloud reconstruction in computer graphics, medical imaging and cartography, previously there are only heuristic solutions on reconstruction, at this time, there are few papers demonstrating algorithms for the two-dimensional problem with firm guaranteed properties.

There are some other algorithms

The algorithm relies on an older algorithm, Boissonnat's algorithm, and correct its mistake in even dense sample sets can give Delaunay tetrahedra with circumspheres that are arbitrarily far from the medial axis, which becomes the second observation which motivates the definition of poles.

##### Method description

- In this paper, the reconstruction process is specified as follows: 
  - Input: A set of point **S** sampled from the original two-dimensional manifold **F** in 3D space
  - Output: A triangular mesh **F'** which possesses **S** as its vertex set, and can converge to **F** in tolerable error.
- The algorithm and some explanations:
  - The algorithm is an extension of points reconstruction algorithm published by one of the authors Amenta, which is called **CRUST**, to simply say???(换说法), **CRUST** first uses Voronoi to approximate the MAT of sample points **S**, then according to duality, applies the Delaunay to recover the surface of points. The algorithm gives its innovations on dealing with dense samples by bringing up a new concept "**poles**", which is a correction for over-sensitive areas (the dense sampled point areas), instead of using Voronoi graph **V** with sample points **S** to reconstruct the surface???（需要改进叙述）, thus making sure the surface in dense-sampled areas also acceptably smooth. When finish filtering with both **Voronoi** and **poles-normals**, almost all the lumps on the surface will be gone.
  - The pseudo code turns out to look like this:
    - Find out the Voronoi diagram for sample points **S**
    - Then traverse through **S**, for point **s** in **S**:
      - If **s** is not on the convex hull of **S** (meaning **s** is not a sample point in an open cell of Voronoi), select the Voronoi point **p+** that fulfills: **p+** belongs to Voronoi(**s**), and Distance(**p+**, **s**) is the maximum.
      - If **s** is on the convex hull of **S** (meaning **s** is a sample point in an open cell of Voronoi), select the Voronoi point **p+** that fulfills: **p+** is at infinite distance outside the convex hull, and the direction of vector **sp+** is Average of outward normals of all hull faces containing **s**.
      - When picked **p+**, pick another pole **p-** to be the projection on vector -**sp+**.
    - Collect all **p+** and **p-** except the ones that are at infinite distance to form a set **P**, apply the Delaunay triangulation of **S AND P**.
    - Remove the triangles that contains points that do not belong to **S**. (**Voronoi Filtering**)
    - Calculate the angle formed by the normal of triangle face left **T** and vector to **p+** from one of the vertex of **T**, if the largest of three angles is bigger than predefined value θ (or 2.2θ for other two angles), delete **T**. (**Filtering by normal**)
    - Orient triangles and poles in order and extract the final result of surface without sharp dihedral angles, it should not be used in reconstructing surface with boundaries. (**Trimming**)

##### The results based on my own understanding for the paper

The algorithm stands out on the great effect of smoothness in reconstruction process, its unique method of using poles to filter over-dense errors or even noise make great contribution to the research of surface reconstruction.

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

