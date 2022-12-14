#include <iostream>
#include <Instance.hpp>

using namespace std;

// Função para printar vetores: debug

void print(std::vector<double> const &input)
{
    for (auto const &i: input) {
        cout << i << " ";
    }
	
	cout << endl;
	
}


int main(){
	
	Instance inst;
	
	inst.read("BB30");
	
	print(inst.x);
	
	
}