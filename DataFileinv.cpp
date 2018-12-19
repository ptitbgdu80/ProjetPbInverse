#ifndef _DATA_FILE_CPP

#include "Datafileinv.h"
#include <fstream>
#include <iostream>

using namespace std;

DataFile::DataFile(string file_name)
: _file_name(file_name), _if_x_min(false), _if_x_max(false), _if_Ligneue(false), _if_y_min(false), _if_y_max(false),
 _if_Nx(false), _if_Ny(false)

{}

void DataFile::ReadDataFile()
{
  ifstream data_file(_file_name.data());
  // ifstream data_file;
  // data_file.open(_file_name);

    if (!data_file.is_open())
    {
      cout << "Impossible d'ouvrir le data_file " << _file_name << endl;
      abort();
    }
    else
    {
      cout << "-------------------------------------------------" << endl;
      cout << "Lecture du data_file " << _file_name << endl;
    }
  string file_line;

  while (!data_file.eof())
  {
    getline(data_file, file_line);

    if (file_line.find("x_min :") != std::string::npos)
    {
      data_file >>_x_min;
    }

    if (file_line.find("x_max :") != std::string::npos)
    {
      data_file >>_x_max;
    }

    if (file_line.find("colonne :") != std::string::npos)
    {
      data_file >>_Ligneue;
    }

    if (file_line.find("y_min :") != std::string::npos)
    {
      data_file >>_y_min;
    }

    if (file_line.find("y_max :") != std::string::npos)
    {
      data_file >>_y_max;
    }

    if (file_line.find("Nx :") != std::string::npos)
    {
      data_file >>_Nx;
    }

    if (file_line.find("Ny :") != std::string::npos)
    {
      data_file >>_Ny;
    }

  }
}

#define _DATA_FILE_CPP
#endif
