#include "Sparse"
#include "Dense"
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <memory>
#include "Datafile.h"

class Chaleur2D
{
  protected: // Les attributs de la classe

  double _x_min, _x_max, _y_min, _y_max, _h_x, _h_y, _a, _deltaT, _Lambda;
  int _Nx, _Ny;
  Eigen::SparseMatrix<double> _LapMat; // matrice creuse du laplacien
  Eigen::VectorXd _f; // vecteur source _f qui prend les données de _sol(i) pour calculer _sol(i+1)
  Eigen::VectorXd _sol; // vecteur solution U
  std::string _CL_bas, _CL_haut, _CL_gauche, _CL_droite;
  double _Val_CL_bas, _Val_CL_haut, _Val_CL_gauche, _Val_CL_droite;

  std::string _Solveur;

  std::string _save_all_file, _save_points_file, _restart_file;
  int _number_saved_points;
  std::vector<std::vector<double>> _saved_points;



  public: // Méthodes et opérateurs de la classe
  Chaleur2D();
  // Constructeur : Initialiser _x_min, _x_max, _y_min; _y_max; _N; _h; _LapMat; _x; _y et _sol.
  ~Chaleur2D();

  void Initialize(DataFile datafile);
  void InitializeMatrix();
  void DirectSolver(int nb_iterations);   // Résout le système _LapMat * _sol = _f avec un solveur direct.
  void IterativeSolver(int nb_iterations);   // Résout le système _LapMat * _sol = _f avec un solveur itératif.
  void SaveSol(std::string name_file); // Écrit les solutions dans le fichier "name_file".
  void ConditionsLimites(int num_it);
  void Advance(int nb_iterations);
};
