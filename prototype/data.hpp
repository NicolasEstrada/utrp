/**  Class that handles the data.  
 *
 * This class contains the data from
 * files and method to work with them
 * during execution of selected 
 * algorimth to solve UTRP.
 *  
 * @include: functions.hpp
 * @see Functions
 */

#ifndef DATA_H_
#define DATA_H_
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cmath>
#include <ctime>
#include <cstdlib>


#include "functions.hpp"

#define MAX_BUSES 100
#define TOURS_STOPS 5

using namespace std;

struct Solution{
	double      fo;  //Objective function value
	int         *sol;  // Solution: Designed tour in each node
    int         *tours; // Numbers of buses by tour
};

class Data {

    private:
        int         *timetable, *demandtable;  //Input matrix.

    public:
        /** Default constructor.
        */
        Data(void);
        /** Default destructor.
        */
        ~Data(void);
        
        int         nIter, changeBest, changeCurrent;
        int         nNodes, nLinks, nBuses, nTours;
        
        Solution    current, best, aux;
        
        void setData(int n_nodes, int n_links);
        
        void insertTime(int node1, int node2, int vtime);
        void insertDemand(int node1, int node2, int demand);
        void insertForbiddenLinks(int node1, int node2);

        void setTours(void);

        void generateSol(void);
        void iterateSol(void);
        void refreshFO(void);

        void copyCurrentToBest(void);
        void copyCurrentToAux(void);
        void copyAuxToCurrent(void);
        
        void printData(void);

        int indexTables(int node1, int node2);
        int indexSol(int node1, int node2);
        int indexTour(int tour);
};

/** Default constructor.
    */
Data::Data(void){

	nIter = 0;
    nNodes = 0; nLinks = 0; nTours = 0;
    nBuses = MAX_BUSES;
    changeBest = 1;
    changeCurrent = 1;
    current.fo = 0;
    current.sol = NULL;
    current.tours = NULL;
    best.fo = 0;
    best.sol = NULL;
    best.tours = NULL;
    aux.fo = 0;
    aux.sol = NULL;
    aux.tours = NULL;
	timetable = NULL;
	demandtable = NULL;
}

/** Default destructor.
    */
Data::~Data(){
	
    delete [] current.sol;
    delete [] best.sol;
    delete [] aux.sol;
    delete [] current.tours;
    delete [] best.tours;
    delete [] aux.tours;
	delete [] timetable;
	delete [] demandtable;
}

/** setData.
    *
    * Initialise Data class values
    * 
    * @param n_nodes Number of nodes.
    * @param n_links Numbers of links.
    *
    * @return void.
    */
void Data::setData(int n_nodes, int n_links){

	int i;
	nNodes = n_nodes;
    nLinks = n_links;
    nTours = (int) (n_nodes/TOURS_STOPS) + 1;

    cout << "nTours: " << nTours << endl; 

	timetable = new int [nNodes*nNodes];
	for (i = 0; i < (nNodes*nNodes); i++)
		timetable[i] = 0;

	demandtable = new int [nNodes*nNodes];
	for (i = 0; i < nNodes*nNodes; i++)
		demandtable[i] = 0;

    current.sol = new int [nNodes*nTours];
    best.sol = new int [nNodes*nTours];
    aux.sol = new int [nNodes*nTours];
    for (i = 0; i < nNodes*nTours; i++){
        current.sol[i] = 0;
        best.sol[i] = 0;
        aux.sol[i] = 0;
    }

    current.tours = new int [nTours];
    best.tours = new int [nTours];
    aux.tours = new int [nTours];
    for (i = 1; i <= nTours; i++){
        current.tours[indexTour(i)] = nRand((int) nBuses/nTours);
        best.tours[indexTour(i)] = nRand((int) nBuses/nTours);
        aux.tours[indexTour(i)] = nRand((int) nBuses/nTours);
    }
}

/** insertTime
    *
    * Method to insert values in Time matrix
    * 
    * @param node1, Row node.
    * @param node2, Column node.
    * @param vtime, Time between node1 and node2.
    */
void Data::insertTime(int node1, int node2, int vtime){

	timetable[((node1-1)*nNodes) + node2-1] = vtime;
	//timetable[index(shift-1,day-1,nShift)] = value;
}

/** insertDemand
    *
    * Method to insert values in Demand matrix
    * 
    * @param node1, Row node.
    * @param node2, Column node.
    * @param demand, Demand between node1 and node2.
    */
void Data::insertDemand(int node1, int node2, int demand){

    demandtable[((node1-1)*nNodes) + node2-1] = demand;
    //demandtable[index(shift-1,day-1,nShift)] = value;
}

/** insertForbiddenLinks
    *
    * Method to insert -1 value to not allowed links
    * 
    * @param node1, Row node.
    * @param node2, Column node.
    *
    */
void Data::insertForbiddenLinks(int node1, int node2){

    current.sol[((node1-1)*nNodes) + node2-1] = -1;
    best.sol[((node1-1)*nNodes) + node2-1] = -1;
    aux.sol[((node1-1)*nNodes) + node2-1] = -1;
}

/** generateSol
    *
    * Initialise sol
    * 
    */
void Data::generateSol(void){
    int i, j, t;
    current.fo = 0;
    for (i = 1; i <= nNodes; i++){
        for (j = 1; j <= nNodes; j++){
            if (current.sol[indexSol(i,j)] >= 0){
                t = nRand(nTours);
                current.sol[indexSol(i,j)] = t;
                current.fo += timetable[indexTables(i, j)]*current.tours[indexTour(t)];
            }
        }
    }
    copyCurrentToBest();

}


