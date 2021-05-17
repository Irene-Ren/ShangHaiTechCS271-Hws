#define _USE_MATH_DEFINES

#include "mesh.h"
#include "matrix.h"
#include <cstring>
#include <iostream>
#include <strstream>
#include <fstream>
#include <cmath>
#include <float.h>
using namespace std;

/////////////////////////////////////////
// helping inline functions
inline double Cot(const Vector3d & p1, const Vector3d & p2, const Vector3d & p3) {
	Vector3d v1 = p1 - p2;
	Vector3d v2 = p3 - p2;

	v1 /= v1.L2Norm();
	v2 /= v2.L2Norm();
	double tmp = v1.Dot(v2);
	return 1.0 / tan(acos(tmp));
}

inline double Area(const Vector3d & p1, const Vector3d & p2, const Vector3d & p3) {
	Vector3d v1 = p2 - p1;
	Vector3d v2 = p3 - p1;
	return v1.Cross(v2).L2Norm() / 2.0;
}


/////////////////////////////////////////
// implementation of OneRingHEdge class
OneRingHEdge::OneRingHEdge(const Vertex * v) {
	if (v == NULL) start = next = NULL;
	else start = next = v->HalfEdge();
}

HEdge * OneRingHEdge::NextHEdge() {
	HEdge *ret = next;
	if (next && next->Prev()->Twin() != start)
		next = next->Prev()->Twin();
	else
		next = NULL;
	return ret;
}

/////////////////////////////////////////
// implementation of Mesh class
//
// function AddFace
// it's only for loading obj model, you do not need to understand it
void Mesh::AddFace(int v1, int v2, int v3) {
	int i;
	HEdge *he[3], *bhe[3];
	Vertex *v[3];
	Face *f;

	// obtain objects
	for (i=0; i<3; i++) he[i] = new HEdge();
	for (i=0; i<3; i++) bhe[i] = new HEdge(true);
	v[0] = vList[v1];
	v[1] = vList[v2];
	v[2] = vList[v3];
	f = new Face();

	// connect prev-next pointers
	SetPrevNext(he[0], he[1]);
	SetPrevNext(he[1], he[2]);
	SetPrevNext(he[2], he[0]);
	SetPrevNext(bhe[0], bhe[1]);
	SetPrevNext(bhe[1], bhe[2]);
	SetPrevNext(bhe[2], bhe[0]);

	// connect twin pointers
	SetTwin(he[0], bhe[0]);
	SetTwin(he[1], bhe[2]);
	SetTwin(he[2], bhe[1]);

	// connect start pointers for bhe
	bhe[0]->SetStart(v[1]);
	bhe[1]->SetStart(v[0]);
	bhe[2]->SetStart(v[2]);
	for (i=0; i<3; i++) he[i]->SetStart(v[i]);

	// connect start pointers
	// connect face-hedge pointers
	for (i=0; i<3; i++) {
		v[i]->SetHalfEdge(he[i]);
		v[i]->adjHEdges.push_back(he[i]);
		SetFace(f, he[i]);
	}
	v[0]->adjHEdges.push_back(bhe[1]);
	v[1]->adjHEdges.push_back(bhe[0]);
	v[2]->adjHEdges.push_back(bhe[2]);

	// mearge boundary if in need
	for (i=0; i<3; i++) {
		Vertex *start = bhe[i]->Start();
		Vertex *end   = bhe[i]->End();
		for (size_t j=0; j<end->adjHEdges.size(); j++) {
			HEdge *curr = end->adjHEdges[j];
			if (curr->IsBoundary() && curr->End()==start) {
				SetPrevNext(bhe[i]->Prev(), curr->Next());
				SetPrevNext(curr->Prev(), bhe[i]->Next());
				SetTwin(bhe[i]->Twin(), curr->Twin());
				bhe[i]->SetStart(NULL);	// mark as unused
				curr->SetStart(NULL);	// mark as unused
				break;
			}
		}
	}

	// finally add hedges and faces to list
	for (i=0; i<3; i++) heList.push_back(he[i]);
	for (i=0; i<3; i++) bheList.push_back(bhe[i]);
	fList.push_back(f);
}

