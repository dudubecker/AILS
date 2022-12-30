#ifndef _Perturbation_H_
#define _Perturbation_H_
#include <iostream>
#include "Sol.hpp"
#include "Instance.hpp"

class Perturbation
{
public:

	// Nome do método de perturbação: 
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
	
	// ... Demais parâmetros específicos de cada operador
	
	//Perturbation();
	
	// Inicialização padrão, apenas com o character
	Perturbation(char perturbation_name){
		
		name = perturbation_name;
		
	}
	
	~Perturbation();
	
	// Methods
	
	// Método de aplicação da perturbação: tem como entrada uma solução e um número de pedidos a serem alterados, e como saída uma solução alterada (Rotas, L e A)
	Sol apply(Sol &S, int n_requests);
	
	
};

#endif // _Perturbation_H_
