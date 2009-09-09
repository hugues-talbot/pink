/* $Id: l3dcollapse.h,v 1.4 2009-09-02 14:23:35 mcouprie Exp $ */
#define TF_NOT_IN_I 0
#define TF_HEAD     1
#define TF_TAIL     2
#define TF_NOT_IN_F 3
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t l3dcollapse(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit);
extern int32_t l3dpardircollapse(struct xvimage * k, int32_t nsteps, struct xvimage * inhibit);
extern int32_t l3dpardircollapse_l(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, uint32_t priomax);
extern int32_t l3dpardircollapse_f(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, float priomax);
extern graphe * l3dtopoflow_l(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, float priomax);
extern graphe * l3dtopoflow_f(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, float priomax);
extern int32_t l3dsurfacecollapse(struct xvimage * k, int32_t nsteps, struct xvimage * inhibit);

#ifdef __cplusplus
}
#endif