/** iterateSol
    *
    * Iterates on current sol
    *
    */
void Data::iterateSol(void){
    int i, j, aux_i, aux_j, aux_b = 0, found = 0;

    for (i = 1; i <= nNodes; i++){
        for (j = 1; j <=nNodes; j++){
            if (current.sol[indexSol(i, j)] > 0){
                while(!found){
                    aux_i = nRand(nNodes);
                    aux_j = nRand(nNodes);
                    if (current.sol[indexSol(aux_i, aux_j)] > 0)
                        found = 1;
                }
                aux_b = current.sol[indexSol(i, j)];
                current.sol[indexSol(i, j)] = current.sol[indexSol(aux_i, aux_j)];
                current.sol[indexSol(aux_i, aux_j)] = aux_b;
            }
        }
    }
    refreshFO();
}

/** refreshFO
    *
    * Update FO value
    * 
    */
void Data::refreshFO(void){
    int i, j;
    current.fo = 0;
    for (i = 1; i <= nNodes; i++){
        for (j = 1; j <= nNodes; j++){
            if (current.sol[indexSol(i,j)] > 0){
                current.fo += timetable[indexTables(i, j)]*current.tours[indexTour(current.sol[indexSol(i,j)])];
            }
        }
    }

}

/** copyCurrentToBest
    *
    * Copy current sol to best.
    * 
    */
void Data::copyCurrentToBest(){
    
    best.fo = current.fo;
    for (int i = 0; i < nNodes*nNodes; i++)
        best.sol[i] = current.sol[i];
    for (int i = 0; i < nTours; i++)
        best.tours[i] = current.tours[i];
}

/** copyCurrentToAux
    *
    * Copy current sol to aux.
    * 
    */
void Data::copyCurrentToAux(){
    
    aux.fo = current.fo;
    for (int i = 0; i < nNodes*nNodes; i++)
        aux.sol[i] = current.sol[i];
    for (int i = 0; i < nTours; i++)
        aux.tours[i] = current.tours[i];
}

/** copyAuxToCurrent
    *
    * Copy aux sol to current.
    * 
    */
void Data::copyAuxToCurrent(){
    
    current.fo = aux.fo;
    for (int i = 0; i < nNodes*nNodes; i++)
        current.sol[i] = aux.sol[i];
    for (int i = 0; i < nTours; i++)
        current.tours[i] = aux.tours[i];
}

/** printData
    *
    * Print Matrix and Solutions.
    * 
    */
void Data::printData(){

	int i, j;
    
    cout << endl << "----------- TIME MATRIX -------------" << endl;

	for (i = 0; i < nNodes; i++)
	{
		for(j = 0; j < nNodes; j++)
		{
			cout << timetable[(i*nNodes) + j] << "\t";
		}
		cout << endl;
	}

	cout << endl << "---------------------------" << endl;

    cout << endl << "----------- DEMAND MATRIX -------------" << endl;

	for (i = 0; i < nNodes; i++)
	{
		for (j = 0; j < nNodes; j++)
		{
			cout << demandtable[(i*nNodes) + j] << "\t";
		}
		cout << endl;
	}
    
    cout << endl << "---------------------------" << endl;
    
    cout << "----------- SOLUTIONS -------------" << endl;
    cout << "CURRENT FO: " << current.fo << endl;
    for (i = 1; i <= nNodes; i++)
    {
        for (j = 1; j <= nNodes; j++)
        {
            if (current.sol[indexSol(i, j)] < 0)
                cout << "-";
            else
                cout << current.sol[indexSol(i, j)];
            cout << "\t";
        }
        cout << endl;
    }
    cout << endl;
    cout << "--- TOURS ---" << endl;
    for (i = 1; i <= nTours; i++){
        cout << current.tours[indexTour(i)];
        cout << "\t";
    }
    cout << endl << endl;
    
    /*********************************************************
    //cout << "BEST FO: " << best.fo << endl;
    
    for (i = 1; i <= nNodes; i++)
    {
        for (j = 1; j <= nNodes; j++)
        {
            if (best.sol[indexSol(i, j)] < 0)
                cout << "-";
            else
                cout << best.sol[indexSol(i, j)];
            cout << "\t";
        }
        cout << endl;
    }
    cout << endl;
    cout << "--- TOURS ---" << endl;
    for (i = 1; i <= nTours; i++){
        if (best.tours[indexTour(i)] < 0)
            cout << best.tours[indexTour(i)];
        cout << "\t";
    }
    **********************************************************/
    cout << endl;
}

/** indexTables
    *
    * Index table (rowNode, columnNode)
    * 
    * @param node1
    * @param node2
    *
    * @return value
    */
int Data::indexTables(int node1, int node2){

    return ((node1-1)*nNodes) + node2-1;
}

/** indexSol
    *
    * Index Sol (rowNode, columnNode)
    *  
    * @param node1
    * @param node2
    *
    * @return bus
    */
int Data::indexSol(int node1, int node2){
    
    return (((node1-1)*nNodes) + node2 -1);
}

/** indexTour
    *
    * Index TOur (nTour)
    *  
    * @param tour
    *
    * @return nbuses
    */
int Data::indexTour(int tour){
    
    return (tour-1);
}

#endif
