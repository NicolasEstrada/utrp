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
#include <iomanip>
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
#define CURRENT 0
#define BEST 1
#define AUX 2
#define MISSNODEPENALTY 100000
#define ROUTECOST 1000

using namespace std;

struct Solution{
	double      fo;  // Objective function value
    int         pairs; // Uncovered demand node pairs
	int         *sol;  // Solution: Designed tour in each node
    //int         *tours; // Numbers of buses by tour
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
        
        int         nIter, uncoverDemand;
        int         nNodes, nLinks, nBuses, nTours;
        
        list<pair<int, int> > allowed;
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
        int alreadyExists(int route, int node, int opt = 0);
        int existsWithoutBeg(int route, int node, int opt = 0);
        int isFinished(int route, int node, int opt = 0);
        int previousConnection(int route, int node, int opt = 0);
        int nextConnection(int route, int node, int opt = 0);

        int tourLen(int tour, int opt = 0);
        int tourBegin(int tour, int opt = 0);
        int tourEnd(int tour, int opt = 0);
        int previousNode(int tour, int node, int opt = 0);
        int indexNode(int tour, int node, int opt = 0);

        int shortestRoute(void);
        void cleanRoute(int route);
        int inEntireRed(int node, int opt = 0);
        int missingNode(int opt = 0);
        int countMissingNode(void);

        void addInitNode(int route);
        void removeEndNode(int route);
        void replaceNode(int route, int node);
        void removeNode(int route, int node);

        int inRoute(int route, int node1, int node2);
        int getMinCost(int node1, int node2);
        int getCostShortestRoute(int node1, int node2);
        int getDemandFO(void);

        void printCurrent(void);
        void printData(void);
        void printRoutes(void);
        void fileRoutes(char *path);
        void fileRoutesLatex(char* path,  int seed);

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
    uncoverDemand = 0;
    current.fo = 0;
    current.pairs = 0;
    current.sol = 0;
    //current.tours = 0;
    best.fo = 0;
    best.pairs = 0;
    best.sol = 0;
    //best.tours = 0;
    aux.fo = 0;
    aux.pairs = 0;
    aux.sol = 0;
    //aux.tours = 0;
	timetable = NULL;
	demandtable = NULL;
}

/** Default destructor.
    */
Data::~Data(){
	
    delete [] current.sol;
    delete [] best.sol;
    delete [] aux.sol;
    //delete [] current.tours;
    //delete [] best.tours;
    //delete [] aux.tours;
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

    //cout << "nTours: " << nTours << endl; 

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

    /*
    current.tours = new int [routes];
    best.tours = new int [routes];
    aux.tours = new int [routes];
    for (i = 1; i <= nTours; i++){
        current.tours[indexTour(i)] = nRand((int) nBuses/nTours);
        best.tours[indexTour(i)] = nRand((int) nBuses/nTours);
        aux.tours[indexTour(i)] = nRand((int) nBuses/nTours);
    }
    */
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

    allowed.push_front(make_pair(node, allowed_node));
}

/** generateSol
    *
    * Initialise sol
    * 
    */
