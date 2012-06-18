/**  Class that handles the data.  
 *
 * This class contains the data from
 * files and method to work with them
 * during execution of selected 
 * algorimth to solve UTRP.
 *
 *  ./exe instancia output #iter #restart #vecindario semilla
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
#include <list>


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
        
        list<int> allowed_nodes;
        list<int> ::iterator it;
        list<pair<int, int> > test;
        list<pair<int, int> > ::iterator it2;

        Solution    current, best, aux;
        
        void setData(int n_nodes, int n_links, int routes);
        
        void insertTime(int node1, int node2, int vtime);
        void insertDemand(int node1, int node2, int demand);
        void insertForbiddenLinks(int node1, int node2);
        void insertAllowedNodes(int node1, int allowed_node);

        void setTours(void);

        void generateSol(void);
        void iterateSol(void);
        void refreshFO(void);

        void copyCurrentToBest(void);
        void copyCurrentToAux(void);
        void copyAuxToCurrent(void);

        int isConnected(int node1, int node2);
        int isLinkedRoad(int node1, int node2, int node3);
        int alreadyExists(int route, int node);
        int isFinished(int route, int node);
        int previousConnection(int route, int node);

        int tourLen(int tour);
        int tourBegin(int tour);
        int tourEnd(int tour);
        int previousNode(int tour, int node);

        void printCurrent(void);
        void printData(void);

        int indexTables(int node1, int node2);
        int indexSol(int tour, int node);
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
    current.sol = 0;
    current.tours = 0;
    best.fo = 0;
    best.sol = 0;
    best.tours = 0;
    aux.fo = 0;
    aux.sol = 0;
    aux.tours = 0;
	timetable = 0;
	demandtable = 0;
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
void Data::setData(int n_nodes, int n_links, int routes){

	int i;
	nNodes = n_nodes;
    nLinks = n_links;
    nTours = routes;
    //nTours = (int) (n_nodes/TOURS_STOPS) + 1;

    cout << "nTours: " << nTours << endl; 

	timetable = new int [nNodes*nNodes];
	for (i = 0; i < (nNodes*nNodes); i++)
		timetable[i] = 0;

	demandtable = new int [nNodes*nNodes];
	for (i = 0; i < nNodes*nNodes; i++)
		demandtable[i] = 0;

    current.sol = new int [nTours*nNodes];
    best.sol = new int [nTours*nNodes];
    aux.sol = new int [nTours*nNodes];
    for (i = 0; i < nTours*nNodes; i++){
        current.sol[i] = 0;
        best.sol[i] = 0;
        aux.sol[i] = 0;
    }

    current.tours = new int [routes];
    best.tours = new int [routes];
    aux.tours = new int [routes];
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

    // Deprecated
    current.sol[((node1-1)*nNodes) + node2-1] = -1;
    best.sol[((node1-1)*nNodes) + node2-1] = -1;
    aux.sol[((node1-1)*nNodes) + node2-1] = -1;
}

/** insertAllowedNodes
    *
    * Method to insert in a list allowed nodes,
    * data set is required.
    * 
    * @param node, Row node.
    * @param allowed_node, Link connected node.
    *
    */
void Data::insertAllowedNodes(int node, int allowed_node){

    allowed_nodes.push_front(allowed_node);
    test.push_front(make_pair(node, allowed_node));
}

/** generateSol
    *
    * Initialise sol
    * 
    */
void Data::generateSol(void){
    int i, j = 1, k, t = 0, valid = 0;
    current.fo = 0;
    // For each tour define covered nodes
    for (i = 1; i <= nTours; i++){
        // Max length of route
        for (k = 0; k < nNodes; k++){
            // If node not asigned and random probability, search new node
            if (randChoice()){
                // Initial route node, only the first time per route
                if (!t)
                    t = nRand(nNodes);
                while(!valid){
                    j = nRand(nNodes);
                    if ((isConnected(t, j)) && !(alreadyExists(i, j))){
                        valid = 1;
                    }
                    else{
                        if (isFinished(i, t))
                            k = nNodes;
                            break;
                    }
                }
                if (valid){
                    current.sol[indexSol(i,t)] = j;
                    current.fo += timetable[indexTables(t, j)];
                    t = j;
                }
            }
            valid = 0;
        }
        t = 0;
    }
    copyCurrentToBest();
}


/** iterateSol
    *
    * Iterates on current sol
    *
    */
void Data::iterateSol(void){
    //int i, j, aux_i, aux_j, aux_b = 0, found = 0;

    int route;

    // Random condition
    if (!randChoice()){
        route = nRand(nTours);
        int pre, beg;
        beg = tourBegin(route);
        pre = previousConnection(route, beg);
        if (pre > 0)
            current.sol[indexSol(route, pre)] = beg;
    }
    else{
        cout << "To define more operators" << endl;
        /*
        route = nRand(nTours);
        while(!found){
            node = nRand(nNodes);
            if ((alreadyExists(route, node)) || (tourBegin(route) == node)){

            }
        }
        */
    }

// FIXME
    /*

    for (i = 1; i <= nTours; i++){
        for (j = 1; j <=nNodes; j++){
            if (current.sol[indexSol(i, j)] > 0){
                while(!found){
                    aux_i = nRand(nTours);
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
    */
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
    for (i = 1; i <= nTours; i++){
        for (j = 1; j <= nNodes; j++){
            if (current.sol[indexSol(i,j)] > 0){
                // FIXME: nTours
                current.fo += timetable[indexTables(current.sol[indexSol(i,j)], j)];
            }
        }
    }
}

