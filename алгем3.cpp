#include <stdio.h>
#include <wchar.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <iostream>
#include <gdiplus.h>
using namespace Gdiplus; 
#pragma comment (lib, "Gdiplus.lib")
FILE *File1; 
FILE *File2; 
int param[3][4]; 

//List of all vertices
struct Vertex
{
	char Name; 
	int Coord[3]; 
	struct Vertex *Next; 
} *FirstVert = NULL, *LastVert = NULL; 
//This face`s list of vertices 
struct MyPoints 
{
	struct MyPoints *Next; 
	struct Vertex *Vertice; 
}; 
struct Edge
{
	struct Edge *Next; 
	struct Vertex *Name[2]; 
}; 
struct Face
{
	int Param[4]; 
	struct Face *Next; 
	int Sign; 
	struct MyPoints *FirstPoint, *LastPoint; 
	struct Edge *Edge; 
} *FirstFace = NULL, *LastFace = NULL; 

void printFace(struct Face *tempFace)
{
	int f = 0; 
	if (tempFace->Param[0] != 0)
	{
		f = 1; 
		if (tempFace->Param[0] != 1 && tempFace->Param[0] != -1) printf("%d", tempFace->Param[0]); 
		if (tempFace->Param[0] == -1) printf("-"); 
		printf("x"); 
	}
	if (tempFace->Param[1] != 0)
		if (!f) 
		{
			if (tempFace->Param[1] != 1 && tempFace->Param[1] != -1) printf("%d", tempFace->Param[1]); 
			if (tempFace->Param[1] == -1) printf("-"); 
			printf("y"); 
		}
		else
		{
			if (tempFace->Param[1] > 0)
			{
				printf("+"); 
				if (tempFace->Param[1] != 1) printf("%d", tempFace->Param[1]); 
			}
			else
			{
				if (tempFace->Param[1] != -1) printf("%d", tempFace->Param[1]); 
				if (tempFace->Param[1] == -1) printf("-"); 
			}
			printf("y"); 
			f = 1; 
		}
	if (tempFace->Param[2] != 0)
		if (!f)
		{
			if (tempFace->Param[2] != 1 && tempFace->Param[2] != -1) printf("%d", tempFace->Param[2]); 
			if (tempFace->Param[2] == -1) printf("-"); 
			printf("z"); 
		}
		else
		{
			if (tempFace->Param[2] > 0)
			{
				printf("+"); 
				if (tempFace->Param[2] != 1) printf("%d", tempFace->Param[2]); 
			}
			else
			{
				if (tempFace->Param[2] != -1) printf("%d", tempFace->Param[2]); 
				if (tempFace->Param[2] == -1) printf("-"); 
			}
			printf("z"); 
			f = 1; 
		}
	if (tempFace->Sign > 0) printf(" >= "); 
	else printf(" <= "); 
		if (tempFace->Param[3] <= 0) printf("%d", abs(tempFace->Param[3])); 
		else printf("%d", -(tempFace->Param[3])); 
	printf("\n"); 		
}

//returns 1 if success and 0 if not
//m, n - sizes of a matrix
int Gauss(int m, int n)
{
	int i, j, I, J, t, d; 

	for(J = 0; J < m; J++)
	{
		i = I = J; 
		while(param[i][J] == 0) i++; 
		if (i > m - 1) return 0; 
		if (I != i)
			for (j = 0; j < n; j++)
			{
				t = param[I][j]; 
				param[I][j] = param[i][j]; 
				param[i][j] = t; 
			}
		for (i = I + 1; i < m; i++)
		{
			if (param[i][J] % param[I][J] != 0)
			{
				for (j = 0; j < n; j++)
				param[i][j] *= abs(param[I][J]); 
			}
			if (param[i][J] != 0)
			{
				d = param[i][J] / param[I][J]; 
				for (j = 0; j < n; j++)
					param[i][j] = param[i][j] - param[I][j] * d; 

			}
		}
	}

	//Reverse part of Gauss
	for(J = m - 1; J >= 0; J--)
	{
		i = I = J; 
		while(param[i][J] == 0) i--; 
		if (I != i)
			for (j = n - 1; j >= 0; j--)
			{
				t = param[I][j]; 
				param[I][j] = param[i][j]; 
				param[i][j] = t; 
			}
		for (i = I - 1; i >= 0; i--)
		{
			if (param[i][J] % param[I][J] != 0)
			{
				for (j = n - 1; j >= 0; j--)
				param[i][j] *= abs(param[I][J]); 
			}
			if (param[i][J] != 0)
			{
				d = param[i][J] / param[I][J]; 
				for (j = n - 1; j >= 0; j--)
					param[i][j] = param[i][j] - param[I][j] * d; 
			}
		}
	}
	return 1; 
}

