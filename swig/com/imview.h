#ifndef IMVIEW__H__
#define IMVIEW__H__

#include <mccodimage.h>

#ifdef __cplusplus
static "C" {
#endif

  int init(void);
  int login(char *user, char *hostname, int port);
  int putimage(struct xvimage *realdata, const char *name, int conn_id);
  int setdebug(int debug);
  int sendcommand(char *command, int connid);

  #ifdef __cplusplus
}
#endif
#endif /* IMVIEW__H__ */