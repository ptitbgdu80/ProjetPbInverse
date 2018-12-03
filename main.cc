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

  double Tfinal, deltaT;
  Tfinal = data_file.Get_T_final();
  deltaT = data_file.Get_deltaT();
  int nb_iterations = int(ceil(Tfinal/deltaT));

  Chaleur2D problem;
  problem.Initialize(data_file);
  problem.InitializeMatrix();
  auto start = chrono::high_resolution_clock::now();
  problem.IterativeSolver(nb_iterations);
  auto finish = chrono::high_resolution_clock::now();
  double t = chrono::duration_cast<chrono::microseconds>(finish-start).count();
  cout << "Le programme a mis " << t*0.000001 << " secondes à s'exécuter" << endl;

  return 0;
}
