#include <random>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>


class Action;
class Stock;
class tradeur;
class Bot;


long int binomialCoefficients(int n, int k) {
	if(k == n){
		return 1;
	}
	if(k==0){
		return 1;
	}
	else{
		int taille=k*n;
		std::vector<int> tab(taille);
		for(int i=0;i<n;i++){
			tab[i]=1;
		}
		for(int j=1;j<k;j++){
			for(int k=j;k<n;k++){
				if(j==k){tab[k+j*n]=1;}
				else{tab[k+j*n]=tab[k-1+(j-1)*n]+tab[k-1+j*n];}
			}
		}
		return tab[taille-1];
	}	

	//~ if (k == 0 || k == n){
		//~ return 1;
	//~ }
    //~ return binomialCoefficients(n - 1, k - 1) + binomialCoefficients(n - 1, k);
}  


class Action{
	protected:
		double prix;
		std::vector<double> historic_price;
	public:
		Action(double val=0):prix(val), historic_price(1){historic_price[0]=val;}
		Action(Action const & a):prix(a.prix),historic_price(a.historic_price){}
		virtual void evolution(){};
		double get_price()const{return prix;}
		void affiche_action(std::ostream & os){
			os<<"action cost : "<<prix<<std::endl;
			//~ os<<"old values are : ";
			//~ for(auto it=historic_price.begin();it!=historic_price.end();it++){
				//~ os<<*it<<" ";
			//~ }
			os<<std::endl;
		}
		virtual~Action() = default;
		double get_previous_value(int j)const{return historic_price[historic_price.size()-(j+1)];}
};




class Bond: public Action {
	private:
		double x;
	public: 
		Bond(double val,double taux): Action(val), x(taux){}
		void evolution();
		double get_x()const{return x;}
		~Bond() = default;
};



class Stock: public Action {
	private:
		double h;
		double b;
		double p;//proba of rise
		std::mt19937 & G;//random generator
		std::bernoulli_distribution dis;//proba of rise
	public:
		//Stock(double, double ,double , double );
		template<class Loi,class Random>
		Stock(double val, double high_val,double low_val,Loi & distribution,Random & generateur,double proba=0):
		Action(val),h(high_val), b(low_val), p(proba), G(generateur),dis(proba) {
			if(proba==0){
				//std::uniform_real_distribution<double> distribution(0.0,1.0);
				proba=distribution(G);
				std::cout<<"the proba is "<<proba<<std::endl;
				dis=std::bernoulli_distribution(proba);
			}
		}	
		
		void evolution();
		
		double get_b()const{return b;}
		double get_h()const{return h;}
		double get_p()const{return p;}
		
};





class ProduitDerive: public Action{
	protected :
		double valeurreel;
	public:
	ProduitDerive(double val):Action(0),valeurreel(val){}
	ProduitDerive(ProduitDerive const & a):Action(a),valeurreel(a.valeurreel){}
	virtual void evolution(){};
	virtual ~ProduitDerive() = default;
	friend class magicien;
	
};






class Market{
	
	private:
	int size;
	std::vector<Action *> actions;
	//Action* actions;
	
	public:
	//Market(double ,double);
	Market(double val,double tx):actions(0),size(0){//create a Market with only a Bond
		Bond * b=new Bond(val, tx);
		actions.push_back(b);
		size++;
	}
	
	Market(std::vector<Action*> & L):actions(L){}
	
	void add_action(Action & a){
		actions.push_back(&a);
		size++;
	}
	
	int get_size()const{return size;}
	
	void evolution(){
		std::for_each(actions.begin(),actions.end(),[](Action* a){a->evolution();});
	}
	
    void affiche_market(std::ostream & os){
		for(int i=0;i<actions.size();i++){
			os<<"Action n°"<<i<<std::endl;
			actions[i]->affiche_action(os);
		}
		os<<"update prices are :"<<std::endl;
		for(int i=0;i<actions.size();i++){
			os<<actions[i]->get_price()/actions[0]->get_price()<<"	";
		}
		os<<std::endl;
	}
	
	~Market()=default;
	friend class tradeur;
	friend class Bot;
};





class Call: public ProduitDerive{
	private:
		double strike;
		int echeance;
		Stock * a;
	 
	public:
		Call(double sk,int date,Stock & act);
		void evolution();
			
};

class Put: public ProduitDerive{
	private:
		double strike;
		int echeance;
		Stock *a;
	public:
		Put(double sk,int date,Stock & act);
		void evolution();
};



class tradeur{
	protected : 
		double money;
		Market * m;
		std::vector<double> nb_actions;
	public:
	
		tradeur(Market & market,double argent):money(argent),nb_actions(market.get_size()){
			m=&market;
		}
			
		virtual void invest(){nb_actions[0]=money/m->actions[0]->get_price();};//invest money in action of m and print the number of action to buy in actions.
		
		double actualisation(){//after market evolution,this function give portfolio value and put it in the variable money.
			double res=0.;
			for(int i=0;i<nb_actions.size();i++){
				res+=nb_actions[i]*m->actions[i]->get_price();
			}
			money=res;
			return res;
		}
		double get_money()const{return money;}
};

