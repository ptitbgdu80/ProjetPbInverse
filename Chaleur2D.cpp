#include "Chaleur2D.h"

using namespace Eigen;
using namespace std;

//Constructeur :
Chaleur2D::Chaleur2D()
{}
//Destructeur :
Chaleur2D::~Chaleur2D()
{}

void Chaleur2D::Initialize(DataFile data_file)
{
  _CL_droite = data_file.Get_CL_droite();
  _CL_gauche = data_file.Get_CL_gauche();
  _CL_haut = data_file.Get_CL_haut();
  _CL_bas = data_file.Get_CL_bas();
  _Val_CL_droite = data_file.Get_Val_CL_droite();
  _Val_CL_gauche = data_file.Get_Val_CL_gauche();
  _Val_CL_haut = data_file.Get_Val_CL_haut();
  _Val_CL_bas = data_file.Get_Val_CL_bas();

  _Nx = data_file.Get_N_x();
  _Ny = data_file.Get_N_y();
  _x_min = data_file.Get_x_min();
  _x_max = data_file.Get_x_max();
  _x_etoile = data_file.Get_x_etoile();
  _y_min = data_file.Get_y_min();
  _y_max = data_file.Get_y_max();
  _Solveur = data_file.Get_Solveur();

  _save_all_file = data_file.Get_save_all_file();
  _save_u_etoile_file = data_file.Get_save_u_etoile_file();

  _h_y = (_y_max-_y_min)/(_Ny+1.);
  _h_x = (_x_max-_x_min)/(_Nx+1.);

  _sol.setZero(_Nx*_Ny);
}

void Chaleur2D::InitializeMatrix()
{
  _LapMat.resize(_Nx*_Ny,_Nx*_Ny);

  double alpha = 2./(_h_x*_h_x) + 2./(_h_y*_h_y);
  double beta = -1./(_h_x*_h_x);
  double gamma = -1./(_h_y*_h_y);

  vector<Triplet<double>> liste_elem;

  for (int i = 0 ; i<_Nx*_Ny ; i++)
  {
    liste_elem.push_back({i,i,alpha});
  }

  for (int i = 0 ; i<_Ny*_Nx-1; i++)
  {
    if ((i+1)%_Nx!=0)
    {
      liste_elem.push_back({i,i+1,beta});
      liste_elem.push_back({i+1,i,beta});
    }
  }
  for (int i = 0 ; i<_Nx*(_Ny-1) ; i++)
  {
    liste_elem.push_back({i,_Nx+i,gamma});
    liste_elem.push_back({_Nx+i,i,gamma});
  }

  _LapMat.setFromTriplets(liste_elem.begin(), liste_elem.end());

  if (_CL_gauche == "Neumann_homogène")
  {
    for (int i = 0 ; i < _Ny; i++)
    {
      _LapMat.coeffRef(_Nx*i,_Nx*i) += beta;  //Bord gauche
    }
  }

  if (_CL_haut == "Neumann_homogène")
  {
    for (int i = 0; i < _Nx ; i++)
    {
      _LapMat.coeffRef(i,i) += gamma; //Bord haut
    }
  }

  if (_CL_bas == "Neumann_homogène")
  {
    for (int i = 0; i < _Nx ; i++)
    {
      _LapMat.coeffRef((_Ny - 1)* _Nx + i , (_Ny - 1)* _Nx + i) += gamma; //Bord bas
    }
  }
}

void Chaleur2D::DirectSolver ()
{
  SimplicialLLT <SparseMatrix<double> > solver;
  solver.compute(_LapMat);

  Chaleur2D::ConditionsLimites();

  _f.resize(_Nx*_Ny);
  for (int j =0; j<_Nx*_Ny ; j++)
  {
    _f(j) = _sol(j);
  }

  _sol = solver.solve(_f);

  Chaleur2D::SaveSol();
}

void Chaleur2D::IterativeSolver ()
{
  ConjugateGradient <SparseMatrix<double> > solver;
  solver.compute(_LapMat);

  Chaleur2D::ConditionsLimites();

  _f.resize(_Nx*_Ny);
  for (int j =0; j<_Nx*_Ny ; j++)
  {
    _f(j) = _sol(j);
  }

  _sol = solver.solve(_f);

  Chaleur2D::SaveSol();
}

