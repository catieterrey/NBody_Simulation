#include <iostream>
#include "csv.h"
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>


using namespace std;
int main() {
	
	io::CSVReader<9> in("qresults.csv");
	double x1;double y1;double z1;
	double x2;double y2;double z2;
	double x3;double y3;double z3;
	while(in.read_row(x1,y1,z1,x2,y2,z2,x3,y3,z3)){ 
		cout << z2 << '\n';
	}
	
	return 0;
}