void Data::generateSol(void){
    // FIXME: add all connecting graph condition
    int i, j = 1, k, t = 0, valid = 0;
    current.fo = 0;
    // For each tour define covered nodes
    for (i = 1; i <= nTours; i++){
        // Max length of route
        for (k = 0; k < nNodes; k++){
            // If node not asigned and random probability, search new node
            // Initial route node, only the first time per route
            if (!t)
                t = nRand(nNodes);
            while(!valid){ 
                j = nRand(nNodes);
                if ((isConnected(t, j)) && !(alreadyExists(i, j)) && (j != indexNode(i, t))){
                    valid = 1;
                }
                else{
                    if (isFinished(i, t)){
                        k = nNodes;
                        break;
                    }
                }
            }
            if (valid){
                current.sol[indexSol(i,t)] = j;
                //current.fo += timetable[indexTables(t, j)];
                t = j;
            }

            valid = 0;
        }
        t = 0;
    }

    int m = missingNode();
    while(m){
        int r = shortestRoute();
        cleanRoute(r);
        for (k = 0; k < nNodes; k++){
            // If node not asigned and random probability, search new node
            // Initial route node, only the first time per route
            if (!t)
                t = m;
            while(!valid){
                j = nRand(nNodes);
                if ((isConnected(t, j)) && !(alreadyExists(r, j)) && (j != indexNode(r, t))){
                    valid = 1;
                }
                else{
                    if (isFinished(r, t)){
                        k = nNodes;
                        break;
                    }
                }
            }
            if (valid){
                current.sol[indexSol(r,t)] = j;
                //current.fo += timetable[indexTables(t, j)];
                t = j;
            }

            valid = 0;
        }
        t = 0;
        m = missingNode();
    }

    refreshFO();
    copyCurrentToBest();
}


/** iterateSol
    *
    * Iterates on current sol
    *
    */
