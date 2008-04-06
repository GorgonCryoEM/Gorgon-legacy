#ifndef GORGON_MARCHING_CUBES_H
#define GORGON_MARCHING_CUBES_H

#include <iostream>
#include <tchar.h>
#include <GL/glut.h>
#include <string>
#include <GraphMatch/VectorMath.h>
#include "Renderer.h"
//#include "VolumeMesh.h"
#include "GlobalConstants.h"
#include <SkeletonMaker/volume.h>
#include <GraySkeletonCPP/VolumeFormatConverter.h>
#include <ProteinMorph/NonManifoldMesh.h>
#include <MathTools/Vector3D.h>

using namespace std;
using namespace wustl_mm::GraySkeletonCPP;
using namespace wustl_mm::Protein_Morph;
using namespace wustl_mm::MathTools;
using namespace wustl_mm::GraphMatch;

namespace wustl_mm {
	namespace Visualization {

		class VolumeRenderer : public Renderer {
		public:
			VolumeRenderer();
			~VolumeRenderer();

			float GetMaxDensity();
			float GetMinDensity();
			float GetSurfaceValue() const ;
			int GetSampleInterval() const ;
			string GetSupportedFileFormats();
			void LoadFile(string fileName);
			void Unload();
			void SetSurfaceValue(const float value);
			void SetSampleInterval(const int size);
			void Draw() const;
			void UpdateBoundingBox() ;
		
		private:
			int GetHashKey(int x, int y, int z, int edge, int iScale);
			float GetVoxelData(int x, int y, int z);
			float GetOffset(float fValue1, float fValue2, float fValueDesired);
			void CalculateSurface();
			void MarchingCube(int iX, int iY, int iZ, int iScale);

		private:
			NonManifoldMesh * _mesh;
			float _surf_value;
			int _sample;
			Volume * _voxel;
		};

		VolumeRenderer::VolumeRenderer() {
			_mesh = new NonManifoldMesh();
			_voxel = NULL;
			_surf_value = 1.5;
			_sample = 1;
		}
		VolumeRenderer::~VolumeRenderer() {
			delete _mesh;
			if(_voxel != NULL) {
				delete _voxel;
			}
		}

		float VolumeRenderer::GetSurfaceValue() const { 
			return _surf_value; 
		}

		float VolumeRenderer::GetOffset(float fValue1, float fValue2, float fValueDesired)
		{
				double fDelta = fValue2 - fValue1;

				if(fDelta == 0.0)
				{
						return 0.5;
				}
				return (fValueDesired - fValue1)/fDelta;
		}

		float VolumeRenderer::GetVoxelData(int x, int y, int z) {
			if((x < 0) || (x > _voxel->getSizeX()-1) || (y < 0) || (y > _voxel->getSizeY()-1) || (z < 0) || (z > _voxel->getSizeZ()-1)) {
				return 0.0f;
			} else {
				return _voxel->getDataAt(x, y, z);
			}
		}

		float VolumeRenderer::GetMinDensity() {
			return _voxel->getMin();
		}

		float VolumeRenderer::GetMaxDensity(){
			return _voxel->getMax();
		}

		int VolumeRenderer::GetHashKey(int x, int y, int z, int edge, int iScale) {

			x += a2iEdgeHash[edge][1]*iScale;
			y += a2iEdgeHash[edge][2]*iScale;
			z += a2iEdgeHash[edge][3]*iScale;

			edge = a2iEdgeHash[edge][0];
			return x * _voxel->getSizeY() * _voxel->getSizeZ() * 3 + y * _voxel->getSizeZ() * 3 + z * 3 + edge;
		}

		int VolumeRenderer::GetSampleInterval() const  { 
			return _sample; 
		}

		string VolumeRenderer::GetSupportedFileFormats() {
			return "Volumes (*.mrc)";
		}

		void VolumeRenderer::Draw() const {
			int vertices[40];
			int lastVertex;
			int k;

			for(unsigned int i = 0; i < _mesh->faces.size(); i++) {
				lastVertex = -1;

				k = 0;
				for(int j = _mesh->faces[i].edgeIds.size()-1; j >= 0; j--) {
					if((_mesh->edges[_mesh->faces[i].edgeIds[j]].vertexIds[0] == _mesh->edges[_mesh->faces[i].edgeIds[(j+1)%_mesh->faces[i].edgeIds.size()]].vertexIds[0]) || 
						(_mesh->edges[_mesh->faces[i].edgeIds[j]].vertexIds[0] == _mesh->edges[_mesh->faces[i].edgeIds[(j+1)%_mesh->faces[i].edgeIds.size()]].vertexIds[1])) {
						lastVertex = _mesh->edges[_mesh->faces[i].edgeIds[j]].vertexIds[1];						
					} else {
						lastVertex = _mesh->edges[_mesh->faces[i].edgeIds[j]].vertexIds[0];
					}
					vertices[k] = lastVertex;
					k++;						
				}

				glBegin(GL_POLYGON);
				Vector3DFloat normal;
				for(int j = 0; j < k; j++) {
					normal = _mesh->GetVertexNormal(vertices[j]);

					glNormal3f(normal.X(), normal.Y(), normal.Z());
					glVertex3f(_mesh->vertices[vertices[j]].position.X(), _mesh->vertices[vertices[j]].position.Y(), _mesh->vertices[vertices[j]].position.Z());
				}
				glEnd();
			}

			glFlush();

		}

		void VolumeRenderer::SetSurfaceValue(const float value) {
			_surf_value = value;
			CalculateSurface();
		}

