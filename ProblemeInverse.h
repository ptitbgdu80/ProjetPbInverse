#include "Sparse"
#include "Dense"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <memory>
#include "Datafileinv.h"

class ProblemeInverse
{
  protected:
  double _x_min, _x_max, _y_min, _y_max, _h_x, _h_y, _alpha, _beta, _gamma,_tolerance,_pas;
  int _Nx, _Ny, _Ligneue, _nombrepara, _choixmethode, _choixparametres;
  Eigen::SparseMatrix<double> _LapMat, _B,_A,_HugeMatrix;
  Eigen::VectorXd _b; //Second Membre
  Eigen::VectorXd _u; // Solution du problème
  Eigen::VectorXd _GrandU; //Solution contenant u et les paramètres alpha
  Eigen::VectorXd _ue;// Solution expérimentale
  Eigen::VectorXd _toutue; // Toute la solution expérimentale
  Eigen::VectorXd _GrandUe;// Solution expérimentale et le nombre de parametres de 0.
  Eigen::VectorXd _para; //Paramètres du problème
  Eigen::VectorXd _db; //Vecteur utilisable pour chaque colonne de la matrice de db/dalpha
  Eigen::VectorXd _du; //Vecteur utilisable pour chaque colonne de la matrice de du/dalpha
  Eigen::VectorXd _gs; //Vecteur des paramètres de bord droit
  Eigen::VectorXd _dI; //Vecteur du gradient de I par rapport à gs.
  Eigen::VectorXd _lambda; //Vecteur des coefficients de Lagrange.
  Eigen::VectorXd _Ulambda; //Vecteur contenant u, les param et les coeff de Lagrange.
  Eigen::VectorXd _gradproj; //Vecteur gradient utilisé dans l'algo gradient projeté

  std::string _Solveur;
  std::string _file_name; //nom du fichier contenant ue


  public: // Méthodes et opérateurs de la classe
  ProblemeInverse();
  // Constructeur : Initialiser _x_min, _x_max, _y_min; _y_max; _N; _h; _LapMat; _x; _y et _sol.
  ~ProblemeInverse();

  void recup_ue();
  void Initialize(DataFile data_file);
  void InitializeMatrixM();
  void InitializeMatrixA();
  void InitializeMatrixB();
  void CalculCL();
  void CalculSecondMembre();
  void Sensibilite();
  void Adjointe();
  void Projection();
  void Resolution();
  void SaveSol();
  void erreur();

};
