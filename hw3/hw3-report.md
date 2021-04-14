# Research Survey for PV-RCNN: Point-Voxel Feature Set Abstraction for 3D Object Detection

#### Paper Title and link

**PV-RCNN: Point-Voxel Feature Set Abstraction for 3D Object Detection**, https://arxiv.org/pdf/1912.13192v1.pdf

#### Retell the paper

##### Background

In the past few years, the rise of autonomous driving and robotics has drawn great attention to 3D object detection. Plenty of methods has already been brought up to serve for realization of detections towards different specific needs, but the updates in hardware provides scientists with more datasets and uncertain factors and the old methods cannot fulfill the needs. Under this condition, the team in this paper propose a new delegate framework.

Before this paper, the main stream method for detections could be classified to two categories: grid-based and point-based method. However, neither methods can receive "high performance" that is both efficient and accurate, therefore this paper make a combination of the advantages of the two methods by carefully designing the framework to mend each others flaws with the two methods above. The launched code currently performs great on KITTI and Waymo benchmarks. 

##### Method description

- In this paper, the reconstruction process is specified as follows: 
  - Input: A 3D point cloud dataset **P**
  - Output: 3D Bounding boxes for target objects in **P**
- The algorithm and some explanations:
  - The algorithm of this paper relies on an older algorithm, **Boissonnat's algorithm**, which gives a good instruction that "sculpts" the interior of a geometry with a subset of Delaunay tetrahedra. This gives the first observation for the algorithm that typical Delaunay tetrahedra possesses  circumspheres approximating maximal empty balls whose center is at the points of medial axis. And by correcting Boissonnat's mistake of not considering dense sample sets being able to give Delaunay tetrahedra with circumspheres that are arbitrarily far from the medial axis, the paper obtains its second observation and propose the crucial definition of poles.
  - The algorithm first uses Voronoi to approximate the MAT of sample points **S**, then according to duality, applies the Delaunay to recover the surface of points. It stands out from other algorithms on dealing with dense samples by bringing up the concept of "**poles**", which is a correction for over-sensitive areas (the dense sampled point areas). By using poles set **P** instead of using Voronoi points set **V** with sample points **S** to reconstruct the surface, the new algorithm makes sure the surface in dense-sampled areas acceptably smooth. When finish filtering with both Voronoi and normals, almost all the lumps on the surface will be gone.
  - When the samples are dense, the Voronoi cell of every sample is **long and skinny**, and they are perpendicular to the surface, thus the two ends at the long skinny direction of Voronoi cell may be too far away from the medial axis, causing the lumps on surface. It is **poles**' job to make corrections to those Voronoi vertices in dense areas.
  - The pseudo code turns out to look like this:
    - Find out the Voronoi diagram for sample points **S**
    - Then traverse through **S**, for point **s** in **S**:
      - If **s** is not on the convex hull of **S** (meaning **s** is not a sample point in an open Voronoi cell), select the Voronoi point **p+** that fulfills: **p+** belongs to Voronoi(**s**), and Distance(**p+**, **s**) is the maximum.
      - If **s** is on the convex hull of **S** (meaning **s** is a sample point in an open cell of Voronoi), select the point **p+** that fulfills: **p+** is at infinite distance outside the convex hull, and the direction of vector **sp+** is the average of outward normals of all hull faces meeting at **s**.
      - When picked **p+**, pick another pole **p-** to be the projection on negative direction of vector **sp+**.
    - Collect all **p+** and **p-** except the ones that are at infinite distance to form a set **P**, apply the Delaunay triangulation of **S AND P**.
    - Remove the triangles that contains points that do not belong to **S**. (**Voronoi Filtering**)
    - Calculate the angle formed by the normal of triangle face **T** and vector to **p+** from one of the vertex of **T**, if the largest of three angles is bigger than predefined value θ (or 2.2θ for other two angles), delete **T**. (**Filtering by normal**)
    - Orient triangles and poles in order and extract the final result of surface without sharp dihedral angles, it should not be used in reconstructing surface with boundaries. (**Trimming**)

##### The results based on my own understanding for the paper

- The paper proposes the PV-RCNN method, which perfectly combines the advantages of both grid-based and point-based methods, making great contribution in improving 3D object detection within affordable memory consumption.
- Brings up the **voxel-to-keypoint** process to compress multiscale voxel features into several key points, which the key points features contains both accurate location and rich scene context, thus improves the detection performance greatly.
- Propose another process called **ROI-grid pooling** method which converts key points to ROI-grid points for accurate box refinement and confidence prediction. The process improves accuracy because it gets information from multiscale voxels compression thus have multiple receptive fields for box refinement.
- The launched trained PV_RCNN AI has beaten all previous detection methods, which leads the first place in KITTI 3D detection and Waymo Open dataset.

#### My own analysis for the paper

##### Pros and cons

###### Pros

- Compared to other algorithms at the same time, it had made great progress in obtaining smooth and accurate surface for many point clouds.
- The algorithm has good robustness in input. It can accept samples whose points spread evenly in the original model space and also samples whose points distributes nonuniformly, which is a great progress considering the actual sampling situation.
- The algorithm has good robustness in output. Even it is not designed to do closure for open surfaces, it can still give out a satisfying result when dealing with these kind of data. The two filtering process also performs perfectly when facing some extra links between parted models or burrs and lumps in models.(see the figure below)

###### Cons

- Although the algorithm works fine with most of the point clouds, the calculation of Voronoi diagrams is very costly. Unfortunately the algorithm doesn't mention any optimizations on Voronoi calculations, thus when facing point clouds with large quantity, the algorithm may be very slow or even fails to handle data.
- The algorithm does well on reconstructing smooth surface, but sample sets with sharp corners will receive bad results including losing correct triangles, getting burrs and so on.

##### The comparison with other related methods

- **α-shapes** and **Euclidean minimum spanning tree**: 
  - They all have guarantees to construct two dimensional curves with point clouds. However, the optimal result these two algorithms depends on the sampling density, which often varies over different parts of the surface, so they are bad at dealing with non-even sampled point clouds. The algorithm in this paper is able to accept unevenly spread point clouds and still gives out satisfying outputs. 
  - Also, the two methods mentions above do not have guarantees for higher dimensional reconstructions. And the algorithm in this paper is able to reconstruct surface in three dimensional space.
- **β-skeleton**：
  - It can accept inputs of nonuniform point clouds, and the algorithm in this paper is also capable of that.
  - Currently β-skeleton is guaranteed to give reconstruction results in two dimensional space, but the direct extension to higher dimensional, like three dimensions, is proved to be wrong for β-skeleton. And the algorithm in this paper can be used for reconstructions of three-dimensional point clouds.

##### The future work

- There are two ways for two dimensional curve reconstruction  that can work with nonuniform point clouds: CRUST(Voronoi filtering) and β-skeleton. In this paper CRUST has been proved to have guaranteed reconstruction quality for three dimensional point cloud reconstruction, it is worth trying to also extend β-skeleton for surface reconstruction in three dimensional space.
- Currently the reconstruction is limited to three dimensional space, it is worthwhile to extend the algorithm to k-dimension.
- The numbers of sampling points for input and the universal limitation value of θ for step 5 in the algorithm are not sure, it still needs more experiments and proofs.
- The algorithm is oriented for smooth surfaces, researches on whether CRUST can be used on reconstructing point clouds with hard corners (like machine parts) will be very contributive.
- The cost of the algorithm is rather high, if some optimizations are made, the algorithm may be able to handle larger point clouds.

