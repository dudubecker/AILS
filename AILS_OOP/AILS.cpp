#include "AILS.hpp"
#include "LocalSearchOperator.hpp"
#include "Sol.hpp"
#include "Instance.hpp"
#include "heuristicsFunctions.h"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>
#include <random>



// Constructor com parâmetros da heurística
AILS::AILS(Sol &S_inicial, std::vector<LocalSearchOperator> &LSOperatorsObjects, std::vector<Perturbation> &PerturbationProceduresObjects,
		double eta_value, double kappa_value, double Gamma_value, double d_b_value, double eta_noise_value, double alpha_value)
		
{
	
	// Atribuindo argumentos aos parâmetros
	eta = eta_value;
	kappa = kappa_value;
	Gamma = Gamma_value;
	d_b = d_b_value;
	
	// Inicializando objetos
	
	// Métodos de perturbação
	PerturbationProcedures = PerturbationProceduresObjects;
	
	// Métodos de busca local
	LSOperators = LSOperatorsObjects;
	
	// Inicializando cálculos de ruído
	
	// Parâmetro alpha - probabilidade de aplicação do ruído - atributo de LSOperators e PerturbationProcedures
	for (auto LSOperator: LSOperators){
		
		LSOperator.alpha = alpha_value;
		
	}
	
	for (auto PerturbationProcedure: PerturbationProcedures){
		
		PerturbationProcedure.alpha = alpha_value;
		
	}
	
	// Parâmetro eta_noise - utilizado no cálculo da dimensão do ruído - atributo de Sol
	S_inicial.eta_noise = eta_noise_value;
	
	
	// Solução incumbente e melhor solução encontrada:
	S_p = S_inicial;
	
	S_r = S_inicial;
	
}

// Default contructor
AILS::AILS()
{
	
	
}



AILS::~AILS()
{
}

Sol AILS::routeReductionHeuristic(Sol &S_i, int it_RRH){
	
	
	// Criando uma cópia da rota:
	Sol S = S_i;
	
	// Solução para armazenar a melhor solução (de menor número de rotas)
	Sol BKS = S_i;
	
	// Variável para o número de iterações:
	int n_it {0};
	
	while (n_it < it_RRH){
		
		// Caso "L" esteja vazio, a rota é excluída e os pedidos são colocados no banco de pedidos não atendidos
		if (S.L.size() == 0){
			
			BKS = S;
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota que será removida
			double index_rota = rand()%(m);
			
			// Removendo rota
			S.remover_rota(index_rota);
			
		}
		
		// Escolhendo perturbações e aplicando-as à solução
		
		// Escolhendo e aplicando método de perturbação
		
		// int randomIndex = std::rand() % PerturbationProcedures.size();
		
		// int perturbationProcedureIndex = trunc(randomIndex);
		
		// Determinando quantidade de nós a serem removidos e reinseridos pela heurística
		
		int RRH_nodes = {};
		
		if (S.Rotas.size() <= 2){
			
			RRH_nodes = 1;
			
		} else {
			
			RRH_nodes = 4;
			
		}
		
		// Aplicando random removal
		PerturbationProcedures.at(0).apply(S, RRH_nodes);
		
		n_it += 1;
		
	}
	
	return BKS;
	
	
	
}

// Método de aplicação iterativa das buscas locais até não haver mais melhorias

Sol AILS::LocalSearch(Sol &S){
	
	// Criando cópia da solução, para controle das melhorias
	
	// Solução de referência da busca local
	Sol S_r_LS = S;
	
	// Criando uma cópia do vetor que contém os operadores de busca local
	std::vector<LocalSearchOperator> LSOperatorsIt = LSOperators;
	
	// Enquanto o vetor não está vazio
	while(LSOperatorsIt.size() > 0){
		
		// Aplicar operador de busca local aleatório
		int index_LS = rand()%(LSOperatorsIt.size());
		
		LocalSearchOperator LSOperator = LSOperatorsIt.at(index_LS);
		
		LSOperator.apply(S);
		
		// Caso a solução tenha sido melhorada:
		if ((S.FO() < S_r_LS.FO())){
			
			// Atualizar solução de referência
			S_r_LS = S;
			
			// Restaurar vetor de operadores
			LSOperatorsIt = LSOperators;
			
		} else { // Caso a solução não tenha sido melhorada:
			
			// Restaurar solução incumbente
			S = S_r_LS;
			
			// Excluir operador do vetor
			LSOperatorsIt.erase(LSOperatorsIt.begin() + index_LS);
			
		}
		
	}
	
	return S_r_LS;
	
}

