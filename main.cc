#include "Datafile.h"
#include "Chaleur2D.h"
#include <chrono>

using namespace Eigen;
using namespace std;

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    cout << "Please, enter the name of your data file." << endl;
    abort();
  }

  // ----------------------- Fichier de données --------------------------------
  DataFile data_file(argv[1]);

  // Lecture du fichier de données
  data_file.ReadDataFile();

  Chaleur2D problem;
  problem.Initialize(data_file);
  problem.InitializeMatrix();
  auto start = chrono::high_resolution_clock::now();
  problem.IterativeSolver();
  auto finish = chrono::high_resolution_clock::now();
  double t = chrono::duration_cast<chrono::microseconds>(finish-start).count();
  cout << "Le programme a mis " << t*0.000001 << " secondes à s'exécuter" << endl;

  return 0;
}
