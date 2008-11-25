
/* --------- MACROS -------------------- */

//Point Types
#define En2d(x,y) ((int32_t)((((x)+1) % 2) + (((y)+1) % 2)))
#define En3d(x,y,z) ((int32_t)((((x)+1) % 2) + (((y)+1) % 2) + (((z)+1) % 2)))

/* ========== FUNCTIONS ========================== */
extern struct xvimage *lhma(struct xvimage *Xh);
extern struct xvimage *lhma_givenTables(struct xvimage *Xh, MLut mlut, RTLutCol Lut, SQDLut SQDn, int32_t *rmax);
