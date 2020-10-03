#include <vector>
#include <Eigen/Dense>
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include "Example.hpp"

double transfert(double);

double derive_g(double);//derivate of tranfer fonction

//void for_each_fait_maison(Eigen::Matrix<double,Eigen::Dynamic,1> & );

template<class Func> void for_each_fait_maison(Eigen::Matrix<double,Eigen::Dynamic,1> & v,Func f){
	for(int i=0;i<v.rows();i++){
		v(i,0)=f(v(i,0));
	}
}


Eigen::Matrix<double,-1,1> product_coef_by_coef(Eigen::Matrix<double,-1,1> & ,Eigen::Matrix<double,-1,1> & );

//class NeuralNetwork;

class NeuralNetwork {
	protected : 
		std::vector<int> sizes;//taille P+1
		std::vector<Eigen::Matrix<double,Eigen::Dynamic,1>> neuronestates;//size P+1
		//for B et Omega the i-th matrix in the vector is the i+1-th
		std::vector<Eigen::Matrix<double,Eigen::Dynamic,1>> B;//size P
		std::vector<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>> Omega;//size P
		
	public :
		NeuralNetwork(std::vector<int> &,std::mt19937 &);
		NeuralNetwork();
		NeuralNetwork(NeuralNetwork const &);
		void read(std::istream &);
		void write(std::ostream &) const;
		Eigen::Matrix<double,Eigen::Dynamic,1> compute(Eigen::Matrix<double,Eigen::Dynamic,1> &);
		double validation(const std::vector<Example> & );
		double learning(const std::vector<Example> & , double , double );
		template<class random> void mutation(random & G,double rate=0.1){
			std::uniform_real_distribution<double> unif(0.0,1.0);
			std::normal_distribution<> normal{0.,1.};
			int p=Omega.size();
			for(int i=0;i<p;i++){
				int n=Omega[i].rows();
				int m=Omega[i].cols();
				for(int j=0;j<n;j++){
					if(unif(G)<rate){
						B[i](j,0)+=normal(G);
					}
					for(int k=0;k<m;k++){
						if(unif(G)<rate){
							Omega[i](j,k)+=normal(G);
						}
					}
				}
			}
		}
		

};

