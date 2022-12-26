#include <iostream>
#include <vector>
#include <Instance.hpp>
#include <Sol.hpp>
#include <Heuristic.hpp>

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
	
	// Inicializando a partir do objeto instância:
	
	Sol S(inst);
	
	Heuristic Constructive('C');
	
	S = Constructive.apply(S);
	
	
	
	
}