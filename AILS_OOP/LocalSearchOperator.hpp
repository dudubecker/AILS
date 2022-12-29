#ifndef _LSOperator_H_
#define _LSOperator_H_
#include <iostream>
#include "Sol.hpp"
#include "Instance.hpp"



class LocalSearchOperator
{
public:

	// Nome do operador: 
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
	
	// Swap: trocando "k1" pedidos de uma rota "r1" e por "k2" pedidos de uma rota "r2"
	int k1 {0};
	
	int k2 {0};
	
	// Shift: retirando-se "k" nós adjacentes de uma rota e inserindo-os em outra:
	int k {0};
	
	// Or-opt: retirando-se "k" pedidos de uma rota e inserindo-os na melhor posição
	int or_opt {0};
	
	// LocalSearchOperator();
	
	// Inicialização padrão, apenas com o character
	LocalSearchOperator(char operator_name){
		
		name = operator_name;
	}
	
	// Inicialização do operador de Swap
	LocalSearchOperator(char operator_name, int k1_value, int k2_value){
		
		name = operator_name;
		
		k1 = k1_value;
		k2 = k2_value;
		
	}
	
	// Inicialização do operador de Shift e Or-opt:
	
	// Funciona para ambos operadores, diferindo-se no switch-case!
	
	LocalSearchOperator(char operator_name, int value){
		
		name = operator_name;
		
		k = value;
		
		or_opt = value;
		
	}
	
	~LocalSearchOperator();
	
	// Methods
	
	// Método de aplicação do operador: tem como entrada uma solução e como saída uma solução alterada (Rotas, L e A)
	Sol apply(Sol &S);
	

};

#endif // _LSOperator_H_
