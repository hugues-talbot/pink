/*
  This software is licensed under 
  CeCILL FREE SOFTWARE LICENSE AGREEMENT

  This software comes in hope that it will be useful but 
  without any warranty to the extent permitted by aplicable law.
  
  (C) UjoImro, 2010
  Université Paris-Est, Laboratoire d'Informatique Gaspard-Monge, Equipe A3SI, ESIEE Paris, 93162, Noisy le Grand CEDEX
  ujoimro@gmail.com
*/
// This is an implementation of optimal ellipse fitting algorithm
// PoSh

#include <pink.h>

#undef error
#define error(msg) {stringstream fullmessage; fullmessage << "in ui_fit_circle.cpp: " << msg; call_error(fullmessage.str());}


USING_PART_OF_NAMESPACE_EIGEN


namespace pink
{

  inline float ui_sqr( float x )
  {
    return x*x;    
  }
  
  // gives the leading dimension (the size of the longer side) of the matrix
  int outer_stride( const MatrixXd & mat )
  {
    int cols = mat.cols();
    int rows = mat.rows();
    return cols>rows?cols:rows;
  }
  
  // Generalised Eigen-Problem
  // Solve:
  // A * v(j) = lambda(j) * B * v(j).
  //
  // v are the eigenvectors and are stored in v.
  // lambda are the eigenvalues and are stored in lambda.
  // The eigenvalues are stored as: (lambda(:, 1) + lambda(:, 2)*i)./lambda(:, 3)
  //
  // returns true on success.
  bool generalized_eigenvalue(const MatrixXd& A, const MatrixXd& B, MatrixXd& v, MatrixXd& lambda)
  {
    int Nval = A.cols(); // Number of columns of A and B. Number of rows of v.
    if (B.cols() != Nval  || A.rows()!=Nval || B.rows()!=Nval)
      return false;

    v.resize(Nval,Nval);
    lambda.resize(Nval, 3);
    
    int LDA = outer_stride(A);
    int LDB = outer_stride(B);
    int LDV = outer_stride(v);
    
    double WORKDUMMY;
    int LWORK = -1; // Request optimum work size.
    int INFO = 0;
    
    double * alphar = const_cast<double*>(lambda.col(0).data());
    double * alphai = const_cast<double*>(lambda.col(1).data());
    double * beta   = const_cast<double*>(lambda.col(2).data());
    
    // Get the optimum work size.
    dggev_("N", "V", &Nval, A.data(), &LDA, B.data(), &LDB, alphar, alphai, beta, 0, &LDV, v.data(), &LDV, &WORKDUMMY, &LWORK, &INFO);
    
    LWORK = int(WORKDUMMY) + 32;
    VectorXd WORK(LWORK);
    
    dggev_("N", "V", &Nval, A.data(), &LDA, B.data(), &LDB, alphar, alphai, beta, 0, &LDV, v.data(), &LDV, WORK.data(), &LWORK, &INFO);
    
    return INFO==0;
  } /* generalized_eigenvalue */
  
  VectorXd fit_circle( const VectorXd & x, const VectorXd & y, const string & filename = "" )
  {
    bool verbose = (filename == "");

    // Build desing matrix
    MatrixXd Dmat(x.size(),4);
    Dmat << x.cwise()*x + y.cwise()*y, x, y, VectorXd(x.size()).setOnes();

    // Build scatter matrix
    MatrixXd Smat;
    Smat = Dmat.transpose()*Dmat;

    // Build 4x4 constraint matrix
    MatrixXd Cmat(4,4);
    Cmat.setZero();    
    Cmat(1,1)=1;
    Cmat(2,2)=1;
    Cmat(3,0)=-2;
    Cmat(0,3)=-2;

    // Solve generalized eigensystem
    MatrixXd gevec, geval;    
    generalized_eigenvalue(Smat, Cmat, gevec, geval);

    double max = 1000000000000; 
    double max_q = -1;    
        
    for (int q=0; q<=geval.rows()-1; q++)
    {
      if (fabs(geval(q,2)) > epsilon) // checking if the regularisation constant is good
                                      // e.g. we are not in infinity
      {
        if (fabs(geval(q,0)/geval(q,2)) < max)
        {
          max_q=q;
          max = fabs(geval(q,0)/geval(q,2));
        } /* if geval... */
      } /* if fabs... */      
    } /* for */
    
    VectorXd result(geval.rows());

    for (int q=0; q<=geval.rows() -1; q++ )
    {
      result[q]=gevec(q,max_q);      
    } /* for */
    
    if (verbose)
    {
      std::fstream mathematica;
      mathematica.open( filename.c_str(), std::fstream::out);

      mathematica << "Show[\n";  
      mathematica << "Graphics[Point[{\n";  
      for (int q=0; q<= x.size()-1 -1; q++)
      {
        mathematica << "{" << x[q] << "," << y[q] << "},";    
      }
      mathematica << "{" << x[x.size()-1] << "," << y[y.size()-1] << "}\n";
      mathematica << "}]\n]\n";  
      mathematica << ", \nContourPlot[\n" 
                  << "{(" << result[0] << ") (x^2+y^2)+(" << result[1] << ") x+("
                  << result[2] << ") y+(" << result[3] << ")==0},\n"
                  << "{x, -40, 40},{y, -40, 40},PlotPoints -> 100\n]\n";
  
      mathematica << "]\n";  
      mathematica.close();  
      
      // printing the circle candidates
      for ( int q=0; q<=geval.rows() -1; q++)
      {     
        cout << "the circle equation is "
             << "( x" << std::showpos << gevec(1,q)/(2*gevec(0,q)) << " )^2 + ( y" << std::showpos << gevec(2,q)/(2*gevec(0,q)) << " )^2"
             << std::showpos << gevec(3,q)/gevec(0,q)-ui_sqr(gevec(1,q))/(4*ui_sqr(gevec(0,q)))-ui_sqr(gevec(2,q))/(4*ui_sqr(gevec(0,q))) << " == 0\n";  
      } /* for */
    } /* if */
    
    return result;    
  } /* fit_circle */
  
} /* namespace pink */

/* LuM end of file */
