#include "ProblemeInverse.h"
#include <algorithm>

using namespace Eigen;
using namespace std;

//Constructeur :
ProblemeInverse::ProblemeInverse()
{}
//Destructeur :
ProblemeInverse::~ProblemeInverse()
{}
void ProblemeInverse::recup_ue()
{
  _file_name="u_etoile.txt";
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
    cout << "Lecture du fichier" << _file_name << endl;
  }

  string file_line;

  while (!fichier.eof())
  {
    getline(fichier, file_line);

    if (file_line.find("u* :") != std::string::npos)
    {
      for (int i=0; i<_Ny; i++)
      {
        fichier >>_ue(_Ligneue-1 + _Nx*i);
      }
    }
  }
  fichier.close();

  _file_name="solue.txt";
  fichier.open(_file_name);

  if (!fichier.is_open())
  {
    cout << "Impossible d'ouvrir le fichier " << _file_name << endl;
    abort();
  }
  else
  {
    cout << "-------------------------------------------------" << endl;
    cout << "Lecture du fichier" << _file_name << endl;
  }

  while (!fichier.eof())
  {
    getline(fichier, file_line);

    if (file_line.find("u :") != std::string::npos)
    for(int i = 0; i < _Ny; i++)
    {
      for(int j = 0; j<_Nx; j++)
      {
        fichier >> _toutue(j + i*_Nx);
      }
    }
  }
  fichier.close();


  cout << "Lecture des fichiers terminée " << endl;
  cout << "-------------------------------------------------" << endl;
}


void ProblemeInverse::Initialize(DataFile data_file)
{
  _Nx = data_file.Get_Nx();
  _Ny = data_file.Get_Ny();

  cout<<"choix de méthode de résolution"<<endl;
  cout<<" 1 pour sensibilité"<<endl;
  cout<<" 2 pour adjointe"<<endl;
  cout<<" 3 pour projection"<<endl;
  int choixmethode;
  cin >> choixmethode;
  _choixmethode=choixmethode;

  cout<<"choix de paramètres"<<endl;
  cout<<" 1 pour CL bord droit"<<endl;
  cout<<" 2 pour polynôme"<<endl;
  int choixparametres;
  cin >> choixparametres;
  _choixparametres=choixparametres;

  if(_choixparametres==2)
  {
    cout<<"choix du degré du polynome"<<endl;
    int degre;
    cin >> degre;
    _nombrepara=degre+1;
  }



  if (choixparametres==1)
  {
    _nombrepara=_Ny;
  }

  _x_min = data_file.Get_x_min();
  _x_max = data_file.Get_x_max();
  _y_min = data_file.Get_y_min();
  _y_max = data_file.Get_y_max();
  _pas = data_file.Get_pas();
  _Ligneue= data_file.Get_Ligneue();
  _h_y = (_y_max-_y_min)/(_Ny+1.);
  _h_x = (_x_max-_x_min)/(_Nx+1.);
  _u.setZero(_Nx*_Ny);
  _ue.setZero(_Nx*_Ny);
  _toutue.setZero(_Nx*_Ny);
  _db.setZero(_Nx*_Ny);
  _du.setZero(_Nx*_Ny);
  _gs.setZero(_Ny);
  _dI.setZero(_Ny);
  _GrandU.setZero(_Nx*_Ny+_nombrepara);
  _GrandUe.setZero(_Nx*_Ny+_nombrepara);
  _gradproj.setZero(_Nx*_Ny+_nombrepara);
  _lambda.setZero(_Nx*_Ny);
  _para.setZero(_nombrepara);
  _Ulambda.setZero(2*_Nx*_Ny+_nombrepara);
  _HugeMatrix.resize(2*_Nx*_Ny+_nombrepara,2*_Nx*_Ny+_nombrepara);
  _tolerance=0.1;
  for (int i=0; i<_nombrepara; i++)
  {
      _para(i)=1.;
  }
}



