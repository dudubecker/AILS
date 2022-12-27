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
	
	// Swap: trocando "k1" pedidos de uma rota "r1" e por "k2" pedidos de uma rota "r2"
	int k1 {0};
	
	int k2 {0};
	
	// Shift: retirando-se "k" nós adjacentes de uma rota e inserindo-os em outra:
	int k {0};
	
	// Or-opt: retirando-se "k" pedidos de uma rota e inserindo-os na melhor posição
	int or_opt {0};
	
	// Heuristic();
	
	// Inicialização padrão, apenas com o character
	Heuristic(char heuristic_name){
		
		name = heuristic_name;
	}
	
	// Inicialização da heurística de Swap
	Heuristic(char heuristic_name, int k1_value, int k2_value){
		
		name = heuristic_name;
		
		k1 = k1_value;
		k2 = k2_value;
		
	}
	
	// Inicialização da heurística de Shift e Or-opt:
	
	// Funciona para ambos operadores, diferindo-se no switch-case!
	
	Heuristic(char heuristic_name, int value){
		
		name = heuristic_name;
		
		k = value;
		
		or_opt = value;
		
	}
	
	
	
	~Heuristic();
	
	
	// Methods
	
	// Método de aplicação da heurística: tem como entrada uma solução e como saída uma solução alterada (Rotas, L e A)
	Sol apply(Sol &S);


};

#endif // _HEURISTIC_H_
