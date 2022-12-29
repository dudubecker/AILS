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
	
	inst.read("YY30");
	
	// Inicializando a partir do objeto instância:
	
	Sol S(inst);
	
	// Heurística construtiva:
	
	Heuristic Constructive('C');
	
	Constructive.apply(S);
	
	S.print_sol();
	
	cout << "\n FO: " << S.FO() << endl;
	
	// Heuristic Swap('S',2,2);
	
	// Swap.apply(S);
	
	// Heuristic Shift('T',4);
	
	// Shift.apply(S);
	
	//Heuristic Or_opt('O', 2);
	
	//Or_opt.apply(S);
	
	Heuristic Two_Opt('W');
	
	Two_Opt.apply(S);
		
	S.print_sol();
	
	cout << "\n FO: " << S.FO() << endl;
		
	

	
}