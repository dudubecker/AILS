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

AILS::AILS()
{
}

AILS::~AILS()
{
}

Sol AILS::routeReductionHeuristic(Sol &S_i){
	
	// Criando uma cópia da solução:
	Sol S = S_i;
	
	// Solução para armazenar a melhor solução (de menor número de rotas)
	// Sol BKS = S_i;
	
	// Para gerar números aleatórios
	srand(time(NULL));
	
	// Caso "L" esteja vazio, a rota é excluída e os pedidos são colocados no banco de pedidos não atendidos
	if (S.L.size() == 0){
		
		// Quantidade "m" de rotas na solução:
		int m = S.Rotas.size();
		
		// Escolhendo índice da rota que será removida
		double index_rota = rand()%(m);
		
		std::vector<double> Rota = S.Rotas.at(index_rota);
		
		// Removendo nós da solução
		for (auto &node: Rota){
			
			// Caso seja um nó correspondente a um pedido de pickup:
			if ((node > 0) && (node <= S.inst.n)){
				
				S.remover_pedido(node);
				
			}
			
		}
		
		// Removendo rota vazia da solução
		S.Rotas.erase(S.Rotas.begin() + index_rota);
		
		// Escolhendo um método de perturbação aleatório
		
		std::mt19937_64 rng;
		uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
		rng.seed(ss);
		// initialize a uniform distribution between 0 and 1
		std::uniform_real_distribution<double> unif(0, PerturbationProcedures.size());
		// ready to generate random numbers
		double n = unif(rng);
		
		int perturbationProcedureIndex = trunc(n);
		
		Perturbation perturbationProcedure = PerturbationProcedures.at(perturbationProcedureIndex);
		
		perturbationProcedure.apply(S, perturbationProcedure.w);
		
		if (S.L.size() == 0){
			
			S_i = S;
			
		}
		
		
	}
	
	
	return S_i;
	
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
		
		int index_LS = rand()%(LSOperatorsIt.size()); // rand()%
		
		LocalSearchOperator LSOperator = LSOperatorsIt.at(index_LS);
		
		// std::cout << "Nome: " << LSOperator.name << std::endl;
		
		LSOperator.apply(S);
		
		// Caso a solução tenha sido melhorada:
		if ((S.FO() < S_r_LS.FO()) && (S.isFeasible())){
		// if ((S.FO() < S_r_LS.FO()) && (S.L.size() <= S_r_LS.L.size()) && (S.isFeasible())){
			
			// std::cout << "Solucao melhorada! LSOperator: " << LSOperator.name << std::endl;
			
			// std::cout << "S_r: \n";
			
			// S_r.print_sol();
			
			// std::cout << "\n FO: " << S_r.FO() << std::endl;
			
			// std::cout << "S: \n";
			
			// S.print_sol();
			
			// std::cout << "\n FO: " << S.FO() << std::endl;
			
			// std::cout << "\n\n";
			
			// S = routeReductionHeuristic(S);
			
			// Atualizar solução de referência
			S_r_LS = S;
			
			// Restaurar vetor de operadores
			LSOperatorsIt = LSOperators;
			
		} else { // Caso a solução não tenha sido melhorada:
			
			// std::cout << "Solucao piorada! LSOperator: " << LSOperator.name << std::endl;
			
			// std::cout << "\nS_r: \n\n";
			
			// S_r.print_sol();
			
			// std::cout << "\n FO: " << S_r.FO() << std::endl;
			
			// std::cout << "\nS: \n\n";
			
			// S.print_sol();
			
			// std::cout << "\n\n";
			
			// std::cout << "\n FO: " << S.FO() << std::endl;
			
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
	
	for (auto Rota: S.Rotas){
		
		for (int index_no {0}; index_no < Rota.size() - 1; index_no++){
			
			E_S.at(Rota.at(index_no)).at(Rota.at(index_no + 1)) = 1;
			
			// std::cout << Rota.at(index_no) << " " << Rota.at(index_no + 1)<< " | ";
			
		}
		
		// std::cout << "\n";
		
	}
	
	// Contabilizando arcos da solução S_r
	
	for (auto Rota: S_r.Rotas){
		
		for (int index_no {0}; index_no < Rota.size() - 1; index_no++){
			
			E_S_r.at(Rota.at(index_no)).at(Rota.at(index_no + 1)) = 1;
			
			// std::cout << Rota.at(index_no) << " " << Rota.at(index_no + 1)<< " | ";
			
		}
		
		// std::cout << "\n";
		
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
			if ( E_S.at(i).at(j) !=  E_S_r.at(i).at(j)){
				
				// std::cout << i << " " << j << " | ";
				
				distance += 1;
				
			}
			
		}
		
	}
	
	// std::cout << "\nDistancia: " << distance << std::endl;
	
	return distance;
}

