#include "AILS.hpp"
#include "LocalSearchOperator.hpp"
#include "Sol.hpp"
#include "Instance.hpp"
#include "heuristicsFunctions.h"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>

AILS::AILS()
{
}

AILS::~AILS()
{
}

// Método de aplicação iterativa das buscas locais até não haver mais melhorias

Sol AILS::LocalSearch(Sol &S){
	
	// Criando cópia da solução, para controle das melhorias
	
	// Solução de referência
	Sol S_r = S;
	
	// Criando uma cópia do vetor que contém os operadores de busca local
	std::vector<LocalSearchOperator> LSOperatorsIt = LSOperators;
	
	// Enquanto o vetor não está vazio
	while(LSOperatorsIt.size() > 0){
		
		// Aplicar operador de busca local aleatório
		
		int index_LS = rand()%(LSOperatorsIt.size()); // rand()%
		
		LocalSearchOperator LSOperator = LSOperatorsIt.at(index_LS);
		
		LSOperator.apply(S);
		
		// Caso a solução tenha sido melhorada:
		if ((S.FO() < S_r.FO()) && (S.L.size() <= S_r.L.size()) && (S.isFeasible())){
			
			std::cout << "Solucao melhorada! LSOperator: " << LSOperator.name << std::endl;
			
			std::cout << "S_r: \n";
			
			S_r.print_sol();
			
			std::cout << "\n FO: " << S_r.FO() << std::endl;
			
			std::cout << "S: \n";
			
			S.print_sol();
			
			std::cout << "\n FO: " << S.FO() << std::endl;
			
			std::cout << "\n\n";
			
			// Atualizar solução de referência
			S_r = S;
			
			// Restaurar vetor de operadores
			LSOperatorsIt = LSOperators;
			
		} else { // Caso a solução não tenha sido melhorada:
			
			std::cout << "Solucao piorada! LSOperator: " << LSOperator.name << std::endl;
			
			std::cout << "\nS_r: \n\n";
			
			S_r.print_sol();
			
			std::cout << "\n FO: " << S_r.FO() << std::endl;
			
			std::cout << "\nS: \n\n";
			
			S.print_sol();
			
			std::cout << "\n\n";
			
			std::cout << "\n FO: " << S.FO() << std::endl;
			
			// Restaurar solução incumbente
			S = S_r;
			
			// Excluir operador do vetor
			LSOperatorsIt.erase(LSOperatorsIt.begin() + index_LS);
			
		}
		
	}
	
	return S_r;
	
}

// Método para determinação da distância simétrica entre duas soluções
int AILS::symmetricDistance(Sol &S, Sol &S_r){
	
	// Variável para armazenar o número de arcos incomuns entre as soluções (distância simétrica)
	int distance {};
	
	// Matriz de 1 e 0 para arcos da solução S
	std::vector<std::vector<int>> E_S (2*S.inst.n + 2, std::vector<int> (2*S.inst.n + 2, 0));
	
	// Matriz de 1 e 0 para arcos da solução S_r
	std::vector<std::vector<int>> E_S_r = E_S;
	
	// Contabilizando arcos da solução S
	
	// std::cout << "\nArcos: ";
	
	for (auto Rota: S.Rotas){
		
		for (int index_no {0}; index_no < Rota.size() - 1; index_no++){
			
			E_S.at(Rota.at(index_no)).at(Rota.at(index_no + 1)) = 1;
			
			// std::cout << Rota.at(index_no) << " " << Rota.at(index_no + 1)<< " | ";
			
		}
		
		// std::cout << "\n";
		
	}
	
	// Contabilizando arcos da solução S_r
	
	for (auto Rota: S_r.Rotas){
		
		for (int index_no {0}; index_no < Rota.size() - 1; index_no++){
			
			E_S_r.at(Rota.at(index_no)).at(Rota.at(index_no + 1)) = 1;
			
			// std::cout << Rota.at(index_no) << " " << Rota.at(index_no + 1)<< " | ";
			
		}
		
		// std::cout << "\n";
		
	}
	
	// Printando matriz:
	// std::cout << "\nMatriz: \n";
	
	// for (int i {0}; i < 2*S.inst.n + 2 ; i++){
		
	// 	for (int j {0}; j < 2*S.inst.n + 2 ; j++){
		
	// 		std::cout << E_S.at(i).at(j) << " ";
	// 	}
		
	// 	std::cout << "\n";
	// }
	
	// Contabilizando arcos incomuns entre as soluções:
	
	for (int i {0}; i < 2*S.inst.n + 2 ; i++){
		
		for (int j {0}; j < 2*S.inst.n + 2 ; j++){
			
			// Caso o número em "i, j" nas matrizes seja diferente:
			if ( E_S.at(i).at(j) !=  E_S_r.at(i).at(j)){
				
				// std::cout << i << " " << j << " | ";
				
				distance += 1;
				
			}
			
		}
		
	}
	
	// std::cout << "\nDistancia: " << distance << std::endl;
	
	return distance;
}

void AILS::updatePerturbationDegree(Sol &S, Sol &S_r, Perturbation perturbationProcedure){
	
	// Contabilizando distância entre soluções:
	int distance = symmetricDistance(S, S_r);
	
	// Incrementando número de iterações da perturbação
	perturbationProcedure.it += 1;
	
	// Alterando valor de distância média encontrada pela perturbação
	perturbationProcedure.avgDist = ((perturbationProcedure.avgDist)*(perturbationProcedure.it - 1)+(distance))/(perturbationProcedure.it);
	
	if (perturbationProcedure.it == Gamma){
		
		perturbationProcedure.w = std::round((perturbationProcedure.w*d_b)/(perturbationProcedure.avgDist));
		
		// perturbationProcedure = std::min(S.inst.n, std::max_element(1, perturbationProcedure.w));
		
		if (perturbationProcedure.w < 1){
			
			perturbationProcedure.w = 1;
			
		} else if (perturbationProcedure.w > S.inst.n){
			
			perturbationProcedure.w = S.inst.n;
			
		}
		
		perturbationProcedure.it = 0;
		
		perturbationProcedure.avgDist = 0;
	}
	
	
	
}
