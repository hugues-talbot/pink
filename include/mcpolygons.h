/* $Id: mcpolygons.h,v 1.3 2009-01-12 08:59:38 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif

// nombre maximum de sommets par face (polygone)
#define MCP_MAXVERTFACE 50

// nombre maximum de sommets intermédiaires par edge (subdivision)
#define MCP_MAXVERTEDGE 25

// nombre maximum de faces adjacentes a un sommet
#define MCP_MAXADJFACES 15

typedef struct {
  double x, y, z;    /* coordonnees */
  int32_t nfaces;
  int32_t face[MCP_MAXADJFACES]; /* indices des faces adjacentes (pas plus de MCP_MAXADJFACES) */
} mcpvertex;

typedef struct {
  int32_t n;         /* nombre de sommets adjacents */
  int32_t na;         /* nombre de sommets auxiliaires adjacents */
  int32_t vert[MCP_MAXVERTFACE];   /* indices des sommets adjacents */
} mcpface;

typedef struct {
  int32_t v1, v2;    /* indices des sommets adjacents */
  int32_t n;         /* nombre de sommets dans la subdivision */
  int32_t vert[MCP_MAXVERTEDGE];   /* indices des sommets de la subdivision */
} mcpedge;

typedef struct {
  int32_t max;       /* taille max du tableau de sommets */
  int32_t cur;       /* taille courante du tableau de sommets */
  int32_t nsp;       /* nombre de sommets "principaux" (hors subdivision) */
  mcpvertex v[1];    /* tableau des elements physiques */
} mcptabvertices;

typedef struct {
  int32_t max;       /* taille max du tableau de faces */
  int32_t cur;       /* taille courante du tableau de faces */
  mcpface f[1];      /* tableau des elements physiques */
} mcptabfaces;

typedef struct {
  int32_t max;       /* taille max du tableau de cotes */
  int32_t cur;       /* taille courante du tableau de cotes */
  mcpedge e[1];      /* tableau des elements physiques */
} mcptabedges;

typedef struct {
  mcptabvertices *Vertices;
  mcptabfaces *Faces;
  mcptabedges *Edges;
  Rbt * RBT;
} MCP;

/* ==================================== */
/* prototypes */
/* ==================================== */

extern mcptabvertices * MCP_AllocVertices(int32_t taillemax);
extern mcptabfaces * MCP_AllocFaces(int32_t taillemax);
extern mcptabedges * MCP_AllocEdges(int32_t taillemax);
extern void MCP_ReAllocVertices(mcptabvertices **A);
extern void MCP_ReAllocFaces(mcptabfaces **A);
extern void MCP_ReAllocEdges(mcptabedges **A);
extern MCP * MCP_Init(int32_t taillemax);
extern void MCP_Termine(MCP *P);
extern int32_t MCP_AddVertex(MCP *P, double x, double y, double z);
extern int32_t MCP_AddAuxVertex(MCP *P, double x, double y, double z);
extern int32_t MCP_AddFace(MCP *P, Liste *Face);
extern void MCP_ComputeFaces(MCP *P);
extern void MCP_ComputeEdges(MCP *P);
extern void MCP_SubdivEdges(MCP *P, double param);
extern void MCP_GetPolyPoints(MCP *P, int32_t indface, int32_t *pi, double *px, double *py, double *pz, int32_t *n);
extern void MCP_Print(MCP *P);
extern void MCP_SaveVTK(MCP *P, FILE *fileout);
extern MCP * MCP_LoadVTK(FILE *fileout);
#ifdef __cplusplus
}
#endif