// function LoadObjFile
// it's only for loading obj model, you do not need to understand it
bool Mesh::LoadObjFile(const char *filename) {
	if (filename==NULL || strlen(filename)==0) return false;
	ifstream ifs(filename);
	if (ifs.fail()) return false;

	Clear();

	char buf[1024], type[1024];
	do {
		buf[0] = type[0] = '\0'; // Flush the char array before reading each line.
		ifs.getline(buf, 1024);
		istrstream iss(buf);
		iss >> type;

		// vertex
		if (strcmp(type, "v") == 0) {
			double x, y, z;
			iss >> x >> y >> z;
            AddVertex(new Vertex(x,y,z));
		}
		// face
		else if (strcmp(type, "f") == 0) {
			int index[3];
			iss >> index[0] >> index[1] >> index[2];
			AddFace(index[0]-1, index[1]-1, index[2]-1);
		}
	} while (!ifs.eof());
	ifs.close();

	size_t i;
	Vector3d box = this->MaxCoord() - this->MinCoord();
	for (i=0; i<vList.size(); i++) vList[i]->SetPosition(vList[i]->Position() / box.X());

	Vector3d tot;
	for (i=0; i<vList.size(); i++) tot += vList[i]->Position();
	Vector3d avg = tot / vList.size();
	for (i=0; i<vList.size(); i++) vList[i]->SetPosition(vList[i]->Position() - avg);

	HEdgeList list;
	for (i=0; i<bheList.size(); i++)
		if (bheList[i]->Start()) list.push_back(bheList[i]);
	bheList = list;

	for (i=0; i<vList.size(); i++) 
	{
		vList[i]->adjHEdges.clear();
		vList[i]->SetIndex((int)i);
		vList[i]->SetFlag(0);
	}

	return true;
}

