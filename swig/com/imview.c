#include "imview.h"

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>

#include "liarp.h"
#include "liarwrap.h"
#include "imclient.h"

#include <mccodimage.h>

//const char *imview_cmd = "imview -debug -sync -server -fork";
const char *imview_cmd = "imview -server -fork -portfile";
int         imview_window = 0;
int         imview_connexion = -1;

#define SIZEOFSTRING 1000

/*
#define DFLTPORT 7600

void imviewclient(struct xvimage* img)
{

  int conn_id = -1;
  
  IMAGE *I;
  char *lbl = "testImview";
  char *host = "localhost";
  ipctype method = SHM_POSIX;
  
  const char *user;
  int         res;
  const char *imenv;
  
  imenv = getenv("IMVIEWCOMMAND");
  if (imenv == NULL)
    imenv = imview_cmd;
  system(imenv);
  
  I = (IMAGE *)calloc(1,sizeof(IMAGE));
  
  imsetfirstx(I,0, 0);
  imsetfirsty(I,0, 0);
  imsetfirstz(I,0, 0);
  imsetfirstt(I,0, 0);

  imsetnx(I, 0, img->row_size);
  imsetny(I, 0, img->col_size);
  imsetnz(I, 0, depth(img));
  imsetnt(I, 0, 1); // one time slice 
  
  imsetnumcomp(I, 1);
  imsetpixtype(I,0,IM_INT1);
  imsetimgtype(I, IM_SINGLE);
  
  I->buff = calloc(1,sizeof(void*));
  I->buff[0] = img->image_data; // it works in this case. to be avoided
  
  fprintf(stderr, "OK 001\n");
  
  if (I != NULL) {
    user = getenv("USER");
    if (user != NULL) {
      imview_set_transfer_method(method);
      fprintf(stderr, "OK 002\n");
		    res = imviewlogin(user, host, DFLTPORT, &conn_id);
                    fprintf(stderr, "OK 003 - %d - %d\n", res, conn_id);
		    if (res != 0)
		      imexception("Login failed\n");
		    res = imviewputimage(I, lbl, conn_id);
                    fprintf(stderr, "OK 004\n");
		    if (res != 0)
		      imexception("Put failed\n");
    }
  }
}  
*/

int init(void)
{
#ifndef _WINDOWS
  const char *directory = getenv("TMPDIR"); // should work on most unices
  if (directory == NULL) {
    directory = "/tmp"; // but just in case
  }
  
  // calling command
  char command[SIZEOFSTRING], portfilename[SIZEOFSTRING];
  // build a deterministic unique file name
  sprintf(portfilename, "%s/%d_%d", directory, imview_window, getpid());

  // Build command string
  sprintf(command, "%s %s", imview_cmd, portfilename);

  // show command
  char debugmsg[SIZEOFSTRING];
  sprintf(debugmsg, "%s\n", command);
  LIARdebug(debugmsg);
  //debugmsg.str(""); // reset the string
            
  // remove FIFO
  unlink(portfilename); // this may fail but doesn't matter.
            
  // create portfile
  int res = mkfifo(portfilename, S_IRUSR | S_IWUSR);
  if (res != 0) {
    fprintf(stderr, "Failed to create FIFO : %s\n", portfilename);
    return(-1);
  }

      /* open the pipe for reading, notice the NO DELAY  */
      int filenum = -1;
    filenum = open(portfilename, O_RDONLY | O_NDELAY, 0);
    if (filenum < 0) {
      fprintf(stderr, "Failed to open FIFO: %s\n", portfilename);
      return(-2);
    }
    
    // call imview !
    system(command);

    char buffer[5] = {0};

    if (read(filenum, buffer, 4) <= 0) {
      fprintf(stderr, "Failed to read the portnumber\n");
      return(-3);
    }
    close(filenum);
    unlink(portfilename);
            
    sprintf(debugmsg, "This will show an image eventually, using port %s\n", buffer);
    LIARdebug(debugmsg);
            
    // fill the port pointer as an integer
    return(atoi(buffer));
#           else /* NOT _WINDOWS */
    fprintf(stderr, "The Windows(tm) Imview named pipes are not yet implemented. Nag the developer.");
    return -1;
#           endif /* NOT _WINDOWS */
}



// login onto the image server.
// if the imview server has died, this is the fcnt that wil first report an error.
int login(char *user, char *hostname, int port)
{
  int connid;
  
  //imview_force_socket();
  
  int retval = imviewlogin(user, hostname, port, &connid);
  if (retval == 0)
    return connid; // connexion will remain open as long a the imview server is running
  else {
    fprintf(stderr, "Login failed\n");
    return -1;
  }
}

int putbufferRGB(long int dataR, long int dataG, long int dataB, int row, int col, int depth, int datatype, const char *name, int conn_id)
{
  IMAGE *tbu = (IMAGE *)calloc(1,sizeof(IMAGE)); // TBU = To Be Uploaded

  // hand copy...
  
  // this could change
  tbu->ox = tbu->oy = tbu->oz = tbu->ot = 0;
  // straightforward I hope
  tbu->nx = row;
  tbu->ny = col;
  tbu->nz = depth;
  tbu->nc = 3;
  tbu->nt = 1;
  tbu->it = IM_RGB;

  tbu->pt = datatype;

  int datasize=0;
  switch(datatype) {
  case IM_UINT1:
    datasize=1;
    break;
  case IM_UINT2:
    datasize=2;
    break;
  case IM_UINT4:
    datasize=4;
    break;
  case IM_FLOAT:
    datasize=4;
    break;
  case IM_DOUBLE:
    datasize=8;
    break;
  default:
    fprintf(stderr, "Input data type not implemented: %d\n", datatype);
    break;
  }

  if (datasize > 0) {
    // at any rate...
    tbu->buff = calloc(tbu->nc,sizeof(void*));
    
    tbu->buff[0] = (uint8_t*)(dataR);
    tbu->buff[1] = (uint8_t*)(dataG);
    tbu->buff[2] = (uint8_t*)(dataB);
  }

  fprintf(stderr, "Uloading data, nx= %d, ny = %d, nz = %d, nt = %d, nc = %d, datasize = %d\n",
	  tbu->nx, tbu->ny, tbu->nz, tbu->nt, tbu->nc, datasize);

  // away we go
  //imview_force_socket();
  int res = imviewputimage(tbu, name, conn_id);
            
  return res;
}

