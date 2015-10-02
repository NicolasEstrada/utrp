/**  Main utrp program.  
 *
 * Nurse Scheduling Problem with Simulated Annealing in C++
 * 
 * Nicolas Estrada Irribarra, 
 * mail: nicoestrada.i@gmail.com,
 * rol: 2773505-3
 * 
 * Implementation of UTRP considering standar input and output
 * to be analised posteriori.
 *  
 * valgrind -v --tool=memcheck --leak-check=full ./utrp tp1.txt tt1.txt td1.txt 534765
 * valgrind -v --tool=memcheck --leak-check=full --track-origins=yes ./utrp tp1.txt tt1.txt td1.txt 534765
 *
 * @include: data.hpp
 * @see Data
 */
/*
MAX_ITERS = 1000000
----------- BEST SOLUTION -------------
BEST FO: 157924
9 -> 15 -> 7 -> 10 -> 8 -> 6 -> 3 -> 2
12 -> 11 -> 10 -> 7 -> 15 -> 8 -> 6 -> 4 -> 5 -> 2 -> 1
12 -> 11 -> 13 -> 14 -> 10 -> 8 -> 6 -> 4 -> 2

real    1m38.574s
user    1m27.737s
sys 0m0.048s
*/
/*
TODO:
    - Consider transfers.
*/

#include <string>
#include <list>
#include "data.hpp"

using namespace std;

#define MAXITER 1000
#define MAXSTUCK 10

int main(int argc, char* argv[]){

    int i = 1, j = 1, k = 1, nodes, links, routes;
    string sline, token, bs = "\n";
    char *p, sep[] = "\t";
    
    ifstream tp_file, tt_file, td_file;
    ofstream fout, report;
    
    Data data;
    data = Data();
    
    int newSeed;
    // newSeed = rand();
    
    
    if (argc != 6)
    {
        cout << "Incorrect format: ./utrp tpX.txt ttX.txt tdX.txt seed output" << endl;
        return 0;
    }
    
    // random init with param seed
    newSeed = atoi(argv[4]);
    srand((unsigned)newSeed);

    tp_file.open(argv[1]);
    if (!tp_file.is_open())
    {
        cout << "File not found." << endl;
        return 0;
    }

    // Open parameters file (nodes, links)
    if (tp_file.is_open())
    {
        while (tp_file.good())
        {
            getline(tp_file, sline);

            p = strtok((char *)sline.c_str(), sep);
            while (p != NULL && k <= 3)
            {
                if (k == 1)
                    nodes = atoi(p);

                if (k == 2)
                    links = atoi(p);

                if (k == 3)
                    routes = atoi(p);
                
                p = strtok(NULL, sep);
                k++;
            }
            while (p != NULL)
                        p = strtok(NULL, sep);
            sline.clear();
            data.setData(nodes, links, routes);
        }
        tp_file.close();
    }
    else
    {
        cout << "File couldn't be open." << endl;
        return 0;
    }

    // Open time matrix (transport time between allowed nodes)
    tt_file.open(argv[2]);
    if (!tt_file.is_open())
    {
        cout << "File not found." << endl;
        return 0;
    }

    if (tt_file.is_open()){
        for(i = 1; i <= data.nNodes ; i++){
            for(j = 1; j <= data.nNodes ; j++){
                tt_file >> sline;
                if (sline.compare("-")){
                    data.insertTime(i, j, atoi(sline.c_str()));
                    data.insertAllowedNodes(i, j);
                }
                else
                {
                    data.insertTime(i, j, -1); 
                    //data.insertForbiddenLinks(i, j);
                }
                sline.clear();
                //data.insertTime(k, i, aux);
                //data.insertForbiddenLinks(k,i);
            }
        }
        tt_file.close();
    }
    else{
        cout << "File couldn't be open." << endl;
        return 0;
    }

    i = 1;
    j = 1;

    // Open time matrix (transport demand between nodes)
    td_file.open(argv[3]);
    if (!td_file.is_open())
    {
        cout << "File not found." << endl;
        return 0;
    }

    if (td_file.is_open()){
        for(i = 1; i <= data.nNodes ; i++){
            for(j = 1; j <= data.nNodes ; j++){
                td_file >> sline;
                data.insertDemand(i, j, atoi(sline.c_str()));
            }
        }
        td_file.close();
    }
    else{
        cout << "File couldn't be open." << endl;
        return 0;
    }

    //data.printData();
    data.generateSol();  // Generates initial sol

    /*
    **************** HILL CLIMBING ****************
    */
    //data.printCurrent();
    int racount = 0, rcount = 0, mcount = 0;
    //data.printRoutes();
    for (i = 0; i < MAXITER; i++)
    {
        //data.printCurrent();
        if (racount > MAXSTUCK){
            racount = 0;
            srand(rand());
            data.generateSol(0);
        }
        else
            data.iterateSol();
        if (data.current.fo < data.best.fo){
            data.copyCurrentToBest();
            if (rcount > mcount){
                mcount = rcount;
                rcount = 0;
                racount = 0;
            }
        }
        racount++;
        rcount++;
    }

    cout << "Max Iters without improvement: " << mcount << endl;

    //data.printData();
    //data.printRoutes();
    data.fileRoutes(argv[5]);
    //data.fileRoutesLatex(argv[5], newSeed);

    delete p;

    return 0;
}