//paint a figure in a new windoow
void Paint()
{
	struct Face *tempFace; 
	struct Edge *tempEdge; 
	struct Vertex *tempVert;
	int x, y, i, j, x1, x2, y1, y2, k, flag; 
	HWND hwnd; 
    HDC hdc; 
	WCHAR string[30] = L""; 
	WCHAR str[5] = L""; 
	wchar_t s; 
	PointF *pointF; 

	//Open new cosole window
	FreeConsole(); 
    AllocConsole(); 

	hwnd = GetConsoleWindow(); 
	MoveWindow(hwnd, 10, 10, 1001, 663, TRUE); 
	hdc = GetDC(hwnd); 
	Graphics graphics(hdc); 

	system("color F0"); 
	graphics.SetPageUnit(UnitPixel); 
	FontFamily   fontFamily(L"Arial"); 
	Font         font(&fontFamily, 10, FontStyleBoldItalic, UnitPoint); 
	SolidBrush   solidBrush(Color(255, 0, 0, 0)); 

	//Position of zero
	x = 400;
	y = 350; 
	//Just coefficient
	i = 100; 

	//Pens
	Pen redPen(Color(100, 255, 0, 0), 2); 
	Pen bluePen(Color(150, 0, 0, 255), 2); 
	Pen blPen(Color(100, 0, 0, 255), 1); 

	//Axis z
	graphics.DrawLine(&bluePen, x, y, x, 0); 
	graphics.DrawLine(&blPen, x, y, x, 2 * y); 
	pointF = new PointF(x, 0); 
	graphics.DrawString(L"z", -1, &font, *pointF, NULL, &solidBrush); 
	delete pointF; 

	//Axis x
	graphics.DrawLine(&bluePen, x, y, 900, y); 
	graphics.DrawLine(&blPen, x, y, 2 * x - 900, y); 
	pointF = new PointF(900, y); 
	graphics.DrawString(L"x", -1, &font, *pointF, NULL, &solidBrush); 
	delete pointF; 

	//Axis y
	graphics.DrawLine(&bluePen, x, y, 150, 600); 
	graphics.DrawLine(&blPen, x, y, 2 * x - 150, 2 * y - 600); 
	pointF = new PointF(150, 600); 
	graphics.DrawString(L"y", -1, &font, *pointF, NULL, &solidBrush); 
	delete pointF; 

	//Draw all Edges
	for (tempFace = FirstFace; tempFace; tempFace = tempFace->Next)
		for (tempEdge = tempFace->Edge; tempEdge; tempEdge = tempEdge->Next)
		{
			x1 = x + int(i * (tempEdge->Name[0]->Coord[0] - 0.4 * tempEdge->Name[0]->Coord[1])); 
			y1 = y - int(i * (tempEdge->Name[0]->Coord[2] - 0.4 * tempEdge->Name[0]->Coord[1])); 
			x2 = x + int(i * (tempEdge->Name[1]->Coord[0] - 0.4 * tempEdge->Name[1]->Coord[1])); 
			y2 = y - int(i * (tempEdge->Name[1]->Coord[2] - 0.4 * tempEdge->Name[1]->Coord[1])); 
			graphics.DrawLine(&redPen, x1, y1, x2, y2);          	
		}

	//Mark all vertices
	for (tempVert = FirstVert; tempVert; tempVert = tempVert->Next)
	{
		s = wchar_t(tempVert->Name); 
		string[0] = s; 
		string[1] = wchar_t('('); 
		string[2] = '\0'; 
		for (j = 0; j < 3; j++)
		{
			if (tempVert->Coord[j] < 0)
			{
				string[wcslen(string) + 1] = '\0'; 
				string[wcslen(string)] = '-'; 
			}
			
			_itow(abs(tempVert->Coord[j]), str, 10); 
			wcscat(string, str); 
			if (j < 2) 
			{
				string[wcslen(string) + 1] = '\0'; 
				string[wcslen(string)] = wchar_t(','); 
			}
		}
		string[wcslen(string) + 1] = '\0'; 
		string[wcslen(string)] = wchar_t(')'); 

		x2 = x + int(i * (tempVert->Coord[0] - 0.4 * tempVert->Coord[1])); 
		y2 = y - int(i * (tempVert->Coord[2] - 0.4 * tempVert->Coord[1])); 
		pointF = new PointF(x2, y2); 
		graphics.DrawString(string, -1, &font, *pointF, NULL, &solidBrush); 
		delete pointF; 
	}

    ReleaseDC(hwnd,  hdc); 
    std::cin.ignore(); 
}