// Método para determinação da distância simétrica entre duas soluções
int AILS::symmetricDistance(Sol &S, Sol &S_r){
	
	// Variável para armazenar o número de arcos incomuns entre as soluções (distância simétrica)
	int distance {};
	
	// Matriz de 1 e 0 para arcos da solução S
	std::vector<std::vector<int>> E_S (2*S.inst.n + 2, std::vector<int> (2*S.inst.n + 2, 0));
	
	// Matriz de 1 e 0 para arcos da solução S_r
	std::vector<std::vector<int>> E_S_r = E_S;
	
	// Contabilizando arcos da solução S
	
	// std::cout << "\nArcos: ";
	
	for (int index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
		
		for (int index_no {0}; index_no < S.RotasSize.at(index_rota) - 1; index_no++){
			
			E_S.at(S.Rotas.at(index_rota).at(index_no)).at(S.Rotas.at(index_rota).at(index_no + 1)) = 1;
			
		}
		
	}
	
	
	// Contabilizando arcos da solução S_r
	
	for (int index_rota {0}; index_rota < S_r.Rotas.size(); index_rota++){
		
		for (int index_no {0}; index_no < S_r.RotasSize.at(index_rota) - 1; index_no++){
			
			E_S_r.at(S_r.Rotas.at(index_rota).at(index_no)).at(S_r.Rotas.at(index_rota).at(index_no + 1)) = 1;
			
		}
		
	}
	
	// Printando matriz:
	// std::cout << "\nMatriz: \n";
	
	// for (int i {0}; i < 2*S.inst.n + 2 ; i++){
		
	// 	for (int j {0}; j < 2*S.inst.n + 2 ; j++){
		
	// 		std::cout << E_S.at(i).at(j) << " ";
	// 	}
		
	// 	std::cout << "\n";
	// }
	
	// Contabilizando arcos incomuns entre as soluções:
	
	for (int i {0}; i < 2*S.inst.n + 2 ; i++){
		
		for (int j {0}; j < 2*S.inst.n + 2 ; j++){
			
			// Caso o número em "i, j" nas matrizes seja diferente:
			if (E_S.at(i).at(j) !=  E_S_r.at(i).at(j)){
				
				// std::cout << i << " " << j << " | ";
				distance += 1;
				
			}
			
		}
		
	}
	
	return distance;
}

void AILS::updatePerturbationDegree(Sol &S, Sol &S_r, int perturbationProcedureIndex){ //Perturbation perturbationProcedure){
	
	
	int distance = symmetricDistance(S, S_r);
	
	// Contabilizando distância entre soluções:
	
	// Incrementando número de iterações da perturbação
	PerturbationProcedures.at(perturbationProcedureIndex).it += 1;
	
	// Alterando valor de distância média encontrada pela perturbação
	PerturbationProcedures.at(perturbationProcedureIndex).avgDist = ((PerturbationProcedures.at(perturbationProcedureIndex).avgDist)*(PerturbationProcedures.at(perturbationProcedureIndex).it - 1)+(distance))/(PerturbationProcedures.at(perturbationProcedureIndex).it);
	
	// std::cout << PerturbationProcedures.at(perturbationProcedureIndex).avgDist << std::endl;
	
	if (PerturbationProcedures.at(perturbationProcedureIndex).it == Gamma){
		
		// Valor do novo grau de perturbação calculado
		
		// Corrigindo bug (relativamente raro) quando avgDist é igual a 0:
		if (PerturbationProcedures.at(perturbationProcedureIndex).avgDist == 0){
			
			PerturbationProcedures.at(perturbationProcedureIndex).avgDist = 1;
			
		}
		
		// Novo valor para o grau de perturbação
		int new_perturbation_degree = std::round((PerturbationProcedures.at(perturbationProcedureIndex).w*d_b)/(PerturbationProcedures.at(perturbationProcedureIndex).avgDist));
		
		PerturbationProcedures.at(perturbationProcedureIndex).w = std::min(S.inst.n/2, (std::max(1, new_perturbation_degree)));
		
		// Reiniciando contagens
		PerturbationProcedures.at(perturbationProcedureIndex).it = 0;
		
		PerturbationProcedures.at(perturbationProcedureIndex).avgDist = 0;
		
	}
}

