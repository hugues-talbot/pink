extern int32_t lmeanfilter(
		    struct xvimage *image,  /* donnee: image ndg */       
		    struct xvimage *roi,    /* donnee: image binaire */
		    struct xvimage *inhib,  /* donnee: image binaire */
		    int32_t niter           /* donnee: entier */
		    );