void ProblemeInverse::InitializeMatrixM()
{
  _B.resize(_Nx*_Ny,_Nx*_Ny+_nombrepara);
  _LapMat.resize(_Nx*_Ny,_Nx*_Ny);
  _alpha = 2*(1/pow(_h_x,2)+1/pow(_h_y,2));
  _beta = -1/pow(_h_x,2);
  _gamma = -1/pow(_h_y,2);

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
  _B.setFromTriplets(liste_elem.begin(), liste_elem.end());

  for (int i = 0; i < _Nx ; i++)
  {
    _LapMat.coeffRef(i,i) += _gamma; //Bord haut
    _B.coeffRef(i,i) += _gamma;
  }
  for (int i = 0; i < _Nx ; i++)
  {
    _LapMat.coeffRef((_Ny - 1)* _Nx + i , (_Ny - 1)* _Nx + i) += _gamma;
    _B.coeffRef((_Ny - 1)* _Nx + i , (_Ny - 1)* _Nx + i) += _gamma; //Bord bas
  }
}


//Matrice B pour equation adjointe.
void ProblemeInverse::InitializeMatrixB()
{
  vector<Triplet<double>> liste_elem;

  for (int i = 0 ; i<_Nx*_Ny ; i++)
  {
    liste_elem.push_back({_Nx*_Ny+_nombrepara+i,i,_alpha});
    liste_elem.push_back({i,_Nx*_Ny+_nombrepara+i,-_alpha});
  }

  for (int i = 0 ; i<_Ny*_Nx-1; i++)
  {
    if ((i+1)%_Nx!=0)
    {
      liste_elem.push_back({_Nx*_Ny+_nombrepara+i,i+1,_beta});
      liste_elem.push_back({_Nx*_Ny+_nombrepara+i+1,i,_beta});
      liste_elem.push_back({i+1,_Nx*_Ny+_nombrepara+i,-_beta});
      liste_elem.push_back({i,_Nx*_Ny+_nombrepara+i+1,-_beta});
    }
  }
  for (int i = 0 ; i<_Nx*(_Ny-1) ; i++)
  {
    liste_elem.push_back({_Nx*_Ny+_nombrepara+i,_Nx+i,_gamma});
    liste_elem.push_back({_Nx+_Nx*_Ny+_nombrepara+i,i,_gamma});
    liste_elem.push_back({_Nx+i,_Nx*_Ny+_nombrepara+i,-_gamma});
    liste_elem.push_back({i,_Nx+_Nx*_Ny+_nombrepara+i,-_gamma});
  }

  _HugeMatrix.setFromTriplets(liste_elem.begin(), liste_elem.end());

  for (int i = 0; i < _Nx ; i++)
  {
    _HugeMatrix.coeffRef(_Nx*_Ny+_nombrepara+i,i) += _gamma; //Bord haut
    _HugeMatrix.coeffRef(i,_Nx*_Ny+_nombrepara+i) += -_gamma; //Bord haut
  }
  for (int i = 0; i < _Nx ; i++)
  {
    _HugeMatrix.coeffRef((_Ny-1)*_Nx+_Nx*_Ny+_nombrepara+i,(_Ny-1)*_Nx+i) += _gamma; //Bord haut
    _HugeMatrix.coeffRef((_Ny-1)*_Nx+i,(_Ny-1)*_Nx+_Nx*_Ny+_nombrepara+i) += -_gamma; //Bord haut
  }


  if(_choixparametres==1)
  {
    for (int i=0; i<_nombrepara; i++)
    {
      _B.coeffRef((i+1)*_Nx-1,_Nx*_Ny+i)=_beta;
      _HugeMatrix.coeffRef(_Nx*_Ny+_nombrepara + (i+1)*_Nx-1,_Nx*_Ny+i)=_beta;
      _HugeMatrix.coeffRef(_Nx*_Ny+i,_Nx*_Ny+_nombrepara+(i+1)*_Nx-1)=-_beta;
    }
  }
  if(_choixparametres==2)
  {
    for (int i=0; i<_nombrepara; i++)
    {
      for(int j=0; j<_Ny; j++)
      {
        _B.coeffRef((j+1)*_Nx-1,_Nx*_Ny+i)=_beta*pow((j+1)*_h_y,i);
        _HugeMatrix.coeffRef(_Nx*_Ny+_nombrepara +(j+1)*_Nx-1,_Nx*_Ny+i)=_beta*pow((j+1)*_h_y,i);
        _HugeMatrix.coeffRef(_Nx*_Ny+i,_Nx*_Ny+_nombrepara+(j+1)*_Nx-1)=-_beta*pow((j+1)*_h_y,i);
      }
    }
  }
}


