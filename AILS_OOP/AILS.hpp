#ifndef _AILS_H_
#define _AILS_H_
#include <iostream>
#include "Sol.hpp"
#include "Instance.hpp"
#include "LocalSearchOperator.hpp"
#include "Perturbation.hpp"


class AILS
{
public:
	
	// Atributos
	
	// Vetor que guardará os operadores de busca local já inicializados
	std::vector<LocalSearchOperator> LSOperators {};
	
	// Vetor que guardará os métodos de perturbação já inicializados
	std::vector<Perturbation>  PerturbationProcedures {};
	
	// Melhor solução encontrada pelo algoritmo:
	Sol S_p;
	
	// Solução de referência, a ser alterada iterativamente e dada pelo critério de aceitação:
	Sol S_r;
	
	// Iterações do algoritmo:
	int it {0};
	
	// Parâmetros para controle do grau de perturbação:
	
	// Gamma: Determina quantas iterações cada heurística de perturbação realizará com um mesmo "peso"
	double Gamma {};
	
	// d_b: Distância de referência ("ideal") entre soluções. A distância de uma solução para outra é o número de arcos diferentes entre elas
	double d_b {};
	
	// Parâmetros para o critério de aceitação:
	
	// b_UP: Qualidade (FO) mínima de uma solução para que ela seja aceita pelo critério de aceitação
	double b_UP {};
	
	// f_UP: Qualidade média das soluções obtidas após as buscas locais
	double f_UP {};
	
	// f_UND: Qualidade da melhor solução encontrada nas últimas "min{it, Gamma}" iterações
	double f_UND {};
	
	double eta {};
	
	double kappa {};
	
	


	AILS();
	~AILS();
	
	// Métodos
	
	// Método para busca local (baseado em Random Variable Neighborhood Ordering)
	Sol LocalSearch(Sol &S);
	
	

};

#endif // _AILS_H_
