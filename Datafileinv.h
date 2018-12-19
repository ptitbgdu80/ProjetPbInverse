#ifndef FILE_DATA_FILE_H  // Pas a jour du tout

#include <string>
#include <iostream>
#include <Eigen>

class DataFile
{
 private:
  //Nom du fichier de donnée d'entrée
  const std::string _file_name;



  double _x_min;
  double _x_max;
  int _Ligneue;
  double _y_min;
  double _y_max;
  int _Nx;
  int _Ny;

  // Pour savoir si l'utilisateur a donner les paramètres
  // ou si il faut utiliser les paramètres par défaut.


  bool _if_x_min;
  bool _if_x_max;
  bool _if_Ligneue;
  bool _if_y_min;
  bool _if_y_max;
  bool _if_Nx;
  bool _if_Ny;

 public:

  // Constructeur
  DataFile(std::string file_name);

  // Lecture du fichier de donnée // Pas fait encore ça !
  void ReadDataFile();
  inline int Get_Nx() {return _Nx;}
  inline int Get_Ny() {return _Ny;}
  inline double Get_x_min() {return _x_min;}
  inline double Get_x_max() {return _x_max;}
  inline int Get_Ligneue() {return _Ligneue;}
  inline double Get_y_min() {return _y_min;}
  inline double Get_y_max() {return _y_max;}

};

#define FILE_DATA_FILE_H
#endif
