# CS271 Spring 2021 Computer Graphics II

# HomeWork 4

**Name:** **任怡静**

**Student ID:2018533144**

**E-mail: renyj@shanghaitech.edu.cn**

### Problem 1: Mesh Info Display

##### Package and Environment

- The convex hull algorithm is realized using language **C++**
- The project only uses the **GLUT** library as provided.
- Follow the instructions (https://piazza.com/class/kl8wmh1uda82ps?cid=29) on Piazza if **GULT** is not available on your computer

##### Instructions

- In **Project -> pa1 Properties... -> Debugging**, Set **Command Arguments** to `models\THE_OBJ_FILENAME`.
  - For example, if you want to check model **bu.obj**, simply write in `models\bu.obj`

##### Output

- It will output in console with **lines** of information of the mesh, containing the number of vertices, half-edges (including boundary edges and their twin edges), faces, boundary loops, genus and connected components.
- It will output a window displaying the mesh, drag left mouse button to rotate and hold on to middle mouse button and drag to scale.

##### Results(Some models are adjusted to have a better view)

- For atet.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\atet.png)

- For bu.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\bu.png)

- For bu_head.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\bu_head.png)

- For cactus.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\cactus.png)

- For deo10k.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\deo10k.png)

- For feline4k.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\feline4k.png)

- For gbones.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\gbones.png)

- For mannequin.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\mannequin.png)

- For skull.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\skull.png)

- For sp.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\sp.png)

- For sphere.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\sphere.png)

- For torus03.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\torus03.png)

### Problem 2: Delete a Vertex

##### Package and Environment

- The same as problem 1

##### Instructions

- Run the code as problem 1
- press **2** on keyboard to enter **Selection Mode**
- **Left click mouse button** to select one vertex in the display window, the selected vertex will show in red.
- Press **D** on keyboard to delete one vertex

##### Output

- The display window will show the result in real time.

##### Notice

- Since there is no maintenance in vertices' Indices, it is not recommended to delete another vertex.
- Due to not maintaining the vertices's Indices, the menu when right click the mouse is disabled if one vertex is deleted
- Sometimes if scale the view too close, the program will provide error, but this seems to be the bug of the template, just restart the program and be careful when scrolling view

##### Result

- Performance on ordinary non-bounding vertex
  - As the first two figures, you can see the vertex has been deleted and the layout of the face around the deleted vertex changes to fit the rules
  - The third and fourth picture shows that the program prohibits the second deletion, it also prevents some unintended triggering.

<figure class="half">
    <img src="DeleteVertexSelect.png" style="zoom:50%;" /><img src="DeleteVertexMerge.png" style="zoom:50%;" /><img src="DeleteVertexSelectSecond.png" style="zoom:50%;" /><img src="DeleteVertexMergeSecond.png" style="zoom:50%;" />
</figure>

- Performance on ordinary bounding vertex
- As in figures, the corresponding vertex has been deleted, you can see the tiny difference in the way the boundary edge goes

<figure class="half">
    <img src="DeleteVertexBoundarySelect.png" style="zoom:50%;" /><img src="DeleteVertexBoundary.png" style="zoom:50%;" />

- Performance on non-bounding Edge vertex (sharp corners)
  - As you can see, even if deleting an sharp corner, the mending of the faces still performs good

<figure class="half">
    <img src="DeleteVertexNonConnectSelect.png" style="zoom:50%;" /><img src="DeleteVertexNonConnect.png" style="zoom:50%;" /><img src="DeleteVertexSharpSelect.png" style="zoom:50%;" /><img src="DeleteVertexSharp.png" style="zoom:50%;" />

