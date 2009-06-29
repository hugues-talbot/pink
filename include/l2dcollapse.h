/* $Id: l2dcollapse.h,v 1.1 2009-06-29 09:10:50 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern int32_t l2dcollapse(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit);
extern int32_t l2dpardircollapse(struct xvimage * k, int32_t nsteps, struct xvimage * inhibit);
extern int32_t l2dpardircollapse_l(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, uint32_t priomax);
extern int32_t l2dpardircollapse_f(struct xvimage * k, struct xvimage * prio, struct xvimage * inhibit, float priomax);
#ifdef __cplusplus
}
#endif
