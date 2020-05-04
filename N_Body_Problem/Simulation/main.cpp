#include <iostream>
#include "rapidcsv.h"
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

int main() {
	
	rapidcsv::Document qvec("C:\\Users\\catie\\Documents\\code\\NBody_Simulation\\N_Body_Problem\\N_Body_Problem\\qresults.csv", rapidcsv::LabelParams(-1,-1));
	cout << "Document Opened" << endl;
	vector<double> row_sample = qvec.GetRow<double>(5);
	cout << "Read " << row_sample.size() << " values." << endl;
	
	return 0;
}
