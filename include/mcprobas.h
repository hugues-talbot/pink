/* $Id: mcprobas.h,v 1.1.1.1 2008-11-25 08:02:37 mcouprie Exp $ */
extern double Prob(void);
extern void ResetProb(int32_t num);
extern double Erlang(double mean, double variance);
extern double Expntl(double mean);
extern double HyperExpntl(double mean, double variance);
extern double Normal(double mean, double deviation);
extern int32_t Random(int32_t low, int32_t high);
extern double Uniform(double low, double high);
