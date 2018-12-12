#ifndef FILE_DATA_FILE_CPP //ça compile et tout mais j'ai pas l'impression que tout marche parfaitement il faudrait reprendre les trucs

#include "Datafile.h"
#include <fstream>
#include <iostream>


using namespace std;
using namespace Eigen;

// Constructeur

DataFile::DataFile(string file_name)
  : _file_name(file_name), _if_CL_droite(false), _if_CL_gauche(false), _if_CL_haut(false), _if_CL_bas(false),
    _if_Val_CL_droite(false), _if_Val_CL_gauche(false), _if_Val_CL_haut(false), _if_Val_CL_bas(false),
    _if_N_x(false), _if_N_y(false),
    _if_x_min(false), _if_x_max(false), _if_x_etoile(false), _if_y_min(false), _if_y_max(false),
    _if_Solveur(false),
    _if_save_all_file(false),
    _if_save_u_etoile_file(false)
{}


//Lecture du fichier de donnée

void DataFile::ReadDataFile()
{
  ifstream data_file(_file_name.data());
  if (!data_file.is_open())
  {
    cout << "Impossible d'ouvrir le fichier " << _file_name << endl;
    abort();
  }
  else
  {
    cout << "--------------------------------------------------" << endl;
    cout << "Lecture du fichier " << _file_name << endl;
  }

  string file_line;

  string truc;
  size_t taille;
  char carac;

  // Parcourir le fichier pour rechercher les paramètres
  while (!data_file.eof())
  {
    getline(data_file, file_line);
    //cout << file_line <<endl;
    if (file_line.find("CL droite :") != std::string::npos)
    {
      data_file >> _CL_droite; _if_CL_droite = true;
      data_file >> truc;
      taille = truc.size();
      data_file.seekg(-(taille),ios::cur);
      data_file.get(carac);
      data_file.seekg(-1,ios::cur);
      if ((carac == '1') or (carac == '2') or (carac == '3') or (carac == '4') or (carac == '5') or (carac == '6') or  (carac == '7') or (carac == '8') or (carac == '9') or (carac == '0'))
      {
        _Val_CL_droite = atoi(truc.c_str());
        _if_Val_CL_droite = true;
        data_file.seekg(taille,ios::cur);
      }
      else
      {
        data_file.seekg(-taille,ios::cur);
      }
    }

    if (file_line.find("CL gauche :") != std::string::npos)
    {
      data_file >> _CL_gauche; _if_CL_gauche = true;
      data_file >> truc;
      taille = truc.size();
      data_file.seekg(-(taille),ios::cur);
      data_file.get(carac);
      data_file.seekg(-1,ios::cur);
      if ((carac == '1') or (carac == '2') or (carac == '3') or (carac == '4') or (carac == '5') or (carac == '6') or  (carac == '7') or (carac == '8') or (carac = '9') or (carac == '0'))
      {
        _Val_CL_gauche = atoi(truc.c_str());
        _if_Val_CL_gauche = true;
        data_file.seekg(taille,ios::cur);
      }
      else
      {data_file.seekg(-taille,ios::cur);}
    }


    if (file_line.find("CL haut :") != std::string::npos)
    {
      data_file >> _CL_haut; _if_CL_haut = true;
      data_file >> truc;
      taille = truc.size();
      data_file.seekg(-(taille),ios::cur);
      data_file.get(carac);
      data_file.seekg(-1,ios::cur);
      if ((carac == '1') or (carac == '2') or (carac == '3') or (carac == '4') or (carac == '5') or (carac == '6') or  (carac == '7') or (carac == '8') or (carac = '9') or (carac == '0'))
      {
        _Val_CL_haut = atoi(truc.c_str());
        _if_Val_CL_haut = true;
        data_file.seekg(taille,ios::cur);
      }
      else
      {data_file.seekg(-taille,ios::cur);}
    }


    if (file_line.find("CL bas :") != std::string::npos)
    {
      data_file >> _CL_bas; _if_CL_bas = true;
      data_file >> truc;
      taille = truc.size();
      data_file.seekg(-(taille),ios::cur);
      data_file.get(carac);
      data_file.seekg(-1,ios::cur);
      if ((carac == '1') or (carac == '2') or (carac == '3') or (carac == '4') or (carac == '5') or (carac == '6') or  (carac == '7') or (carac == '8') or (carac = '9') or (carac == '0'))
      {
        _Val_CL_bas = atoi(truc.c_str());
        _if_Val_CL_bas = true;
        data_file.seekg(taille,ios::cur);
      }
      else
      {data_file.seekg(-taille,ios::cur);}
    }

    if (file_line.find("N_x :") != std::string::npos)
    {
      data_file >> _N_x; _if_N_x = true;
    }
    if (file_line.find("N_y :") != std::string::npos)
    {
      data_file >> _N_y; _if_N_y = true;
    }
    if (file_line.find("x_min :") != std::string::npos)
    {
      data_file >> _x_min; _if_x_min = true;
    }
    if (file_line.find("x_max :") != std::string::npos)
    {
      data_file >> _x_max; _if_x_max = true;
    }
    if (file_line.find("x* :") != std::string::npos)
    {
      data_file >> _x_etoile; _if_x_etoile = true;
    }
    if (file_line.find("y_min :") != std::string::npos)
    {
      data_file >> _y_min; _if_y_min = true;
    }
    if (file_line.find("y_max :") != std::string::npos)
    {
      data_file >> _y_max; _if_y_max = true;
    }
    if (file_line.find("Solveur :") != std::string::npos)
    {
      data_file >> _Solveur; _if_Solveur = true;
    }
    if (file_line.find("Fichier solution entière :") != std::string::npos)
    {
      data_file >> _save_all_file; _if_save_all_file = true;
    }
    if (file_line.find("Fichier sauvegarde u* :") != std::string::npos)
    {
      data_file >> _save_u_etoile_file; _if_save_u_etoile_file = true;
    }
  }


  // Initialisation par défaut des paramètres non fixés dans le fichier
  // Un message prévient l'utilisateur

  if ((!_if_CL_droite) or ((_CL_droite != "Dirichlet_constant") and (_CL_droite != "Dirichlet_affine") and (_CL_droite != "Dirichlet_sinusoïdal")))
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention la valeur par défaut pour la CL droite (Dirichlet_constant) est utilisée" << endl;
    _CL_droite = "Dirichlet_constant";
  }
  if ((!_if_CL_gauche) or ((_CL_gauche != "Dirichlet_constant") and (_CL_gauche != "Neumann_homogène")))
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention la valeur par défaut pour la CL gauche (Dirichlet_constant) est utilisée" << endl;
    _CL_gauche = "Dirichlet_constant";
  }
  if ((!_if_CL_haut) or ((_CL_haut != "Dirichlet_constant") and (_CL_haut != "Neumann_homogène")))
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention la valeur par défaut pour la CL haut (Neumann_homogène) est utilisée" << endl;
    _CL_haut = "Neumann_homogène";
  }
  if ((!_if_CL_bas) or ((_CL_bas != "Dirichlet_constant") and (_CL_bas != "Neumann_homogène")))
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention la valeur par défaut pour la CL bas (Neumann_homogène) est utilisée" << endl;
    _CL_bas = "Neumann_homogène";
  }



  if (!_if_Val_CL_droite)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, la valeur par défaut de la CL droite pour "<<_CL_droite<<" (500) est utilisée"  << endl;
    _Val_CL_droite = 500;
  }
  if (!_if_Val_CL_gauche)
  {
    if (_CL_gauche =="Neumann_homogène")
    {
      cout << "---------------------------------------------------" << endl;
      cout << "Attention, la valeur par défaut de la CL gauche pour "<<_CL_gauche<<" (0) est utilisée"  << endl;
      _Val_CL_gauche = 0;
    }
    if (_CL_gauche == "Dirichlet_constant")
    {
      cout << "---------------------------------------------------" << endl;
      cout << "Attention, la valeur par défaut de la CL gauche pour "<<_CL_gauche<<" (0) est utilisée"  << endl;
      _Val_CL_gauche = 0;
    }
  }
  if (!_if_Val_CL_haut)
  {
    if (_CL_haut =="Neumann_homogène")
    {
      cout << "---------------------------------------------------" << endl;
      cout << "Attention, la valeur par défaut de la CL haut pour  "<<_CL_haut<<" (0) est utilisée"  << endl;
      _Val_CL_haut = 0;
    }
    if (_CL_haut == "Dirichlet_constant")
    {
      cout << "---------------------------------------------------" << endl;
      cout << "Attention, la valeur par défaut de la CL haut pour "<<_CL_haut<<" (500) est utilisée"  << endl;
      _Val_CL_haut = 500;
    }
  }
  if (!_if_Val_CL_bas)
  {
    if (_CL_bas =="Neumann_homogène")
    {
      cout << "---------------------------------------------------" << endl;
      cout << "Attention, la valeur par défaut de la CL bas pour "<<_CL_bas<<" (0) est utilisée"  << endl;
      _Val_CL_bas = 0;
    }
    if (_CL_bas == "Dirichlet_constant")
    {
      cout << "---------------------------------------------------" << endl;
      cout << "Attention, la valeur par défaut de la CL bas pour "<<_CL_bas<<" (500) est utilisée"  << endl;
      _Val_CL_bas = 500;
    }
  }

  if (!_if_N_x)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, le N_x par défaut est utilisé (100).----" << endl;
    _N_x = 100;
  }
  if (!_if_N_y)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, le N_y par défaut est utilisé (100).----" << endl;
    _N_y = 100;
  }
  if (!_if_x_min)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, le x_min par défaut est utilisé (0).----" << endl;
    _x_min = 0;
  }
  if (!_if_x_max)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, le x_max par défaut est utilisé (1).----" << endl;
    _x_max = 1;
  }
  if (!_if_x_etoile)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, le x_etoile par défaut est utilisé (à 20% du domaine)" << endl;
    _x_etoile = _x_min + 0.2*(_x_max-_x_min);
  }
  if (_x_etoile < _x_min or _x_etoile > _x_max)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, x_etoile est en dehors du domaine spécifié" << endl;
    cout << "Le x_etoile par défaut est utilisé (à 20% du domaine)" << endl;
    _x_etoile = _x_min + 0.2*(_x_max-_x_min);
  }
  if (!_if_y_min)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, le y_min par défaut est utilisé (0).----" << endl;
    _y_min = 0;
  }
  if (!_if_y_max)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, le y_max par défaut est utilisé (1).----" << endl;
    _y_max = 1;
  }
  if (!_if_Solveur)
  {
    cout << "--------------------------------------------------" << endl;
    cout << "Attention, le solveur par défaut (BiCGStab) est utilisé." << endl;
    _Solveur = "BiCGStab";
  }
  if ((_if_Solveur)and(_Solveur != "direct")and(_Solveur != "iterative")and(_Solveur != "BiCGStab"))
  {
    cout << "--------------------------------------------------" << endl;
    cout << "Attention, le Solveur que vous avez choisi("<<_Solveur<<"), n'existe pas"<<endl;
    cout << "Le solveur par défaut (BiCGStab) va donc etre utilisé." << endl;
    _Solveur = "BiCGStab";
  }
  if (!_if_save_all_file)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, la solution ne sera pas sauvegardé dans sa totalité" << endl;
    _save_all_file = "non";
  }
  if (!_if_save_u_etoile_file)
  {
    cout << "---------------------------------------------------" << endl;
    cout << "Attention, vous n'avez pas donné de nom de fichier pour sauvegarder u*" << endl;
    cout << "le nom par défaut u_etoile.txt est utilisé" << endl;
    _save_u_etoile_file = "u_etoile.txt";
  }

  cout << "Fin de la lecture du fichier " << _file_name << endl;
  cout << "---------------------------------------------------" << endl;
}



#define FILE_DATA_FILE_CPP
#endif
