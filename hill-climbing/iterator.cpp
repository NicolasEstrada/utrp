#include <iostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#define INTERVALS 10
#define FACTORS 10
#define TEMP 10
#define FACTOR 0.1
#define CONSTVALUE 237537

using namespace std;

int main(int argc, char **argv){
    
    int i, j;
    
    ostringstream oss (ostringstream::out);
    
    string cline("./utrp "), aux, foo;
    
    if (argc != 2)
    {
        cout << "Incorrect Format: numeroArchivos" << endl;
        return 0;
    }
    
    for (i = 1; i <= atoi(argv[1]); i++)
    {
        for (j = 1; j <= 10; j++)
        {
            oss << cline << "tp" << i  << ".txt tt1.txt td1.txt "
            << j*CONSTVALUE << " ./salidas/" << j*CONSTVALUE
            << ".outrp";
            aux = oss.str();
            system((char *)aux.c_str());
            cout << aux << endl;
            aux.clear();
            oss.str("");
        }
    }
    return 0;
}
