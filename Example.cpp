#include "Example.hpp"

Example::Example(int N0, int NP):E(N0,1), A(NP,1){
	E=Eigen::Matrix<double,-1,1>::Zero(N0,1);
	A=Eigen::Matrix<double,-1,1>::Zero(NP,1);
}

Example::Example():E(0,1),A(0,1){}
Eigen::Matrix<double,-1,1> Example::entry() const{ return E;}

Eigen::Matrix<double,-1,1> Example::answer() const{ return A;}

void Example::clear(){ E.resize(0,1); A.resize(0,1);}

void Example::read(std::istream & flux){
	//file like : N N entree1 entree2 .... entreeN sortie1 ... sortieM
	this->clear();
	int N,M;
	flux>>N;
	flux>>M;
	E.resize(N,1);
	A.resize(M,1);
	for(int i=0;i<N;i++){
		flux>>E(i,0);
	}
	for(int i=0;i<M;i++){
		flux>>A(i,0);
	}
}

void Example::write(std::ostream & flux)const{
	//same form of file than read
	int N=E.rows();
	int M=A.rows();
	flux<<N<<" "<<M<<" ";
	for(int i=0;i<N;i++){
		flux<<E(i,0)<<" ";
	}
	for(int i=0;i<M;i++){
		flux<<A(i,0)<<" ";
	}
}
	
		
	
	
	