// Método para o critério de aceitação 
bool AILS::acceptanceCriteria(Sol &S){
	
	// Valor da função objetivo correspondente à solução f(S):
	double f_S = S.FO();
	
	// Variável para retornar resultado do critério de aceitação
	bool Accept = false;
	
	// Atualizando o valor de f_UP e f_UND
	
	// Caso o número de iterações não tenha chegado a "Gamma":
	if (it <= Gamma){
		
		f_UP = ((f_UP)*(it - 1) + (f_S) )/(it);
		f_UND = S_p.FO();
		
		
	// Caso o algoritmo já tenha executado mais do que "Gamma" iterações:
	}else{
		
		f_UP = f_UP*(1 - 1/Gamma) + f_S/Gamma;
		
	}
	
	// Atualizando valor de eta
	
	// O valor de eta é atualizado se o número de soluções aceitas atinge o valor Gamma
	//if (qtdSolucoesAceitas == Gamma){
	if (qtdSolucoesTotais == Gamma){
		
		eta = (kappa*eta)/(qtdSolucoesAceitas/qtdSolucoesTotais);
		
		// Caso eta resulte um valor muito pequeno
		if (eta < 0.001){
			
			eta = 0.001;
			
		} else if (eta > 1){
			
			eta = 1;
			
		}
		
		// Zerando valores com quantidades de soluções totais e aceitas
		qtdSolucoesAceitas = 0;
		qtdSolucoesTotais = 0;
		
	}
	
	// Atualizando valor de b_UP
	
	// std::cout << "f_UP: " << f_UP << std::endl;
	
	b_UP = f_UND + eta*(f_UP - f_UND);
	
	// std::cout << "b_UP: " << b_UP << std::endl;
	
	// Se a solução atual for menor do que o limite calculado, haverá uma chance de "kappa" de ela ser escolhida
	
	if (f_S < b_UP){
		
		// Valor aleatório "n" entre 0 e 1
		double n = static_cast<double>(std::rand()) / RAND_MAX;
		
		// Caso o número aleatório seja menor do que kappa (kappa*100% chance)
		if (n < kappa){
			
			// Solução aceita!
			Accept = true;
			
			// std::cout << "Solucao aceita! " << std::endl;
			
			qtdSolucoesAceitas += 1;
			
		}
		
		// Independentemente da solução ter sido aceita, com kappa*100% de chance, computa-se a quantidade de soluções menores do que b_UP
		qtdSolucoesTotais += 1;
		
	}
	
	return Accept;
	
	
}

void AILS::executeAILS(int max_it, int max_it_no_improv, int it_RRH_interval, int it_RRH){
	
	// Para gerar números aleatórios
	// srand(time(NULL));
	
	// Variável para o número de iterações sem melhoria:
	int no_improvement_iterations = 0;
	
	while (it < max_it){
		
		if (no_improvement_iterations == max_it_no_improv){
			
			break;
			
		}
		
		
		//if (it%1000 == 0){
			
		//	std::cout << "Iteracao " << it << std::endl;
			
		//}
		
		
		// Criando uma cópia da solução de referência
		
		Sol S = S_r;
		
		// Aplicando código para redução de rotas, a cada it_RRH_interval iterações
		
		if ((it%it_RRH_interval == 0) && (S.Rotas.size() > 1)){
			
			routeReductionHeuristic(S, it_RRH);
			
		}
		
		// Escolhendo e aplicando método de perturbação
		
		int randomIndex = std::rand() % PerturbationProcedures.size();
		
		int perturbationProcedureIndex = trunc(randomIndex);
		
		PerturbationProcedures.at(perturbationProcedureIndex).apply(S, PerturbationProcedures.at(perturbationProcedureIndex).w);
		
		// Aplicando buscas locais, na forma de RVND
		LocalSearch(S);
		
		// Atualizando grau de perturbação
		updatePerturbationDegree(S, S_r, perturbationProcedureIndex);
		
		
		// Aplicando critério de aceitação
		if (acceptanceCriteria(S)){
			
			S_r = S;
			
			// std::cout << "Nova solucao de referencia" << std::endl;
			
			// S_r.print_sol();
			
			// std::cout << "\n FO: " << S_r.FO() << std::endl;
			
			
		}
		
		// Atualizando melhor solução encontrada
		
		if (S.FO() < S_p.FO()){
			
			S_p = S;
			
			no_improvement_iterations = 0;
			
		} else {
			
			no_improvement_iterations += 1;
			
		}
		
		

		it += 1;
		
	}
	
}