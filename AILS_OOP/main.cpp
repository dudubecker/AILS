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

void print(std::vector<double> const &input)
{
    for (auto const &i: input) {
        cout << i << " ";
    }
	
	cout << endl;
	
}


int main(){
	
	
	Instance inst;
	
	inst.read("CC50");
	
	// Inicializando solução a partir do objeto instância:
	
	Sol S(inst);
	
	// Heurística construtiva:
	
	LocalSearchOperator Constructive('C');
	
	Constructive.apply(S);
	
	// Inicializando objeto da AILS
	
	
	AILS AILSObject;
	
	// Solução incumbente e melhor solução encontrada:
	
	AILSObject.S_p = S;
	
	AILSObject.S_r = S;
	
	// Inicializando métodos de perturbação:
	
	Perturbation Random('R');
	
	Perturbation Worst('W');
	
	Perturbation Concentric('C');
	
	// Inicializando operadores de busca local:
	
	LocalSearchOperator Or_opt_1('O', 1);
	
	LocalSearchOperator Or_opt_2('O', 3);
	
	LocalSearchOperator Two_Opt('W');
	
	LocalSearchOperator Shaw_1('H', 2,0.3,0.4,0.3);
	
	LocalSearchOperator Shift_5('T', 5);
	
	LocalSearchOperator Shift_10('T', 10);
	
	LocalSearchOperator Shift_15('T', 15);
	
	LocalSearchOperator Swap_1_1('S', 1,1);
	
	LocalSearchOperator Swap_2_1('S', 2,1);
	
	LocalSearchOperator Swap_2_2('S', 2,2);
	
	LocalSearchOperator Swap_4_4('S', 4,4);
	
	LocalSearchOperator Swap_3_3('S', 3,3);
	
	// Atribuindo ao objeto da AILS
	
	AILSObject.PerturbationProcedures = {Random, Worst, Concentric};
	
	// AILSObject.LSOperators = {Or_opt_1, Or_opt_2, Shaw_1, Shift, Swap_1_1, Swap_2_1, Swap_2_2, Two_Opt};
	
	AILSObject.LSOperators = {Shaw_1, Or_opt_1, Swap_1_1, Two_Opt};
	
	// AILSObject.LSOperators = {Shaw_1};
	
	cout << S.FO();
	
	AILSObject.executeAILS(10000);
	
	AILSObject.S_p.print_sol();
	
	cout << "\n FO: " << std::setprecision(7) << AILSObject.S_p.FO() << endl;
	
	
}