//find common edges of faces
//print info about figure
//New edges appear
void Skeleton (int n)
{
	struct Face *Face1, *Face2; 
	struct MyPoints *Points1, *Points2; 
	struct Edge *Edge1, *Edge2, *tempEdge; 
	struct Vertex *iVert, *jVert; 
	int i, j; 
	int **A = new int*[n]; 
    for (int Count = 0; Count < n; Count++)
        A[Count] = new int[n]; 

	for (i = 0; i < n; i++)
		for (j = 0; j < n; j++)
			A[i][j] = 0; 

	//Enumerate all pairs of edges
	for (Face1 = FirstFace; Face1; Face1 = Face1->Next)
		for (Face2 = Face1->Next; Face2; Face2 = Face2->Next)
		{
			iVert = NULL; 
			jVert = NULL; 

			//Enumerate all pairs of vertices and find the same
			for (Points1 = Face1->FirstPoint; Points1 && jVert == NULL; Points1 = Points1->Next)
			{
				for (Points2 = Face2->FirstPoint; Points2  && jVert == NULL; Points2 = Points2->Next)
				{
					if (Points1->Vertice == Points2->Vertice)
					{
						if (iVert == NULL) iVert = Points1->Vertice; 
						else jVert = Points1->Vertice; 
					}
				}
			}

			//If we have 2 the same vertices, we add this edge to the list
			if (iVert && jVert)
			{
				//26 - count of letters in alfabeth
				i = (iVert->Name - 65) % 26; 
				j = (jVert->Name - 65) % 26; 
				
				//Add the edge to the lists of both faces
				if (A[i][j] != 1)
				{
					Edge1 = new struct Edge; 
					Edge1->Name[0] = iVert; 
					Edge1->Name[1] = jVert; 
					Edge1->Next = Face1->Edge; 
					Face1->Edge = Edge1; 

					Edge2 = new struct Edge; 
					Edge2->Name[0] = iVert; 
					Edge2->Name[1] = jVert; 
					Edge2->Next = Face2->Edge; 
					Face2->Edge = Edge2; 
				}

				A[i][j] = 1; 
				A[j][i] = 1; 
			}
		}
	
	//Print the adjancency matrix
	printf("Adjacency matrix\n"); 
	printf("  "); 
	for (i = 0; i < n; i++)
		printf("%c ", i+65); 
	printf("\n"); 
	for (i = 0; i < n; i++)
	{
		printf("%c ", i+65); 
		for (j = 0; j < n; j++)
			printf("%d ", A[i][j]); 
		printf("\n"); 
	}
	printf("\n"); 

	//Print information about faces
	for (Face1 = FirstFace; Face1; Face1 = Face1->Next)
	{
		printf("Face: "); 
		printFace(Face1); 
		printf("Vertices: "); 
		for (Points1 = Face1->FirstPoint; Points1; Points1 = Points1->Next)
		{
			if (Points1 != Face1->FirstPoint) printf(", ");  
			if (Points1->Vertice->Name > 'Z') 
				printf("%c%d", Points1->Vertice->Name % 'Z' + 'A' - 1, Points1->Vertice->Name / 'Z'); 
			else printf("%c", Points1->Vertice->Name); 
		}
		printf("\n"); 
		printf("Edges: "); 
		for (tempEdge = Face1->Edge; tempEdge; tempEdge = tempEdge->Next)
		{
			if (tempEdge != Face1->Edge) printf(", "); 
			printf("%c%c", tempEdge->Name[0]->Name, tempEdge->Name[1]->Name); 
		}
		printf("\n\n"); 
	}

	for (int count = 0; count < n; count++)
        delete []A[count]; 

	printf("Press any key to paint a figure.\n"); 
	_getch(); 
	Paint(); 
}

