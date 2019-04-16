#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	char buffer[256];
	ifstream PDB("PDB07.txt");
    while(!PDB.eof()){
    	PDB.getline (buffer,100);  
        cout << buffer << endl;  
	}    
    return 0;
}
