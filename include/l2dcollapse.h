/* $Id: l2dcollapse.h,v 1.3 2009-09-02 14:23:35 mcouprie Exp $ */
#define TF_NOT_IN_I 0
#define TF_HEAD     1
#define TF_TAIL     2
#define TF_NOT_IN_F 3
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t l2dcollapse(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit);
extern int32_t l2dpardircollapse(struct xvimage * k, int32_t nsteps, struct xvimage * inhibit);
extern int32_t l2dpardircollapse_l(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, int32_t priomax);
extern int32_t l2dpardircollapse_f(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, float priomax);
extern graphe * l2dtopoflow_l(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, float priomax);
extern graphe * l2dtopoflow_f(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, float priomax);
#ifdef __cplusplus
}
#endif