void Mesh::DisplayMeshInfo()
{
	/*************************/
	/* insert your code here */
	/*************************/
	int numVertices = Mesh::Vertices().size();
	int numHEdges = Mesh::Edges().size() + Mesh::BoundaryEdges().size();
	int numFaces = Mesh::Faces().size();
	int numBoundaryLoops = Mesh::CountBoundaryLoops();
	int numConnectedComponent = Mesh::CountConnectedComponents();
	int numGenus = numConnectedComponent - (numVertices - numHEdges / 2 + numFaces + numBoundaryLoops) / 2;
	std::cout << "The number of vertices in this mesh is: " << numVertices <<std::endl;
	std::cout << "The number of half edges in this mesh is: " << numHEdges << std::endl;
	std::cout << "The number of faces in this mesh is: " << numFaces << std::endl;
	std::cout << "The number of boundary loops in this mesh is: " << numBoundaryLoops << std::endl;
	std::cout << "The number of connected components in this mesh is: " << numConnectedComponent << std::endl;
	std::cout << "The number of genus in this mesh is: " << numGenus << std::endl;
}
HEdge* Mesh::BoundaryAllChecked()
{
	for (int i = 0; i < bheList.size(); i++)
	{
		if (!bheList[i]->Flag())
		{
			return bheList[i];
		}
	}
	return NULL;
}
int Mesh::CountBoundaryLoops()
{
	if (bheList.size() == 0)
	{
		return 0;
	}
	HEdge* startEdge = bheList[0];
	startEdge->SetFlag(true);
	int count = 0;
	
	while (startEdge != NULL)
	{	
		startEdge->SetFlag(true);
		HEdge* next = startEdge->Next();
		while (next != startEdge)
		{
			next->SetFlag(true);
			next = next->Next();
		}
		count++;
		startEdge = Mesh::BoundaryAllChecked();
	}
	Mesh::ClearFlags();
	return count;
}
int Mesh::CountConnectedComponents()
{
	if (vList.size() == 0)
	{
		return 0;
	}
	std::vector<Vertex*> startVertex = Mesh::VerticesAllChecked();
	int count = 0;

	while (startVertex.size() != 0)
	{
		BFSConnected(startVertex);
		count++;
		startVertex = Mesh::VerticesAllChecked();
	}
	Mesh::ClearFlags();
	return count;
}
std::vector<Vertex*> Mesh::VerticesAllChecked()
{
	std::vector<Vertex*> l;

	for (int i = 0; i < vList.size(); i++)
	{
		if (vList[i]->Flag() == 0)
		{
			l.push_back(vList[i]);
			return l;
		}
	}
	return l;
}
void Mesh::ClearFlags()
{
	for (int i = 0; i < vList.size(); i++)
	{
		vList[i]->SetFlag(0);
	}
	for (int i = 0; i < bheList.size(); i++)
	{
		bheList[i]->SetFlag(false);
	}
}
int IsInList(Vertex* v,std::vector<Vertex*> vertexList)
{
	for (int i = 0; i < vertexList.size(); i++)
	{
		if (vertexList[i] == v)
		{
			return i;
		}
	}
	return -1;
}
void BFSConnected(std::vector<Vertex*> vertexList)
{
	if (vertexList.size() == 0)
	{
		return;
	}
	else
	{
		std::vector<Vertex*> vertexList_i;
		for (int i = 0; i < vertexList.size(); i++)
		{
			vertexList[i]->SetFlag(1);
			OneRingVertex ring(vertexList[i]);
			Vertex *curr = NULL;
			while (curr = ring.NextVertex())
			{
				if (curr->Flag() == 0 && IsInList(curr,vertexList_i) == -1)
				{
					vertexList_i.push_back(curr);
				}
			}
		}
		BFSConnected(vertexList_i);
	}
}
// -------------------------------------------------------
// DO NOT TOUCH THE FOLLOWING FOR NOW
// -------------------------------------------------------
void Mesh::ComputeVertexNormals() 
{
	/*************************/
	/* insert your code here */
	/*************************/
	for (int i = 0; i < vList.size(); i++)
	{
		ComputeVertexNormalOne(vList[i]);
	}
}
void ComputeVertexNormalOne(Vertex* v)
{
	OneRingVertex ring(v);
	Vertex *curr = NULL;
	std::vector<Vertex*> valenceVertices;
	while (curr = ring.NextVertex())
	{
		valenceVertices.push_back(curr);
	}
	Vector3d t1, t2;
	int k = v->Valence();
	if (!v->IsBoundary())
	{
		for (int i = 0; i < k; i++)
		{
			t1 += cos(2 * M_PI * i / k) * valenceVertices[i]->Position();
			t2 += sin(2 * M_PI * i / k) * valenceVertices[i]->Position();
		}
	}
	else
	{
		t1 = valenceVertices[0]->Position() - valenceVertices[k - 1]->Position();
		if (valenceVertices.size() == 2)
		{
			t2 = valenceVertices[0]->Position() + valenceVertices[1]->Position() - 2 * v->Position();
		}
		else if (valenceVertices.size() == 3)
		{
			t2 = valenceVertices[1]->Position() - v->Position();
		}
		else
		{
			double theta = M_PI / (k - 1);
			Vector3d tmp;
			for (int i = 1; i < k - 1; i++)
			{
				tmp += sin(i*theta) * valenceVertices[i]->Position();
			}
			t2 = sin(theta)*(valenceVertices[0]->Position() + valenceVertices[k - 1]->Position())
				+ (2 * cos(theta) - 2) * tmp;
		}
	}
	Vector3d normal = t1.Cross(t2)/(t1.Cross(t2)).L2Norm();
	v->SetNormal(normal);
}