void ProblemeInverse::InitializeMatrixA()
{
  _A.resize(_Nx*_Ny+_nombrepara,_Nx*_Ny+_nombrepara);

  for (int i=0; i<_Ny; i++)
  {
    _A.coeffRef(_Ligneue-1  +i*_Nx,_Ligneue-1 +i*_Nx)=1.;
    _HugeMatrix.coeffRef(_Ligneue-1  +i*_Nx,_Ligneue-1 +i*_Nx)=1.;
  }
}

void ProblemeInverse::CalculCL()
{
  if(_choixparametres==1)
  {
    if(_choixmethode==1)
    {
      for(int i=0; i<_Ny;i++)
      {
        _gs(i)=_para(i);
      }
    }
    if(_choixmethode==2)
    {
      for(int i=0; i<_Ny;i++)
      {
        _gs(i)=_Ulambda(_Nx*_Ny+i);
      }
    }
    if(_choixmethode==3)
    {
      for(int i=0; i<_Ny;i++)
      {
        _gs(i)=_GrandU(_Nx*_Ny+i);
      }
    }
  }
  if(_choixparametres==2)
  {
    for(int i=0; i<_Ny;i++)
    {
      _gs(i)=0.;
      if(_choixmethode==1)
      {
        for(int j=0; j<_nombrepara; j++)
        {
          _gs(i)+=_para(j)*pow((i+1)*_h_y,j);
        }
      }
      if(_choixmethode==2)
      {
        for(int j=0; j<_nombrepara; j++)
        {
          _gs(i)+=_Ulambda(_Nx*_Ny+j)*pow((i+1)*_h_y,j);
        }
      }
      if(_choixmethode==3)
      {
        for(int j=0; j<_nombrepara; j++)
        {
          _gs(i)+=_GrandU(_Nx*_Ny+j)*pow((i+1)*_h_y,j);
        }
      }
    }
  }
}

void ProblemeInverse::CalculSecondMembre()
{
  if(_choixmethode==1)
  {
    _b.setZero(_Nx*_Ny);
    for(int i=0 ; i<_Ny ; i++)
    {
      _b((i+1)*_Nx-1)-=_beta*_gs(i);
    }
  }
  if(_choixmethode==2)
  {
    _b.setZero(2*_Nx*_Ny+_nombrepara);

    for(int i=0 ; i<_Nx*_Ny ; i++)
    {
      _b(i)=_ue(i);
    }
  }
}

void ProblemeInverse::Sensibilite()
{
  double Sum;
  double a=0.;
  VectorXd diffuue;
  diffuue.setZero(_Nx*_Ny);
  Sum=1.;

  SimplicialLLT <SparseMatrix<double> > solver;
  solver.compute(_LapMat);

  while(Sum>=_tolerance)
  {
    Sum=0.;
    CalculCL();
    CalculSecondMembre();
    _u = solver.solve(_b); //STEP 1
    for (int i=0; i<_Ny; i++)
    {
      diffuue(i*_Nx+_Ligneue-1)=_u(i*_Nx+_Ligneue-1)-_ue(i*_Nx+_Ligneue-1);
    }
    for (int i=0; i<_nombrepara; i++)
    {
      _db.setZero(_db.size()); //STEP 2
      if(_choixparametres==1)
      {
        _db((i+1)*_Nx-1)=-_beta;
      }
      if(_choixparametres==2)
      {
        for (int j=0; j<_Ny;j++)
        {
          _db((j+1)*_Nx-1)=-_beta*pow((j+1)*_h_y,i);
        }
      }
      _du = solver.solve(_db);
      a = diffuue.dot(_du);
      _dI(i)=a;
      Sum+=pow(a,2);
      _du.setZero(_du.size());
    }
    _para=_para-_pas*_dI; //STEP 3
    Sum=sqrt(Sum);
    cout << Sum << endl;
  }
}

void ProblemeInverse::Adjointe()
{
  //cout << _HugeMatrix << endl;
  SparseLU<SparseMatrix<double,1 > >  solver;
  solver.compute(_HugeMatrix);
  _Ulambda = solver.solve(_b);
}

