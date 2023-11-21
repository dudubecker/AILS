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
	
	// Probabilidade de aplicação do ruído aleatório
	double alpha = {};
	
	// Métodos
	
	// Método "padrão" para os objetos LocalSearchOperator
	virtual Sol aplicar(Sol &S);
	
	virtual Sol aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido) = 0;
	
	virtual ~LocalSearchOperator() {} // Adicionamos um destrutor virtual
	
	protected:
	// Construtor protegido para evitar instanciar a classe Heuristic diretamente
	LocalSearchOperator() {}
	
	
};

class Swap : public LocalSearchOperator {
public:
	
	// Swap: trocando "k1" pedidos de uma rota "r1" e por "k2" pedidos de uma rota "r2"
	int k1 {0};
	
	int k2 {0};
	
	// Constructor
	Swap(int k1_value, int k2_value){
		
		k1 = k1_value;
		k2 = k2_value;
		
	}
	
	Sol aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido) override;
	
};

class Shift : public LocalSearchOperator {
public:
	
	// Shift: retirando-se "k" nós adjacentes de uma rota e inserindo-os em outra:
	int k {0};
	
	// Constructor
	Shift(int k_value){
		
		k = k_value;
		
	}
	
	Sol aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido) override;
	
};

class OrOpt : public LocalSearchOperator {
public:

	// Or-opt: retirando-se "k" pedidos de uma rota e inserindo-os na melhor posição
	int or_opt {0};
	
	// Constructor
	OrOpt(int or_opt_value){
		
		or_opt = or_opt_value;
		
	}
	
	Sol aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido) override;
};

class Shaw : public LocalSearchOperator {
public:
	
	// Shaw's Removal: parâmetros da própria heurística e quantidade de pedidos analisados
	
	int k_shaw {0};
	
	double phi {};
	double chi {};
	double psi {};
	
	// Constructor
	Shaw(int k_shaw_value, double phi_value, double chi_value, double psi_value){
		
		k_shaw = k_shaw_value;
		
		phi = phi_value;
		chi = chi_value;
		psi = psi_value;
		
	}
	
	Sol aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido) override;
};

#endif // _LSOperator_H_
