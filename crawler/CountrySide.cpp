#include <Eagle.h>
#include <Prey.h>
#include <ThreadPool.h>
using namespace std;
using namespace std::tr1;
using namespace shrek;

int main(int argc, char** argv) {
  //init preys
  vector<Prey*> preys;
  ifstream fin(argv[1]);
  string line;
  while(fin) {
    if(fin.eof()) break;
    getline(fin, line);
    if(line.empty()) { continue; }
    Prey* prey = new Prey(line); // lazy to delete...
    preys.push_back(prey);
  }
  fin.close();

  //eagle born
  Eagle eagle;

  ThreadPool pool(30);
  pool.start();
  int outputInterval = 100;
  //run
  int64_t ct = -1;
  ofstream fout(argv[2]);
  while(true) {
    for(int i = 0; i < preys.size(); ++i) {
      Func func = bind(&Eagle::operator(), &eagle, preys[i]);
      pool.run(func);
      if(ct % outputInterval == 0) {
	fout<<*preys[i]<<endl;
      }
    }
    sleep(10);
    ++ct;
    cout<<"task queue:"<<pool.size()<<endl;
  }
  fout.close();
  return 0;
}


