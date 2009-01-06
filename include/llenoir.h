/*  Prototype Lenoir */
extern void lenoir1(struct xvimage * image,//xvimage entree
	     int32_t x,
	     int32_t y,
	     int32_t connex, //connexité (4 ou 8 connexité)
	     struct xvimage * etat,//xvimage contenant la propriete etat pour tous les pixels  
	     struct xvimage * distance,//xvimage contenant la propriete distance pour tous les pixels
	     struct xvimage * path,//xvimage contenant la propriete path pour tous les pixels  
	     struct xvimage * out//xvimage contenant la propriete out pour tous les pixels  
);
extern void Lenoir(struct xvimage * image, 
            int32_t x, 
	    int32_t y, 
	    int32_t connex, //type de connexité (4 6 8 18 26)
	    struct xvimage * out) //image contenant la geodesique
;
