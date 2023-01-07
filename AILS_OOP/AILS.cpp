#include "AILS.hpp"

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