void Mesh::UmbrellaSmooth(bool is_mean)
{
	/*************************/
	/* insert your code here */
	/*************************/
	//construct matrix L
	double lambda = 0.2;

	int n = vList.size();
	double* inX = new double[n];
	double* inY = new double[n];
	double* inZ = new double[n];
	for (int i = 0; i < n; i++)
	{
		inX[i] = vList[i]->Position().X();
		inY[i] = vList[i]->Position().Y();
		inZ[i] = vList[i]->Position().Z();
	}

	Matrix L(n, n);

	for (int i = 0; i < n; i++)
	{

		L.AddElement(i, i, -lambda);//add element to L
		//compute 'w_sum': the sum of weight incident to the current vertex
		Vertex* v = vList[i];

		if (is_mean)
		{
			if (!v->IsBoundary())
			{
				int k = v->Valence();
				OneRingVertex vring(v);
				Vertex* vcurr = NULL;
				while (vcurr = vring.NextVertex())
				{
					L.AddElement(i, vcurr->Index(), lambda / k);
				}
			}
		}
		else
		{
			if (!v->IsBoundary())
			{
				OneRingHEdge ring(v);
				HEdge* curr = NULL;
				double w_sum = 0.0;//initialize the total weight of neighboring vertices of the current vertex
				double w_local = 0.0;//weight due to the current neighboring vertex

				vector<Vertex*> adj_vertices;
				vector<double> weights;
				while (curr = ring.NextHEdge())
				{
					if (!curr->IsBoundary())
					{
						adj_vertices.push_back(curr->Prev()->Start());
						const Vector3d& p1 = v->Position();
						const Vector3d& p2 = curr->End()->Position();
						const Vector3d& p3 = curr->Prev()->Start()->Position();
						const Vector3d& p4 = curr->Prev()->Twin()->Prev()->Start()->Position();
						w_local = Cot(p1, p2, p3) + Cot(p1, p4, p3);
						weights.push_back(w_local);
						w_sum += w_local;
					}
				}
				for (int s = 0; s < adj_vertices.size(); s++)
				{
					L.AddElement(i, adj_vertices[s]->Index(), lambda * weights[s] / w_sum);
				}
			}
		}
	}

	//sort matrix L
	L.SortMatrix();
	//std::cout << L << endl;

	double* outX = new double[n];
	double* outY = new double[n];
	double* outZ = new double[n];
	
	L.Multiply(inX, outX);
	L.Multiply(inY, outY);
	L.Multiply(inZ, outZ);

	for (int i = 0; i < n; i++)
	{
		outX[i] = outX[i] + inX[i];
		outY[i] = outY[i] + inY[i];
		outZ[i] = outZ[i] + inZ[i];
	}

	delete inX;
	delete inY;
	delete inZ;

	for (int i = 0; i < n; i++)
	{
		if (!vList[i]->IsBoundary())
		{
			double x = outX[i];
			double y = outY[i];
			double z = outZ[i];

			Vector3d new_pos(x, y, z);
			vList[i]->SetPosition(new_pos);
		}
	}

	delete outX;
	delete outY;
	delete outZ;

	cout << "Explicit Smoothing Finished " << endl;
}

