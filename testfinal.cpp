#include "genetic.hpp"
int main(){
	
	std::uniform_real_distribution<double> distribution(0.0,1.0);
	std::mt19937 G(time(NULL));
	Market m(1,0.01);
	Stock s(1,0.1,-0.1,distribution,G);
	Stock s2(5,0.1,-0.1,distribution,G);
	Call c(3,20,s);
	Put p(2,20,s);
	Call c2(7,20,s2);
	
	m.add_action(s);
	m.add_action(s2);
	m.add_action(c);
	m.add_action(p);
	m.add_action(c2);
	
	for(int i=0;i<6;i++){
		m.evolution();
	}
	
	std::vector<int> topo={5*m.get_size(), 3*m.get_size(), 2*m.get_size(),m.get_size()};
	/*first layer 5 last value of each action,
	second layer : value + and - and real proba, 
	third layer : real proba and prn, 
	last layer : weight for next investement*/
	int taille=500;
	std::vector<Bot *> v(taille);
	for(int i=0;i<taille;i++){
		v[i]=new Bot(m,100,5,topo,G);
	}
	//~ for(int i=0;i<100;i++){
		//~ v[i]=new Bot(m,100,5,topo,G);
	//~ }
	
	tradeur t(m,100);
	Bot b(m,100,5,topo,G);
	for(int i=0;i<100;i++){
		invest(v);
		t.invest();
		b.invest();
		b.mutation(G,0.5);
		//b.get_nn().write(std::cout);
		m.evolution();
		v=selection(v,G);
		mutation(v,G,0.5);
		//v[0]->get_nn().write(std::cout);
		
		//std::cout<<"---------------------------------------"<<std::endl;
		std::cout<<"tradeur's budget is :"<<t.actualisation()<<std::endl;
		std::cout<<"bot's budget is :"<<b.actualisation()<<std::endl;
		
	}
	std::cout<<"---------------------------------------"<<std::endl;
	std::cout<<"first bot's budget is :"<<v[0]->get_money()<<std::endl;
	m.affiche_market(std::cout);
		
	
	for(int i=0;i<taille;i++){
		delete v[i];
	}
	return 0;
}
