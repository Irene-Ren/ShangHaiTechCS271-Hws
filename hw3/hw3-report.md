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
  - The network is based on **Voxel CNN with 3D sparse convolution**, which divides input points **P** into small voxels and do down sampling with 3D sparse convolution, then convert the down-sampled 3D feature volumes into 2D birdview map to obtain 3D bounding box proposal. However, the Voxel CNN receives low resolution results due to the down sampling, the conventional pooling to obtain features to sparse points will waste much time on computing 0-value results, therefore an alternative is needed for feature extraction.
  - The **set abstraction** has strong capability of obtain feature points from an arbitrary size neighborhood, which perfectly solves the problem mentioned above. But the naive solution using set abstraction by directly aggregate the multi-scale feature to RoI grids is wasteful in space. Thus we need to combine the two methods together for better feature extraction and bounding box proposal process.
  - The proposed framework works like this:
    - **Keypoint Sampling**: Find the key point set **K** by using the **Furthest-Point-Sampling** (FPS).
    - **Voxel Set Abstraction Module**: Encode the multi-scale semantic features from 3D CNN feature volumes to the keypoints. That is to say, collect the non-zero voxels within the range of $r_k$, and transform them 

- The paper proposes the PV-RCNN method, which perfectly combines the advantages of both grid-based and point-based methods, making great contribution in improving 3D object detection within affordable memory consumption.
- Brings up the **voxel-to-keypoint** process to compress multiscale voxel features into several key points, which the key points features contains both accurate location and rich scene context, thus improves the detection performance greatly.
- Propose another process called **ROI-grid pooling** method which converts key points to ROI-grid points for accurate box refinement and confidence prediction. The process improves accuracy because it gets information from multiscale voxels compression thus have multiple receptive fields for box refinement.
- The launched trained PV_RCNN AI has beaten all previous detection methods, which leads the first place in KITTI 3D detection and Waymo Open dataset.

#### My own analysis for the paper

##### Pros and cons

###### Pros

- The method outperforms most of the algorithms in 3D detection, it shows great improvement in accuracy, meanwhile it computes at a good speed.

###### Cons

- The keypoints extraction step of using the FPS do not have good guarantee to produce representative and uniformly distributed keypoints.
- In set abstraction of VSA, the calculation of local point cloud can be of high time complexity since it calculates for multiple keypoints in multiple levels.

##### The comparison with other related methods

- **3D Object Detection with Grid-based Methods**: 
  - Compared with PV-RCNN, both methods' main idea is to distribute the points into small voxels and process them in 3D CNN.
  - However, due to the fixed kernel size of 3D convolution, the receptive fields are limited in the traditional grid-based methods. For PV-RCNN, it uses set abstraction operation proposed by a point-based method called PointRCNN, which enables it to have flexible receptive fields for point cloud feature learning, therefore its resolution quality and accuracy of bounding box proposal will be much better than traditional grid-based methods.
- **3D Object Detection with Point-based Methods**：
  - PV-RCNN and point-based methods have the similarity in abstracting features
  - But doing set abstraction directly on the original dataset is rather wasteful in space. PV-RCNN first uses grid-based method to preprocess the point cloud into keypoint features, which is much smaller than the original data size, and give them to point-based method for further calculations. This move will greatly improve the spatial performance.

##### The future work

- The algorithm is now based on pure LiDAR, which contains plenty geometrical information but little contextual information, thus it is worth while to extend the algorithm to use the combination of LiDAR and RGB dataset.
- The upcoming datasets like Waymo contains almost 4 times more sensing area, which requires more "higher performance" algorithms.

