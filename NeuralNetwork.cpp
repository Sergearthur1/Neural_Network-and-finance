#include "NeuralNetwork.hpp"




double transfert(double x) { return 1./(1.+exp(-x));}

double derive_g(double x) { 
	double tamp=transfert(x);
	return tamp*(1-tamp);
}



Eigen::Matrix<double,-1,1> product_coef_by_coef(Eigen::Matrix<double,-1,1> & M1,Eigen::Matrix<double,-1,1> & M2){
	int taille =std::min(M1.rows(),M2.rows());
	Eigen::Matrix<double,-1,1> res(taille,1);
	for(int i=0;i<taille;i++){
		res(i,0)=M1(i,0)*M2(i,0);
	}
	return res;
}

/*
NeuralNetwork::NeuralNetwork(std::vector<int> sizes_init):
sizes(sizes_init), neuronestates(sizes_init.size()), B(sizes_init.size()), Omega(sizes_init.size()){
	std::uniform_real_distribution<> unif(0.0, 1.0);
	std::mt19937 G(time(NULL));
	for(int i=0;i<sizes_init.size();i++){
		int k=sizes_init[i];
		neuronestates[i].resize(1,sizes_init[i]);
		B[i].resize(1,k);
		for(int j=0;j<k;j++){
			B[i](j,0)=unif(G);
		}
		if(i>0){
			Omega[i].resize(sizes_init[i-1],k);
			for(int j=0;j<k;j++){
				for(int l=0;l<sizes_init[i-1];l++){
					Omega[i](l,j)=unif(G);
				}
			}
		}
	}
}
*/
NeuralNetwork::NeuralNetwork(std::vector<int> & sizes_init,std::mt19937 & G):sizes(sizes_init), neuronestates(0),B(0),Omega(0){
	std::uniform_real_distribution<> unif(0.0, 1.0);
	neuronestates.push_back(Eigen::Matrix<double,-1,-1>(sizes_init[0],1));
	for(int i=1;i<sizes_init.size();i++){
		int k=sizes_init[i];
		neuronestates.push_back(Eigen::Matrix<double,-1,-1>(k,1));
		B.push_back(Eigen::Matrix<double,-1,-1>(k,1));
		Omega.push_back(Eigen::Matrix<double,-1,-1>(k,sizes_init[i-1]));
		for(int j=0;j<k;j++){
			B[i-1](j,0)=unif(G);
			for(int l=0;l<sizes_init[i-1];l++){
				Omega[i-1](j,l)=unif(G);
			}
		}
		
	}
}


NeuralNetwork::NeuralNetwork():sizes(0), neuronestates(0),B(0),Omega(0){}

NeuralNetwork::NeuralNetwork(NeuralNetwork const & nn):
sizes(nn.sizes), neuronestates(nn.neuronestates),B(nn.B), Omega(nn.Omega){}


void NeuralNetwork::read(std::istream & is){
	int P;
	is>>P;
	sizes.resize(P+1);
	B.resize(P);
	Omega.resize(P);
	neuronestates.resize(P+1);
	for(int i=0;i<=P;i++){
		is>>sizes[i];
	}
	double tamp;
	for(int i=1;i<=P;i++){
		B[i-1].resize(sizes[i],1);
		for(int j=0;j<sizes[i];j++){
			is>>tamp;
			B[i-1](j,0)=tamp;
		}
	}
		
	for(int i=1;i<=P;i++){
		Omega[i-1].resize(sizes[i],sizes[i-1]);
		for(int j=0;j<sizes[i];j++){
			for(int k=0;k<sizes[i-1];k++){
				is>>tamp;
				Omega[i-1](j,k)=tamp;
			}
		}
	}
}


void NeuralNetwork::write(std::ostream & os)const {
	int P=sizes.size()-1;
	os<<P<<std::endl;
	for(int i=0;i<=P;i++){
		os<<sizes[i]<<" ";
	}
	os<<std::endl;
	for(int i=0;i<P;i++){
		os<<B[i].transpose()<<std::endl;
	}
	for(int i=0;i<P;i++){
		os<<Omega[i];
		os<<std::endl;
	}
}



Eigen::Matrix<double,Eigen::Dynamic,1> NeuralNetwork::compute(Eigen::Matrix<double,Eigen::Dynamic,1> & X0){
	int taille=sizes.size();
	neuronestates[0]=X0;
	for(int i=0;i<taille-1;i++){
		neuronestates[i+1]=Omega[i]*neuronestates[i]+B[i];
		for_each_fait_maison(neuronestates[i+1],transfert);
		//std::for_each(neuronestates[i].begin(),neuronestates[i].end(),[](double *x){x=g(*x);});
	}
	return neuronestates[taille-1];
}

double NeuralNetwork::validation(const std::vector<Example> & Bv){
	double sum=0.;
	double tamp;
	for(int i=0;i<Bv.size();i++){
		Eigen::Matrix<double,-1,1> tamp2=Bv[i].entry();
		Eigen::Matrix<double,-1,1> res=this->compute(tamp2);
		Eigen::Matrix<double,-1,1> sortie=Bv[i].answer();
		for(int j=0;j<res.rows();j++){
			tamp=res(j,0)-sortie(j,0);
			sum+=tamp*tamp;
		}
	}
	return sum/Bv.size();
	
}

double NeuralNetwork::learning(const std::vector<Example> & Ba, double lambda, double alpha){
	int P=sizes.size()-1;
	//allocation of variable
	std::vector<Eigen::Matrix<double,-1,1>> U(P);
	std::vector<Eigen::Matrix<double,-1,-1>> delta(P);
	std::vector<Eigen::Matrix<double,-1,1>>deltabar(P);
	//initialisation of delta at 0
	for(int i=0;i<P;i++){
		delta[i]=Eigen::Matrix<double,-1,-1>::Zero(Omega[i].rows(),Omega[i].cols());
		deltabar[i]=Eigen::Matrix<double,-1,1>::Zero(B[i].rows(),1);
	}
	//way in example
	for(int i=0;i<Ba.size();i++){
		Eigen::Matrix<double,-1,1> entre=Ba[i].entry();
		Eigen::Matrix<double,-1,1> TE=this->compute(entre);
		//way in layers
		for(int a=P-1;a>=0;a--){
			Eigen::Matrix<double,-1,1> H=Omega[a]*neuronestates[a]+B[a];
			for_each_fait_maison(H,derive_g);
			if(a==P-1){
				U[a]=Ba[i].answer()-TE;
				U[a]=product_coef_by_coef(U[a],H);
			}
			else{
				U[a]=Omega[a+1].transpose()*U[a+1];
				U[a]=product_coef_by_coef(U[a],H);
			}
			//way in coeff in the cost matrix
			for(int n=0;n<U[a].rows();n++){
				deltabar[a](n,0)=lambda*U[a](n,0)+alpha*deltabar[a](n,0);
				for(int m=0;m<neuronestates[a].size();m++){
					delta[a](n,m)=lambda*U[a](n,0)*neuronestates[a](m,0)+alpha*delta[a](n,m);
				}
			}
		}
		//update coeff
		for(int i=0;i<P;i++){
			B[i]=B[i]+deltabar[i];
			Omega[i]=Omega[i]+delta[i];
		}
	}
	return this->validation(Ba);
}
			
				
				

	
		
		

	
		
		