void Mesh::ImplicitUmbrellaSmooth(bool is_mean)
{
	/*************************/
	/* insert your code here */
	/*************************/
	double lambda = 0.2;

	int n = vList.size();
	double* inX = new double[n];
	double* inY = new double[n];
	double* inZ = new double[n];
	double* outX = new double[n];
	double* outY = new double[n];
	double* outZ = new double[n];
	for (int i = 0; i < n; i++)
	{
		inX[i] = vList[i]->Position().X();
		outX[i] = vList[i]->Position().X();
		inY[i] = vList[i]->Position().Y();
		outY[i] = vList[i]->Position().Y();
		inZ[i] = vList[i]->Position().Z();
		outZ[i] = vList[i]->Position().Z();
	}
	Matrix L(n, n);

	for (int i = 0; i < n; i++)
	{

		L.AddElement(i, i, 1+lambda);//add element to L
		//compute 'w_sum': the sum of weight incident to the current vertex
		Vertex* v = vList[i];
		if (is_mean)
		{
			if (!v->IsBoundary())
			{
				int k = v->Valence();
				OneRingVertex vring(v);
				Vertex* vcurr = NULL;
				while (vcurr = vring.NextVertex())
				{
					L.AddElement(i, vcurr->Index(), 0-lambda / k);
				}
			}
		}
		else
		{
			if (!v->IsBoundary())
			{
				OneRingHEdge ring(v);
				HEdge* curr = NULL;
				double w_sum = 0.0;//initialize the total weight of neighboring vertices of the current vertex
				double w_local = 0.0;//weight due to the current neighboring vertex

				vector<Vertex*> adj_vertices;
				vector<double> weights;
				while (curr = ring.NextHEdge())
				{
					if (!curr->IsBoundary())
					{
						adj_vertices.push_back(curr->Prev()->Start());
						const Vector3d& p1 = v->Position();
						const Vector3d& p2 = curr->End()->Position();
						const Vector3d& p3 = curr->Prev()->Start()->Position();
						const Vector3d& p4 = curr->Prev()->Twin()->Prev()->Start()->Position();
						w_local = Cot(p1, p2, p3) + Cot(p1, p4, p3);
						weights.push_back(w_local);
						w_sum += w_local;
					}
				}
				for (int s = 0; s < adj_vertices.size(); s++)
				{
					L.AddElement(i, adj_vertices[s]->Index(), 0 - lambda * weights[s] / w_sum);
				}
			}
		}
	}

	//sort matrix L
	L.SortMatrix();
	/*A.Multiply(X, X_out);
	A.Multiply(Y, Y_out);
	A.Multiply(Z, Z_out);*/
	L.BCG(inX, outX, 10, 0.1);
	L.BCG(inY, outY, 10, 0.1);
	L.BCG(inZ, outZ, 10, 0.1);
	delete inX;
	delete inY;
	delete inZ;

	for (int i = 0; i < n; i++)
	{
		if (!vList[i]->IsBoundary())
		{
			double x = outX[i];
			double y = outY[i];
			double z = outZ[i];

			Vector3d new_pos(x, y, z);
			vList[i]->SetPosition(new_pos);
		}
	}

	delete outX;
	delete outY;
	delete outZ;
	cout << "Implicit Smoothing Finished " << endl;
}
void Mesh::ComputeVertexCurvatures()
{
	/*************************/
	/* insert your code here */
	/*************************/
	int n = vList.size();
	double max_curv = -1000000;
	double min_curv = 1000000;
	for (int i = 0; i < n; i++)
	{
		//compute 'w_sum': the sum of weight incident to the current vertex
		Vertex* v = vList[i];
		if (!v->IsBoundary())
		{
			OneRingHEdge ring(v);
			HEdge* curr = NULL;
			double area_sum = 0.0;//initialize the total weight of neighboring vertices of the current vertex
			double w_local = 0.0;//weight due to the current neighboring vertex
			Vector3d color(0.0, 0.0, 0.0);

			while (curr = ring.NextHEdge())
			{
				if (!curr->IsBoundary())
				{
					const Vector3d& p1 = v->Position();
					const Vector3d& p2 = curr->End()->Position();
					const Vector3d& p3 = curr->Prev()->Start()->Position();
					const Vector3d& p4 = curr->Prev()->Twin()->Prev()->Start()->Position();
					w_local = Cot(p1, p2, p3) + Cot(p1, p4, p3);
					color += w_local * (p2 - p1);
					area_sum += Area(p1, p2, p3);
				}
			}
			double H = -color.L2Norm() / (4 * area_sum);
			v->H = H;
			if (min_curv > H)
			{
				min_curv = H;
			}
			if (max_curv < H)
			{
				max_curv = H;
			}
		}
		
	}
	//"mean curvature" scheme
	for (int i=0; i< vList.size(); i++) 
	{ 
	 	if(vList[i]->IsBoundary())//process interior vertices
	 	{
			Vector3d color_mean(0.0, 0.0, 0.0);
			vList[i]->SetColor(color_mean);
	 		
	 	}
	 	else //process boundary vertices
	 	{
			Vector3d color_mean((10*vList[i]->H - min_curv) / (max_curv - min_curv), 0.5, 1 - (10*vList[i]->H - min_curv) / (max_curv - min_curv));
			vList[i]->SetColor(color_mean);
	 	}
	}
}

