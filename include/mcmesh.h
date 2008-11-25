/* $Id: mcmesh.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
#define MAXADJFACES 25

typedef struct {
  double x, y, z;    /* coordonnees */
  double xp, yp, zp; /* coordonnees bis (utilisees aussi pour stocker la normale) */
  double xo, yo, zo; /* coordonnees ter (pour memoriser la position originale) */
                     // utilisees seulement par RegulMeshHC
  int32_t nfaces;
  int32_t face[MAXADJFACES]; /* indices des faces adjacentes (pas plus de MAXADJFACES) */
  int32_t nedges;
  int32_t edge[MAXADJFACES]; /* indices des cotes adjacents (pas plus de MAXADJFACES) */

  float curv1, curv2; // Pour les courbures
  uint8_t red, green, blue; // pour la couleur
} meshvertex;

typedef struct {
  int32_t vert[3];       /* indices des sommets adjacents */
  double xn, yn, zn; /* normale a la face */
} meshface;

typedef struct {
  int32_t v1, v2;        /* indices des sommets adjacents */
  int32_t f1, f2;        /* indices des faces adjacentes */
  double curv;       /* pour stocker la courbure (angle entre -pi et pi) */
} meshedge;

typedef struct {
  int32_t max;            /* taille max du tableau de sommets */
  int32_t cur;            /* taille courante du tableau de sommets */
  uint8_t *lab; /* tableau de labels associes aux sommets */
  uint8_t *tmp; /* tableau de valeurs associes aux sommets */
  meshvertex v[1];    /* tableau des elements physiques */
} meshtabvertices;

typedef struct {
  int32_t max;         /* taille max du tableau de faces */
  int32_t cur;         /* taille courante du tableau de faces */
  meshface f[1];   /* tableau des elements physiques */
} meshtabfaces;

typedef struct {
  int32_t max;         /* taille max du tableau de cotes */
  int32_t cur;         /* taille courante du tableau de cotes */
  meshedge e[1];   /* tableau des elements physiques */
} meshtabedges;

typedef struct {
  int32_t nvert;       /* nombre de sommets */
  int32_t nedge;       /* nombre de cotes */
  int32_t *lastneigh;  /* tableau des index des derniers successeurs */
  int32_t *neigh;      /* tableau des successeurs */
} meshtablinks;

/* boite englobante */
typedef struct {
  double bxmin;
  double bxmax;
  double bymin;
  double bymax;
  double bzmin;
  double bzmax;
} meshbox;

extern meshtabvertices *Vertices;
extern meshtabfaces *Faces;
extern meshtabedges *Edges;
extern meshtablinks *Links;

/* ==================================== */
/* prototypes */
/* ==================================== */

extern meshtabvertices * AllocVertices(int32_t taillemax);
extern meshtabfaces * AllocFaces(int32_t taillemax);
extern meshtabedges * AllocEdges(int32_t taillemax);
extern meshtablinks * AllocLinks(int32_t nvert, int32_t nedge);
extern void ReAllocVertices(meshtabvertices **A);
extern void ReAllocFaces(meshtabfaces **A);
extern void ReAllocEdges(meshtabedges **A);
extern void InitMesh(int32_t taillemax);
extern void TermineMesh();
extern void AddFace(double x1, double y1, double z1, 
             double x2, double y2, double z2, 
             double x3, double y3, double z3
	     );
extern void AddFaceFixe(double x1, double y1, double z1, 
                 double x2, double y2, double z2, 
                 double x3, double y3, double z3,
                 int32_t fix1, int32_t fix2, int32_t fix3
	         );
extern void SaveCoords();
extern void RestoreCoords();
extern void ComputeEdges();
extern void ComputeLinks();
extern void AddNoiseMesh(double alpha);
extern void RegulMeshLaplacian(int32_t nsteps);
extern void RegulMeshHamam(double theta);
extern void RegulMeshHamam1(double theta);
extern void RegulMeshHamam2(int32_t nitermax);
extern void RegulMeshHamam3(double theta);
extern void RegulMeshHC(double alpha, double beta);
extern void RegulMeshTaubin(double lambda, double mu, int nitermax);
extern void CalculNormales();
extern void CalculNormalesFaces();
extern void BoundingBoxMesh(meshbox *B);
extern void IsobarMesh(double *X, double *Y, double *Z);
extern double MeanDistCenter();
extern void TranslateMesh(double x, double y, double z);
extern void ZoomMesh(double k);
extern void ZoomMeshX(double k);
extern void ZoomMeshY(double k);
extern void ZoomMeshZ(double k);
extern double VolMesh();
extern void PrintMesh();
extern double AngleFaces(int32_t f1, int32_t f2);
extern double MaxAngleFaces();
extern void MeanAngleFaces(double *mean, double *standev);
extern double MaxLengthEdges();
extern void ComputeCurvatures();

