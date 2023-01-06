#include "AILS.hpp"

AILS::AILS()
{
}

AILS::~AILS()
{
}


Sol AILS::LocalSearch(Sol &S){
	
	// Criando uma cópia do vetor que contém os operadores de busca local
	std::vector<LocalSearchOperator> LSOperatorsIt = LSOperators;
	
	// Enquanto o vetor não está vazio
	while(LSOperatorsIt.size() > 0){
		
		// Aplicar operador de busca local aleatório
		
		
		// Caso a solução tenha sido melhorada:
		
			// Atualizar solução incumbente
			// Restaurar vetor de operadores
			
		// Caso a solução não tenha sido melhorada:
		
			// Excluir operador do vetor
		
		
	}
	
	
	
}
