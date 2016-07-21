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
    
    int i = 1, k = 1, nodes = 0, links = 0, value, populationSize, seed, routes;
    std::string sline, token, bs = "\n";
    char *p = nullptr, sep[] = " ";
    
    std::ifstream tp_file, tt_file, td_file;
    // std::ofstream fout, report;
    
    if (argc != 8)
    {
        std::cout << "Incorrect format: ./utrp tpX.txt ttX.txt tdX.txt outputFile nRoutes populationSize seed" << std::endl;
        return 0;
    }
    
    tp_file.open(argv[1]);
    if (!tp_file.is_open())
    {
        std::cout << "File not found." << std::endl;
        return 0;
    }
    
    std::cout << "General Paramenters:" << std::endl;

    populationSize = atoi(argv[6]);
    seed = atoi(argv[7]);
    routes = atoi(argv[5]);

    std::cout << "Seed: = " << seed << ", populationSize = " << populationSize << ", nRoutes = " << routes << std::endl;
    
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
        
        // std::cout << "Nodes: " << nodes << ", Links: " << links << std::endl;
    }
    else
    {
        std::cout << "File couldn't be open." << std::endl;
        return 0;
    }

    Data data;
    data = Data();
    
    data.setData(nodes, links, routes, populationSize, seed);
    
    data.printTime();
    data.printDemand();
    
    // Open time matrix (transport time between allowed nodes)
    tt_file.open(argv[2]);
    if (!tt_file.is_open())
    {
        std::cout << "File not found." << std::endl;
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
            while (p != NULL && k <= data.nNodes)
            // while (p != NULL && k <= nodes)
            {
                if (*p == '-')
                {
                    data.insertTime(k, i, -1);
                    // data.insertForbiddenLinks(k,i);
                }
                
                else
                {
                    value = atoi(p);
                    data.insertTime(k, i, value);
                    data.insertAllowedNodes(k, i);
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
        std::cout << "File couldn't be open." << std::endl;
        return 0;
    }

    data.printTime();
    
    i = 1;
    // Open time matrix (transport demand between nodes)
    td_file.open(argv[3]);
    if (!td_file.is_open())
    {
        std::cout << "File not found." << std::endl;
        return 0;
    }
    
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
            while (p != NULL && k <= data.nNodes)
            //  while (p != NULL && k <= nodes)
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
        std::cout << "File couldn't be open." << std::endl;
        return 0;
    }
    
    delete p;

    data.printDemand();
    
    data.printSolutionSet();
    data.printLocalBest();
    data.printGlobalBest();
    data.printAllowedNodes();
    
    std::cout << std::endl << data.isConnected(2, 5);
    std::cout << std::endl << data.isConnected(1, 5);
    std::cout << std::endl << data.isConnected(2, 3);
    std::cout << std::endl;
    std::cout << std::endl << data.isLinkedRoad(1, 2, 5);
    std::cout << std::endl << data.isLinkedRoad(6, 3, 8);
    std::cout << std::endl << data.isLinkedRoad(14, 3, 11);
    std::cout << std::endl << data.isLinkedRoad(15, 6, 9);
    
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

    data.generateSolutionSet();
    // data.printSolutionSet();
    
    // std::cout << std::endl << "Hello, Friend!\n";
    return 0;
}
