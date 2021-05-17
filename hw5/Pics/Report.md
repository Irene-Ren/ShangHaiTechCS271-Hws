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

- If you use command lines **(Windows Powershell recommended)**
  - First Enter the `BUILD_EXE` folder in the terminal window
  - then run the EXE with `.\pa1.exe ..\models\{THE_OBJ_FILENAME} {USE_UNIFORM_WEIGHT}`
  - (`./pa1.exe ../models/{THE_OBJ_FILENAME}` in Linux based system maybe, I haven't tried)
  - For example
    - if you want to check model **bu.obj** with uniform weight, simply type `.\pa1.exe ..\models\bu.obj m`
    -  Or if you want to check model **bu.obj** with cotangent weight, simply type `.\pa1.exe ..\models\bu.obj`  <img src="D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw5\Pics\instruction_ps.png" style="zoom:40%;" />
  
- If you use visual studio (2017) to open the pa1.sln

  - In **Project -> pa1 Properties... -> Debugging**, Set **Command Arguments** to `models\{THE_OBJ_FILENAME} {USE_UNIFORM_WEIGHT}`.                                         <img src="D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw5\Pics\instruction_vs.png" style="zoom: 40%;" />

    

  - For example
  
  - if you want to check model **bu.obj** with uniform weight, simply write  in`models\bu.obj m`
  
  -  Or if you want to check model **bu.obj** with cotangent weight, simply write in`models\bu.obj`  

##### Output

- It will output in console a line of code showing the required type of smoothing is finished.
- It will output a window displaying the mesh, drag left mouse button to rotate and hold on to middle mouse button and drag to scale. The good areas will be in blue, and the areas that are not so good will appear as blue.

##### Results(Just picked some obvious models for demostration)

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

##### Result (Selected Some rough models (less vertices) for better view)

- Performance on ordinary non-bounding vertex **(mannequin.obj)**
  - As the first two figures, you can see the vertex has been deleted and the layout of the face around the deleted vertex changes to fit the rules

 <img src="DeleteVertexSelect.png" style="zoom:30%;" /><img src="DeleteVertexMerge.png" style="zoom:30%;" />

- The following two pictures shows that the program prohibits the second deletion, it also prevents some unintended triggering.

<img src="DeleteVertexSelectSecond.png" style="zoom:25%;" /><img src="DeleteVertexMergeSecond.png" style="zoom:25%;" />

- Performance on ordinary bounding vertex **(mannequin.obj)**

  - As in figures, the corresponding vertex has been deleted, you can see the tiny difference in the way the boundary edge goes

<figure class="half">
    <img src="DeleteVertexBoundarySelect.png" style="zoom:30%;" /><img src="DeleteVertexBoundary.png" style="zoom:30%;" />


- Performance on non-bounding Edge vertex (sharp corners) **(gbones.obj, atet.obj)**
  - As you can see, even if deleting an sharp corner, the mending of the faces still performs good

<figure class="half">
    <img src="DeleteVertexNonConnectSelect.png" style="zoom:30%;" /><img src="DeleteVertexNonConnect.png" style="zoom:30%;" /><img src="DeleteVertexSharpSelect.png" style="zoom:30%;" /><img src="DeleteVertexSharp.png" style="zoom:30%;" />


