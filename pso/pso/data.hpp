//
//  data.cpp
//  pso
//
//  Created by Nicolás Estrada on 7/10/16.
//  Copyright © 2016 Nicolás Estrada. All rights reserved.
//

#ifndef DATA_H_
#define DATA_H_

#include <stdio.h>
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
#include <unistd.h>

//using namespace std;

#include "functions.hpp"

#define ITER_DEFAULT 1000

class Data{
    
  private:
    
    int *timetable, *demandtable;
    int *solutionSet, *localBest, *globalBest;
    
  public:
    // default constructor
    Data(void);
    // default destructor
    ~Data(void);

    int nIter, uncoverDemand;
    int nNodes, nLinks, nRoutes, populationSize, seed;

    std::list<std::pair<int, int> > allowed;
    std::list<std::pair<int, int> > ::iterator it2;

    void setData(int n_nodes, int n_links, int n_routes, int population_size, int input_seed);

    // visualisation functions
    void printTime(void);
    void printDemand(void);
    void printSolutionSet(void);
    void printLocalBest(void);
    void printGlobalBest(void);
    void printAllowedNodes(void);
  
    // data initialization
    void insertTime(int x, int y, int time);
    void insertDemand(int x, int y, int demand);

    // initial solution generation functions
    void generateSolutionSet(void);
  
    // node-routes-solution tools
    void insertAllowedNodes(int node, int allowed_node);

    int isConnected(int node1, int node2);
    int isLinkedRoad(int node1, int node2, int node3);
  
    int alreadyExists(int particle, int route, int node);
    int existsWithBeg(int particle, int route, int node);
    int isFinished(int particle, int route, int node);
    int previousConnection(int particle, int route, int node);
    int nextConnection(int particle, int route, int node);
  
    int routeLen(int particle, int route);
    int routeBegin(int particle, int route);
    int routeEnd(int particle, int route);
    int previousNode(int particle, int route, int node);
    int indexNode(int particle, int route, int node);
    int indexRoute(int particle, int route);
    int indexParticle(int particle);

    int inEntireRed(int particle, int node);
    int missingNode(int particle);
    int shortestRoute(int particle);
    void cleanRoute(int particle, int route);
  
};


Data::Data(){
  // init Data class
  nIter = 0;
  uncoverDemand = 0;
  nNodes = 0;
  nLinks = 0;
  nRoutes = 0;
  populationSize = 0;
  seed = 0;

  timetable = NULL;
  demandtable = NULL;
  solutionSet = NULL;
  localBest = NULL;
  globalBest = NULL;

}


Data::~Data(){
  // destroy Data class
  delete [] timetable;
  delete [] demandtable;
  delete [] solutionSet;
  delete [] localBest;
  delete [] globalBest;

}


void Data::setData(int n_nodes, int n_links, int n_routes, int population_size, int input_seed){
  
  int i;
  
  nNodes = n_nodes;
  nLinks = n_links;
  nRoutes = n_routes;
  populationSize = population_size;
  seed = input_seed;
  
  timetable = new int[nNodes * nNodes];
  for (i=0; i < nNodes * nNodes; i++)
    timetable[i] = 0;

  demandtable = new int[nNodes * nNodes];
  for (i=0; i < nNodes * nNodes; i++)
    demandtable[i] = 0;

  solutionSet = new int[populationSize * nRoutes * nNodes];
  for (i=0; i < populationSize * nRoutes * nNodes; i++)
    solutionSet[i] = 0;
  
  localBest = new int[nRoutes * nNodes];
  for (i=0; i < nRoutes * nNodes; i++)
    localBest[i] = 0;
  
  globalBest = new int[nRoutes * nNodes];
  for (i=0; i < nRoutes * nNodes; i++)
    globalBest[i] = 0;
  
}


void Data::printTime(void){

  std::cout << std::setw((nNodes * 2) + 8) << "Time Travel Table" << std::endl;
  
  for (int i = 0; i < nNodes; i++){
    for (int j = 0; j < nNodes; j++){
      std::cout << std::setw(4) << timetable[i * nNodes + j];
    }
    std::cout << std::endl;
  }
}


void Data::printDemand(void){

  std::cout << std::setw((nNodes * 2) + 9) << "Demand Travel Table" << std::endl;
  
  for (int i = 0; i < nNodes; i++){
    for (int j = 0; j < nNodes; j++){
      std::cout << std::setw(4) << demandtable[i * nNodes + j];
    }
    std::cout << std::endl;
  }
}


void Data::insertTime(int x, int y, int time){

  timetable[((x - 1) * nNodes) + y -1] = time;
}


void Data::insertDemand(int x, int y, int demand){
  
  demandtable[((x - 1) * nNodes) + y - 1] = demand;
}