//~ class magicien:public tradeur{//WARNING!! use only if maket is fill with derivate product
	//~ public:
		//~ magicien(Market & market,double argent):tradeur(market,argent){}
		//~ void invest(){
			//~ double sum=0;
			//~ for(int i=1;i<m->get_size();i++){
				//~ ProduitDerive *tamp=(m->actions[i]);
				//~ sum+=std::max(0,tamp->valeurreel-tamp->get_price());
			//~ }
			//~ if(sum==0){
				//~ tradeur::invest();
			//~ }
			//~ else{
				//~ nb_actions[0]=0;
				//~ for(int i=1;i<m->get_size();i++){
					//~ ProduitDerive *tamp=(m->actions[i]);
					//~ nb_actions[i]=(std::max(0,tamp->valeurreel-tamp->get_price())/sum)*(money/tamp->get_price());
				//~ }
			//~ }
		//~ }
//~ };

		
			
		
	
	
			
	
void Stock::evolution(){
	
	int tamp=dis(G);
	if(tamp==0){
		this->prix=this->prix*(1+b);
	}
	else{
		this->prix=this->prix*(1+h);
	}
	historic_price.push_back(prix);
}

void Bond::evolution(){
	prix=prix*(1+x);
	historic_price.push_back(prix);
}

Call::Call(double sk,int date,Stock & act):ProduitDerive(0.),strike(sk),echeance(date){
	a=&act;
	double S0= act.get_price();
	double S1moins=S0*(1+act.get_b());
	double S1plus=S0*(1+act.get_h());
	double prn=(S0 - S1moins)/(S1plus-S1moins);
	prix=0.;
	valeurreel=0.;
	for(int k=0;k<date;k++){
		int cnk=binomialCoefficients(date,k);
		double Sn=S0*std::pow((1+act.get_b()),date-k)*std::pow(1+act.get_h(),k);
		prix+=cnk*std::pow(prn,k)*std::pow(1-prn,date-k)*std::max(0.,Sn-sk);
		valeurreel+=cnk*std::pow(act.get_p(),k)*std::pow(1-act.get_p(),date-sk)*std::max(0.,Sn-sk);
	}
	historic_price[0]=prix;
}

void Call::evolution(){
		if(historic_price.size()>=echeance){
			prix=0.;
			valeurreel=0;
		}
		else{
			double S0= a->get_price();
			double S1moins=S0*(1+a->get_b());
			double S1plus=S0*(1+a->get_h());	
			double prn=(S0 - S1moins)/(S1plus-S1moins);
			prix=0.;
			valeurreel=0.;
			int date=echeance-historic_price.size();
			for(int k=1;k<=date;k++){
				int cnk=binomialCoefficients(date,k);
				double Sn=S0*std::pow((1+a->get_b()),date-k)*std::pow(1+a->get_h(),k);
				prix+=cnk*pow(prn,k)*pow(1-prn,date-k)*std::max(0.,Sn-strike);
				valeurreel+=cnk*pow(a->get_p(),k)*pow(1-a->get_p(),date-k)*std::max(0.,Sn-strike);
			}
			historic_price.push_back(prix);
		}
}

Put::Put(double sk,int date,Stock & act):ProduitDerive(0.),strike(sk),echeance(date){
	a=&act;
	double S0= act.get_price();
	double S1moins=S0*(1+act.get_b());
	double S1plus=S0*(1+act.get_h());
	double prn=(S0 - S1moins)/(S1plus-S1moins);
	prix=0.;
	valeurreel=0.;
	for(int k=0;k<date;k++){
		int cnk=binomialCoefficients(date,k);
		double Sn=S0*std::pow((1+act.get_b()),date-k)*std::pow(1+act.get_h(),k);
		prix+=cnk*std::pow(prn,k)*std::pow(1-prn,date-k)*std::max(0.,sk-Sn);
		valeurreel+=cnk*std::pow(act.get_p(),k)*std::pow(1-act.get_p(),date-sk)*std::max(0.,sk-Sn);
	}
	historic_price[0]=prix;
}

void Put::evolution(){
		if(historic_price.size()>=echeance){
			prix=0.;
			valeurreel=0.;
		}
		else{
			
			double S0= a->get_price();
			double S1moins=S0*(1+a->get_b());
			double S1plus=S0*(1+a->get_h());	
			double prn=(S0 - S1moins)/(S1plus-S1moins);
			prix=0.;
			valeurreel=0.;
			int date=echeance-historic_price.size();
			for(int k=1;k<=date;k++){
				int cnk=binomialCoefficients(date,k);
				double Sn=S0*std::pow((1+a->get_b()),date-k)*std::pow(1+a->get_h(),k);
				prix+=cnk*pow(prn,k)*pow(1-prn,date-k)*std::max(0.,strike-Sn);
				valeurreel+=cnk*pow(a->get_p(),k)*pow(1-a->get_p(),date-k)*std::max(0.,strike-Sn);
			}
			historic_price.push_back(prix);
		}
}
		
			
