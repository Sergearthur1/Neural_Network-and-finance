#include "marche.hpp"
#include "NeuralNetwork.hpp"

class Bot: public tradeur{//k is the number of old value for each action we use in the NN
	private:
		NeuralNetwork nn;
		int const k;
	public:
		Bot(Market & m, double argent,int n, std::vector<int> & topo,std::mt19937 & G):tradeur(m,argent),k(n),nn(topo,G){}
		Bot(Bot const & b):tradeur(*(b.m),b.money),k(b.k),nn(b.nn){}
		
		template<class random>
		void mutation(random & G,double rate=0.1){nn.mutation(G,rate);}
		
		void invest(){
			int taille=m->get_size();
			Eigen::Matrix<double,-1,1> input(k*taille,1);
			for(int i=0;i<taille;i++){
				input(i*k,0)=m->actions[i]->get_price();
				for(int j=1;j<k;j++){
					input(i*k+j,0)=m->actions[i]->get_previous_value(j);
				}
			}
			Eigen::Matrix<double,-1,1>output=nn.compute(input);
			double sum=0.;
			for(int i=0;i<taille;i++){
				sum+=output(i,0);
			}
			for_each_fait_maison(output,[sum](double x){return x/sum;});
			for(int i=0;i<taille;i++){
				if(m->actions[i]->get_price()!=0.){
					nb_actions[i]=output(i,0)*money/m->actions[i]->get_price();
				}
			}
		}
		
		NeuralNetwork & get_nn(){return nn;}
};

template<class Agent,class Random>
std::vector<Agent *>selection(std::vector<Agent*> & v,Random & G){
	double tot=0.;
	int taille=v.size();
	double tamp;
	std::vector<double>coeff(taille);
	std::vector<Agent*>res(taille);
	for(int i=0;i<taille;i++){
		tamp=v[i]->actualisation();
		coeff[i]=tamp;
		tot+=tamp;
	}
	std::uniform_real_distribution<double> unif(0.0,tot);
	for(int i=0;i<taille;i++){
		double ite=0.;
		double lim=unif(G);
		int increment=0;
		while(ite<=lim){
			ite=ite+coeff[increment];
			increment++;
		}
		res[i]=new Bot(*(v[increment-1]));
	}
	for(int i=0;i<taille;i++){
		delete v[i];
	}
	return res;
	
}

template<class Agent,class random>
void mutation(std::vector<Agent*> & v,random & G,double rate=0.1){
	
	std::for_each(v.begin(),v.end(),[&](Agent * a){a->mutation(G,rate);});
}
	
template<class Agent>
void invest(std::vector<Agent *> & v){
	
std::for_each(v.begin(),v.end(),[](Agent *a){a->invest();});
}	
	
	
