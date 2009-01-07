/* $Id: mciomesh.h,v 1.4 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

#define UNKNOWN -1
#define T_POV   0  /* PovRay 3.1 */ 
#define T_COL   1  /* private format (ESIEE - IFC students) */
#define T_MCM   2  /* private format (MC) */
#define T_AC    3  /* AC3D format */
#define T_POVB  4  /* PovRay 3.1 without header and footer */ 
#define T_GL    5  /* Structures de donnees C pour OpenGL */
#define T_DXF   6  /* DXF */
#define T_VTK   7  /* VTK PolyData */
#define T_IFS   8  /* Indexed Face Set (Brown University) */
#define T_PGM   9  /* 3D pgm extension (point clouds only) */
#define T_RAW  10  /* private format for the "mesh" software: see http://mesh.berlios.de/ */
#define T_CGAL 11  /* CGAL output format  */

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
#ifdef __cplusplus
}
#endif
