/**  Programa principal.  
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

#include "data.hpp"

#define MAXITER 1000

int main(int argc, char* argv[]){

    int i = 1, k = 1, nodes, links, value;
	string sline, token, bs = "\n";
	char *p, sep[] = "\t";
	
    ifstream tp_file, tt_file, td_file;
    ofstream fout, report;
    
    Data data;
	data = Data();
    
    int newSeed;
	newSeed = rand();
	srand((unsigned)newSeed);
    
    if (argc != 4)
    {
        cout << "Incorrect format: ./utrp tpX.txt ttX.txt tdX.txt outputFile" << endl;
        return 0;
    }
    
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
			while (p != NULL && k <= 2)
			{
				if (k == 1){
					nodes = atoi(p);
                }
				if (k == 2){
					links = atoi(p);
                }
                
				p = strtok(NULL, sep);
				k++;
			}
            while (p != NULL)
                        p = strtok(NULL, sep);
            sline.clear();
			data.setData(nodes, links);
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

    if (tt_file.is_open())
    {
        while (tt_file.good())
        {
            getline(tt_file, sline);
            k = 1;
    		p = strtok((char *)sline.c_str(), sep);
    		while (p != NULL && k <= data.nNodes)
    		{
                if (*p == '-')
                {
                    data.insertTime(k, i, -1);
                    data.insertForbiddenLinks(k,i);
                }
                
                else
                {
                    value = atoi(p);
                    data.insertTime(k, i, value);
                }
    			p = strtok(NULL, sep);
    			k++;
    		}
            while (p != NULL)
                        p = strtok(NULL, sep);
            sline.clear();
            i++;
        }
        tt_file.close();
    }
    else
    {
        cout << "File couldn't be open." << endl;
        return 0;
    }


    i = 1;
    // Open time matrix (transport demand between nodes)
    td_file.open(argv[3]);
    if (!td_file.is_open())
    {
        cout << "File not found." << endl;
        return 0;
    }

    if (td_file.is_open())
    {
        while (td_file.good())
        {
            getline(td_file, sline);
            k = 1;
            p = strtok((char *)sline.c_str(), sep);
            while (p != NULL && k <= data.nNodes)
            {
                value = atoi(p);
                data.insertDemand(k, i, value);
                p = strtok(NULL, sep);
                k++;
            }
            while (p != NULL)
                        p = strtok(NULL, sep);
            sline.clear();
            i++;
        }
        td_file.close();
    }
    else
    {
        cout << "File couldn't be open." << endl;
        return 0;
    }

	delete p;

    data.generateSol();  // Generates initial sol
    //data.copyCurrentToBest();

    /*
    **************** HILL CLIMBING ****************
    */
    for (i = 0; i < MAXITER; i++)
    {
        data.iterateSol();
        if (data.current.fo < data.best.fo)
            data.copyCurrentToBest();
        cout << " HILL CLIMBING ITERATION: " << i << endl;
        cout << " (*) CurrentFO: " << data.current.fo << endl;
        cout << " (*) BestFO:    " << data.best.fo << endl;
    }

    data.printData();

	return 0;
}
