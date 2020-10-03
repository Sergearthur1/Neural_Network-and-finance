#include <Eigen/Dense>
#include <iostream>
#include <fstream>
class Example{
	private:
		Eigen::Matrix<double,-1,1> E;
		Eigen::Matrix<double,-1,1> A;
	public:
	 Example(int ,int);
	 Example();
	 Eigen::Matrix<double,-1,1> entry() const; //access for E
	 Eigen::Matrix<double,-1,1> answer() const;//access for A
	 void clear();//clear the content of E and A
	 void read(std::istream & );
	 void write(std::ostream & )const;
	 

};