void ProblemeInverse::Projection()
{
  VectorXd Temp;
  double Sum;
  Sum=1.;

  SimplicialLLT <SparseMatrix<double> > solver;
  SparseMatrix<double> BTB(_B*_B.transpose());
  solver.compute(BTB);
  InitializeMatrixA();
  for(int i=0; i<_Nx*_Ny; i++)
  {
    _GrandUe(i)=_ue(i);
  }
  while(Sum>=_tolerance)
  {
    Sum=0.;
    _gradproj = _A*_GrandU-_GrandUe;
    Temp=_B*_gradproj;
    _lambda = solver.solve(Temp);
    _gradproj=_gradproj - _B.transpose()*_lambda;
    Sum=_gradproj.squaredNorm();
    _GrandU=_GrandU-_pas*_gradproj;
    cout << Sum <<endl;
  }
}


void ProblemeInverse::Resolution()
{
  if(_choixmethode==1)
  {
    if (_choixparametres==1)
    {
      _pas=1.;
    }
    if (_choixparametres==2)
    {
      _pas=0.1;
    }
    Sensibilite();
  }
  if(_choixmethode==2)
    {
      Adjointe();
    }
  if(_choixmethode==3)
    {
      if (_choixparametres==1)
      {
        _pas=5.;
      }
      if (_choixparametres==2)
      {
        _pas=5.;
      }
     Projection();
   }
}

void ProblemeInverse::erreur()
{
  double erreur;
  if(_choixmethode==1)
  {
    erreur=(_u-_toutue).lpNorm<Infinity>();
  }
  if(_choixmethode==2)
  {
    for (int i = 0; i< _Nx*_Ny; i++)
    {
      _u(i)=_Ulambda(i);
    }
    erreur=(_u-_toutue).lpNorm<Infinity>();
  }
  if(_choixmethode==3)
  {
    for (int i = 0; i< _Nx*_Ny; i++)
    {
      _u(i)=_GrandU(i);
    }
    erreur=(_u-_toutue).lpNorm<Infinity>();
  }
  cout << "Erreur max sur l'ensemble de la grille : " << erreur << endl;
}
void ProblemeInverse::SaveSol()
{
  ofstream mon_flux;
  mon_flux.open("solPbInverse.txt", ios::out);
  mon_flux << "# Équation de la chaleur sur un domaine rectangulaire avec un maillage cartésien" << endl
  << "# Avec x_min=" << _x_min << ", x_max=" << _x_max << ", _y_min=" << _y_min << ", y_max=" << _y_max << endl
  << "# Nx =" << _Nx << ", Ny=" << _Ny << endl
  << "# Les points sont sauvegardés ci-dessous (x, y, T) : " << endl << endl
  << "u :" << endl;

  if (_choixmethode == 1)
  {
    for(int i = 0; i < _Ny; i++)
    {
      for(int j = 0; j<_Nx; j++)
      {
        mon_flux << j*_h_x << " " << i*_h_y << " " << _u(j + i*_Nx) << endl;
      }
    }
  }
  if (_choixmethode == 2)
  {
    for(int i = 0; i <_Ny; i++)
    {
      for(int j = 0; j<_Nx; j++)
      {
        mon_flux << j*_h_x << " " << i*_h_y << " " << _Ulambda(j + i*_Nx) << endl;
      }
    }
  }
  if (_choixmethode == 3)
  {
    for(int i = 0; i <_Ny; i++)
    {
      for(int j = 0; j<_Nx; j++)
      {
        mon_flux << j*_h_x << " " << i*_h_y << " " << _GrandU(j + i*_Nx) << endl;
      }
    }
  }
  mon_flux.close();

  if (_choixmethode==1 || _choixmethode==3)
  {
    CalculCL();
    mon_flux.open("g(s).txt", ios::out);
    mon_flux << 0 << " " << 0 << endl;
    for(int i = 0; i < _Ny; i++)
    {
      mon_flux << (i+1)*_h_y << " " << _gs(i) << endl;
    }
    mon_flux << 1 << " " << 0 << endl;
    mon_flux.close();
  }

  if (_choixmethode==2)
  {
    CalculCL();
    mon_flux.open("g(s).txt", ios::out);
    mon_flux << 0 << " " << 0 << endl;
    for(int i = 0; i < _Ny; i++)
    {
      mon_flux << (i+1)*_h_y << " " << _gs(i) << endl;
    }
    mon_flux << 1 << " " << 0 << endl;
    mon_flux.close();
  }
}
