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
			
			// Aleatorizar "pedidos_nao_inseridos"?
			
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
			
			// *obs: aleatorizar "pedidos_nao_inseridos"?
			
			for (auto pedido: pedidos_nao_inseridos){
				
				// std::cout << "\nPedido: " << pedido << std::endl;
				
				S = primeira_insercao_factivel(S, pedido);
				
			}
			
			std::cout << "Solucao apos insercoes" << std::endl;
			
			S.print_sol();
			
			// Fim da heurística worst removal
			break;
		}
		
		/*
		// Shaw's removal + Primeira Inserção Factível Aleatória
		case 'S':{
			
			srand(time(NULL));
			
			std::cout << "Solucao apos shaws removal" << std::endl;
			
			// Parâmetro "delta" para controle da aleatoriedade 
			const int delta {6};
			
			// Obtendo tempos de visita (T_i) de cada nó "i" na solução S
			
			std::vector <double> T (2*(S.inst.n) + 2, 0);
			
			// Iterando para cada rota na solução 
			for (auto &rota: S.Rotas){
				
				// Inicializando tempo da rota
				int tempo_rota {0};
				
				// Iterando para cada nó na rota 
				for(unsigned index_no {0}; index_no < rota.size() - 1; index_no++){
					
					// Nó atual
					int no_atual = rota.at(index_no);
					
					// Nó seguinte
					int no_seguinte = rota.at(index_no+1);
					
					// Atualizando valor de tempo de visita no vetor T
					T[no_atual] = tempo_rota;
					
					// Incrementando tempo da rota
					tempo_rota += S.inst.t.at(no_atual).at(no_seguinte);
					
				}
			}
			
			// Tempos de visita (T):
			// Vetor com tempos de visita normalizados
			std::vector <double> T_norm (2*(S.inst.n) + 2, 0);
			
			// Valor mínimo para tempos de visita
			double min_T = *min_element(T.begin(), T.end());
			// Valor máximo para tempos de visita
			double max_T = *max_element(T.begin(), T.end());
			
			// Normalização de T
			
			for (int i {0}; i < 2*(S.inst.n) + 2; i++){
				
				// Normalizando valores de tempos de visita
				
				T_norm[i] = (T[i] - min_T)/(max_T - min_T);
				
			}
			
			// Obtendo grau de semelhança R entre pedidos da instância
			// Pontos importantes: entre nós iguais, esse grau de semelhança será igual a 0! É necessário tratar isso!
			// Além disso, R não estará definido para o depósito central, já que esses nós não poderão ser retirados!
			
			// Vetor com graus de semelhança
			std::vector<std::vector<double>> R(S.inst.n + 1, std::vector<double> (S.inst.n + 1, 9999));
			
			for (unsigned i {1}; i < S.inst.n + 1; i++){
				for (unsigned j {1}; j < S.inst.n + 1; j++){
					
					 // Calculando valor de R_ij
					
					// Caso o pedido não esteja contido na solução, seu grau de proximidade com qualquer outro pedido mantem-se em 9999
					if ((count(S.L.begin(), S.L.end(), i)) || (count(S.L.begin(), S.L.end(), j))){
						;
					}else{
						R[i][j] = phi*(S.inst.t_norm[i][j] + S.inst.t_norm[i+S.inst.n][j+S.inst.n]) + chi*(abs(T_norm[i] - T_norm[j]) + abs(T_norm[i+S.inst.n] - T_norm[j+S.inst.n])) + psi*(abs(S.inst.q_norm[i] - S.inst.q_norm[j]));
					}
					
				}
			}
			
			
			// Vetor com pedidos disponíveis para remoção
			std::vector<double> pedidos = S.A;
			
			// Criando vetor D com pedidos removidos, inicializado pelo primeiro pedido escolhido
			std::vector <double> D {};
			
			if (S.L.size() == 0){
				
				double r = S.A.at(rand()%((S.A).size()));
				D.push_back(r);
				
				// Removendo pedido da lista de pedidos disponíveis para remoção
				pedidos.erase(std::remove_if(pedidos.begin(), pedidos.end(), [&r](double value) -> bool { return value == r; }), pedidos.end());
				
				
			} else {
				
				double r = S.L.at(rand()%((S.L).size()));
				D.push_back(r);
			}
			
			
			// Variável com quantidade de pedidos removidos
			int n_pedidos_removidos {0};
			
			
			// *obs: n_requests - 1 porque o primeiro pedido de referência já foi removido!
			
			while (n_pedidos_removidos < n_requests - 1){
				
				// Pedido r em "D" a ser levado em conta:
				double r = D.at(rand()%D.size());
				
				// Criando objeto com graus de semelhança para os pedidos em A
				std::vector<double> R_r {};
				
				for (auto &pedido: pedidos){
					
					R_r.push_back(R[r][pedido]);
					
				}
					
				// Criando objeto com graus de semelhança ordenados para os pedidos em A
				std::vector<double> R_r_sorted (R_r);
				sort(R_r_sorted.begin(), R_r_sorted.end());
				
				// Vetor "pedidos_sorted", com ordenação dos pedidos de acordo com custos de remoção
				std::vector<double> pedidos_sorted {};
				
				// Iterando no vetor de graus de semelhança ordenados, buscando índice correspondente no vetor de graus não ordenados e trazendo pedido correspondente
				for (auto &R: R_r_sorted){
				
					// Índice no vetor não ordenado
					
					// Obtendo posição do valor da lista de semelhanças ordenada na lista de semelhanças não-ordenada
					auto index_R = std::find(R_r.begin(), R_r.end(), R);
					
					int index_pedido = std::distance(R_r.begin(), index_R);
					
					// Guardando pedido correspondente na lista ordenada
					
					pedidos_sorted.push_back(pedidos.at(index_pedido));
					
				
				} 
				
				// Retirando pedido:
				
				double p = (double) rand()/RAND_MAX;
				
				// Posição na lista ordenada de pedidos do pedido a ser retirado da solução
				double pos_retirada = round(pow(p, delta)*((pedidos_sorted).size()-1));
				
				// Pedido retirado
				double pedido_retirado = pedidos_sorted[pos_retirada];
				
				D.push_back(pedido_retirado);
				
				// Removendo pedido da lista de pedidos disponíveis para remoção
				pedidos.erase(std::remove_if(pedidos.begin(), pedidos.end(), [&pedido_retirado](double value) -> bool { return value == pedido_retirado; }), pedidos.end());
				
				n_pedidos_removidos += 1;
			}
			
			// Retirando pedidos em D do objeto de solução
			
			// Caso o algoritmo não tenha sido inicializado com um pedido em L:
			
			//if (S.L.size() == 0){
			
			//	for (auto &pedido: D){
					
			//		S.remover_pedido(pedido);
					
			//	}
			
			//} else {
				
				// Inicia-se por 1, pois o pedido de índice 0 é o pedido que já está em L!
				
			//	for (int index {1}; index < D.size() ; index++){
					
			//		S.remover_pedido(D.at(index));
					
			//	}
				
			//}
			
			S.print_sol();
			
			// Inserindo pedidos
			
			// Pedidos não inseridos na solução
			
			
			
			std::vector<double> pedidos_nao_inseridos = S.L;
			
			// Aleatorizando:
			
			std::random_shuffle(pedidos_nao_inseridos.begin(), pedidos_nao_inseridos.end());
			
			// Cada pedido tentará ser inserido em cada uma das posições 
			
			for (auto pedido: pedidos_nao_inseridos){
				
				
				
			}
			
			
			
			// Fim da heurística Shaw's removal
			break;
		
		}
		*/
		
		
		default:
		std::cout << "Invalido" << std::endl;
			
	}
	
	// if S.isfeasible // se a mudança melhorou a função objetivo
	
	return S;
		
}

