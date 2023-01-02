#include <iostream>
#include <vector>
#include <Instance.hpp>
#include <Sol.hpp>
#include <LocalSearchOperator.hpp>
#include <heuristicsFunctions.h>
#include <Perturbation.hpp>
#include <iomanip>

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
	
	LocalSearchOperator Constructive('C');
	
	Constructive.apply(S);
	
	S.print_sol();
	
	cout << "\n FO: " << std::setprecision(7) << S.FO() << endl;
	
	// LocalSearchOperator Swap('S',1,1);
	
	// Swap.apply(S);
	
	// LocalSearchOperator Shift('T',4);
	
	// Shift.apply(S);
	
	// LocalSearchOperator Or_opt('O', 2);
	
	// Or_opt.apply(S);
	
	// LocalSearchOperator Two_Opt('W');
	
	// Two_Opt.apply(S);
	
	// Perturbation Random('R');
	
	// Random.apply(S, 6);
	
	// Perturbation Worst('W');
	
	// Worst.apply(S, 4);
	
	Perturbation Shaw('S', 0.3,0.4,0.3);
	
	Shaw.apply(S, 5);
	
	S.print_sol();
	
	cout << "\n FO: "  << S.FO() << endl;
	
	// cout << "\n Factibilidade: " << S.isFeasible() << endl;
		
	
	
	
}