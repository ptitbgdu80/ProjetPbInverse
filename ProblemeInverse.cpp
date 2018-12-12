#include "ProblemeInverse.h"

using namespace Eigen;
using namespace std;

//Constructeur :
ProblemeInverse::ProblemeInverse()
{}
//Destructeur :
ProblemeInverse::~ProblemeInverse()
{}

void ProblemeInverse::Initialize(DataFile data_file)
{
  _Nx = data_file.Get_N_x();
  _Ny = data_file.Get_N_y();
  _x_min = data_file.Get_x_min();
  _x_max = data_file.Get_x_max();
  _y_min = data_file.Get_y_min();
  _y_max = data_file.Get_y_max();
  _Solveur = data_file.Get_Solveur();
  _Tolerance = data_file.Get_Tolerance();

  // _save_all_file = data_file.Get_save_all_file();
  //
  // _save_points_file = data_file.Get_save_points_file();
  // _number_saved_points = data_file.Get_number_saved_points();
  // _saved_points = data_file.Get_saved_points();
  //
  // _restart_file = data_file.Get_restart_file();

  _h_y = (_y_max-_y_min)/(_Ny+1.);
  _h_x = (_x_max-_x_min)/(_Nx+1.);

  _u.setZero(_Nx*_Ny);
  _b.setZero(_Nx*_Ny)
  _db.setZero(_Nx*_Ny);
  _vectalpha.setZero(_Ny);
  _du.setZero(_Nx*_Ny);
  _ue.setZero(_Nx*_Ny);

  // if (_save_all_file !="non")
  // {
  //   system(("rm -Rf "+_save_all_file).c_str());
  //   system(("mkdir -p ./"+_save_all_file).c_str());
  // }
}

void ProblemeInverse::InitializeMatrix();
{
  _LapMat.setZero(_Nx*_Ny,_Nx*_Ny);
  double _alpha = 2*(1/pow(_h_x,2)+1/pow(_h_y,2));
  double _beta = -1/pow(_h_x,2);
  double _gamma = -1/pow(_h_y,2);

  vector<Triplet<double>> liste_elem;

  for (int i = 0 ; i<_Nx*_Ny ; i++)
  {
    liste_elem.push_back({i,i,_alpha});
  }

  for (int i = 0 ; i<_Ny*_Nx-1; i++)
  {
    if ((i+1)%_Nx!=0)
    {
      liste_elem.push_back({i,i+1,_beta});
      liste_elem.push_back({i+1,i,_beta});
    }
  }
  for (int i = 0 ; i<_Nx*(_Ny-1) ; i++)
  {
    liste_elem.push_back({i,_Nx+i,_gamma});
    liste_elem.push_back({_Nx+i,i,_gamma});
  }

  _LapMat.setFromTriplets(liste_elem.begin(), liste_elem.end());

  _LapMat.coeffRef(i,i) += _gamma; //Bord Haut
  _LapMat.coeffRef((_Ny - 1)* _Nx + i , (_Ny - 1)* _Nx + i) += _gamma; //Bord Bas


  _B.setZero(_Nx*_Ny,_Nx*_Ny+1)
  for (int i=0; i<_Nx*_Ny; i++)
  {
    for (int j=0; j<_Nx*_Ny; i++)
    {
      _B(i,j)=_LapMat(i,j)
    }
  }
  for (i=0; i<_Ny; i++)
  {
    _B((i+1)*_Nx-1,_Nx*_Ny)=-_beta
  }
}

void ProblemeInverse::InitializeSecondMembre();
{
  _gs=1.;
  _b.SetZero();
  for(int i=0 ; i<_Ny ; i++)
  {
    _b((i+1)*N_y-1)-=_gs(i)*_beta
  }
}

void ProblemeInverse::Sensibilite()
{
  double Sum;
  Sum=1.;

  ConjugateGradient <SparseMatrix<double> > solver;
  solver.compute(_LapMat);
  while(Sum>=_Tolerance)
  {
    Sum=0.;
    _u = solver.solve(_b);

    for (int i=0, i<_Ny, i++)
    {
      _db.setZero(_db.size());
      _db((i+1)*_Nx-1)=-_beta;
      _du = solver.solve(_db);
      Sum+=pow((_u-_ue).dot(_du),2);
      _du.setZero(_du.size());
    }
    Sum=sqrt(Sum);
  }
}
