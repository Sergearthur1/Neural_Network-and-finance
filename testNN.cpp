#include "NeuralNetwork.hpp"
#include <cmath>
#include <math.h>

int main(){
	//~ std::ifstream F("neurones_test.dat");
	//~ std::ofstream F2("neural_confirm.dat");
	//~ std::vector<int> v={4, 5, 4, 6, 3, 3};
	//~ NeuralNetwork nn(v);
	//~ //nn.read(F);
	//~ nn.write(F2);
	//~ Eigen::Matrix<double,-1,1> w(4,1);
	//~ w<< 0.1,
	 //~ 0.2, 
	 //~ 0.3,
	 //~ 0.1;
	//~ std::cout<< nn.compute(w)<<std::endl;
	
	
	//~ std::ifstream F("neurones_test.dat");
	//~ Example e(4,4);
	//~ e.read(F);
	//~ e.write(std::cout);
	//~ std::cout<<e.answer()<<std::endl;
	//~ std::cout<<e.entry()<<std::endl;
	
	std::ofstream F("data_sin.dat");
	//F<<200<<std::endl;
	std::vector<double> x(200);
	std::vector<double> y(200);
	std::mt19937 G(time(NULL));
	std::uniform_real_distribution<double> dis(0.0,1.0);
	for(int i=0;i<200;i++){
		x[i]=dis(G);
		y[i]=(std::sin(x[i]*(2*M_PI))+1.)/2;
		F<<1<<" "<<1<<" "<<x[i]<<" "<<y[i]<<std::endl;
	}
	F.close();
	std::ifstream F2("data_sin.dat");
	std::vector<Example> Ba(170);
	std::vector<Example> Bv(30);
	for(int i=0;i<170;i++){
		Ba[i].read(F2);
	}
	for(int i=0;i<30;i++){
		Bv[i].read(F2);
	}
	//Ba[5].write(std::cout);
	std::cout<<std::endl;
	//Bv[4].write(std::cout);
	F2.close();
	std::vector<int> v={1,12,1};
	NeuralNetwork Albert(v,G);
	for(int i=0;i<2000;i++){
	std::cout<<Albert.learning(Ba,0.2,0.2)<<" "<<Albert.validation(Bv)<<std::endl;
	}
	Albert.mutation(G);
	std::ofstream F3("verif1.dat");
	for(int i=0;i<=100;i++){
		Eigen::Matrix<double,-1,1> tamp(1,1);
		tamp(0,0)=i*0.01;
		F3<<i*0.01<<" "<<Albert.compute(tamp)<<std::endl;
	}
	
		
		
	return 0;
}
