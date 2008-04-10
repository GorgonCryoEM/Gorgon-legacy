#ifndef GORGON_SSE_RENDERER_H
#define GORGON_SSE_RENDERER_H

#include "Renderer.h"
#include "MeshRenderer.h"
#include <GraphMatch/SkeletonReader.h>
#include <GraphMatch/GeometricShape.h>
#include <GraphMatch/VectorMath.h>
#include <vector>

using namespace wustl_mm::Protein_Morph;
using namespace wustl_mm::GraySkeletonCPP;
using namespace wustl_mm::GraphMatch;
using namespace std;

namespace wustl_mm {
	namespace Visualization {	
		typedef NonManifoldMesh<bool, bool, unsigned char> NonManifoldMesh_SheetIds;
		class SSERenderer : public Renderer{
		public:
			SSERenderer();
			~SSERenderer();

			void Draw(int subSceneIndex);
			void LoadHelixFile(string fileName);
			void LoadSheetFile(string fileName);
			void Unload();
			string GetSupportedLoadFileFormats();
			string GetSupportedSaveFileFormats();
		private:
			void UpdateBoundingBox();
			vector<GeometricShape*> helices;
			vector<GLUquadric *> helixQuadrics;
			NonManifoldMesh_SheetIds * sheetMesh;
		};


		SSERenderer::SSERenderer() {
			helices.clear();
			helixQuadrics.clear();	
			sheetMesh = NULL;
		}

		SSERenderer::~SSERenderer() {
			for(unsigned int i = 0; i < helices.size(); i++) {
				delete helices[i];
				gluDeleteQuadric(helixQuadrics[i]);
			}
			if(sheetMesh != NULL) {
				delete sheetMesh;
			}
		}

		void SSERenderer::Draw(int subSceneIndex) {
			glPushName(subSceneIndex);
			if(subSceneIndex == 0) {
				glPushName(0);
				
				Point3 pt;
				for(unsigned int i = 0; i < helices.size(); i++) {
					glPushMatrix();
					glMultMatrixd(helices[i]->worldToObject.mat);
					glRotated(90, 1,0,0);
					glTranslated(0.0, 0.0, -0.5);
					glLoadName(i);
					gluCylinder(gluNewQuadric(), 0.5, 0.5, 1.0, 10, 10);
					glPopMatrix();
				}
				glPopName();
			}
			else if((subSceneIndex == 1) && (sheetMesh != NULL)) {
				sheetMesh->Draw(true, false, false);
			}
			glPopName();
		}

		void SSERenderer::LoadHelixFile(string fileName) {
			for(unsigned int i = 0; i < helices.size(); i++) {
				delete helices[i];
				gluDeleteQuadric(helixQuadrics[i]);
			}
			helices.clear();
			helixQuadrics.clear();
			SkeletonReader::ReadHelixFile((char *)fileName.c_str(), NULL, helices);
			for(unsigned int i = 0; i < helices.size(); i++) {
				helixQuadrics.push_back(gluNewQuadric());
			}	
			UpdateBoundingBox();			
		}

		void SSERenderer::LoadSheetFile(string fileName) {
			vector<GeometricShape *> sheets;
			sheets.clear();
			if(sheetMesh != NULL) {
				delete sheetMesh;
			}
			sheetMesh = new NonManifoldMesh_SheetIds();
			SkeletonReader::ReadSheetFile((char *)fileName.c_str(), sheets);
			Point3 pt;
			vector<int> indices;
			for(unsigned int i = 0; i < sheets.size(); i++) {
				indices.clear();
				for(unsigned int j = 0; j < sheets[i]->polygonPoints.size(); j++) {
					pt = sheets[i]->polygonPoints[j];
					indices.push_back(sheetMesh->AddVertex(Vector3DFloat((float)pt[0], (float)pt[1], (float)pt[2])));
				}

				for(unsigned int j = 0; j < sheets[i]->polygons.size(); j++) {								
					sheetMesh->AddTriangle(indices[sheets[i]->polygons[j].pointIndex1], indices[sheets[i]->polygons[j].pointIndex2], indices[sheets[i]->polygons[j].pointIndex3], NULL, i+1);					
				}				
			}
			for(unsigned int i = 0; i < sheets.size(); i++) { 
				delete sheets[i];
			}
			indices.clear();
			UpdateBoundingBox();			
		}

		void SSERenderer::Unload() {
			for(unsigned int i = 0; i < helices.size(); i++) {
				delete helices[i];
				gluDeleteQuadric(helixQuadrics[i]);
			}
			helices.clear();
			if(sheetMesh != NULL) {
				delete sheetMesh;
			}
			sheetMesh = NULL;
			UpdateBoundingBox();
		}

		void SSERenderer::UpdateBoundingBox() {
			Point3 pt;
			if(sheetMesh != NULL && sheetMesh->vertices.size() > 0) {
				for(int i = 0; i < 3; i++) {
					minPts[i] = sheetMesh->vertices[0].position.values[i];
					maxPts[i] = sheetMesh->vertices[0].position.values[i];
				}
			} else if (helices.size() > 0) {
				pt = helices[0]->GetWorldCoordinates(Point3(0,0.5,0));
				for(int j = 0; j < 3; j++) {
					minPts[j] = pt[j];
					maxPts[j] = pt[j];
				}					
			}

			if(helices.size() > 0 || sheetMesh != NULL) {
				for(unsigned int i = 0; i < helices.size(); i++) {
					pt = helices[i]->GetWorldCoordinates(Point3(0,0.5,0));
					for(int j = 0; j < 3; j++) {
						minPts[j] = min(minPts[j], (float)pt[j]);
						maxPts[j] = max(maxPts[j], (float)pt[j]);
					}
					pt = helices[i]->GetWorldCoordinates(Point3(0,-0.5,0));
					for(int j = 0; j < 3; j++) {
						minPts[j] = min(minPts[j], (float)pt[j]);
						maxPts[j] = max(maxPts[j], (float)pt[j]);
					}
				}
				if(sheetMesh != NULL) {
					for(unsigned int i = 0; i < sheetMesh->vertices.size(); i++) {
						for(int j = 0; j < 3; j++) {
							minPts[j] = min(minPts[j], sheetMesh->vertices[i].position.values[j]);
							maxPts[j] = max(maxPts[j], sheetMesh->vertices[i].position.values[j]);
						}	
					}
				}
			} else {
				for(int i = 0; i < 3; i++) {
					minPts[i] = -0.5;
					maxPts[i] = 0.5;
				}
			}
		}

		string SSERenderer::GetSupportedLoadFileFormats() {
			return "VRML models (*.vrml *.wrl)";
		}
		string SSERenderer::GetSupportedSaveFileFormats() {
			return "VRML models (*.vrml, *.wrl)";
		}
	}
}


#endif