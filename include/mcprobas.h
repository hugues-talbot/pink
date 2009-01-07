/* $Id: mcprobas.h,v 1.2 2009-01-07 12:46:34 mcouprie Exp $ */
#ifdef __cplusplus
extern "C" {
#endif
extern double Prob(void);
extern void ResetProb(int32_t num);
extern double Erlang(double mean, double variance);
extern double Expntl(double mean);
extern double HyperExpntl(double mean, double variance);
extern double Normal(double mean, double deviation);
extern int32_t Random(int32_t low, int32_t high);
extern double Uniform(double low, double high);
#ifdef __cplusplus
}
#endif
