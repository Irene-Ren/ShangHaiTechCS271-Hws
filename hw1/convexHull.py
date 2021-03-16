# import open3d as o3d
# import numpy as np

# file_name = raw_input() or "test.txt"

# matrix = np.loadtxt(file_name, dtype = 'float')


# #pcl = o3dtut.get_bunny_mesh().sample_points_poisson_disk(number_of_points=2000)
# hull, _ = pcd.compute_convex_hull()
# hull_ls = o3d.geometry.LineSet.create_from_triangle_mesh(hull)
# hull_ls.paint_uniform_color((1, 0, 0))
# o3d.visualization.draw_geometries([pcd, hull_ls])

import numpy as np
import open3d as o3d
MAX_N = 2000
ERROR = 0
class Point3D:
    m_x = 0
    m_y = 0
    m_z = 0
    def __init__(self,x,y,z):
        self.m_x = x
        self.m_y = y
        self.m_z = z
    def DotProduct(self, other):
        return self.m_x * other.m_x + self.m_y * other.m_y + self.m_z * other.m_z
    def CrossProduct(self, other):
        coord_x = self.m_y * other.m_z - self.m_z * other.m_y
        coord_y = self.m_z * other.m_x - self.m_x * other.m_z
        coord_z = self.m_x * other.m_y - self.m_y * other.m_x
        result = Point3D(coord_x, coord_y, coord_z)
        return result
    def Minus(self,other):
        p = Point3D(0,0,0)
        p.m_x = self.m_x - other.m_x
        p.m_y = self.m_y - other.m_y
        p.m_z = self.m_z - other.m_z
        return p

class Face:
    is_hull = False
    v1 = 0
    v2 = 0
    v3 = 0
    def __init__(self, x, y, z, h):
        self.is_hull = h
        self.v1 = x
        self.v2 = y
        self.v3 = z

class Convex3D:
    n = 0
    points = []
    triangleF = []
    orderedFaces = np.zeros((MAX_N,MAX_N),dtype=int)
    triangleNum = 0

    def DirectedVolume(self, p, f):
        vector1 = Point3D.Minus(self.points[f.v2], self.points[f.v1])
        vector2 = Point3D.Minus(self.points[f.v3], self.points[f.v1])
        vector3 = Point3D.Minus(p, self.points[f.v1])
        return Point3D.DotProduct(Point3D.CrossProduct(vector1, vector2), vector3)

    def AddPointP(self, p,a,b):
        print("Current vertex: ",[self.points[p].m_x, self.points[p].m_y,self.points[p].m_z])
        face = self.orderedFaces[a][b]
        if self.triangleF[face].is_hull:
            print("Current Face: ", [self.points[self.triangleF[face].v1].m_x,self.points[self.triangleF[face].v1].m_y,self.points[self.triangleF[face].v1].m_z], 
                                    [self.points[self.triangleF[face].v2].m_x,self.points[self.triangleF[face].v2].m_y,self.points[self.triangleF[face].v2].m_z], 
                                    [self.points[self.triangleF[face].v3].m_x,self.points[self.triangleF[face].v3].m_y,self.points[self.triangleF[face].v3].m_z])
            print(self.DirectedVolume(self.points[p],self.triangleF[face]))
            if self.DirectedVolume(self.points[p],self.triangleF[face]) >  ERROR:
                print("Visible to P")
                self.ExtendVisiblePlane(p, face)
            else:
                newFace = Face(b,a,p,True)
                self.orderedFaces[p][b] = self.triangleNum
                self.orderedFaces[b][a] = self.triangleNum
                self.orderedFaces[a][p] = self.triangleNum
                
                self.triangleF.append(newFace)
                print("Add face: ", [self.points[newFace.v1].m_x,self.points[newFace.v1].m_y,self.points[newFace.v1].m_z], 
                                    [self.points[newFace.v2].m_x,self.points[newFace.v2].m_y,self.points[newFace.v2].m_z], 
                                    [self.points[newFace.v3].m_x,self.points[newFace.v3].m_y,self.points[newFace.v3].m_z])
                self.triangleNum += 1
    def ExtendVisiblePlane(self, p, tri_index):
        print("Ori Face: ",[self.points[self.triangleF[tri_index].v1].m_x,self.points[self.triangleF[tri_index].v1].m_y,self.points[self.triangleF[tri_index].v1].m_z],
                            [self.points[self.triangleF[tri_index].v2].m_x,self.points[self.triangleF[tri_index].v2].m_y,self.points[self.triangleF[tri_index].v2].m_z],
                            [self.points[self.triangleF[tri_index].v3].m_x,self.points[self.triangleF[tri_index].v3].m_y,self.points[self.triangleF[tri_index].v3].m_z])
        self.triangleF[tri_index].is_hull = False
        self.AddPointP(p,self.triangleF[tri_index].v2, self.triangleF[tri_index].v1)
        self.AddPointP(p,self.triangleF[tri_index].v3, self.triangleF[tri_index].v2)
        self.AddPointP(p,self.triangleF[tri_index].v1, self.triangleF[tri_index].v3)
    def CreateOriginTetrahedron(self):
        if len(self.points) < 4:
            print("Not enough points for 3D convexhull")
            return False
        success = False
        for i in range(1, self.n):
            if Dist(self.points[0], self.points[i]) > ERROR:
                self.points[1],self.points[i] = self.points[i], self.points[1]
                success = True
                print("Find two points")
                break
        if not success:
            print("Same vertex for all in list")
            return success
        success = False
        for i in range(2, self.n):
            if Area(self.points[0],self.points[1],self.points[i]) > ERROR:
                self.points[2], self.points[i] = self.points[i], self.points[2]
                success = True
                print("Find three points")
                break
        if not success:
            print("Same line for all in list")
            return success
        for i in range(3, self.n):
            if Volume(self.points[0], self.points[1], self.points[2], self.points[i]) > ERROR:
                self.points[3], self.points[i] = self.points[i], self.points[3]
                success = True
                print("Find Four Points")
                break
        if not success:
            print("Same plane for all in list")
            return success
        for i in range(4):
            face_tmp = Face((i+1)%4,(i+2)%4,(i+3)%4,True)
            if self.DirectedVolume(self.points[i], face_tmp) > 0:
                face_tmp.v2, face_tmp.v3 = face_tmp.v3, face_tmp.v2
            self.orderedFaces[face_tmp.v1][face_tmp.v2] = self.triangleNum
            self.orderedFaces[face_tmp.v2][face_tmp.v3] = self.triangleNum
            self.orderedFaces[face_tmp.v3][face_tmp.v1] = self.triangleNum
            self.triangleF.append(face_tmp)
            self.triangleNum += 1
        return success
    def ExtendConvexHull(self):
        for i in range(4,self.n):
            for j in range(self.triangleNum):
                if self.triangleF[j].is_hull and self.DirectedVolume(self.points[i], self.triangleF[j]) > ERROR:
                    self.ExtendVisiblePlane(i,j)
                    break
        for face in self.triangleF:
            if not face.is_hull:
                self.triangleF.remove(face)
