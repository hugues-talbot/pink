/* $Id: mciomesh.h,v 1.3 2009-01-06 13:18:06 mcouprie Exp $ */

const int UNKNOWN = -1;
const int T_POV =   0;  /* PovRay 3.1 */ 
const int T_COL =   1;  /* private format (ESIEE - IFC students) */
const int T_MCM =   2;  /* private format (MC) */
const int T_AC =    3;  /* AC3D format */
const int T_POVB =  4;  /* PovRay 3.1 without header and footer */ 
const int T_GL =    5;  /* Structures de donnees C pour OpenGL */
const int T_DXF =   6;  /* DXF */
const int T_VTK =   7;  /* VTK PolyData */
const int T_IFS =   8;  /* Indexed Face Set (Brown University) */
const int T_PGM =   9;  /* 3D pgm extension (point clouds only) */
const int T_RAW =  10;  /* private format for the "mesh" software: see http://mesh.berlios.de/ */
const int T_CGAL = 11;  /* CGAL output format  */

extern void genheaderPOV(FILE *fileout, int32_t obj_id, meshbox MB);
extern void genheaderAC(FILE *fileout, meshbox MB, double red, double green, double blue, char *name);
extern void genheaderCOL(FILE *fileout, int32_t nbfaces);
extern void genheaderDXF(FILE *fileout);
extern void genheaderVTK(FILE *fileout, char *name);
extern void genfooterPOV(FILE *fileout);
extern void genfooterAC(FILE *fileout);
extern void genfooterDXF(FILE *fileout);
extern void genfacePOV(FILE *fileout,
             double x1, double y1, double z1, 
             double x2, double y2, double z2, 
             double x3, double y3, double z3
		       );
extern void gencarrePOV(FILE *fileout,
             double x1, double y1, double z1, 
             double x2, double y2, double z2, 
             double x3, double y3, double z3, 
             double x4, double y4, double z4
		 );
extern void genfaceCOL(FILE *fileout, int32_t face_id, int32_t object_id,
             double x1, double y1, double z1, 
             double x2, double y2, double z2, 
	     double x3, double y3, double z3,
             double xp1, double yp1, double zp1, 
             double xp2, double yp2, double zp2, 
             double xp3, double yp3, double zp3);
extern void genfaceDXF(FILE *fileout,
             double x1, double y1, double z1, 
             double x2, double y2, double z2, 
	     double x3, double y3, double z3,
             double xp1, double yp1, double zp1, 
             double xp2, double yp2, double zp2, 
             double xp3, double yp3, double zp3);
extern void SaveMeshPOV(FILE *fileout);
extern void SaveMeshSPOV(FILE *fileout);
extern void SaveMeshCOL(FILE *fileout, int32_t obj_id);
extern void SaveMeshMCM(FILE *fileout);
extern void SaveMeshVTK(FILE *fileout);
extern void SaveMeshAC(FILE *fileout);
extern void SaveMeshGL(FILE *fileout, int32_t obj_id);
extern void SaveMeshDXF(FILE *fileout, int32_t obj_id);
extern void LoadMeshMCM(FILE *filein);
extern void LoadMeshIFS(FILE *filein);
extern void LoadMeshCGAL(FILE *filein);
extern void LoadMeshAC(FILE *filein);
extern void LoadBuildMCM(FILE *filein);
extern void LoadBuildVTK(FILE *filein);
extern void LoadBuildIFS(FILE *filein);
extern void LoadBuildCGAL(FILE *filein);

extern void MCM_SaveVTK(MCM *M, FILE *fileout);
