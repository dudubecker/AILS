#include "LocalSearchOperator.hpp"
#include "Sol.hpp"
#include "Instance.hpp"
#include "heuristicsFunctions.h"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>


//LocalSearchOperator::LocalSearchOperator()
//{
//}

LocalSearchOperator::~LocalSearchOperator()
{
}

// Método principal dos operadores

// Método de aplicação dos operadores
// *OBS: vou colocar um switch case por enquanto, mas certamente existe um jeito melhor de fazer isso...

Sol LocalSearchOperator::apply(Sol &S){
	
	
	switch (name){
		
		// Estrutura de busca local: Swap
		
		case 'S':{
			
			// std::cout << "Solucao apos heuristica Swap" << std::endl;
			
			// Para gerar números aleatórios (índices das rotas e pedidos escolhidos)
			srand(time(NULL));
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota R1:
			double index_R1 = rand()%(m);
			
			// Escolhendo índice da rota R2, necessariamente diferente de R1:
			double index_R2 = rand()%(m);
			
			while (index_R1 == index_R2){
				
				index_R2 = rand()%(m);
				
			}
			
			// Vetor com pedidos retirados da rota R1:
			std::vector<double> pedidosRemovidos_R1 {};
			
			// Vetor com pedidos retirados da rota R2:
			std::vector<double> pedidosRemovidos_R2 {};
			
			// std::cout << "\nindex_R1: " << index_R1 << "\nindex_R2: " << index_R2 << std::endl;
			
			// Resolvendo bug quando a rota se esvazia: o número de pedidos a serem retirados não pode ser maior do que o número de pedidos na rota!!
			
			// Contabilizando número de pedidos atendidos pela rota R1
			
			int n_requests_R1 {};
			
			for (auto &node: S.Rotas.at(index_R1)){
				
				if ((node > 0) && (node <= S.inst.n)){
					
					n_requests_R1 += 1;
					
				}
				
			}
			
			// Removendo pedidos:
			
			// Escolhendo aleatoriamente k1 pedidos da rota R1 para serem retirados:
			for (int i {0}; i < k1; i++){
				
				// Número de nós da rota R1 (talvez fique mais rápido contabilizando isso fora do laço e subtraindo 2 a cada remoção);
				int n_nodes = S.Rotas.at(index_R1).size();
				
				// Inicializando pedido a ser removido:
				double pedido {9999};
				
				int n_it = 0;
				
				// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
				while (pedido > S.inst.n){
				
					// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
					double index_pedido_removido = 1 + rand()%(n_nodes - 2);
					
					pedido = S.Rotas.at(index_R1).at(index_pedido_removido);
					
					
					n_it += 1;
					
					if (n_it == 1000){
						
						break;
					}
					
				}
				
				// std::cout << "\nPedido escolhido: " << pedido << std::endl;
				
				// Removendo pedido de R1:
				
				S.remover_pedido(pedido);
				pedidosRemovidos_R1.push_back(pedido);
				
			}
			
			// Escolhendo aleatoriamente k2 pedidos da rota R2 para serem retirados:
			for (int i {0}; i < k2; i++){
				
				// Número de nós da rota R2 (talvez fique mais rápido contabilizando isso fora do laço e subtraindo 2 a cada remoção);
				int n_nodes = S.Rotas.at(index_R2).size();
				
				// Inicializando pedido a ser removido:
				double pedido {9999};
				
				int n_it = 0;
				
				// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
				while (pedido > S.inst.n){
				
					// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
					double index_pedido_removido = 1 + rand()%(n_nodes - 2);
					
					pedido = S.Rotas.at(index_R2).at(index_pedido_removido);
					
					
					n_it += 1;
					
					if (n_it == 1000){
						
						break;
					}
					
					
				}
				
				// std::cout << "\nPedido escolhido: " << pedido << std::endl;
				
				// Removendo pedido de R2:
				
				S.remover_pedido(pedido);
				pedidosRemovidos_R2.push_back(pedido);
				
			}
			
			
			// std::cout << "\nSolucao apos remocoes: \n" << std::endl;
			
			// S.print_sol();
			
			// Inserindo os "k1" pedidos na rota R2, na melhor posição factível:
			for (auto &pedido: pedidosRemovidos_R1){
				
				S.executar_melhor_insercao(pedido, index_R2);
				
			}
			
			// Inserindo os "k2" pedidos na rota R2, na melhor posição factível:
			
			// Obs -> inserir na melhor posição considerando todos os pedidos ou apenas a ordem deles na lista?
			
			for (auto &pedido: pedidosRemovidos_R2){
				
				S.executar_melhor_insercao(pedido, index_R1);
				
			}
			
			// Teste: inserindo em quaisquer posições os pedidos após tentar trocá-los:
			
			std::vector<double> pedidos_nao_atendidos = S.L;
			
			for (auto &pedido: pedidos_nao_atendidos){
				
				S.executar_melhor_insercao(pedido);
				
			}
			
			break;
			
		}
		
		// Estrutura de busca local: Shift
		
		case 'T':{
			
			// Possível ponto de melhoria na heurística:
			
			// Calculando probabilidades de escolha: desejável que, quanto maior a rota, mais provável é sua escolha para ter pedidos removidos!
			
			// *obs: para reduzir o número de rotas, a lógica deve ser a oposta: quanto maior o número de pedidos na rota, menor a chance de ela ser destruída
			
			// Para gerar números aleatórios
			srand(time(NULL));
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota R1, que terá os nós removidos
			double index_R1 = rand()%(m);
			
			// Escolhendo índice da rota R2, necessariamente diferente de R1:
			double index_R2 = rand()%(m);
			
			while (index_R1 == index_R2){
				
				index_R2 = rand()%(m);
				
			}
			
			// Retirando subseção com "k" elementos da rota:
			
			// Número de nós da rota R1
			int n_nodes = S.Rotas.at(index_R1).size();
			
			// Escolhendo índice do nó inicial para remoção (a partir do índice 1)
			// int index_no_inicial = 1 + rand()%(n_nodes - 1); // -> Problema: se selecionasse um nó de índice muito grande (perto do tamanho da rota), o número de pedidos removidos não é suficientemente grande!
			
			int index_no_inicial = 1 + rand()%(n_nodes - 3);
			
			// Escolhendo índice do nó final para remoção: mínimo entre o último nó visitado e "no_inicial + k" (não considerando o depósito central) 
			int index_no_final = std::min(n_nodes - 2, index_no_inicial + k);
			
			// Contabilizando todos os pedidos contidos na seção de "index_no_inicial" a "index_no_final":
			
			// std::cout << index_no_final - index_no_inicial << std::endl;
			
			std::vector<double> k_nodes = {};
			
			for (int index {index_no_inicial}; index < index_no_final; index++){
				
				k_nodes.push_back(S.Rotas.at(index_R1).at(index));
				
			}
			
			
			// std::cout << "\nSubsecao escolhida: ";
			
			// for (auto &node: k_nodes){
				
			// 	std::cout << node << " ";
				
			// }
			
			// std::cout << "\n";
			
			for (auto &node: k_nodes){
				
				// Se o nó referido for de pickup e o nó de delivery correspondente também estiver na subseção:
				if ((node <= S.inst.n) && (count(k_nodes.begin(), k_nodes.end(), node + S.inst.n))){
					
					// std::cout << "Pedido removido: " << node << std::endl;
					
					S.remover_pedido(node);
					
				}
				
			}
			
			// Questão: tentar inserir todos pedidos de L ou apenas os recém-removidos pela heurística?
			
			std::vector<double> pedidos_nao_atendidos = S.L;
			
			for (auto &pedido: pedidos_nao_atendidos){
				
				S.executar_melhor_insercao(pedido, index_R2);
				
			}
			
			// Caso não tenha sido possível realizar a inserção
			
			pedidos_nao_atendidos = S.L;
			
			for (auto &pedido: pedidos_nao_atendidos){
				
				S.executar_melhor_insercao(pedido);
				
			}
			
			break;
			
		}
		
		// Estrutura de busca local: Or-opt
		
		case 'O':{
			
			// Para gerar números aleatórios
			srand(time(NULL));
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota que terá os nós removidos
			double index_rota = rand()%(m);
			
			// Número de nós contidos na rota:
			int n_nodes = S.Rotas.at(index_rota).size();
			
			
			// Corrigindo bug quanto "or_opt" é maior do que o número de pedidos na rota
			int n_requests {};
			
			for (auto &node: S.Rotas.at(index_rota)){
				
				if ((node > 0) && (node <= S.inst.n)){
					
					n_requests += 1;
					
				}
				
			}
			
			// "or_opt" deve ser o mínimo entre o valor atribuído à heurística e o número de pedidos atendidos pela rota
			
			or_opt = std::min(or_opt, n_requests);
			
			// Removendo pedidos:
			
			// Removendo "or_opt" pedidos da rota
			for (auto i {0}; i < or_opt; i++){
				
				// Inicializando pedido a ser removido:
				double pedido {9999};
				
				int n_it = {0};
				
				// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
				while (pedido > S.inst.n){
					
					// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
					double index_pedido_removido = 1 + rand()%(n_nodes - 3);
					
					pedido = S.Rotas.at(index_rota).at(index_pedido_removido);
					
					n_it += 1;
					
					if (n_it == 1000){
						
						break;
					}
					
					
				}
				
				S.remover_pedido(pedido);
				
				// Atualizando variável com tamanho da rota, após remoção do pedido
				n_nodes -= 2;
				
			}
			
			// Reinserindo pedidos:
			
			// Obs -> inserir na melhor posição considerando todos os pedidos ou apenas a ordem deles na lista?
			
			// Questão: tentar inserir todos pedidos de L ou apenas os recém-removidos pela heurística?
			
			
			std::vector<double> pedidos_nao_atendidos = S.L;
			
			for (auto &pedido: pedidos_nao_atendidos){
				
				S.executar_melhor_insercao(pedido, index_rota);
				
			}
			
			// Caso não tenha sido possível realizar a inserção
			
			pedidos_nao_atendidos = S.L;
			
			for (auto &pedido: pedidos_nao_atendidos){
				
				S.executar_melhor_insercao(pedido);
				
			}
			
			break;
		}
		
		// Estrutura de busca local: 2-opt
		
		case 'W':{
			
			// Para gerar números aleatórios
			srand(time(NULL));
			
			// Quantidade "m" de rotas na solução:
			int m = S.Rotas.size();
			
			// Escolhendo índice da rota que terá os nós removidos
			double index_rota = rand()%(m);
			
			// Número de nós contidos na rota:
			int n_nodes = S.Rotas.at(index_rota).size();
			
			// std::cout << "\nRota: " << index_rota << std::endl;
			
			// Escolhendo pedidos 1 e 2, tal que D1 é servido anteriormente à P2
			
			// Índice do último nó de pickup visitado
			int index_ultimo_no_pickup {};
			
			// Achando último nó de pickup visitado:
			for (int index_no {n_nodes - 2}; index_no > 0; index_no--){
				
				int node = S.Rotas.at(index_rota).at(index_no);
				
				if (node <= S.inst.n){
					
					index_ultimo_no_pickup = index_no;
					
					break;
					
				}
			}
			
			// Escolhendo pedido D1 -> Deve estar antes do último nó de pickup visitado
			int index_D1 {};
			
			int no_D1 {};
			
			int n_it = 0;
			
			// O pedido deve ser de delivery!
			while (no_D1 <= S.inst.n){
				
				// std::cout << "\nPrimeiro while" << std::endl;
				
				index_D1 = 1 + rand()%(index_ultimo_no_pickup - 1);
				
				no_D1 = S.Rotas.at(index_rota).at(index_D1);
				
				n_it += 1;
				
				if (n_it == 1000){
					
					break;
				}
			
				
			}
			
			// Escolhendo pedido P2, com ordinalidade necessariamente maior que D1
			int index_P2 {};
			
			int no_P2 {9999};
			
			n_it = 0;
			
			// O pedido deve ser de pickup!
			while (no_P2 > S.inst.n){
				
				// std::cout << "\nSegundo while" << std::endl;
				
				index_P2 = index_D1 + rand()%(n_nodes - index_D1 - 2);
				
				no_P2 = S.Rotas.at(index_rota).at(index_P2);
				
				n_it += 1;
					
				if (n_it == 1000){
					
					break;
				}
				
			}
			
			// std::cout << "D1: " << no_D1 << "\nP2: " << no_P2 << std::endl;
			
			// Trocando nós de posição:
			
			S.Rotas.at(index_rota).at(index_D1) = no_P2;
			
			S.Rotas.at(index_rota).at(index_P2) = no_D1;
			
			
			break;
		}
		
		// Estrutura de busca local: Shaw's
		
		case 'H':{
			
			srand(time(NULL));
			
			// std::cout << "Solucao apos shaws removal" << std::endl;
			
			// Parâmetro "delta" para controle da aleatoriedade 
			const int delta {2};
			
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
			
			
			// *obs: k_shaws - 1 porque o primeiro pedido de referência já foi removido ou está em L!
			
			while (n_pedidos_removidos < k_shaw - 1){
				
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
			
			for (auto &pedido: D){
				
				if (!count(S.L.begin(), S.L.end(), pedido)){
					
					S.remover_pedido(pedido);
					
				}
				
			}
			
			std::vector<double> pedidos_nao_atendidos = S.L;
			
			for (auto &pedido: pedidos_nao_atendidos){
				
				S.executar_melhor_insercao(pedido);
				
			}
			
			
			// Fim da heurística Shaw's removal
			
			
			break;
			
		}
			
			
	//}
		
		default:
			std::cout << "Invalido" << std::endl;
			
	}
	
	// Checando se há rotas vazias: se houver, a rota é removida!
	
	for (int index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
		
		if (S.Rotas.at(index_rota).size() <= 2){
			
			
	// 		std::cout << "\n\nAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n\n" << std::endl;
			
	 		S.Rotas.erase(S.Rotas.begin() + index_rota);
			
		}
	}
	
	return S;
	
	
	
}