def Mod(a):
    return np.sqrt(a.m_x * a.m_x + a.m_y * a.m_y + a.m_z * a.m_z)
def Dist(a, b):
    return Mod(Point3D.Minus(a,b))
def Area(a, b, c):
    vector1 = Point3D.Minus(b,a)
    vector2 = Point3D.Minus(c,a)
    return Mod(Point3D.CrossProduct(vector1, vector2)) / 2.0
def Volume(a, b, c, d):
    vector1 = Point3D.Minus(b, a)
    vector2 = Point3D.Minus(c, a)
    vector3 = Point3D.Minus(d, a)
    volume = abs(Point3D.DotProduct(Point3D.CrossProduct(vector1, vector2),vector3)) / 6.0
    return volume

if __name__ == "__main__":
    file_name = raw_input() or "test"
    matrix = np.loadtxt(file_name, dtype = 'float')
    convexHull = Convex3D()
    convexHull.n = np.size(matrix, axis = 0)
    # TODO: If the first row do not contain exactly one float, tell n is missing
    for i in range(convexHull.n):
        convexHull.points.append(Point3D(matrix[i][0],matrix[i][1],matrix[i][2]))
    if convexHull.CreateOriginTetrahedron():
        print("Extending ConvexHull")
        convexHull.ExtendConvexHull()
        for f in convexHull.triangleF:
            print("Calculated Faces: ",[convexHull.points[f.v1].m_x,convexHull.points[f.v1].m_y,convexHull.points[f.v1].m_z],
                            [convexHull.points[f.v2].m_x,convexHull.points[f.v2].m_y,convexHull.points[f.v2].m_z],
                            [convexHull.points[f.v3].m_x,convexHull.points[f.v3].m_y,convexHull.points[f.v3].m_z])

    tri_faces = []
    for f in convexHull.triangleF:
        tri_faces.append([f.v1, f.v2, f.v3])
    tri_faces = o3d.utility.Vector3iVector(np.array(tri_faces).reshape(-1,3))

    # tri_points_index = []
    # for f in convexHull.triangleF:
    #     if f.v1 not in tri_points_index:
    #         tri_points_index.append(f.v1)
    #     if f.v2 not in tri_points_index:
    #         tri_points_index.append(f.v2)
    #     if f.v3 not in tri_points_index:
    #         tri_points_index.append(f.v3)    
    # tri_points = []
    # for p in tri_points_index:
    #     coord = convexHull.points[p]
    #     tri_points.append([coord.m_x, coord.m_y, coord.m_z])
    # print(np.array(tri_points))

    # tri_points = o3d.utility.Vector3dVector(np.array(tri_points))
    tri_vertices = []
    for v in convexHull.points:
        tri_vertices.append([v.m_x, v.m_y, v.m_z])
    tri_vertices = o3d.utility.Vector3dVector(np.array(tri_vertices).reshape(-1,3))

    vertices = o3d.utility.Vector3dVector(matrix)
    pcd = o3d.geometry.PointCloud()
    pcd.points = vertices
    
    tri_mesh = o3d.geometry.TriangleMesh(tri_vertices, tri_faces)
    
    hull_ls = o3d.geometry.LineSet.create_from_triangle_mesh(tri_mesh)
    hull_ls.paint_uniform_color((1, 0, 0))
    mesh_frame = o3d.geometry.TriangleMesh.create_coordinate_frame(
    size=1, origin=[0, 0, 0])
    o3d.visualization.draw_geometries([pcd, hull_ls])
