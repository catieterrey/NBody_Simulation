#include <iostream>
#include <vector>
//#include <boost/numeric/odeint.hpp>
//#include <nbodyalgorithm.hpp>
#include <numeric>
#include <math.h>
#include <fstream>
#include <iterator>

//function to multiply a scalar by a vector
std::vector<std::vector<double>> mult(double x, std::vector<std::vector<double>> v){
	for (int i = 0; i< v.size();i++){
		for (int j=0;j<v[i].size();j++){
			v[i][j]=v[i][j]*x;
		}
	}
	return v;
}

//function to multiply a scalar by a vector
std::vector<double> mult2(double x, std::vector<double> v){
	for (int i = 0; i< v.size();i++){
		v[i]=v[i]*x;
	}
	return v;
}

//function to add vectors of vectors
std::vector<std::vector<double>> add(std::vector<std::vector<double>> v1,std::vector<std::vector<double>> v2){
	if (v1.size()==v2.size()){
		for (int i=0; i< v1.size(); i++){
			for (int j=0; j < v1[0].size(); j++) {
			v1[i][j]+=v2[i][j];
			}
		}
	}
	return v1;
}
//function to add vectors
std::vector<double> add2(std::vector<double> v1, std::vector<double> v2){
	if (v1.size()==v2.size()){
		for (int i=0; i< v1.size(); i++){
			v1[i]+=v2[i];
		}
	}
	return v1;
}


//function to invert every element of a vector
std::vector<double> invert(std::vector<double> v) {
	for (int i=0; i < v.size(); i++) {
		if (v[i] != 0) {
			v[i] = 1/v[i];
		}
		else {
			v[i] = 1000000000;
		}
	}
	return v;
}

//function to multiply vector of vectors by vector of scalars
std::vector<std::vector<double>> smult(std::vector<std::vector<double>> v, std::vector<double> scalars) {
	for (int i=0; i<v.size(); i++) {
		v[i] = mult2(scalars[i],v[i]);
	}
	return v;
}


// function to compute the dot product of two vectors
double dot (std::vector<double> v1, std::vector<double> v2) {
	double n = 0;
	for (int i=0; i < v1.size(); i++) {
		n += v1[i] * v2[i];
	}
	return n;
}

//function to compute the Hamiltonian ie the total energy of the system
double Hamiltonian(std::vector<double> mass, std::vector<std::vector<double>> qvec, std::vector<std::vector<double>> pvec) {
	double G = 6.67 * pow(10,-11);
    double Etot = 0;
    for (int i=0; i < qvec.size(); i++) {
        for (int j; j < qvec.size(); j++) {
            if (i != j) {Etot -= (G * mass[i] * mass[j]) / (std::sqrt(dot(add2(qvec[i],mult2(-1, qvec[j])), add2(qvec[i],mult2(-1, qvec[j])))));
            }
        }
		Etot += (dot(pvec[i], pvec[i])) / (2 * mass[i]);
    }
}

//change in position function
std::vector<std::vector<double>> qvecdot(std::vector<double> mass, std::vector<std::vector<double>> pvec) {
	for(int i=0; i < pvec.size(); i++) {
		for(int j = 0; j < pvec[0].size();j++){
			pvec[i][j] = pvec[i][j] / mass[i];
		}
	}
	return pvec;
}

//change in momentum function
std::vector<std::vector<double>> pvecdot(std::vector<double> mass, std::vector<std::vector<double>> qvec) {
	double G = 6.67 * pow(10,-11);
	std::vector<std::vector<double>> Pvecdot = qvec;
	for(int i=0; i < qvec.size(); i++) {
		Pvecdot[i] = {0,0,0};
		for(int j=0; i<qvec.size(); j++) {
			for(int k=0; k < qvec[0].size(); k++) {
				std::cout<<(qvec[i][k]-qvec[j][k])<<'\n';
				std::cout<< (pow( (std::sqrt(dot(add2(qvec[i],mult2(-1, qvec[j])), add2(qvec[i],mult2(-1, qvec[j]))))), 3)) <<'\n';
				Pvecdot[i][k] += G * mass[i] * mass[j] * ((qvec[i][k]-qvec[j][k]) / (pow( (std::sqrt(dot(add2(qvec[i],mult2(-1, qvec[j])), add2(qvec[i],mult2(-1, qvec[j]))))), 3)));
			}
		}
	}
	return Pvecdot;
}


//main integration function
void integrator(std::vector<std::vector<double>> qvec0, std::vector<std::vector<double>> pvec0, std::vector<double> mass, double dt, int N) {
	std::vector<std::vector<std::vector<double>>> qresults{qvec0};
	std::vector<std::vector<std::vector<double>>> presults{pvec0};
	
	for (int i = 1; i< N; i++){
		qresults.push_back(add(qresults[i-1], mult(dt, add(presults[i-1],smult(mult(dt/2, pvecdot(mass,qresults[i-1])),invert(mass))))));
		presults.push_back(add(presults[i-1], add(mult(dt/2,smult(pvecdot(mass,qresults[i-1]),mass)),mult(dt/2,smult(pvecdot(mass,qresults[i]),mass)))));
	}	
	
	std::ofstream output_file("qresults.txt");
	for(const auto& qvect : qresults){
		for(const auto& pos : qvect){
			std::copy(pos.cbegin(),pos.cend(),
				std::ostream_iterator<double>(output_file," "));
			output_file << ", ";
		}
		output_file << '\n';
	}
	std::ofstream output_file2("presults.txt");
	for(const auto& pvect : presults){
		for(const auto& mom : pvect){
			std::copy(mom.cbegin(),mom.cend(),
				std::ostream_iterator<double>(output_file2," "));
			output_file2 << ", ";
		}
		output_file2 << '\n';
	}
}

int main(){
	std::vector<std::vector<double>> q0{{1,1,1},{200,200,200}};
	std::vector<std::vector<double>> p0{{-1,-1,-1.5},{1,0.5,1}};
	int N=1000;
	double dt=0.1;
	std::vector<double> mass{10000,10000};
	integrator(q0,p0,mass,dt,N);
	return 0;
}