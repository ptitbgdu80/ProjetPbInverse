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
  Pb1.recup_ue();
  Pb1.InitializeMatrixM();
  Pb1.InitializeMatrixA();
  Pb1.InitializeMatrixB();
  Pb1.CalculCL();
  Pb1.CalculSecondMembre();

  cout << "Now we BEGIN" << endl ;
  std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
  Pb1.Resolution();
  std::chrono::duration<double> sec = std::chrono::system_clock::now() - start;
  std::cout << "took " << sec.count() << " seconds\n";

  Pb1.SaveSol();
  Pb1.erreur();


  return 0;
}
