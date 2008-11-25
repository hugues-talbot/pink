int32_t lsaliency(
        struct xvimage *image,
        struct xvimage *masque,
        struct xvimage *saliency,
        int32_t connex);

int32_t lsaliency6b(
        struct xvimage *image,
        struct xvimage *masque,
        struct xvimage *saliency,
        int32_t parite);

/*
typedef struct basinT {
  TypListe *neighbor;
  TypListe *passvalue;
  int32_t altitude;
} basinT;
*/