void Data::printSolutionSet(void){
  
  std::cout << std::setw((nNodes * 2) + 6) << "Solution Set" << std::endl << std::endl;
  
  for (int i = 1; i <= populationSize; i++){
    
    std::cout << "Particle # " << i << std::endl;

    for (int j = 1; j <= nRoutes; j++){
      for (int k = 1; k <= nNodes; k++){
        
        std::cout << std::setw(4) << solutionSet[indexNode(i, j, k)];
      }
      
      std::cout << std::endl;
    }
  }
}


void Data::printLocalBest(void){
  
  std::cout << std::setw((nNodes * 2) + 5) << "Local Best" << std::endl << std::endl;
  
  for (int i = 0; i < nRoutes; i ++){
    for (int j = 0; j < nNodes; j++){
      
      std::cout << std::setw(4) << localBest[((i * nRoutes) + j)];
    }

    std::cout << std::endl;
  }
}


void Data::printGlobalBest(void){
  
  std::cout << std::setw((nNodes * 2) + 5) << "Global Best" << std::endl << std::endl;
  
  for (int i = 0; i < nRoutes; i ++){
    for (int j = 0; j < nNodes; j++){
      
      std::cout << std::setw(4) << globalBest[((i * nRoutes) + j)];
    }
    
    std::cout << std::endl;
  }
}


void Data::printAllowedNodes(void){

  int origin_node = 0, destination_node = 0, org_end = 0, dest_end = 0;
  std::cout << std::setw((nNodes * 2) + 6) << "Allowed Nodes" << std::endl << std::endl;
  
  for (it2 = allowed.begin(); it2 != allowed.end(); it2++){
    if (origin_node == 0){
      origin_node = (*it2).first;
      destination_node = (*it2).second;
    }

    if (origin_node != (*it2).first){
      origin_node =(*it2).first;
      org_end = 1;
    }

    if (destination_node != (*it2).second){
    
      destination_node = (*it2).second;
      dest_end = 1;
    }
    
    if (org_end && dest_end){
      std::cout << std::endl;
      org_end = 0;
      dest_end = 0;
    }
    else{
      org_end = 0;
      dest_end = 0;
    }
    
    std::cout << std::setw(2) << "( " << (*it2).first << ", " << (*it2).second << " ) ";
  }
}


void Data::generateSolutionSet(void){
  int auxNode = 0, valid = 0, m = 0;
  int j = 1;

  for (int p = 1; p <= populationSize; p++){
    for (int i = 1; i <= nRoutes; i ++){
      // solution initialization for each particle
    
      for (int k = 1; k <= nNodes; k++){
      
        if (!auxNode){
          auxNode = nRand(nNodes);
        }
    
        while (!valid){
          j = nRand(nNodes);

          if ((isConnected(auxNode, j))
                && !(alreadyExists(p, i, j))
                && (j != previousNode(p, i, auxNode))){
            valid = 1;
          }
          else {
            if (isFinished(p, i, auxNode)){
              k = nNodes;
              break;
            }
          }
        }

        if (valid){
          solutionSet[indexNode(p, i, auxNode)] = j;
          auxNode = j;
        }
        
        valid = 0;
      }

      auxNode = 0;
    }
  }

  valid = 0;
  auxNode = 0;
  j = 1;
  
  for (int p = 1; p <= populationSize; p++){
    
    m = missingNode(p);
    int shortRoute = shortestRoute(p);
    
    while (m) {
      
      cleanRoute(p, shortRoute);
      
      for (int i = 1; i <= nNodes; i++){
        
        if (!auxNode)
          auxNode = m;
        while (!valid){
          
          j = nRand(nNodes);
          
          if ((isConnected(auxNode, j))
              && !(alreadyExists(p, shortRoute, j))
              && (j != previousNode(p, shortRoute, auxNode))){
            valid = 1;
          }
          else {
            if (isFinished(p, shortRoute, auxNode)){
              i = nNodes;
              break;
            }
          }
          
        }
        
        if (valid){

          solutionSet[indexNode(p, shortRoute, auxNode)] = j;
          auxNode = j;
        }
        
        valid = 0;
      }

      auxNode = 0;
      m = missingNode(p);
    }
  }
}


