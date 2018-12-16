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

  //_tolerance = data_file.Get_tolerance();

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
  _b.setZero(_Nx*_Ny);
  _db.setZero(_Nx*_Ny);
  _vectalpha.setZero(_Ny);
  _du.setZero(_Nx*_Ny);

  _gs.setZero(_Ny);
  _b.setZero(_Nx*_Ny);
  _dI.setZero(_Ny);
  _GrandU.setZero((_Nx+1)*_Ny);
  _gradproj.setZero((_Nx+1)*_Ny);

  // if (_save_all_file !="non")
  // {
  //   system(("rm -Rf "+_save_all_file).c_str());
  //   system(("mkdir -p ./"+_save_all_file).c_str());
  // }
}

void ProblemeInverse::recup_ue()
{
  _ue.setZero(_Nx*_Ny);
  ifstream fichier;
  fichier.open(_file_name);

  if (!fichier.is_open())
  {
    cout << "Impossible d'ouvrir le fichier " << _file_name << endl;
    abort();
  }
  else
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Lecture du fichier " << _file_name << endl;
  }

  string file_line;

  while (!fichier.eof())
  {
    getline(fichier, file_line);

    if (file_line.find("x_min :") != std::string::npos)
    {
      fichier >>_x_min;
    }

    if (file_line.find("x_max :") != std::string::npos)
    {
      fichier >>_x_max
    }

    if (file_line.find("x* :") != std::string::npos)
    {
      fichier >>_x_etoile;
    }

    if (file_line.find("y_min :") != std::string::npos)
    {
      fichier >>_y_min;
    }

    if (file_line.find("y_max :") != std::string::npos)
    {
      fichier >>_y_max;
    }

    if (file_line.find("Nx :") != std::string::npos)
    {
      fichier >>_Nx;
    }

    if (file_line.find("Ny :") != std::string::npos)
    {
      fichier >>_Ny;
    }

    if (file_line.find("u*:") != std::string::npos)
    {
      for (int i=0; i<_Ny; i++)
      {
        fichier >>_ue[_Ligneue + _Nx*(i-1)];
      }
    }
  }
}


void ProblemeInverse::InitializeMatrixM()
{
  _LapMat.resize(_Nx*_Ny,_Nx*_Ny);
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

  for (int i = 0; i < _Nx ; i++)
  {
    _LapMat.coeffRef(i,i) += _gamma; //Bord haut
  }
  for (int i = 0; i < _Nx ; i++)
  {
    _LapMat.coeffRef((_Ny - 1)* _Nx + i , (_Ny - 1)* _Nx + i) += _gamma; //Bord bas
  }
}


//Matrice B pour equation adjointe.
void ProblemeInverse::InitializeMatrixB()
{
  _B.resize(_Nx*_Ny,_Nx*_Ny+_Ny);
  for (int i=0; i<_Nx*_Ny; i++)
  {
    for (int j=0; j<_Nx*_Ny; j++)
    {
      _B.coeffRef(i,j)=_LapMat.coeffRef(i,j);
    }
  }
  for (int i=0; i<_Ny; i++)
  {
    _B.coeffRef((i+1)*_Nx-1,_Nx*_Ny+i)=_beta;
  }
}


//Matrice B pour equation adjointe.
void ProblemeInverse::InitializeMatrixA()
{
  _A.resize(_Nx*_Ny+_Ny,_Nx*_Ny+_Ny);

  for (int i=0; i<_Nx*_Ny; i++)
  {
    _A.coeffRef(i,i)=1.;
  }

  for (int i=0; i<_Ny; i++)
  {
    _A.coeffRef((i+1)*_Nx-1,_Nx*_Ny+i*_Ny)=_beta;
  }
}


void ProblemeInverse::CalculSecondMembre()
{
  for(int i=0 ; i<_Ny ; i++)
  {
    _b((i+1)*_Ny-1)-=_gs(i)*_beta;
  }
}

void ProblemeInverse::Sensibilite()
{
  double Sum;
  double a=0.;
  Sum=1.;

  ConjugateGradient <SparseMatrix<double> > solver;
  solver.compute(_LapMat);

  while(Sum>=_tolerance)
  {
    Sum=0.;
    CalculSecondMembre();
    _u = solver.solve(_b);
    for (int i=0; i<_Ny; i++)
    {
      _db.setZero(_db.size());
      _db((i+1)*_Nx-1)=-_beta;
      _du = solver.solve(_db);
      a = (_u-_ue).dot(_du);
      _dI(i)=a;
      Sum+=pow(a,2);
      _du.setZero(_du.size());
    }
    _gs=_gs-_pas*_dI;
    Sum=sqrt(Sum);
  }
}

void ProblemeInverse::Adjointe()
{
  double Sum;
  Sum=1.;

  ConjugateGradient <SparseMatrix<double> > solver;
  solver.compute(_LapMat);

  while(Sum>=_tolerance)
  {
    Sum=0.;
    CalculSecondMembre();
    _u = solver.solve(_b);
    _lambda = solver.solve(_u-_ue);

    for (int i=0; i<_Ny; i++)
    {
      _db.setZero(_db.size());
      _db((i+1)*_Nx-1)=-_beta;
      _dI(i)=_lambda.dot(_db);
    }

    for (int i=0; i<_Ny; i++)
    {
      Sum+=pow(_dI(i),2);
    }
    _gs=_gs-_pas*_dI;
    Sum=sqrt(Sum);
  }
}

void ProblemeInverse::Projection()
{
  double Sum;
  Sum=1.;

  ConjugateGradient <SparseMatrix<double> > solver;
  SparseMatrix<double> BTB(_B*_B.transpose());
  solver.compute(BTB);
  InitializeMatrixA();

  while(Sum>=_tolerance)
  {
    Sum=0.;
    _gradproj = _A*_GrandU-_GrandUe;
    _lambda = solver.solve(MatrixXd(_B)*_gradproj);
    _gradproj=_gradproj - MatrixXd(_B.transpose())*_lambda;
    for (int i=0; i<(_Nx+1)*_Ny; i++)
    {
      Sum+=pow(_gradproj(i),2);
    }
    _GrandU=_GrandU-_pas*_gradproj;
    Sum=sqrt(Sum);
  }
}