void AILS::updatePerturbationDegree(Sol &S, Sol &S_r, int perturbationProcedureIndex){ //Perturbation perturbationProcedure){
	
	// Contabilizando distância entre soluções:
	int distance = symmetricDistance(S, S_r);
	
	// Incrementando número de iterações da perturbação
	PerturbationProcedures.at(perturbationProcedureIndex).it += 1;
	
	// Alterando valor de distância média encontrada pela perturbação
	PerturbationProcedures.at(perturbationProcedureIndex).avgDist = ((PerturbationProcedures.at(perturbationProcedureIndex).avgDist)*(PerturbationProcedures.at(perturbationProcedureIndex).it - 1)+(distance))/(PerturbationProcedures.at(perturbationProcedureIndex).it);
	
	if (PerturbationProcedures.at(perturbationProcedureIndex).it == Gamma){
		
		PerturbationProcedures.at(perturbationProcedureIndex).w = std::round((PerturbationProcedures.at(perturbationProcedureIndex).w*d_b)/(PerturbationProcedures.at(perturbationProcedureIndex).avgDist));
		
		if (PerturbationProcedures.at(perturbationProcedureIndex).w < 0){
			
			PerturbationProcedures.at(perturbationProcedureIndex).w = 1;
			
		}
		
		// std::cout << "Valor atualizado, igual a: " << PerturbationProcedures.at(perturbationProcedureIndex).w << std::endl;
		
		// perturbationProcedure = std::min(S.inst.n, std::max_element(1, perturbationProcedure.w));
		
		if (PerturbationProcedures.at(perturbationProcedureIndex).w < 1){
			
			PerturbationProcedures.at(perturbationProcedureIndex).w = 1;
			
		} else if (PerturbationProcedures.at(perturbationProcedureIndex).w > S.inst.n){
			
			PerturbationProcedures.at(perturbationProcedureIndex).w = S.inst.n;
			
		}
		
		PerturbationProcedures.at(perturbationProcedureIndex).it = 0;
		
		PerturbationProcedures.at(perturbationProcedureIndex).avgDist = 0;
	}
	
}

// Método para o critério de aceitação 
bool AILS::acceptationCriterion(Sol &S){
	
	// Para gerar números aleatórios
	srand(time(NULL));
	
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
		
		std::mt19937_64 rng;
		uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
		rng.seed(ss);
		// initialize a uniform distribution between 0 and 1
		std::uniform_real_distribution<double> unif(0, 1);
		// ready to generate random numbers
		double n = unif(rng);
		
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

void AILS::executeAILS(int max_it){
	
	// Para gerar números aleatórios
	srand(time(NULL));
	
	// Variável para o número de iterações:
	
	while (it < max_it){
		
		// std::cout << "Iteracao: " << it << std::endl;
		
		// Escolhendo um método de perturbação aleatório
		
		std::mt19937_64 rng;
		uint64_t timeSeed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		std::seed_seq ss{uint32_t(timeSeed & 0xffffffff), uint32_t(timeSeed>>32)};
		rng.seed(ss);
		// initialize a uniform distribution between 0 and 1
		std::uniform_real_distribution<double> unif(0, PerturbationProcedures.size());
		// ready to generate random numbers
		double n = unif(rng);
		
		int perturbationProcedureIndex = trunc(n);
		
		Perturbation perturbationProcedure = PerturbationProcedures.at(perturbationProcedureIndex);
		
		// Aplicando método de perturbação

		// Assegurar que aqui o objeto "S_r" não está sendo alterado!
		
		Sol S = S_r;
		
		perturbationProcedure.apply(S, perturbationProcedure.w);
		
		try {
			
			S = LocalSearch(S);
			
		}
		
		catch (...){
			
			std::cout << "Erro!" << std::endl;
			
		}
		
		
		// só "LocalSearch(S)" funcionaria?
		
		// Atualizando grau de perturbação
		updatePerturbationDegree(S, S_r, perturbationProcedureIndex);
		
		// Aplicando critério de aceitação
		if (acceptationCriterion(S)){
			
			S_r = S;
			
			// std::cout << "Nova solucao de referencia" << std::endl;
			
			// S_r.print_sol();
			
			// std::cout << "\n FO: " << S_r.FO() << std::endl;
			
			
		}
		
		// Atualizando melhor solução encontrada
		
		if (S.FO() < S_p.FO()){
			
			S_p = S;
			
			// std::cout << "Novo otimo global descoberto" << std::endl;
			
			
		}
		
		it += 1;
		
	}
	
}
