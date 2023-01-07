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
	
	// Número de nós a serem removidos e reinseridos pela perturbação (grau de perturbação)
	int w {};
	
	// Quantidade de iterações pelas quais o método de perturbação passou (até o valor Gamma)
	double it {};
	
	// Distância média entre as soluções obtidas pela perturbação e a solução de referência s_r
	double avgDist {};
	
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
