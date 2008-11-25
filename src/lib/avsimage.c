#include <sys/types.h>
#include <stdint.h>
#include <mccodimage.h>
#include <mcimage.h>
#include <avsimage.h>

int32_t samedimsimages(struct xvimage *im1, struct xvimage *im2) {
  if (rowsize(im1) != rowsize(im2) ||
      colsize(im1) != colsize(im2) ||
      depth(im1) != depth(im2)) {
    return 0;
  }
  return 1;
}

//----------------------------------
int32_t findMaxLong(uint32_t *gg, int32_t n)
//----------------------------------
/*
   Returns the maximum value in an array of int32_t
*/
{
  int32_t i, max = 0;
  for (i = 0; i < n; i++) if (gg[i] > max) max = gg[i];
  return max;
}