//Find vertices and print info about them
//New vertices appear
void Vertexenum()
{
	int i, j, flag = 0, f = 1; 
	struct Face *tempFace, *Face1, *Face2, *f3; 
	struct Vertex *temp, *t; 
	struct MyPoints *MyPoints; 
	char c = 'A'; 

	//Enumerate all trinity of faces
	for (Face1 = FirstFace; Face1->Next->Next; Face1 = Face1->Next)
		for (Face2 = Face1->Next; Face2->Next; Face2 = Face2->Next)
			for (f3 = Face2->Next; f3; f3 = f3->Next)
			{
				for (i = 0; i < 4; i++)
					param[0][i] = Face1->Param[i]; 
				param[0][3] *= -1; 
				for (i = 0; i < 4; i++)
					param[1][i] = Face2->Param[i]; 
				param[1][3] *= -1; 
				for (i = 0; i < 4; i++)
					param[2][i] = f3->Param[i]; 
				param[2][3] *= -1; 
				flag = 0; 

				//Find the vertices (intersections of 3 faces) with Gauss`s method
				if (Gauss(3, 4))
				{
					temp = new struct Vertex; 
				
					//Memorize coordinates of vertex
					for (i = 0; i < 3; i++)
					{
						if (param[i][i] == 0) flag = 1; 
						else temp->Coord[i] = param[i][3] / param[i][i]; 
					}

					//Check if this vertex lies on the surface
					for (tempFace = FirstFace; tempFace && !flag; tempFace = tempFace->Next)
					{
						j = 0; 
						for (i = 0; i < 3; i++)
							j += (tempFace->Param[i]) * (temp->Coord[i]); 
						if (j + tempFace->Param[3] > 0) flag = 1; 
					}
					
					//Check if we already have this vertex
					for (t = FirstVert; t && !flag; t = t->Next)
					{
						f = 1; 
						for (j = 0; j < 3 && f; j++)
							if (temp->Coord[j] != t->Coord[j]) f = 0; 
						if (f && temp)
						{
							delete temp; 
							flag = 1; 
						}
					}

					//Add the vertex to the list
					if (!flag)
					{
						if (!FirstVert)
						{
							FirstVert = temp; 
							LastVert = temp; 
						}
						else
						{
							LastVert->Next = temp; 
							LastVert = temp; 
						}
						temp->Next = NULL; 
						temp->Name = c; 
						c++; 	
					}
				}
			}

	//Check all vertices and edges, if the vertex lie on face, we add it to the list
	for (tempFace = FirstFace; tempFace; tempFace = tempFace->Next)
		for (temp = FirstVert; temp; temp = temp->Next)
		{
			j = 0; 
			for (i = 0; i < 3; i++)
				j += (tempFace->Param[i]) * (temp->Coord[i]); 
			if (j + tempFace->Param[3] == 0)
			{
				MyPoints = new struct MyPoints; 
				MyPoints->Vertice = temp; 
				MyPoints->Next = NULL; 
				if (!tempFace->FirstPoint)
				{
					tempFace->FirstPoint = MyPoints; 
					tempFace->LastPoint = MyPoints; 
				}
				else
				{
					tempFace->LastPoint->Next = MyPoints; 
					tempFace->LastPoint = MyPoints; 
				}
			}
		}

	//Print count of vertices and their coordinates
	for (t = FirstVert, i = 0; t; t = t->Next, i++); 
	printf("\nNumber of vertices: %d\n", i); 
	for (t = FirstVert; t; t = t->Next)
	{
		if (c > 'Z') printf("%c%d:", t->Name % 'Z' + 'A' - 1, c / 'Z'); 
		else printf("%c:", t->Name); 
		for (j = 0; j < 3; j++)
			printf("%d ", t->Coord[j]); 
		printf("\n"); 
	}
	printf("\n"); 

	Skeleton(i); 
}

