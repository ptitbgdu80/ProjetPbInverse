#include "ProblemeInverse.h"
#include <chrono>

using namespace Eigen;
using namespace std;

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "Pas de fichier d'entrée" << endl;
    exit(1);
  }

  // ----------------------- Fichier de données --------------------------------
  DataFile data_file(argv[1]);

  // Lecture du fichier de données
  data_file.ReadDataFile();

  ProblemeInverse Pb1;
  Pb1.Initialize(data_file);
    cout<<"TESSSSSSSSSSSSSSSSSSSSSSSST"<<endl;
  Pb1.recup_ue();
  Pb1.InitializeMatrixM();
  Pb1.InitializeMatrixA();
  Pb1.InitializeMatrixB();
  // Pb1.CalculCL();
  Pb1.CalculSecondMembre();
 //  void Sensibilite();
 //  void Adjointe();
 //  void Projection();
 //  void Resolution();

// {
//   if (argc < 2)
//   {
//     cout << "Pas de fichier d'entrée" << endl;
//     exit(1);
//   }

  return 0;
}
