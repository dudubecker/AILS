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
	
	// Swap: trocando "k1" pedidos de uma rota "r1" e por "k2" pedidos de uma rota "r2"
	int k1 {0};
	
	int k2 {0};
	
	// Shift: retirando-se "k" nós adjacentes de uma rota e inserindo-os em outra:
	int k {0};
	
	// Or-opt: retirando-se "k" pedidos de uma rota e inserindo-os na melhor posição
	int or_opt {0};
	
	// Shaw's Removal: parâmetros da própria heurística e quantidade de pedidos analisados
	
	int k_shaw {0};
	
	double phi {};
	double chi {};
	double psi {};
	
	// Probabilidade de aplicação do ruído aleatório
	double alpha = {};
	
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
	
	// Inicialização para operador Shaw's, com diferentes parâmetros
	
	LocalSearchOperator(char perturbation_name, int k_shaw_value,double phi_value, double chi_value, double psi_value){
		
		name = perturbation_name;
		
		phi = phi_value;
		chi = chi_value;
		psi = psi_value;
		
		k_shaw = k_shaw_value;
		
	}
	
	~LocalSearchOperator();
	
	// Methods
	
	// Método de aplicação do operador: tem como entrada uma solução e como saída uma solução alterada (Rotas, L e A)
	Sol apply(Sol &S);
	

};

#endif // _LSOperator_H_
