#include <Thread.h>
#include <SqliteReader.h>
#include <Priority.h>
#include <Record.h>
#include <QGramsReader.h>
#include <Checker.h>
#include <Detector.h>
#include <Arbitrator.h>
#include <Util.h>
using namespace std;
using namespace shrek;
int main(int argc, char** argv)
{
    int64_t bt = currentTime();
    SqliteReader reader("external/places.sqlite");
    reader();
    cout<<"total urls: "<<reader._urlVec.size();
    Priority priority(reader._urlVec);
    priority();
    RecordVec& recordVec = priority._recordVec;

    //RecordVec recordVec;
    //Record record("http://www.kimiss.com/")
    //record._vtime = 1336034973556000;
    //record._nextone

    cout<<"recordVec size: "<<recordVec.size()<<endl;
    cout<<"detectVec size: "<<priority._detectUrls.size()<<endl;
    //    cout<<"urlVec and recordVec:\n"<<priority<<endl;
    for(RecordVec::iterator iter = recordVec.begin();
        iter != recordVec.end(); ++iter) {
        cout<<*iter<<endl;
    }
    cout<<endl;

    QGramsReader qgramsReader;
    qgramsReader("external/qgrams_map");
    Checker checker(qgramsReader._qgrams);

    Detector detector(checker);
    checker.setDetector(&detector);
    //detector.warmUp();
    detector.start(16);
    detector(priority._detectUrls);
    Arbitrator arbitrator(checker, recordVec, detector);
	
    arbitrator.start(16);
    arbitrator();
    //arbitrator.warmUp();
    arbitrator.stop();
    detector.stop();
    //result
    bt = currentTime() - bt;
    cout<<arbitrator<<endl;
    cout<<"=============net flow kb=================="<<endl
        <<gNetFlowKB<<endl;
    cout<<"===bananaCat work like a dog time========="<<endl
        <<bt/1000000.0/60<<" min"<<endl;
    return 0;
}
