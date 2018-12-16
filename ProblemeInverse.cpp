#include "ProblemeInverse.h"

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
        fichier >>_ue(_Ligneue + _Nx*i);
      }
    }
  }
  cout << "Lecture du fichier terminée " << endl;
  cout << "-------------------------------------------------" << endl;
  // cout << _Ligneue<<endl;
  // cout << "ueeeeeeeeeee            "<< _ue(_Ligneue+_Nx) <<endl;
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

  if (choixparametres==1)
  {
    _nombrepara=_Ny;
  }
  else if (choixparametres==2)
  {
    _nombrepara=3;
  }

  _x_min = data_file.Get_x_min();
  _x_max = data_file.Get_x_max();
  _y_min = data_file.Get_y_min();
  _y_max = data_file.Get_y_max();
  _Ligneue= data_file.Get_Ligneue();
  _h_y = (_y_max-_y_min)/(_Ny+1.);
  _h_x = (_x_max-_x_min)/(_Nx+1.);
  _u.setZero(_Nx*_Ny);
  _ue.setZero(_Nx*_Ny);
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
  _pas=1.;
  _tolerance=0.001;

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
  _B.resize(_Nx*_Ny,_Nx*_Ny+_nombrepara);
  for (int i=0; i<_Nx*_Ny; i++)
  {
    // cout<<"hello"<<endl;
    for (int j=0; j<_Nx*_Ny; j++)
    {
      // cout<<"coucou"<<endl;
      _B.coeffRef(i,j)=_LapMat.coeffRef(i,j); //marchera pas
      // _HugeMatrix.coeffRef(_Nx*_Ny+_nombrepara+i,j)=_LapMat.coeffRef(i,j);
      // _HugeMatrix.coeffRef(j,_Nx*_Ny+_nombrepara+i)=-_LapMat.coeffRef(i,j);
    }
  }
  if(_choixparametres==1)
  {
    for (int i=0; i<_nombrepara; i++)
    {
      _B.coeffRef((i+1)*_Nx-1,_Nx*_Ny+i)=_beta;
      _HugeMatrix.coeffRef(_Nx*_Ny+(i+1)*_Nx-1,_Nx*_Ny+i)=_beta;
      _HugeMatrix.coeffRef(_Nx*_Ny+i,_Nx*_Ny+(i+1)*_Nx-1)=-_beta;
    }
  }
  if(_choixparametres==2)
  {
cout << "Cas non implémenté" << endl;
  }

}


//Matrice B pour equation adjointe.
void ProblemeInverse::InitializeMatrixA()
{
  _A.resize(_Nx*_Ny+_nombrepara,_Nx*_Ny+_nombrepara);

  for (int i=0; i<_Ny; i++)
  {
    _A.coeffRef(_Ligneue  +i*_Nx,_Ligneue +i*_Nx)=1.;
    _HugeMatrix.coeffRef(_Ligneue  +i*_Nx,_Ligneue +i*_Nx)=1.;
  }
}

void ProblemeInverse::CalculCL()
{
  if(_choixparametres==1)
  {
    for(int i=0; i<_Ny;i++)
    {
      _gs(i)=_para(i);
    }
  }
  if(_choixparametres==2)
  {
    for(int i=0; i<_Ny;i++)
    {
      _gs(i)=0.;
      for(int j=0; j<_nombrepara; j++)
      {
        _gs(i)+=_para(j)*pow(i*_h_y,j);
      }
    }
  }
}

void ProblemeInverse::CalculSecondMembre()
{
  if(_choixmethode==1)
  _b.setZero(_Nx*_Ny);
  {
    for(int i=0 ; i<_Ny ; i++)
    {
      _b((i+1)*_Ny-1)-=_gs(i)*_beta;
    }
  }
  if(_choixmethode==2)
  _b.setZero(2*_Nx*_Ny+_nombrepara);
  {
    for(int i=0 ; i<_Nx*_Ny ; i++)
    {
      _b(i)-=_ue(i);
    }
  }

}

void ProblemeInverse::Sensibilite()
{
  double Sum;
  double a=0.;
  VectorXd diffuue;
  Sum=1.;

  ConjugateGradient <SparseMatrix<double> > solver;
  solver.compute(_LapMat);

  while(Sum>=_tolerance)
  {
    Sum=0.;
    CalculCL();
    CalculSecondMembre();
    _u = solver.solve(_b); //STEP 1
    diffuue=_A*(_u-_ue);
    for (int i=0; i<_nombrepara; i++)
    {
      _db.setZero(_db.size()); //STEP 2
      if(_choixparametres==1)
      {
        _db((i+1)*_Nx-1)=-_beta;
      }
      if(_choixparametres==2)
      {
        cout << "Cas Non implémenté" << endl;
      }
      _du = solver.solve(_db);
      a = diffuue.dot(_du);
      _dI(i)=a;
      Sum+=pow(a,2);
      _du.setZero(_du.size());
    }
    _para=_para-_pas*_dI; //STEP 3
    Sum=sqrt(Sum);
  }
}

void ProblemeInverse::Adjointe()
{
  ConjugateGradient <SparseMatrix<double> > solver;
  solver.compute(_HugeMatrix);
  CalculSecondMembre();
  _Ulambda = solver.solve(_b);
}

void ProblemeInverse::Projection()
{
  double Sum;
  Sum=1.;

  ConjugateGradient <SparseMatrix<double> > solver;
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

void ProblemeInverse::Resolution()
{
  if(_choixmethode==1)
  {
    Sensibilite();
  }
  if(_choixmethode==2)
  {
    Adjointe();
  }
  if(_choixmethode==3)
  {
    Projection();
  }
}
