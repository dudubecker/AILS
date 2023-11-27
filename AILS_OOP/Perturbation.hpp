#ifndef _Perturbation_H_
#define _Perturbation_H_
#include "Sol.hpp"
#include "Instance.hpp"
#include <iostream>


class Perturbation
{
public:
	
	// Número de nós a serem removidos e reinseridos pela perturbação (grau de perturbação)
	int w {3};
	
	// Quantidade de iterações pelas quais o método de perturbação passou (até o valor Gamma)
	int it {};
	
	// Distância média entre as soluções obtidas pela perturbação e a solução de referência s_r
	double avgDist {};
	
	// Probabilidade de aplicação do ruído aleatório
	double alpha = {};
	
	// Métodos
	
	// Método "padrão" para os objetos LocalSearchOperator
	virtual Sol aplicar(Sol &S);
	
	virtual Sol aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido) = 0;
	
	virtual ~Perturbation() {} // Adicionamos um destrutor virtual
	
	protected:
	// Construtor protegido para evitar instanciar a classe Heuristic diretamente
	Perturbation() {}
	
	
};

class RandomRemoval : public Perturbation {
public:
	
	// Constructor
	RandomRemoval(){};
	
	Sol aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido) override;
	
};

class WorstRemoval : public Perturbation {
public:
	
	// Parâmetro "delta" para controle da aleatoriedade 
	int delta {6};
	
	// Constructor
	WorstRemoval(int delta_value){
		
		delta = delta_value;
		
	};
	
	Sol aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido) override;
	
};

class ConcentricRemoval : public Perturbation {
public:
	
	// Constructor
	ConcentricRemoval(){};
	
	Sol aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido) override;
	
};

#endif // _Perturbation_H_