//New faces appear
void ReadFace(FILE *File)
{
	int Count, i, j; 
	struct Face *tempFace; 

	//Memorize all given faces
	fscanf(File, "%d", &Count); 
	for (i = 0; i < Count; i++)
	{
		tempFace = new struct Face; 
		if (!FirstFace)
		{
			FirstFace = tempFace; 
			LastFace = tempFace; 
		}
		else
		{
			LastFace->Next = tempFace; 
			LastFace = tempFace; 
		}
		tempFace->Next = NULL; 
		
		for (j = 0; j < 4; j++)
			fscanf(File, "%d", &tempFace->Param[j]); 
		tempFace->Param[3] *= -1; 
		tempFace->FirstPoint = NULL; 
		tempFace->LastPoint = NULL; 
		tempFace->Edge = NULL; 
		tempFace->Sign = -1; 
	}
	for (tempFace = FirstFace; tempFace; tempFace = tempFace->Next)
		printFace(tempFace); 
	printf("\n"); 

	 Vertexenum(); 
}

//New faces and their vertices appear
//returns 1 if the plane is face and 0 if not
int MakeFace(struct Vertex *Vert1, struct Vertex *Vert2, struct Vertex *Vert3)
{
	int A, B, C, D, x1, x2, x3, y1, y2, y3, z1, z2, z3, len, flag = 0, i, t; 
	struct Vertex *tempVert; 
	struct Face *tempFace; 

	x1 = Vert1->Coord[0]; 
	x2 = Vert2->Coord[0]; 
	x3 = Vert3->Coord[0];
	y1 = Vert1->Coord[1]; 
	y2 = Vert2->Coord[1]; 
	y3 = Vert3->Coord[1]; 
	z1 = Vert1->Coord[2]; 
	z2 = Vert2->Coord[2]; 
	z3 = Vert3->Coord[2]; 
	A = (y2 - y1) * (z3 - z1) - (z2 - z1) * (y3 - y1); 
	B = (z2 - z1) * (x3 - x1) - (x2 - x1) * (z3 - z1); 
	C = (x2 - x1) * (y3 - y1) - (y2 - y1) * (x3 - x1); 
	D = -(x1 * A) - (y1 * B) - (z1 * C); 
	if (A == 0 && B == 0 && C == 0) return 0; 

	//Reduce parameters of face
	if (A != 0) t = A; 
	else if (B != 0) t = B; 
			else t = C; 

	for (i = 2; i <= abs(t); i++)
	{
		if (A % i == 0 && B % i == 0 && C % i == 0 && D % i == 0)
		{
			A = A / i; 
			B = B / i; 
			C = C / i; 
			D = D / i; 
			i--; 
		}
	}

	if (t < 0)
	{
		A = -A; 
		B = -B; 
		C = -C; 
		D = -D; 
		flag = -flag; 
	}

	//Check if we have this face
	for (tempFace = FirstFace; tempFace; tempFace = tempFace->Next)
	{
		if (tempFace->Param[0] == A && tempFace->Param[1] == B && tempFace->Param[2] == C && tempFace->Param[3] == D)
			return 0; 
	}

	tempFace = new struct Face; 
	tempFace->FirstPoint = NULL; 
	tempFace->LastPoint = NULL; 

	//Check if plane is the face
	for (tempVert = FirstVert; tempVert; tempVert = tempVert->Next)
	{
		len = A * tempVert->Coord[0] + B * tempVert->Coord[1] + C * tempVert->Coord[2] + D; 
		if (len == 0) 
		{
			continue; 
		}
		if (flag == 0 && len > 0)
		{
			flag = 1; 
			continue; 
		}
		if (flag == 0 && len < 0)
		{
			flag = -1; 
			continue; 
		} 
		if (len * flag > 0) continue; 
		else 
		{
			delete tempFace; 
			return 0; 
		}
	}
	tempFace->Param[0] = A; 
	tempFace->Param[1] = B; 
	tempFace->Param[2] = C; 
	tempFace->Param[3] = D; 
	tempFace->Sign = flag; 
	if (!FirstFace)
	{
		FirstFace = tempFace; 
		LastFace = tempFace; 
	}
	else
	{
		LastFace->Next = tempFace; 
		LastFace = tempFace; 
	}
	tempFace->Next = NULL; 
	tempFace->Edge = NULL; 

	return 1; 
}

