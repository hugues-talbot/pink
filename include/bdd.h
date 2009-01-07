/* $Id: bdd.h,v 1.2 2009-01-07 12:46:33 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int bddAlpha3SimpleCube(
  int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, 
  int k, int l, int m, int n, int o, int p, int q, int r, int s, int t, 
  int u, int v, int w, int x, int y, int z);
extern int bddAlpha3SimpleCarreX(int a, int b, int c, int d, int e, int f, int g, int h);
extern int bddAlpha3SimpleCarreY(int a, int b, int c, int d, int e, int f, int g, int h);
extern int bddAlpha3SimpleCarreZ(int a, int b, int c, int d, int e, int f, int g, int h);
extern int bddAlpha3SimpleInterX(int a, int b);
extern int bddAlpha3SimpleInterY(int a, int b);
extern int bddAlpha3SimpleInterZ(int a, int b);
extern int bddBeta3SimpleCarreX(int a, int b);
extern int bddBeta3SimpleCarreY(int a, int b);
extern int bddBeta3SimpleCarreZ(int a, int b);
extern int bddBeta3SimpleInterX(int a, int b, int c, int d, int e, int f, int g, int h);
extern int bddBeta3SimpleInterY(int a, int b, int c, int d, int e, int f, int g, int h);
extern int bddBeta3SimpleInterZ(int a, int b, int c, int d, int e, int f, int g, int h);
extern int bddSurfendCarreX(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);
extern int bddSurfendCarreY(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);
extern int bddSurfendCarreZ(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);
extern int bddSurfendCube(
  int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, 
  int k, int l, int m, int n, int o, int p, int q, int r, int s, int t, 
  int u, int v, int w, int x, int y, int z);
extern int bddSurfsingCarreX(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);
extern int bddSurfsingCarreY(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);
extern int bddSurfsingCarreZ(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j);
extern int bddSurfsingCube(
  int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, 
  int k, int l, int m, int n, int o, int p, int q, int r, int s, int t, 
  int u, int v, int w, int x, int y, int z);

#ifdef __cplusplus
}
#endif
