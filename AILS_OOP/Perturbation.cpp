#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>
#include "Perturbation.hpp"
#include "heuristicsFunctions.h"
#include <numeric>

//Perturbation::Perturbation()
//{
//}

Perturbation::~Perturbation()
{
}

// Método principal das perturbações

// Método de aplicação das perturbações
// *OBS: vou colocar um switch case por enquanto, mas certamente existe um jeito melhor de fazer isso...

Sol Perturbation::apply(Sol &S, int n_requests){
	
	switch (name){
		
		// Random removal + Primeira Inserção Factível Aleatória
		case 'R':{
			
			std::cout << "Solucao apos random removal" << std::endl;
			
			srand(time(NULL));
			
			// Variável com quantidade de pedidos removidos
			int n_pedidos_removidos {0};
			
			// Retirando pedidos da solução iterativamente
			while (n_pedidos_removidos < n_requests){
				
				double pedido_aleatorio = S.A.at(rand()%S.A.size());
				
				S.remover_pedido(pedido_aleatorio);
				
				n_pedidos_removidos += 1;
				
			}
			
			
			S.print_sol();
			
			// Inserindo pedidos com método de primeira inserção factível aleatória
			
			double pedido = S.L.at(0);
			
			// Criando vetor 2D para armazenar todas as combinações possíveis (rota, pos_insercao_no_pickup, pos_insercao_no_delivery)
			std::vector<std::vector<int>> pos_insercoes {};
			
			for (auto index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
				
				for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < S.Rotas[index_rota].size() + 1; pos_insercao_no_pickup++){
					
					for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < S.Rotas[index_rota].size() + 1; pos_insercao_no_delivery++){
						
						// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
						// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
						// if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
						if (pos_insercao_no_pickup < pos_insercao_no_delivery){
							
							pos_insercoes.push_back({index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery});
							
						}
					}
				}
			}
			
			// Printando vetor:
			
			//for (auto vetor: pos_insercoes){
				
			//	std::cout << "\n";
				
			//	for (auto i: vetor){
					
			//		std::cout << i << " ";
					
			//	}
			//}
			
			// Método para "sortear" uma combinação aleatória e assegurar que todas foram testadas:
			
			// Número de combinações disponíveis para teste, atualizado dinamicamente
			int n_combinacoes = pos_insercoes.size();
			
			// Vetor com índices de combinações
			std::vector<int> index_combinacoes(n_combinacoes);
			std::iota(index_combinacoes.begin(), index_combinacoes.end(), 0);
			
			// Aleatorizando vetor com índices de combinações
			std::random_shuffle(index_combinacoes.begin(), index_combinacoes.end());
			
			int contador {0};
			
			while (n_combinacoes > 0){
				
				// Escolhendo objeto correspondente ao primeiro índice aleatório
				std::vector<int> combinacao = pos_insercoes.at(index_combinacoes.at(0));
				
				// Removendo índice do vetor de índices de combinações
				index_combinacoes.erase(index_combinacoes.begin());
				
				std::cout << "Testando posicao numero: " << contador << std::endl;
				contador++;
				
				// Avaliando factibilidade da inserção
				if (isInsertionFeasible(S, pedido, combinacao.at(0), combinacao.at(1), combinacao.at(2))){
					
					S.inserir_pedido(pedido, combinacao.at(0), combinacao.at(1), combinacao.at(2));
					
					break;
					
				}
				
				// Se a inserção for factível, o loop se interrompe
				
				// Atualizando valor do número de combinações disponíveis para serem testadas
				n_combinacoes -= 1;
				
			}
			
			std::cout << "Solucao apos insercao" << std::endl;
			
			S.print_sol();
			
			// Fim da heurística random removal
			break;
			
		}
		
		default:
		std::cout << "Invalido" << std::endl;
			
	}
	
	// if S.isfeasible // se a mudança melhorou a função objetivo
	
	return S;
		
}

