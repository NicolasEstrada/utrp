//
//  main.cpp
//  pso
//
//  Created by Nicolás Estrada on 7/10/16.
//  Copyright © 2016 Nicolás Estrada. All rights reserved.
//

//  Main utrp program.
//
//  Urban Transit Route Problem using Particle Swarm OptimizationNurse in C++
//
//  Created by Nicolas Estrada Irribarra
//  mail: nicoestrada.i@gmail.com
//
//  Implementation of UTRP considering standar input and output
//  to be analised posteriori.
//
//  @include: data.hpp
//  @see Data
//


#include "data.hpp"


int main(int argc, const char * argv[]) {
    // main program, iterator and executor
    
    int i = 1, k = 1, nodes = 0, links = 0, value;
    string sline, token, bs = "\n";
    char *p = nullptr, sep[] = " ";
    
    ifstream tp_file, tt_file, td_file;
    ofstream fout, report;
    
    int newSeed;
    newSeed = rand();
    srand((unsigned)newSeed);
    
    if (argc != 5)
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
    
    cout << "General Paramenters:" << endl;
    
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
            // data.setData(nodes, links);
        }
        tp_file.close();
        
        cout << "Nodes: " << nodes << ", Links: " << links << endl;
    }
    else
    {
        cout << "File couldn't be open." << endl;
        return 0;
    }
    
    cout << "Time Matrix:" << endl;
    
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

            if (sline.length() <= 1){
                sline.clear();
                continue;
            }
            
            p = strtok((char *)sline.c_str(), sep);
            // while (p != NULL && k <= data.nNodes)
            while (p != NULL && k <= nodes)
            {
                if (*p == '-')
                {
                    // data.insertTime(k, i, -1);
                    // data.insertForbiddenLinks(k,i);
                    cout << "-    ";
                }
                
                else
                {
                    value = atoi(p);
                    // data.insertTime(k, i, value);
                    cout << value << "    ";
                }
                p = strtok(NULL, sep);
                k++;
            }
            while (p != NULL)
                p = strtok(NULL, sep);
            sline.clear();
            i++;
            
            cout << endl;

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

    cout << endl << "Demand Matrix:" << endl;
    
    if (td_file.is_open())
    {
        while (td_file.good())
        {
            getline(td_file, sline);
            k = 1;

            if (sline.length() <= nodes){
                sline.clear();
                continue;
            }

            p = strtok((char *)sline.c_str(), sep);
            // while (p != NULL && k <= data.nNodes)
            while (p != NULL && k <= nodes)
            {
                value = atoi(p);
                // data.insertDemand(k, i, value);
                cout << value << "    ";
                p = strtok(NULL, sep);
                k++;
            }
            while (p != NULL)
                p = strtok(NULL, sep);
            sline.clear();
            i++;
            
            cout << endl;
        }
        td_file.close();
    }
    else
    {
        cout << "File couldn't be open." << endl;
        return 0;
    }
    
    delete p;
    
    // data.generateSol();  // Generates initial sol
    //data.copyCurrentToBest();
    
    /*
     **************** HILL CLIMBING ****************
     */
//    for (i = 0; i < MAXITER; i++)
//    {
//        if (i%10 == 0){
//            data.iterateSol();
//            if (data.current.fo < data.best.fo)
//                data.copyCurrentToBest();
//            cout << " HILL CLIMBING ITERATION: " << i << endl;
//            cout << " (*) CurrentFO: " << data.current.fo << endl;
//            cout << " (*) BestFO:    " << data.best.fo << endl;
//        }
//    }
    
    // data.printData();
    
    std::cout << endl << "Hello, Friend!\n";
    return 0;
}
