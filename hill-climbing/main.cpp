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
 * @include: data.hpp
 * @see Data
 */

#include <string>
#include <list>
#include "data.hpp"

using namespace std;

#define MAXITER 1000

int main(int argc, char* argv[]){

    int i = 1, j = 1, k = 1, nodes, links, routes;
    string sline, token, bs = "\n";
    char *p, sep[] = "\t";
    
    ifstream tp_file, tt_file, td_file;
    ofstream fout, report;
    
    Data data;
    data = Data();
    
    int newSeed;
    //newSeed = rand();
    
    
    if (argc != 5)
    {
        cout << "Incorrect format: ./utrp tpX.txt ttX.txt tdX.txt seed" << endl;
        return 0;
    }
    
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
                    data.insertForbiddenLinks(i, j);
                }
                //data.insertTime(k, i, aux);
                //data.insertForbiddenLinks(k,i);
            }
        }
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
    }
    else{
        cout << "File couldn't be open." << endl;
        return 0;
    }

    //data.printData();

    data.generateSol();  // Generates initial sol
    //data.copyCurrentToBest();

    /*
    **************** HILL CLIMBING ****************
    */
    for (i = 0; i < MAXITER; i++)
    {
        if (i%10 == 0){
            data.iterateSol();
            if (data.current.fo < data.best.fo)
                data.copyCurrentToBest();
            cout << " HILL CLIMBING ITERATION: " << i << endl;
            cout << " (*) CurrentFO: " << data.current.fo << endl;
            cout << " (*) BestFO:    " << data.best.fo << endl;
        }
    }

    data.printData();

    return 0;
}