//Calculate parameters of polyhedron with the given vertices
//New vertices appear
void ConvexHull(FILE *File)
{
	int Count, i, j, f = 0, flag; 
	struct Vertex *tempVert, *Vert1, *Vert2, *Vert3; 
	struct Face *tempFace; 
	//Count - count of vertices
	fscanf(File, "%d", &Count); 

	//Add all vertices to the list
	for (i = 0; i < Count; i++)
	{
		tempVert = new struct Vertex; 
		if (!FirstVert)
		{
			FirstVert = tempVert; 
			LastVert = tempVert; 
			tempVert->Name = 'A'; 
		}
		else
		{
			tempVert->Name = LastVert->Name + 1; 
			LastVert->Next = tempVert; 
			LastVert = tempVert; 
		}
		tempVert->Next = NULL; 
		for (j = 0; j < 3; j++)
		{
			fscanf(File, "%d", &tempVert->Coord[j]); 
			printf("%d ", tempVert->Coord[j]); 
		}
		printf("\n"); 
	}
	for (Vert1 = FirstVert; Vert1->Next->Next; Vert1 = Vert1->Next)
		for (Vert2 = Vert1->Next; Vert2->Next; Vert2 = Vert2->Next)
			for (Vert3 = Vert2->Next; Vert3; Vert3 = Vert3->Next)
			flag = MakeFace(Vert1, Vert2, Vert3); 

	//Print equations of faces
	printf("\n"); 
	for(tempFace = FirstFace, i = 0; tempFace; tempFace = tempFace->Next, i++); 
	printf("Number of faces: %d\n", i); 
	for(tempFace = FirstFace; tempFace; tempFace = tempFace->Next)
		printFace(tempFace); 

	//Clean the list of vertices
	for (Vert1 = FirstVert->Next; Vert1->Next; )
	{
		Vert2 = Vert1; 
		Vert1 = Vert1->Next; 
		delete Vert2; 
	}
	delete Vert1; 
	FirstVert = NULL; 

	//Make all sings the same (change sings in parameteres)
	for(tempFace = FirstFace; tempFace; tempFace = tempFace->Next)
		if (tempFace->Sign == 1)
		{
			for (i = 0; i < 4; i++)
				tempFace->Param[i] *= -1; 
			tempFace->Sign = -1; 
		}

	//Find Vertices
	Vertexenum(); 
}

