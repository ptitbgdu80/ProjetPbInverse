#include "Sparse"
#include "Dense"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <memory>
#include "Datafile.h"

class ProblemeInverse
{
  protected:
  double _x_min, _x_max, _y_min, _y_max, _h_x, _h_y, _alpha, _beta, _gamma,_tolerance;
  int _Nx, _Ny;
  Eigen::SparseMatrix<double> _LapMat, _B;
  Eigen::VectorXd _b; //Second Membre
  Eigen::VectorXd _u; // Solution du problème
  Eigen::VectorXd _ue;// Solution expérimentale Initialize
  Eigen::VectorXd _vectalpha; //Condition au bord droit
  Eigen::VectorXd _db; //Vecteur utilisable pour chaque colonne de la matrice de db/dalpha
  Eigen::VectorXd _du; //Vecteur utilisable pour chaque colonne de la matrice de du/dalpha
  Eigen::VectorXd _gs;

  std::string _Solveur;


  public: // Méthodes et opérateurs de la classe
  ProblemeInverse();
  // Constructeur : Initialiser _x_min, _x_max, _y_min; _y_max; _N; _h; _LapMat; _x; _y et _sol.
  ~ProblemeInverse();

  void Initialize(DataFile datafile);
  void InitializeMatrix();
  void InitializeSecondMembre();
  void Sensibilite();
};
