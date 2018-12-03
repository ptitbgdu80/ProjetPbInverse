#ifndef FILE_DATA_FILE_H  // Pas a jour du tout

#include <string>
#include <iostream>
#include <Eigen>

class DataFile
{
 private:
  //Nom du fichier de donnée d'entrée
  const std::string _file_name;

  // Variables à utiliser // Ajouter un truc pour le systeme d'adaptation du maillage
  std::string _CL_droite;
  std::string _CL_haut;
  std::string _CL_bas;
  std::string _CL_gauche;
  double _Val_CL_droite;
  double _Val_CL_haut;
  double _Val_CL_bas;
  double _Val_CL_gauche;

  double _CI; // Voir si besoin de changer ça ou pas (CI temperature constante)

  int _N_x;
  int _N_y;
  double _x_min;
  double _x_max;
  double _y_min;
  double _y_max;
  double _deltaT;
  double _T_final;

  double _lambda;
  double _lambdav;
  double _lambdap;

  double _rho;
  double _rhov;
  double _rhop;

  double _Cp;
  double _Cpp;
  double _Cpv;

  double _Aexp;
  double _Ta;

  std::string _Solveur;
  std::string _Schema; // A voir si vraiment utile !

  std::string _save_all_file;  // Nom du fichier qui contiendra la solution pour tout points
  std::string _save_points_file; // Nom du ou des fichiers qui contiendra l'évolution de T en fonction du temps pour 1 unique point ( si plusieurs points selectionnés les fichiers s'apellerons tous comme _save_points_file + le numéro du point
  int _number_saved_points;
  std::vector<std::vector<double>> _saved_points;

  std::string _restart_file;



  // Pour savoir si l'utilisateur a donner les paramètres
  // ou si il faut utiliser les paramètres par défaut.
  bool _if_CL_droite;
  bool _if_CL_gauche;
  bool _if_CL_haut;
  bool _if_CL_bas;
  bool _if_Val_CL_droite;
  bool _if_Val_CL_gauche;
  bool _if_Val_CL_haut;
  bool _if_Val_CL_bas;
  bool _if_CI;
  bool _if_N_x;
  bool _if_N_y;
  bool _if_x_min;
  bool _if_x_max;
  bool _if_y_min;
  bool _if_y_max;
  bool _if_deltaT;
  bool _if_T_final;

  bool _if_lambda;
  bool _if_lambdap;
  bool _if_lambdav;

  bool _if_rho;
  bool _if_rhop;
  bool _if_rhov;

  bool _if_Cp;
  bool _if_Cpp;
  bool _if_Cpv;

  bool _if_Aexp;
  bool _if_Ta;

  bool _if_Solveur;
  bool _if_Schema; // a voir si utile ou pas pas fait ça et la suite

  bool _if_save_all_file;

  bool _if_save_points_file;
  bool _if_number_saved_points;
  bool _if_saved_points;

  bool _if_restart_file;


 public:

  // Constructeur
  DataFile(std::string file_name);

  // Lecture du fichier de donnée // Pas fait encore ça !
  void ReadDataFile();

  inline std::string Get_CL_droite() {return _CL_droite;}
  inline std::string Get_CL_gauche() {return _CL_gauche;}
  inline std::string Get_CL_haut() {return _CL_haut;}
  inline std::string Get_CL_bas() {return _CL_bas;}
  inline double Get_Val_CL_droite() {return _Val_CL_droite;}
  inline double Get_Val_CL_gauche() {return _Val_CL_gauche;}
  inline double Get_Val_CL_haut() {return _Val_CL_haut;}
  inline double Get_Val_CL_bas() {return _Val_CL_bas;}

  inline double Get_CI() {return _CI;}
  inline int Get_N_x() {return _N_x;}
  inline int Get_N_y() {return _N_y;}
  inline double Get_x_min() {return _x_min;}
  inline double Get_x_max() {return _x_max;}
  inline double Get_y_min() {return _y_min;}
  inline double Get_y_max() {return _y_max;}
  inline double Get_deltaT() {return _deltaT;}
  inline double Get_T_final() {return _T_final;}

  inline double Get_lambda() {return _lambda;}
  inline double Get_lambdap() {return _lambdap;}
  inline double Get_lambdav() {return _lambdav;}

  inline double Get_rho() {return _rho;}
  inline double Get_rhop() {return _rhop;}
  inline double Get_rhov() {return _rhov;}

  inline double Get_Cp() {return _Cp;}
  inline double Get_Cpp() {return _Cpp;}
  inline double Get_Cpv() {return _Cpv;}

  inline double Get_Aexp() {return _Aexp;}
  inline double Get_Ta() {return _Ta;}

  inline std::string Get_Solveur() {return _Solveur;}
  inline std::string Get_Schema() {return _Schema;}

  inline std::string Get_save_all_file() {return _save_all_file;}

  inline std::string Get_save_points_file() {return _save_points_file;}
  inline int Get_number_saved_points() {return _number_saved_points;}
  inline  std::vector<std::vector<double>> Get_saved_points() {return _saved_points;}

  inline std::string Get_restart_file() {return _restart_file;}

};

#define FILE_DATA_FILE_H
#endif