//Check if 2 figures cross
int CollDet (FILE *File1,  FILE *File2)
{
	struct Vertex *Face1 = NULL, *Face2 = NULL, *Vert1, *Vert2, *Vert3, *temp; 
	struct Face *tempFace; 
	int Count, i, j; 

	//Add all vertices to the list
	printf("First polyhedron:\n"); 
	fscanf(File1, "%d", &Count); 
	for (i = 0; i < Count; i++)
	{
		temp = new struct Vertex; 
		temp->Next = Face1; 
		Face1 = temp; 
		for (j = 0; j < 3; j++)
		{
			fscanf(File1, "%d", &temp->Coord[j]); 
			printf("%d ", temp->Coord[j]); 
		}
		printf("\n"); 
	}
	
	printf("\nSecond polyhedron:\n"); 
	fscanf(File2, "%d", &Count); 
	for (i = 0; i < Count; i++)
	{
		temp = new struct Vertex; 
		temp->Next = Face2; 
		Face2 = temp; 
		for (j = 0; j < 3; j++)
		{
			fscanf(File2, "%d", &temp->Coord[j]); 
			printf("%d ", temp->Coord[j]); 
		}
		printf("\n"); 
	}

	//Enumerate all pairs of vertices of both polyhedrons. Make a Minkovskiy`s difference.
	for (Vert1 = Face1; Vert1; Vert1 = Vert1->Next)
		for (Vert2 = Face2; Vert2; Vert2 = Vert2->Next)
		{
			temp = new struct Vertex; 
			for (j = 0; j < 3; j++)
				temp->Coord[j] = Vert1->Coord[j]-Vert2->Coord[j]; 
			temp->Next = FirstVert; 
			FirstVert = temp; 
		}
	
	//Clean memory from the vertices
	for (Vert1 = Face1; Vert1; )
	{
		temp = Vert1; 
		Vert1 = Vert1->Next; 
		delete temp; 
	}
	delete Vert1; 
	for (Vert1 = Face2; Vert1; )
	{
		temp = Vert1; 
		Vert1 = Vert1->Next; 
		delete temp; 
	}
	delete Vert1; 

	//Make a convex hull
	for (Vert1 = FirstVert; Vert1->Next->Next; Vert1 = Vert1->Next)
		for (Vert2 = Vert1->Next; Vert2->Next; Vert2 = Vert2->Next)
			for (Vert3 = Vert2->Next; Vert3; Vert3 = Vert3->Next)
				MakeFace(Vert1, Vert2, Vert3); 
	
	//Check if 0 lie on hull
	for (tempFace = FirstFace; tempFace; tempFace = tempFace->Next)
		if (tempFace->Sign*tempFace->Param[3] < 0) 
		{
			printf("\nNo collision.\n"); 
			return 0; 
		}
		printf("\nCollision is detected.\n"); 
	return 1; 
}

void main(int argc,  char *argv[])
{
	char formate; 
	struct Face *tempFace, *Face1; 
	struct Vertex *tempVert, *Vert1; 
	struct MyPoints *tempPoints, *Points1; 
	struct Edge *tempEdge, *Edge1; 

	setlocale(LC_ALL,  "RUS"); 
	GdiplusStartupInput gdiplusStartupInput; 
    ULONG_PTR gdiplusToken; 
    GdiplusStartup(&gdiplusToken,  &gdiplusStartupInput,  NULL); 

	//Check the count of arguments
	if(!(File1 = fopen(argv[1], "rt")))
	{
		printf("Ошибка открытия файла.\n"); 
		return; 
	}
	if (argc > 2)
	{
		if(!(File2 = fopen(argv[2], "rt")))
		{
			printf("Ошибка открытия файла.\n"); 
			return; 
		}
		CollDet(File1,  File2); 
	}
	fscanf(File1, "%c", &formate); 
	if (formate == 'V') ConvexHull(File1); 
	if (formate == 'H') ReadFace(File1); 

	//Clear all memory
	for (tempFace = FirstFace; tempFace; )
	{
		Face1 = tempFace; 
		tempFace = tempFace->Next; 

		for (tempPoints = Face1->FirstPoint; tempPoints; )
		{
			Points1 = tempPoints; 
			tempPoints = tempPoints->Next; 
			delete Points1; 
		}
		delete tempPoints;

		for (tempEdge = Face1->Edge; tempEdge; )
		{
			Edge1 = tempEdge; 
			tempEdge = tempEdge->Next; 
			delete Edge1; 
		}

		delete Face1; 
	}
	delete tempFace;

	for (tempVert = FirstVert; tempVert; )
	{
		Vert1 = tempVert; 
		tempVert = tempVert->Next; 
		delete Vert1; 
	}
	delete tempVert; 

	fclose(File1); 
	if (argc > 2) fclose(File2); 
	GdiplusShutdown; 
}