int putbuffer(long int data, int row, int col, int depth, int datatype, const char *name, int conn_id)
{
  IMAGE *tbu = (IMAGE *)calloc(1,sizeof(IMAGE)); // TBU = To Be Uploaded

  // hand copy...
  
  // this could change
  tbu->ox = tbu->oy = tbu->oz = tbu->ot = 0;
  // straightforward I hope
  tbu->nx = row;
  tbu->ny = col;
  tbu->nz = depth;
  tbu->nc = 1;
  tbu->nt = 1;
  tbu->it = IM_RGB;

  tbu->pt = datatype;

  int datasize=0;
  switch(datatype) {
  case IM_UINT1:
    datasize=1;
    break;
  case IM_UINT2:
    datasize=2;
    break;
  case IM_UINT4:
    datasize=4;
    break;
  case IM_FLOAT:
    datasize=4;
    break;
  case IM_DOUBLE:
    datasize=8;
    break;
  default:
    fprintf(stderr, "Input data type not implemented: %d\n", datatype);
    break;
  }

  if (datasize > 0) {
    // at any rate...
    tbu->buff = calloc(tbu->nc,sizeof(void*));
    
    tbu->buff[0] = (uint8_t*)(data);
  }

  fprintf(stderr, "Uloading data, nx= %d, ny = %d, nz = %d, nt = %d, nc = %d, datasize = %d\n",
	  tbu->nx, tbu->ny, tbu->nz, tbu->nt, tbu->nc, datasize);

  // away we go
  //imview_force_socket();
  int res = imviewputimage(tbu, name, conn_id);
            
  return res;
}

int putimage(struct xvimage *realdata,const char *name, int conn_id)
{
  IMAGE *tbu = (IMAGE *)calloc(1,sizeof(IMAGE)); // TBU = To Be Uploaded

  // hand copy...
  
  // this could change
  tbu->ox = tbu->oy = tbu->oz = tbu->ot = 0;
  // straightforward I hope
  tbu->nx = realdata->row_size;
  tbu->ny = realdata->col_size;
  tbu->nz = realdata->depth_size;
  tbu->nt = realdata->time_size;
  tbu->nc = realdata->num_data_bands;

  // not very precise
  if ((tbu->nc == 2) || (tbu->nc > 3))
    tbu->it = IM_SPECTRUM;
  else if (tbu->nc == 3)
    tbu->it = IM_RGB;
  else if (tbu->nc == 1)
    tbu->it = IM_SINGLE;
  else
    tbu->it = IM_UNSPEC; // means "we don't know" in other words...

  // pixel type (no one-to-one correspondance)
  int datasize = 0;
  switch (realdata->data_storage_type) {
  case VFF_TYP_BIT :
    tbu->pt = IM_BINARY; // this is incorrect actually
    fprintf(stderr, "Incorrect binary type\n");
    break;
  case VFF_TYP_1_BYTE :
    tbu->pt = IM_UINT1;
    datasize = 1;
    break;
  case VFF_TYP_2_BYTE :
    tbu->pt = IM_UINT2;
    datasize = 2;
    break;
  case VFF_TYP_4_BYTE :
    tbu->pt = IM_INT4;
    datasize = 4;
    break;
  case VFF_TYP_FLOAT:
    tbu->pt = IM_FLOAT;
    datasize = 4;
    break;
  case VFF_TYP_DOUBLE:
    tbu->pt = IM_DOUBLE;
    datasize = 8;
    break;
  default:
    fprintf(stderr, "Unknown input data type: %d\n", realdata->data_storage_type);
    break;
  }

  if (datasize > 0) {
    // at any rate...
    tbu->buff = calloc(tbu->nc,sizeof(void*));
    for (int i = 0 ; i < tbu->nc ; ++i) {
      // is this ugly or what ?
      tbu->buff[i] = ((uint8_t*)(realdata->image_data) + (datasize*tbu->nx*tbu->ny*tbu->nz*tbu->nt)*i);
    }
  }

  fprintf(stderr, "Uloading data, nx= %d, ny = %d, nz = %d, nt = %d, nc = %d, datasize = %d\n",
	  tbu->nx, tbu->ny, tbu->nz, tbu->nt, tbu->nc, datasize);

  // away we go
  //imview_force_socket();
  int res = imviewputimage(tbu, name, conn_id);
            
  return res;
}

int setdebug(int debug)
{
  int retval = 0;
  if (debug) {
    LIAREnableDebug();
    retval = 1;
  }  else
    LIARDisableDebug();
  
  return retval;
}

int sendcommand(char *command, int connid)
{
  int retval = 0;
  char *result = 0;
  if ((result=imviewsendcommand(command, connid)) == NULL) {
    fprintf(stderr, "Sending command: '%s' failed\n", command);
    retval = 1;
  } else {
    LIARdebug("Result of command = '%s' was '%s'", command, result);
  }
  return retval;
}

