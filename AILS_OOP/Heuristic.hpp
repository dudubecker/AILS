#ifndef _HEURISTIC_H_
#define _HEURISTIC_H_
#include <iostream>
#include "Sol.hpp"
#include "Instance.hpp"


class Heuristic
{
public:
	
		// Nome da heurística: 
	char name {};
	
	// Score da heurística, atualizado dinamicamente no algoritmo
	// double score {0};
	
	// Peso (probabilidade de escolha) da heurística, atualizado dinamicamente no algoritmo
	// double weight {0};
	
	// Número de iterações pelas quais a heurística passou
	// int n_it {0};
	
		// Tempo computacional empregado na heurística:
	// double processing_time {};
	
	
	// Número total de iterações pelas quais a heurística passou
	// int n_it_total {0};
	
	// ... Demais parâmetros específicos de cada heurística
	
	
	
	
	
	// Heuristic();
	
	// Inicialização padrão, apenas com o character
	Heuristic(char heuristic_name){
		
		name = heuristic_name;
	}
	
	
	~Heuristic();
	
	
	// Methods
	
	// Método de aplicação da heurística: tem como entrada uma solução e como saída uma solução alterada (Rotas, L e A)
	Sol apply(Sol &S);


};

#endif // _HEURISTIC_H_