		void VolumeRenderer::SetSampleInterval(const int size) {
			_sample = size;
			CalculateSurface();
		}

		void VolumeRenderer::CalculateSurface() {
			_mesh->Clear();
			if(_voxel != NULL) {
				int iX, iY, iZ;
				for(iX = 0; iX < _voxel->getSizeX(); iX+=_sample) {
					for(iY = 0; iY < _voxel->getSizeY(); iY+=_sample) {
						for(iZ = 0; iZ < _voxel->getSizeZ(); iZ+=_sample) {
								MarchingCube(iX, iY, iZ, _sample);
						}
					}
				}
			}
		}



		void VolumeRenderer::MarchingCube(int iX, int iY, int iZ, int iScale){
			extern int aiCubeEdgeFlags[256];
			extern int a2iTriangleConnectionTable[256][16];

			int iCorner, iVertex, iVertexTest, iEdge, iTriangle, iFlagIndex, iEdgeFlags;
			float fOffset;
			float afCubeValue[8];
			Vector3 asEdgeVertex[12];
			Vector3 asEdgeNorm[12];
			int vertexIds[12];

			//Make a local copy of the values at the cube's corners
			for(iVertex = 0; iVertex < 8; iVertex++) {
				afCubeValue[iVertex] = GetVoxelData(iX + a2iVertexOffset[iVertex][0]*iScale,
													iY + a2iVertexOffset[iVertex][1]*iScale,
													iZ + a2iVertexOffset[iVertex][2]*iScale);
			}

			//Find which vertices are inside of the surface and which are outside
			iFlagIndex = 0;
			for(iVertexTest = 0; iVertexTest < 8; iVertexTest++)
			{
					if(afCubeValue[iVertexTest] <= _surf_value) 
							iFlagIndex |= 1<<iVertexTest;
			}

			//Find which edges are intersected by the surface
			iEdgeFlags = aiCubeEdgeFlags[iFlagIndex];

			//If the cube is entirely inside or outside of the surface, then there will be no intersections
			if(iEdgeFlags == 0) 
			{
					return;
			}

			//Find the point of intersection of the surface with each edge
			//Then find the normal to the surface at those points
			for(iEdge = 0; iEdge < 12; iEdge++)
			{
					//if there is an intersection on this edge
					if(iEdgeFlags & (1<<iEdge))
					{
							fOffset = GetOffset(afCubeValue[ a2iEdgeConnection[iEdge][0] ], afCubeValue[ a2iEdgeConnection[iEdge][1] ], _surf_value);

							asEdgeVertex[iEdge][0] = (float)iX + ((float)a2iVertexOffset[ a2iEdgeConnection[iEdge][0] ][0] +  fOffset * (float)a2iEdgeDirection[iEdge][0]) * (float)iScale;
							asEdgeVertex[iEdge][1] = (float)iY + ((float)a2iVertexOffset[ a2iEdgeConnection[iEdge][0] ][1] +  fOffset * (float)a2iEdgeDirection[iEdge][1]) * (float)iScale;
							asEdgeVertex[iEdge][2] = (float)iZ + ((float)a2iVertexOffset[ a2iEdgeConnection[iEdge][0] ][2] +  fOffset * (float)a2iEdgeDirection[iEdge][2]) * (float)iScale;
				
							vertexIds[iEdge] = _mesh->AddHashedVertex(Vector3DFloat(asEdgeVertex[iEdge][0], asEdgeVertex[iEdge][1], asEdgeVertex[iEdge][2]), false, false, -1, GetHashKey(iX, iY, iZ, iEdge, iScale)); 
					}
			}


			//Draw the triangles that were found.  There can be up to five per cube
			for(iTriangle = 0; iTriangle < 5; iTriangle++)
			{
					if(a2iTriangleConnectionTable[iFlagIndex][3*iTriangle] < 0)
							break;
					Point3 pts[3];
					int triangleVertices[3];
					for(iCorner = 0; iCorner < 3; iCorner++)
					{
						iVertex = a2iTriangleConnectionTable[iFlagIndex][3*iTriangle+iCorner];
						triangleVertices[iCorner] = vertexIds[iVertex];
					}

					for(iCorner = 0; iCorner < 3; iCorner++) {
						if(!_mesh->IsEdgePresent(triangleVertices[iCorner], triangleVertices[(iCorner + 1) % 3])) {
							_mesh->AddEdge(triangleVertices[iCorner], triangleVertices[(iCorner + 1) % 3], false);
						}
					}

					_mesh->AddTriangle(triangleVertices[0], triangleVertices[1], triangleVertices[2], false);
			}
		}

		void VolumeRenderer::LoadFile(string fileName) {
			if(_voxel != NULL) {
				delete _voxel;
			}
			_voxel = VolumeFormatConverter::LoadVolume(fileName);
			CalculateSurface();
			UpdateBoundingBox();
		}

		void VolumeRenderer::Unload() {
			if(_voxel != NULL) {
				delete _voxel;
			}
			_voxel = NULL;
			CalculateSurface();
			UpdateBoundingBox();

		}
		void VolumeRenderer::UpdateBoundingBox() {
			if(_voxel == NULL) {
				for(int i = 0; i < 3; i++) {
					minPts[i] = 0;
					maxPts[i] = 1;
				}
			} else {
				for(int i = 0; i < 3; i++) {
					minPts[i] = 0;
				}
				maxPts[0] = _voxel->getSizeX()-1;
				maxPts[1] = _voxel->getSizeY()-1;
				maxPts[2] = _voxel->getSizeZ()-1;
			}
		}

	}
}

#endif