void Chaleur2D::SaveSol()
{
  ofstream mon_flux;
  if (_save_all_file != "non")
  {
    mon_flux.open(_save_all_file, ios::out);
    mon_flux << "# Équation de la chaleur sur un domaine rectangulaire avec un maillage cartésien" << endl
    << "# Avec x_min=" << _x_min << ", x_max=" << _x_max << ", _y_min=" << _y_min << ", y_max=" << _y_max << endl
    << "# Nx =" << _Nx << ", Ny=" << _Ny << ", g(s) : " << _CL_droite << " " << _Val_CL_gauche << endl
    << "# Les points sont sauvegardés ci-dessous (x, y, T) : " << endl;

    for(int i = 0; i < _Ny; i++)
    {
      for(int j = 0; j<_Nx; j++)
      {
        mon_flux << j*_h_x << " " << i*_h_y << " " << _sol(j + i*_Nx) << endl;
      }
    }
    mon_flux.close();
  }

  mon_flux.open(_save_u_etoile_file, ios::out);
  mon_flux << "# Équation de la chaleur sur un domaine rectangulaire avec un maillage cartésien" << endl << endl
  << "x_min :" << endl << _x_min << endl << endl
  << "x_max :" << endl << _x_max << endl << endl
  << "x* :" << endl << _x_etoile << endl << endl
  << "y_min :" << endl << _y_min << endl << endl
  << "y_max :" << endl << _y_max << endl << endl
  << "Nx :" << endl << _Nx << endl << endl
  << "Ny :" << endl << _Ny << endl << endl
  << "g(s) :" << endl << _CL_droite << " " << _Val_CL_droite << endl << endl
  << "u* :" << endl;

  int j = floor((_x_etoile - _x_min)/_h_x+0.1);

  for(int i = 0; i < _Ny; i++)
  {
    for(int j = 0; j<_Nx; j++)
    {
    mon_flux << _sol(j + i*_Nx) << endl;
    }
  }
  mon_flux.close();
}

void Chaleur2D::ConditionsLimites()
{
  double gamma = -1/(_h_y*_h_y);
  double beta = -1/(_h_x*_h_x);

  if (_CL_haut == "Dirichlet_constant") //Condition de température en haut
  {
    for (int j = 0; j < _Nx ; j++)
    {
      _sol(j) = _sol(j)-gamma*_Val_CL_haut;
    }
  }
  if (_CL_bas == "Dirichlet_constant") //Condition de température en bas
  {
    for (int j = 0; j < _Nx ; j++)
    {
      _sol(_Nx*(_Ny -1)+ j) = _sol(_Nx*(_Ny -1)+ j)-gamma*_Val_CL_bas;
    }
  }

  if (_CL_gauche == "Dirichlet_constant")  //Condition de température à gauche
  {
    for (int i = 0; i < _Ny; i++)
    {
      _sol(i*_Nx) = _sol(i*_Nx)-beta*_Val_CL_gauche;
    }
  }
  if (_CL_droite == "Dirichlet_constant") //Condition de température à droite
  {
    for (int i = 0; i < _Ny; i++)
    {
      _sol((i+1)*_Nx - 1) = _sol((i+1)*_Nx - 1)-beta*_Val_CL_droite;
    }
  }
  else if (_CL_droite == "Dirichlet_affine") //Condition de température à droite
  {
    for (int i = 0; i < _Ny; i++)
    {
      _sol((i+1)*_Nx - 1) = _sol((i+1)*_Nx - 1)-beta*_Val_CL_droite*i*_h_y;
    }
  }
  else if (_CL_droite == "Dirichlet_sinusoïdal") //Condition de température à droite
  {
    for (int i = 0; i < _Ny; i++)
    {
      _sol((i+1)*_Nx - 1) = _sol((i+1)*_Nx - 1)-beta*_Val_CL_droite*sin(2*acos(-1)*i*_h_y);
    }
  }
}
