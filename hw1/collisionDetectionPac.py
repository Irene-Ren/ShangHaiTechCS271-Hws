import numpy as np
import open3d as o3d
from shapely.geometry import Polygon
MAX_N = 2000
ERROR = 0
class Point3D:
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
    def __init__(self, x, y, z, h):
        self.is_hull = h
        self.v1 = x
        self.v2 = y
        self.v3 = z
class Convex3D:
    def __init__(self):
        self.points = []
        self.triangleF = []
    # DirectedVolume(Point3D p, Face f)
    def DirectedVolume(self, p, f):
        vector1 = Point3D.Minus(self.points[f.v2], self.points[f.v1])
        vector2 = Point3D.Minus(self.points[f.v3], self.points[f.v1])
        vector3 = Point3D.Minus(p, self.points[f.v1])
        return Point3D.DotProduct(Point3D.CrossProduct(vector1, vector2), vector3)
    def CreateOriginTetrahedron(self):
        if len(self.points) < 4:
            print("Not enough points for 3D convexhull")
            return None
        success = False
        for i in range(1, len(self.points)):
            if Dist(self.points[0], self.points[i]) > ERROR:
                self.points[1],self.points[i] = self.points[i], self.points[1]
                success = True
                print("Find two points")
                break
        if not success:
            print("Same vertex for all in list")
            return None
        success = False
        for i in range(2, len(self.points)):
            if Area(self.points[0],self.points[1],self.points[i]) > ERROR:
                self.points[2], self.points[i] = self.points[i], self.points[2]
                success = True
                print("Find three points")
                break
        if not success:
            print("Same line for all in list")
            return None
        for i in range(3, len(self.points)):
            if Volume(self.points[0], self.points[1], self.points[2], self.points[i]) > ERROR:
                self.points[3], self.points[i] = self.points[i], self.points[3]
                success = True
                print("Find Four Points")
                break
        if not success:
            print("Same plane for all in list")
            return None
        else:
            # originConv = Convex3D()
            for i in range(4):
                face_tmp = Face((i+1)%4,(i+2)%4,(i+3)%4,True)
                if self.DirectedVolume(self.points[i], face_tmp) > 0:
                    face_tmp.v2, face_tmp.v3 = face_tmp.v3, face_tmp.v2
                self.triangleF.append(face_tmp)
            return self
    def CleanUp(self, insideFaces):
        insideF = []
        outsideF = []
        for face in insideFaces:
            insideF.append((face.v1, face.v2))
            insideF.append((face.v2, face.v3))
            insideF.append((face.v3, face.v1))

            outsideF.append((face.v2, face.v1))
            outsideF.append((face.v3, face.v2))
            outsideF.append((face.v1, face.v3))
        insideF = set(insideF)
        outsideF = set(outsideF)
        return insideF - outsideF
        
    def AddPointP(self, hull, p):
        visibleF = []
        for face in hull.triangleF:
            if self.DirectedVolume(self.points[p], face) > ERROR:
                visibleF.append(face)
        for face in visibleF:
            hull.triangleF.remove(face)
        for edge in self.CleanUp(visibleF):
            newFace = Face(edge[0], edge[1], p, True)
            hull.triangleF.append(newFace)
    def ExtendConvexHull(self):
        hull = self.CreateOriginTetrahedron()
        if hull is not None:
            for i in range(4, len(self.points)):
                self.AddPointP(hull, i)
        return hull
    def ExtractHullPoints(self):
        vertSet = []
        for f in self.triangleF:
            if self.points[f.v1] not in vertSet:
                vertSet.append(self.points[f.v1])
            if self.points[f.v2] not in vertSet:
                vertSet.append(self.points[f.v2])
            if self.points[f.v3] not in vertSet:
                vertSet.append(self.points[f.v3])
        print(len(vertSet))
        return vertSet

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
def TotalVolume(hull):
    oriP = Point3D(0,0,0)
    points = hull.points
    ret = 0
    for f in hull.triangleF:
        ret += Volume(oriP, points[f.v1], points[f.v2], points[f.v3])
    return ret
def IsInside(points, p, f):
    vector1 = Point3D.Minus(points[f.v2], points[f.v1])
    vector2 = Point3D.Minus(points[f.v3], points[f.v1])
    vector3 = Point3D.Minus(p, points[f.v1])
    return Point3D.DotProduct(Point3D.CrossProduct(vector1, vector2), vector3) < ERROR
def CollisionDetection(hull_1, hull_2):
    # combineHull = Convex3D()
    # combineHull.points = hull_1.points + hull_2.points
    # combineHull.ExtendConvexHull()
    # v_total = TotalVolume(combineHull)
    # v1 = TotalVolume(hull_1)
    # v2 = TotalVolume(hull_2)
    # print(v_total, v1,v2)
    # if v_total < v1 + v2:
    #     print("Collide")
    # else:
    #     print("No collision")    
    ptSet1 = hull_1.ExtractHullPoints()
    ptSet2 = hull_2.ExtractHullPoints()
    pts1 = []
    pts2 = []
    for i in ptSet1:
        pts1.append((i.m_x, i.m_y, i.m_z))
    for i in ptSet2:
        pts2.append((i.m_x, i.m_y, i.m_z))

    p1 = Polygon(pts1)
    p2 = Polygon(pts2)
    print(p1.intersects(p2))
if __name__ == "__main__":
    numModels = 2
    Hulls = []
    for i in range(numModels):
        # ptcloud = o3d.geometry.PointCloud()
        # ptcloud.points = o3d.utility.Vector3dVector(np.random.randn(10,3))
        # matrix = np.asarray(ptcloud.points, dtype = 'double')

        file_name = raw_input() or "test"
        matrix = np.loadtxt(file_name, dtype = 'float')
        ptcloud = o3d.geometry.PointCloud()
        ptcloud.points = o3d.utility.Vector3dVector(matrix)

        convexHull = Convex3D()
        for i in range(np.size(matrix, axis = 0)):
            convexHull.points.append(Point3D(matrix[i][0],matrix[i][1],matrix[i][2]))
        print(len(convexHull.points))
        convexHull.ExtendConvexHull()
        hullShellPoints = convexHull.ExtractHullPoints()

        Hulls.append(convexHull)

    CollisionDetection(Hulls[0], Hulls[1])

    HullsGFX = []
    for i in range(numModels):
        
        tri_faces = []
        for f in Hulls[i].triangleF:
            tri_faces.append([f.v1, f.v2, f.v3])
        tri_faces = o3d.utility.Vector3iVector(np.array(tri_faces).reshape(-1,3))

        tri_vertices = []
        for v in Hulls[i].points:
            tri_vertices.append([v.m_x, v.m_y, v.m_z])
        tri_vertices = o3d.utility.Vector3dVector(np.array(tri_vertices).reshape(-1,3))

        pcd = o3d.geometry.PointCloud()
        pcd.points =o3d.utility.Vector3dVector(np.array(tri_vertices).reshape(-1,3))

        tri_mesh = o3d.geometry.TriangleMesh(tri_vertices, tri_faces)
        
        hull_ls = o3d.geometry.LineSet.create_from_triangle_mesh(tri_mesh)
        hull_ls.paint_uniform_color((1, 0, 0))

        # HullsGFX.append(tri_vertices)
        HullsGFX.append(hull_ls)
        HullsGFX.append(pcd)
    o3d.visualization.draw_geometries(HullsGFX)
