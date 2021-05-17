# CS271 Spring 2021 Computer Graphics II

# HomeWork 5

**Name:** **任怡静**

**Student ID:2018533144**

**E-mail: renyj@shanghaitech.edu.cn**

### Problem 1: Use Explicit and Implicit Laplacian method with Uniform weights and Cotangent weights, also show the point normals and curvatures

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
    - Or if you want to check model **bu.obj** with cotangent weight, simply type `.\pa1.exe ..\models\bu.obj`  <img src="D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw5\Pics\instruction_ps.png" style="zoom:40%;" />
    -  **Press "U" on keyboard for explicit smoothing, press "S" on keyboard for implicit smoothing, KEEP PRESSING UNTIL YOU FEEL SATISFIED WITH YOUR RESULT.**
  
- If you use visual studio (2017) to open the pa1.sln

  - In **Project -> pa1 Properties... -> Debugging**, Set **Command Arguments** to `models\{THE_OBJ_FILENAME} {USE_UNIFORM_WEIGHT}`.                                         <img src="D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw5\Pics\instruction_vs.png" style="zoom: 40%;" />

    

  - For example
  
    - if you want to check model **bu.obj** with uniform weight, simply write  in`models\bu.obj m`
    - Or if you want to check model **bu.obj** with cotangent weight, simply write in`models\bu.obj`  
  
  -  **Press "U" on keyboard for explicit smoothing, press "S" on keyboard for implicit smoothing, KEEP PRESSING UNTIL YOU FEEL SATISFIED WITH YOUR RESULT.**

##### Output

- It will output in console a line of code showing the required type of smoothing is finished.
- It will output a window displaying the mesh, drag left mouse button to rotate and hold on to middle mouse button and drag to scale. The good areas will be in blue, and the areas that are not so good will appear as blue. The curvature and normal are demostrated as the colors on the mesh.

##### Results(Just picked some obvious models for demostration)

- For bu.obj

  - Explicit Smoothing with lambda = 0.5 (Left Uniform weight, right Cotangent weight)

    <figure class="half">
        <img src="UniformExplicit1.png" style="zoom:30%;" /><img src="CotangentExplicit1.png" style="zoom:30%;" /><img src="UniformExplicit10.png" style="zoom:30%;" /><img src="CotangentExplicit10.png" style="zoom:30%;" /><img src="UniformExplicit30.png" style="zoom:30%;" /><img src="CotangentExplicit30.png" style="zoom:30%;" />

  - Implicit Smoothing with lambda = 0.5, iteration = 1 per BCG, tolerance 10% per BCG (Left Uniform weight, right Cotangent weight)

    <figure class="half">
        <img src="UniformImplicit1.png" style="zoom:30%;" /><img src="CotangentImplicit1.png" style="zoom:30%;" /><img src="UniformImplicit10.png" style="zoom:30%;" /><img src="CotangentImplicit10.png" style="zoom:30%;" /><img src="UniformImplicit30.png" style="zoom:30%;" /><img src="CotangentImplicit30.png" style="zoom:30%;" />

- For cactus.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw5\Pics\cactus.png)

- For gbones.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw5\Pics\gbones.png)

- For torus03.obj

![](D:\Rigin_Rain\Classes\CS271\ShangHaiTechCS271-Hws\hw4\Pics\torus03.png)

##### Notice

- When using Cotangent weights, there will be some time that too many iterations of smoothing causing some of the vertices' coordinates to perform oddly, leading to face loss  and inversion of point normals, so try to limit the times of pressing "S" or "U" so that you get the right result.
- Sometimes if scale the view too close, the program will provide error, but this seems to be the bug of the template, just restart the program and be careful when scrolling view
