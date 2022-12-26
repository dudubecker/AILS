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
	
	// Heurística construtiva:
	
	Heuristic Constructive('C');
	
	S = Constructive.apply(S);
	
	S.print_sol();
	
	Heuristic Swap('S',2,2);
	
	Swap.apply(S);
	
	cout << "\nSolucao apos insercoes: \n" << endl;
	
	S.print_sol();
	
}