#include <iostream>
#include <vector>
#include <Instance.hpp>
#include <Sol.hpp>
#include <LocalSearchOperator.hpp>
#include <AILS.hpp>
#include <heuristicsFunctions.h>
#include <Perturbation.hpp>
#include <iomanip>
#include <iterator>
#include <algorithm>
#include <thread>
#include <random>

using namespace std;

// Função para printar vetores: debug

void print(std::vector<int> const &input)
{
    for (auto const &i: input) {
        cout << i << " ";
    }
	
	cout << endl;
	
}


int main(){
	
	Instance inst;
	
	inst.read("AA10");
	
	
	// Inicializando a partir do objeto instância:
	
	// Printando valores de t_med
	//for (int i {0}; i < inst.n + 1; i++){
		
	//	for (int j {0}; j < inst.n + 1; j++){
			
			
	//		cout << inst.t_med[i][j] << " ";
			
	//	}
		
	//	cout << "\n";
	//}
	
	
	Sol S(inst);
	
	// Heurística construtiva:
	
	LocalSearchOperator Constructive('C');
	
	Constructive.apply(S);
	
	S.print_sol();
	
	cout << "\n FO: " << std::setprecision(7) << S.FO() << endl;
	
	/*
	
	// Testando Random Variable Neighborhood Ordering:
	
	LocalSearchOperator Or_opt_1('O', 2);
	
	LocalSearchOperator Or_opt_2('O', 3);
	
	LocalSearchOperator Two_Opt('W');
	
	LocalSearchOperator Shaw_1('H', 2,0.3,0.4,0.3);
	
	LocalSearchOperator Shift('T', 8);
	
	LocalSearchOperator Swap_1_1('S', 1,1);
	
	LocalSearchOperator Swap_2_1('S', 2,1);
	
	LocalSearchOperator Swap_2_2('S', 2,2);
	
	AILS AILSObject;
	
	AILSObject.LSOperators = {Or_opt_1, Or_opt_2, Shaw_1, Shift, Swap_1_1, Swap_2_1, Swap_2_2, Two_Opt};
	
	// AILSObject.LSOperators = {Or_opt_1, Or_opt_2, Two_Opt};
	
	cout << "\nRealizando perturbacoes: " << endl;
	
	// Perturbation Random('R');
	
	// Random.apply(S, 2);
	
	Perturbation Worst('W');
	
	Worst.apply(S, 3);
	
	S.print_sol();
	
	cout << "\n FO: " << std::setprecision(7) << S.FO() << endl;
	
	S = AILSObject.LocalSearch(S);
	
	cout << "\n Solucao Final: " << endl;
	
	S.print_sol();
	
	cout << "\n FO: " << std::setprecision(7) << S.FO() << endl;
	
	cout << "\n\n\n\n\n";
	
	cout <<"Solucao S: \n";
	
	S.print_sol();
	cout << "\n FO: " << std::setprecision(7) << S.FO() << endl;
	
	cout <<"Solucao S_cons: \n";
	S_cons.print_sol();
	cout << "\n FO: " << std::setprecision(7) << S_cons.FO() << endl;
	
	// AILSObject.symmetricDistance(S, S_cons);
	*/
}