/** copyCurrentToBest
    *
    * Copy current sol to best.
    * 
    */
void Data::copyCurrentToBest(void){
    
    best.fo = current.fo;
    for (int i = 0; i < nTours*nNodes; i++)
        best.sol[i] = current.sol[i];
    for (int i = 0; i < nTours; i++)
        best.tours[i] = current.tours[i];
}

/** copyCurrentToAux
    *
    * Copy current sol to aux.
    * 
    */
void Data::copyCurrentToAux(void){
    
    aux.fo = current.fo;
    for (int i = 0; i < nTours*nNodes; i++)
        aux.sol[i] = current.sol[i];
    for (int i = 0; i < nTours; i++)
        aux.tours[i] = current.tours[i];
}

/** copyAuxToCurrent
    *
    * Copy aux sol to current.
    * 
    */
void Data::copyAuxToCurrent(void){
    
    current.fo = aux.fo;
    for (int i = 0; i < nTours*nNodes; i++)
        current.sol[i] = aux.sol[i];
    for (int i = 0; i < nTours; i++)
        current.tours[i] = aux.tours[i];
}


/** isConnected
    *
    * Return 0 if node1 isn't connected to node2
    *
    * @param node1, origin node
    * @param node2, destination node
    *
    * return 1 if both nodes are connected, otherwise 0
    */
int Data::isConnected(int node1, int node2){

    for(it2 = test.begin(); it2 != test.end(); it2++){
        int a = (*it2).first;
        int b = (*it2).second;
        if ((a == node1) && (b == node2))
            return 1;
    }
    return 0;
}

/**
    *
    */
int Data::isLinkedRoad(int node1, int node2, int node3){

    int first = 0, second = 0;
    for(it2 = test.begin(); it2 != test.end(); it2++){
        int a = (*it2).first;
        int b = (*it2).second;
        if ((a == node1) && (b == node2))
            first = 1;
        if ((a == node2) && (b == node3))
            second = 1;
    }
    if (first && second)
        return 1;
    else
        return 0;
}

/**
    *
    */
int Data::alreadyExists(int route, int node){
    int i;
    for (i = 1; i <= nNodes; i++)
        if (current.sol[indexSol(route, i)] == node)
            return 1;
    return 0;
}

/**
    *
    */
int Data::isFinished(int route, int node){

    for (it2 = test.begin(); it2 != test.end(); it2++){
        int a = (*it2).first;
        int b = (*it2).second;
        if (a == node)
            if (!alreadyExists(route, b))
                return 0;
    }
    return 1;
}

/**
    *
    */
int Data::previousConnection(int route, int node){

    for (it2 = test.begin(); it2 != test.end(); it2++){
        int a = (*it2).first;
        int b = (*it2).second;
        if (b == node)
            if (!alreadyExists(route, a))
                return a;
    }
    return 0;
}

/**
    *
    */
int Data::tourLen(int tour){
    int len = 1;
    // len = 1 because considers the initial node
    for (int i = 1; i <= nNodes; i++)
        if (current.sol[indexSol(tour, i)] >0)
            len += 1;
    return len;
}


/**
    *
    */
int Data::tourBegin(int tour){
    int i;
    for (i = 1; i <= nNodes; i++)
        if (current.sol[indexSol(tour, i)] > 0)
            if (!alreadyExists(tour, i))
                return i;
    return 0;
}

/**
    *
    */
int Data::tourEnd(int tour){

    for (int i = 1; i <= nNodes; i++)
        if (current.sol[indexSol(tour, i)] == 0)
            if (alreadyExists(tour, i))
                return i;
    return 0;
}


/**
    *
    */
int Data::previousNode(int tour, int node){

    for (int i = 1; i <= nNodes; i++)
        if (current.sol[indexSol(tour, i)] == node)
                return i;
    return 0;
}

void Data::printCurrent(void){
    int i, j;
    cout << "----------- SOLUTIONS -------------" << endl;
    cout << "CURRENT FO: " << current.fo << endl;
    for (j = 1; j <= nNodes; j++)
    {
        cout << j << "\t"; 
    }
    cout << endl;
    for (j = 1; j <= nNodes; j++)
    {
        cout << "-" << "\t"; 
    }
    cout << endl;
    for (i = 1; i <= nTours; i++)
    {
        cout << "Begin Node: " << tourBegin(i) << endl;
        cout << "End Node: " << tourEnd(i) << endl;
        cout << "Len Node: " << tourLen(i) << endl;
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
    for (i = 1; i <= nTours; i++)
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

    cout << "----------- ALLOWED -------------" << endl;
    for(it = allowed_nodes.begin(); it != allowed_nodes.end(); it++)
             cout << *it << " ";
    cout << endl;
    
    cout << "----------- ALLOWED TEST -------------" << endl;
    for(it2 = test.begin(); it2 != test.end(); it2++){
        int a = (*it2).first;
        int b = (*it2).second;
        cout << a << " " << b << " ";
        cout << endl;
    }
    cout << endl;

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

    return ((node2-1)*nTours) + node1-1;
}

/** indexSol
    *
    * Index Sol (tour, node)
    *  
    * @param tour
    * @param node
    *
    * @return previous bus stop
    */
int Data::indexSol(int tour, int node){
    
    return (((tour-1)*nNodes) + node -1);
}

/** indexTour
    *
    * Index Tour (nTour)
    *  
    * @param tour
    *
    * @return nbuses
    */
int Data::indexTour(int tour){
    
    return (tour-1);
}

#endif
