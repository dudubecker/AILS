#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>
#include "Perturbation.hpp"
#include "heuristicsFunctions.h"
#include <numeric>
#include <iomanip>

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
			
			// Inserindo pedidos com método de primeira inserção factível aleatória (sem critérios para a ordem dos pedidos)
			
			// Pedidos não inseridos na solução (referenciar diretamente no for loop deu um bug)
			
			std::vector<double> pedidos_nao_inseridos = S.L;
			
			for (auto pedido: pedidos_nao_inseridos){
				
				// std::cout << "\nPedido: " << pedido << std::endl;
				
				S = primeira_insercao_factivel(S, pedido);
				
			}
			
			std::cout << "Solucao apos insercoes" << std::endl;
			
			S.print_sol();
			
			
			// Fim da heurística random removal
			break;
			
		}
		
		// Worst removal + Primeira Inserção Factível Aleatória
		case 'W':{
			
			std::cout << "Solucao apos worst removal" << std::endl;
			
			srand(time(NULL));
			
			// Parâmetro "delta" para controle da aleatoriedade 
			const int delta {6};
			
			// Pedidos contidos na solução: complemento de L!
			std::vector<double> pedidos = S.A;
			
			// Variável com quantidade de pedidos removidos
			int n_pedidos_removidos {0};
			
			// Início do algoritmo
			while (n_pedidos_removidos < n_requests){
				
				// Custos incrementais de cada pedido (não-ordenados)
				std::vector<double> delta_custos {};
				
				for (auto pedido: pedidos){
					
					// Calculando variação no custo da rota com a remoção do pedido
					double delta = delta_FO_rem(S, pedido);
					
					delta_custos.push_back(delta);
					
				}
				
				// Ordenando lista
				
				// Vetor "delta_custos_sorted", com ordenação dos custos incrementais em ordem decrescente
				std::vector<double> delta_custos_sorted = delta_custos;
				sort(delta_custos_sorted.begin(), delta_custos_sorted.end(), std::greater<double>());
				
				// Vetor "pedidos_sorted", com ordenação dos pedidos de acordo com custos de remoção
				std::vector<double> pedidos_sorted {};
				
				// Iterando no vetor de deltas ordenados, buscando índice correspondente no vetor de deltas não ordenados e trazendo pedido correspondente
				for (auto delta: delta_custos_sorted){
				
					// Índice no vetor não ordenado
					
					// Obtendo posição do valor da lista de semelhanças ordenada na lista de semelhanças não-ordenada
					auto index_delta = std::find(delta_custos.begin(), delta_custos.end(), delta);
					
					int index_pedido = std::distance(delta_custos.begin(), index_delta);
					
					// Guardando pedido correspondente na lista ordenada
					
					pedidos_sorted.push_back(pedidos.at(index_pedido));
					
				
				} 
				
				// Escolhendo pedido a ser removido
				
				// Valor aleatório p entre 0 e 1
				
				double p = (double) rand()/(RAND_MAX);
				
				// *obs: ver se está ok com a ALNS esse cálculo!
				
				// Posição na lista ordenada de pedidos do pedido a ser retirado da solução
				
				// Obs: "-1" é necessário porque, caso o valor p seja muito próximo de 1, a posição de retirada será igual ao tamanho do vetor e isso resultará em um erro de índice!
				
				double pos_retirada = round(pow(p, delta)*((pedidos_sorted).size()-1));
				
				// Pedido retirado
				
				double pedido_retirado = pedidos_sorted.at(pos_retirada);
				
				// Retirando pedido do vetor "pedidos"
				
				pedidos.erase(std::remove_if(pedidos.begin(), pedidos.end(), [&pedido_retirado](double value) -> bool { return value == pedido_retirado; }), pedidos.end());
				
				// Retirando pedido da solução
				
				S.remover_pedido(pedido_retirado);
				
				n_pedidos_removidos += 1;
				
			}
			
			S.print_sol();
			
			
			// Inserindo pedidos com método de primeira inserção factível aleatória (sem critérios para a ordem dos pedidos)
		
			// Pedidos não inseridos na solução (referenciar diretamente no for loop deu um bug)
			
			std::vector<double> pedidos_nao_inseridos = S.L;
			
			for (auto pedido: pedidos_nao_inseridos){
				
				// std::cout << "\nPedido: " << pedido << std::endl;
				
				S = primeira_insercao_factivel(S, pedido);
				
			}
			
			std::cout << "Solucao apos insercoes" << std::endl;
			
			S.print_sol();
			
			
			// Fim da heurística worst removal
			break;
		}
		
		default:
		std::cout << "Invalido" << std::endl;
			
	}
	
	// if S.isfeasible // se a mudança melhorou a função objetivo
	
	return S;
		
}

