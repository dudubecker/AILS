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
	std::vector<LocalSearchOperator*> operadores_busca_local {};
	
	// Vetor que guardará os métodos de perturbação já inicializados
	std::vector<Perturbation*> metodos_perturbacao {};
	
	// Melhor solução encontrada pelo algoritmo:
	Sol S_p;
	
	// Solução de referência, a ser alterada iterativamente e dada pelo critério de aceitação:
	Sol S_r;
	
	// Iterações do algoritmo:
	int it {1};
	
	// Parâmetros para controle do grau de perturbação:
	
	// Gamma: Determina quantas iterações cada heurística de perturbação realizará com um mesmo "peso"
	double gamma {};
	
	// d_b: Distância de referência ("ideal") entre soluções. A distância de uma solução para outra é o número de arcos diferentes entre elas
	double d_b {};
	
	// Parâmetros para o critério de aceitação:
	
	// b_UP: Qualidade (FO) mínima de uma solução para que ela seja aceita pelo critério de aceitação
	double b_UP {};
	
	// f_UP: Qualidade média das soluções obtidas após as buscas locais
	double f_UP {};
	
	// f_UND: Qualidade da melhor solução encontrada nas últimas "min{it, Gamma}" iterações
	double f_UND {};
	
	// determina b_UP no critério de aceitação -> qual valor usar inicialmente?
	double eta {};
	
	// Porcentagem de soluções aceitas
	double kappa {};
	
	// Atributos para determinação do valor de k_r (porcentagem de soluções menores do que b_up que foram, de fato, aceitas)
	
	// Quantidade de soluções menores do que b_up
	double qtd_solucoes_totais {};
	
	// Quantidade de soluções menores do que b_up que foram aceitas
	double qtd_solucoes_aceitas {};
	
	// Constructor
	AILS(Sol &S_i, std::vector<LocalSearchOperator*> &objetos_busca_local, std::vector<Perturbation*> &objetos_perturbacoes,
		double eta_value, double kappa_value, double gamma_value, double d_b_value, double eta_noise_value, double alpha_value);
	
	AILS();
	
	~AILS();
	
	// Métodos
	
	// Método para busca local (baseado em Random Variable Neighborhood Ordering)
	Sol executarBuscaLocal(Sol &S);
	
	// Método para determinação da distância simétrica entre duas soluções, utilizado no controle da perturbação
	int calcularDistanciaSimetrica(Sol &S, Sol &S_r);
	
	// Método para atualização dos graus de perturbação
	void atualizarGrauDePerturbacao(Sol &S, Sol &S_r, int index_metodo_perturbacao);
	
	// Método para o critério de aceitação 
	bool avaliarCriterioDeAceitacao(Sol &S);
	
	// Método para execução do algoritmo em si:
	void executarAILS(int max_it, int max_it_sem_melhoria, int it_RRH_intervalo, int it_RRH, double max_t);
	
	// Método que tentará reduzir rotas
	Sol reduzirRotas(Sol &S_i, int it_RRH);
	
};


#endif // _AILS_H_