void Data::iterateSol(void){
    //int i, j, aux_i, aux_j, aux_b = 0, found = 0;

    int route, node;

    switch(randChoice()){
        case 0:
            // addInitNode();
            route = nRand(nTours);
            addInitNode(route);
            break;

        case 1:
            // removeEndNode();
            route = nRand(nTours);
            removeEndNode(route);
            break;

        case 2:
            route = nRand(nTours);
            node = nRand(nNodes);
            //replaceNode(route, node);
            break;

        case 3:
            route = nRand(nTours);
            node = nRand(nNodes);
            removeNode(route, node);
            break;

        default:
            break;
    }

        //cout << "To define more operators" << endl;
        /*
        route = nRand(nTours);
        while(!found){
            node = nRand(nNodes);
            if ((alreadyExists(route, node)) || (tourBegin(route) == node)){

            }
        }
        */

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
                current.fo += timetable[indexTables(j, current.sol[indexSol(i,j)])]*ROUTECOST;
            }
        }
    }
    int count = countMissingNode();
    if (count)
        current.fo += MISSNODEPENALTY*count;
    current.fo += getDemandFO();
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
    /*
    for (int i = 0; i < nTours; i++)
        best.tours[i] = current.tours[i];
        */
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
    /*
    for (int i = 0; i < nTours; i++)
        aux.tours[i] = current.tours[i];
        */
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
    /*
    for (int i = 0; i < nTours; i++)
        current.tours[i] = aux.tours[i];
        */
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

    for(it2 = allowed.begin(); it2 != allowed.end(); it2++){
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
    for(it2 = allowed.begin(); it2 != allowed.end(); it2++){
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
int Data::alreadyExists(int route, int node, int opt){
    int i, auxi;
    switch(opt){
        case 0:
            for (i = 1; i <= nNodes; i++){
                    //if (current.sol[indexSol(route, i)] == node)
                    auxi = current.sol[indexSol(route, i)];
                    if  (auxi > 0)
                        if (auxi == node || i == node)
                            return 1;
            }
            return 0;

        case 1:
            for (i = 1; i <= nNodes; i++)
                if (best.sol[indexSol(route, i)] == node)
                    return 1;
            return 0;

        case 2:
            for (i = 1; i <= nNodes; i++)
                if (aux.sol[indexSol(route, i)] == node)
                    return 1;
            return 0;

        default:
            return 0;
    }
}

/**
    *
    */
int Data::existsWithoutBeg(int route, int node, int opt){
    int i;
    switch(opt){
        case 0:
            for (i = 1; i <= nNodes; i++)
                if (current.sol[indexSol(route, i)] == node)
                    return 1;
            return 0;

        case 1:
            for (i = 1; i <= nNodes; i++)
                if (best.sol[indexSol(route, i)] == node)
                    return 1;
            return 0;

        case 2:
            for (i = 1; i <= nNodes; i++)
                if (aux.sol[indexSol(route, i)] == node)
                    return 1;
            return 0;

        default:
            return 0;
    }
}

/**
    *
    */
int Data::isFinished(int route, int node, int opt){

    for (it2 = allowed.begin(); it2 != allowed.end(); it2++){
        int a = (*it2).first;
        int b = (*it2).second;
        if (a == node)
            if ((!alreadyExists(route, b, opt)) && (indexNode(route, node, opt) != b))
                return 0;
    }
    return 1;
}

/**
    *
    */
int Data::previousConnection(int route, int node, int opt){

    for (it2 = allowed.begin(); it2 != allowed.end(); it2++){
        int a = (*it2).first;
        int b = (*it2).second;
        if (b == node)
            if (!alreadyExists(route, a, opt))
                return a;
    }
    return 0;
}

/**
    *
    */
int Data::nextConnection(int route, int node, int opt){
    for (it2 = allowed.begin(); it2 != allowed.end(); it2++){
        int a = (*it2).first;
        int b = (*it2).second;
        if (a == node)
            if (!alreadyExists(route, b, opt))
                return b;
    }
    return 0;
}

/**
    *
    */
int Data::tourLen(int tour, int opt){
    int len = 1;
    // len = 1 because considers the initial node
    switch(opt){
        case 0:
            for (int i = 1; i <= nNodes; i++)
                if (current.sol[indexSol(tour, i)] >0)
                    len += 1;
            return len;
        case 1:
            for (int i = 1; i <= nNodes; i++)
                if (best.sol[indexSol(tour, i)] >0)
                    len += 1;
            return len;
        case 2:
            for (int i = 1; i <= nNodes; i++)
                if (aux.sol[indexSol(tour, i)] >0)
                    len += 1;
            return len;
        default:
            return 0;
    }
}

/**
    *
    */
int Data::tourBegin(int tour, int opt){
    int i;
    switch(opt){
        case 0:
            for (i = 1; i <= nNodes; i++)
                if (current.sol[indexSol(tour, i)] > 0)
                    if (!existsWithoutBeg(tour, i, opt))
                        return i;
            return 0;

        case 1:
            for (i = 1; i <= nNodes; i++)
                if (best.sol[indexSol(tour, i)] > 0)
                    if (!existsWithoutBeg(tour, i, opt))
                        return i;
            return 0;
        case 2:
            for (i = 1; i <= nNodes; i++)
                if (aux.sol[indexSol(tour, i)] > 0)
                    if (!existsWithoutBeg(tour, i, opt))
                        return i;
            return 0;
        default:
            return 0;
    }
}

/**
    *
    */
int Data::tourEnd(int tour, int opt){

    switch(opt){
        case 0:
            for (int i = 1; i <= nNodes; i++)
                if (current.sol[indexSol(tour, i)] == 0)
                    if (alreadyExists(tour, i, opt))
                        return i;
            return 0;

        case 1:
            for (int i = 1; i <= nNodes; i++)
                if (best.sol[indexSol(tour, i)] == 0)
                    if (alreadyExists(tour, i, opt))
                        return i;
            return 0;

        case 2:
            for (int i = 1; i <= nNodes; i++)
                if (aux.sol[indexSol(tour, i)] == 0)
                    if (alreadyExists(tour, i, opt))
                        return i;
            return 0;

        default:
            return 0;
    }
}


/**
    *
    */
int Data::previousNode(int tour, int node, int opt){

    switch(opt){
        case 0:
            for (int i = 1; i <= nNodes; i++)
                if (current.sol[indexSol(tour, i)] == node)
                        return i;
            return 0;

        case 1:
            for (int i = 1; i <= nNodes; i++)
                if (best.sol[indexSol(tour, i)] == node)
                        return i;
            return 0;

        case 2:
            for (int i = 1; i <= nNodes; i++)
                if (aux.sol[indexSol(tour, i)] == node)
                        return i;
            return 0;

        default:
            return 0;
    }
}

/**
    *
    */
int Data::indexNode(int tour, int node, int opt){

    switch(opt){
        case 0:
            for (int i = 1; i <= nNodes; i++)
                if (current.sol[indexSol(tour, i)] == node)
                    return i;
            return 0;

        case 1:
            for (int i = 1; i <= nNodes; i++)
                if (best.sol[indexSol(tour, i)] == node)
                    return i;
            return 0;

        case 2:
            for (int i = 1; i <= nNodes; i++)
                if (aux.sol[indexSol(tour, i)] == node)
                    return i;
            return 0;

        default:
            return 0;
    }
}

/**
    *
    */
int Data::shortestRoute(void){

    int len_route = nNodes + 1;
    int index_route = 0;
    for (int i = 1; i <= nTours; i++)
        if (tourLen(i) < len_route){
            len_route = tourLen(i);
            index_route = i;
        }
    return index_route;
}

/**
    *
    */
void Data::cleanRoute(int route){

    for (int i = 1; i <= nNodes; i++)
        current.sol[indexSol(route, i)] = 0;
}


/**
    *
    */
int Data::inEntireRed(int node, int opt){

    switch (opt){
        case 0:
            for (int i = 1; i <= nTours; i++){
                if (tourBegin(i, opt) == node )
                    return 1;
                for (int j = 1; j <= nNodes; j++)
                    if (current.sol[indexSol(i, j)] == node)
                        return 1;
            }
            return 0;

        case 1:
            for (int i = 1; i <= nTours; i++){
                if (tourBegin(i, opt) == node )
                    return 1;
                for (int j = 1; j <= nNodes; j++)
                    if (best.sol[indexSol(i, j)] == node)
                        return 1;
            }
            return 0;

        case 2:
            for (int i = 1; i <= nTours; i++){
                if (tourBegin(i, opt) == node )
                    return 1;
                for (int j = 1; j <= nNodes; j++)
                    if (aux.sol[indexSol(i, j)] == node)
                        return 1;
            }
            return 0;

        default:
            break;
    }
    return 0;
}

/**
    *
    */
int Data::missingNode(int opt){

    for (int i = 1; i <= nNodes; i++)
        if (!inEntireRed(i, opt))
            return i;
    return 0;

}

/**
    *
    */
int Data::countMissingNode(void){

    int c = 0;
    for (int i = 1; i <= nNodes; i++)
        if (!inEntireRed(i))
            c += 1;
    return c;
}

/**
    *
    */
void Data::addInitNode(int route){
    int beg, pre;
    beg = tourBegin(route);
    pre = previousConnection(route, beg);
    if (pre > 0)
        current.sol[indexSol(route, pre)] = beg;
}

/**
    *
    */
void Data::removeEndNode(int route){
    int end, pre;
    end = tourEnd(route);
    pre = previousNode(route, end);
    if (tourLen(route) > 2)
        current.sol[indexSol(route, pre)] = 0;
}

/**
    *
    */
void Data::replaceNode(int route, int node){

    int pre, next;
   
    if (tourBegin(route) == node){
        next = current.sol[indexSol(route, node)];
        pre = previousConnection(route, next);
        if (pre){
            current.sol[indexSol(route, node)] = 0;
            current.sol[indexSol(route, pre)] = next;
        }
    }
    else{
        if (tourEnd(route) == node){
            pre = previousNode(route, node);
            next = nextConnection(route, pre);
            if (pre)
                current.sol[indexSol(route, pre)] = next;
        }
        else{
            pre = previousNode(route, node);
            next = current.sol[indexSol(route, node)];
            for (int i = 1; i <= nNodes; i++){
                if ((isLinkedRoad(pre, i ,next)) && (i != node) && (!alreadyExists(route, i))){
                    current.sol[indexSol(route, node)] = i;
                    break;
                }
            }
        }
    }
}

/**
    *
    */
void Data::removeNode(int route, int node){

    int pre, next;
    if (tourLen(route) <= 2)
        return;
    if (tourBegin(route) == node)
        current.sol[indexSol(route, node)] = 0;
    else
        if (tourEnd(route) == node)
            current.sol[indexSol(route, previousNode(route, node))] = 0;
        else{
            pre = previousNode(route, node);
            next = current.sol[indexSol(route, node)];
            if (isConnected(pre, next)){
                current.sol[indexSol(route, node)] = 0;
                current.sol[indexSol(route, pre)] = next;
            }
        }
}

/**
    *
    * return int, first value of route.
    */
int Data::inRoute(int route, int node1, int node2){

    int cursor, n1 = 0, n2 = 0;
    cursor = tourBegin(route);
    while(cursor){
        if (cursor == node1){
            if (n2 != 0)
                return n2;
            else
                n1 = node1;
        }
        if (cursor == node2)
        {
            if (n1 != 0)
                return n1;
            else
                n2 = node2;
        }
        cursor = current.sol[indexSol(route, cursor)];
    }
    return 0;
}

/**
    *
    */
int Data::getMinCost(int node1, int node2){

    int cur, next, min_cost = 10000000, cost, t = 0;
    cost = 0;
    for (int i = 1; i <= nTours; i++){
        cur = inRoute(i, node1, node2);
        //cout << i << " | " << cur << endl;
        if (cur){
            next = current.sol[indexSol(i, cur)];
            while (cur && next){
                t = timetable[indexTables(cur, next)];
                //cout << cur << " | " << next << " = " << t << endl;
                cost += t;
                cur = next;
                next = current.sol[indexSol(i, cur)];
            }
            if ((cost < min_cost) && (cost != 0))
                min_cost = cost;
            cost = 0;
        }
    }

    if (min_cost == 10000000)
        return 0;
    else
        return min_cost;
}


/**
    *
    */
int Data::getCostShortestRoute(int node1, int node2){

    int min_cost, p;
    min_cost = getMinCost(node1, node2);
    p = demandtable[indexTables(node1, node2)];
    //cout << node1 << "   ||   " << node2 << endl;
    //cout << min_cost << " ->>>> " << p << endl;
    return min_cost*p;
}

/**
    *
    */
int Data::getDemandFO(void){

    int total = 0, count = 0, cost;
    for (int i = 1; i <= nNodes; i++){
        for (int j = i+1; j <= nNodes; j++){
            cost = getCostShortestRoute(i, j);
            if (!cost)
                count++;
            total += cost;
        }
    }
    uncoverDemand = count;
    current.pairs = count;
    //cout << "DEMAND FO: " << total << " | UNCOVER: " << count << endl;
    return total;
}

/**
    *
    */
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
    /******************************************************
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
    

    int allow = 0;
    cout << "----------- ALLOWED -------------" << endl;
    for(it2 = allowed.begin(); it2 != allowed.end(); it2++){
        int a = (*it2).first;
        int b = (*it2).second;
        //cout << a << "| |" << b << endl;
        if (allow == 0){
            allow = a;
            cout << a << ": ";
        }
        else{
            if (allow == a){
                cout << " - " << b ;
            }
            else{
                allow = a;
                cout << "," << endl << a << ": " << b << " - ";
            }
        }
    }
    cout << endl;
    ********************************************************/

    /**********************************************************    
    cout << "----------- ALLOWED TEST -------------" << endl;
    for(it2 = allowed.begin(); it2 != allowed.end(); it2++){
        int a = (*it2).first;
        int b = (*it2).second;
        cout << a << " " << b << " ";
        cout << endl;
    }
    cout << endl;
    *********************************************************/

    cout << "BEST FO: " << best.fo << endl;
    
    for (i = 1; i <= nTours; i++)
    {
        //cout << "Begin Node: " << tourBegin(i) << endl;
        //cout << "End Node: " << tourEnd(i) << endl;
        //cout << "Len Node: " << tourLen(i) << endl;
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
    /*
    cout << "--- TOURS ---" << endl;
    for (i = 1; i <= nTours; i++){
        if (best.tours[indexTour(i)] < 0)
            cout << best.tours[indexTour(i)];
        cout << "\t";
    }
    cout << endl;
    */
}

/** printRoutes
    *
    * This method prints all routes within best solution
    *
    * @return void
    */
void Data::printRoutes(void){
    int i, ind;
    cout << "----------- BEST SOLUTION -------------" << endl;
    cout << "BEST FO: " << best.fo << endl;
    for (i = 1; i <= nTours; i++)
    {
        //cout << "|||||||||| Route " << i << " ||||||||||" << endl;
        //cout << "Begin Node: " << tourBegin(i, BEST) << endl;
        //cout << "End Node: " << tourEnd(i, BEST) << endl;
        //cout << "Len Node: " << tourLen(i, BEST) << endl;

        ind = tourBegin(i, BEST);
        cout << ind;
        while(ind){
            ind = best.sol[indexSol(i, ind)];
            if (ind != 0)
                cout << " -> " << ind;
        }
        cout << endl;
    }
}

/** printRoutes
    *
    * This method prints all routes within best solution
    *
    * @return void
    */
void Data::fileRoutes(char* path){
    int i, ind;
    ofstream output, routput;
    output.open(path, fstream::in | fstream::out | fstream::app);
    ostringstream oss (ostringstream::out);
    string s;
    oss << path << ".resume";
    s = oss.str();
    cout << s << " OK ";
    routput.open(s.c_str(), fstream::in | fstream::out | fstream::app);
    oss.str("");

    //output << "----------- BEST SOLUTION -------------" << endl;
    output << "BEST FO: " << best.fo << endl;
    routput << setprecision(3) << "BEST FO: " << best.fo;

    float average = 0;

    for (i = 1; i <= nTours; i++)
    {
        output << "| Route " << i << " |";
        //output << " Begin Node: " << tourBegin(i, BEST);
        //output << " End Node: " << tourEnd(i, BEST);
        output << " Len Route: " << tourLen(i, BEST) << " [ ";
        average += (float) tourLen(i, BEST);
        ind = tourBegin(i, BEST);
        output << ind;
        while(ind){
            ind = best.sol[indexSol(i, ind)];
            if (ind != 0)
                output << " -> " << ind;
        }
        output << " ]" << endl;
    }
    average /= nTours;
    routput << setprecision(2) << "\t" << " | Avg Len: " << average << "\t";
    routput << " | Mis Nodes: " << missingNode(BEST) << "\t";
    routput << " | Mis 2Nodes DD: " << best.pairs << endl;
    output.close();
    routput.close();
}

/** printRoutes
    *
    * This method prints all routes within best solution
    *
    * @return void
    */
void Data::fileRoutesLatex(char* path, int seed){
    int i;
    ofstream routput;
    ostringstream oss (ostringstream::out);
    string s;
    oss << path << ".resume.ltx";
    s = oss.str();
    cout << s << " OK ";
    routput.open(s.c_str(), fstream::in | fstream::out | fstream::app);
    oss.str("");

    //output << "----------- BEST SOLUTION -------------" << endl;
    //output << "BEST FO: " << best.fo << endl;

    float average = 0;

    for (i = 1; i <= nTours; i++)
    {
        average += (float) tourLen(i, BEST);
        //ind = tourBegin(i, BEST);
    }
    average /= nTours;
    routput << seed << " & " << nTours << " & ";
    routput << setprecision(2) << average << " & ";
    routput << missingNode(BEST) << " & ";
    routput << best.pairs << " & ";
    routput << setprecision(3) << best.fo << " \\\\ \\hline" << endl;

    routput.close();
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

    return ((node2-1)*nNodes) + node1-1;
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
