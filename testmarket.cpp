#include "marche.hpp"


int main(){ 
std::uniform_real_distribution<double> distribution(0.0,1.0);
std::mt19937 G(time(NULL));

Market m(3,0.1);
Stock s(1,0.2,-0.2,distribution,G,0.9);
Stock s2(5,0.5,-0.4,distribution,G);
Call c(3,10,s);
Put p(2,10,s);
Call c2(7,8,s2);

m.add_action(s);
m.add_action(s2);
m.add_action(c);
m.add_action(p);
m.add_action(c2);
for(int i=0;i<50;i++){
 m.evolution();
}
m.affiche_market(std::cout);
return 0;
}
	