void Data::insertAllowedNodes(int node, int allowed_node){
  
  allowed.push_front(std::make_pair(node, allowed_node));
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

/** isLinkedRoad
 *
 * Return 0 if node1, node2 and node 3 are not connected
 *
 * @param node1, origin node
 * @param node2, intermediary node
 * @param node3, destination node
 *
 * return 1 if the nodes are connected, otherwise 0
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


/** alreadyExists
 *
 * Return 0 if the node isn't in the route
 *
 * @param particle, particle to look into
 * @param route, route to look the node
 * @param node, node to check
 *
 * return 1 if node is in route, otherwise 0
 */
int Data::alreadyExists(int particle, int route, int node){
  int auxi;
  
  for (int i = 1; i <= nNodes; i++){
    auxi = solutionSet[indexNode(particle, route, i)];
    if  (auxi > 0)
      if (auxi == node || i == node)
        return 1;

  }

  return 0;
}


/** existsWithBeg
 *
 */
int Data::existsWithBeg(int particle, int route, int node){
  
  for (int i = 1; i <= nNodes; i++)
    if (solutionSet[indexNode(particle, route, i)] == node)
      return 1;
  
  return 0;
}


/** isFinished
 *
 * Return 1 if Route can't be expanded
 *
 * @param particle, particle to lookup
 * @param route, route to analize
 * @param node, check if node has posibilities
 *
 * return 0 if route can't be continued, otherwise 1
 */
int Data::isFinished(int particle, int route, int node){
  
  for (it2 = allowed.begin(); it2 != allowed.end(); it2++){
    int a = (*it2).first;
    int b = (*it2).second;
    if (a == node)
      if ((!alreadyExists(particle, route, b)) && (previousNode(particle, route, node) != b))
        return 0;
  }

  return 1;
}


/** previousConnection
 *
 */
int Data::previousConnection(int particle, int route, int node){
  
  for (it2 = allowed.begin(); it2 != allowed.end(); it2++){
    int a = (*it2).first;
    int b = (*it2).second;
    if (b == node)
      if (!alreadyExists(particle, route, a))
        return a;
  }

  return 0;
}

/** nextConnection
 *
 */
int Data::nextConnection(int particle, int route, int node){

  for (it2 = allowed.begin(); it2 != allowed.end(); it2++){
    int a = (*it2).first;
    int b = (*it2).second;
    if (a == node)
      if (!alreadyExists(particle, route, b))
        return b;
  }

  return 0;
}


/** routeLen
 *
 */
int Data::routeLen(int particle, int route){
  int len = 0;

  for (int i = 1; i <= nNodes; i++)
    if (solutionSet[indexNode(particle, route, i)] > 0)
      len += 1;
  
  if (len)
    return len + 1;
  else
    return len;

}


/** routeBegin
 *
 */
int Data::routeBegin(int particle, int route){

  for (int i = 1; i <= nNodes; i++)
    if (solutionSet[indexNode(particle, route, i)] > 0)
      if (!existsWithBeg(particle, route, i))
        return i;

  return 0;

}


/** routeEnd
 *
 */
int Data::routeEnd(int particle, int route){
  
  for (int i = 1; i <= nNodes; i++)
    if (solutionSet[indexNode(particle, route, i)] == 0)
      if (alreadyExists(particle, route, i))
        return i;

  return 0;
}


/** previousNode
 *
 * Return node pointing to argument node
 *
 * @param particle, particle to lookup
 * @param route, route to analize
 * @param node, node to find its parent
 *
 * return parent's node
 */
int Data::previousNode(int particle, int route, int node){
  
  for (int i = 1; i <= nNodes; i++){
    
    if (solutionSet[indexNode(particle, route, i)] == node)
      return i;
  }
  
  return 0;
}

int Data::indexNode(int particle, int route, int node){
  return ((particle - 1) * nRoutes * nNodes
          + (route - 1) * nNodes
          + (node - 1));
}


int Data::indexRoute(int particle, int route){
  return ((particle - 1) * nRoutes * nNodes
          + (route -1) * nNodes);
}


int Data::indexParticle(int particle){
  return ((particle - 1) * nRoutes * nNodes);
}


int Data::inEntireRed(int particle, int node){
  
  for (int i = 1; i <= nRoutes; i++){
    
    if (routeBegin(particle, i) == node)
      return 1;  // node is beggining
    
    for (int j = 1; j <= nNodes; j++)
      if (solutionSet[indexNode(particle, i, j)] == node)
        return 1;  // node is part of the route
  }

  return 0;  // node not found
}


int Data::missingNode(int particle){

  for (int i = 1; i <= nNodes; i++){
    if (!inEntireRed(particle, i))
      return i;
  }
  
  return 0;
}


int Data::shortestRoute(int particle){
  
  int auxLen = nNodes + 1, auxIndexRoute = 0, currentLen = 0;

  for (int i = 1; i <= nRoutes; i++){
    
     currentLen = routeLen(particle, i);
    if (currentLen < auxLen){
      auxLen = currentLen;
      auxIndexRoute = i;
    }
  }
      
  return auxIndexRoute;
}


void Data::cleanRoute(int particle, int route){
  
  for (int i = 1; i <= nNodes; i++)
    solutionSet[indexNode(particle, route, i)] = 0;
}


#endif