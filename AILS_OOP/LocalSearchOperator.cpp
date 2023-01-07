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
	
	// Contabilizando características da solução fornecida
	
	// Solução:
	// Sol S_ins = S;
	
	
	// FO:
	// double FO_S = S.FO();
	
	// Número de pedidos não atendidos:
	
	// Factibilidade:
	
	
	switch (name){
		
		// Heurística construtiva
		case 'C':{
			
			std::cout << "Solucao apos heuristica construtiva" << std::endl;
			
			// Criando variáveis com valores atualizados a cada inserção
			// Inserindo no conjunto L os pedidos não atendidos
			for(double value = 1; value < S.inst.n + 1; value++){
				
				S.L.push_back(value);
			}
			
			// Criando uma rota inicial vazia para a solução:
			S.Rotas.push_back({0, 2*(S.inst.n) + 1});
			
			// Quantidade de requests atendidos (inicia-se em 0)
			int qtd_atendidos {0};
			
			// Início do algoritmo de inserção:
			while (qtd_atendidos < S.inst.n){
				
				// Variável que abrigará o request a ser inserido na iteração
				double pedido {S.L.at(0)};
				
				// Objeto solução, que tentará fazer melhor inserção
				Sol S_best = melhor_insercao(S, pedido);
				
				// Caso o pedido tenha sido inserido:
				if (S_best.L.size() != S.L.size()){
					
					S = S_best;
					
				// Caso não seja possível fazer a inserção, isso significará que não foram encontradas posições de inserção factíveis para as rotas em questão
				// fazendo-se necessária uma nova rota
				
				} else {
					
					// Rota vazia com os nós da iteração
					std::vector <double> nova_rota {0, pedido, pedido + S.inst.n, 2*(S.inst.n) + 1};
					S.Rotas.push_back(nova_rota);
					
					// Removendo pedido de L
					S.L.erase(S.L.begin());
					
					// Adicionando pedido em A
					S.A.push_back(pedido);
					
				}
				
				// Atualizando quantidade de pedidos atendidos
				qtd_atendidos += 1;
				
				
			}
			// Fim da heurística construtiva
			
			break;
		}
		
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
			
			// Removendo pedidos:
			
			// Escolhendo aleatoriamente k1 pedidos da rota R1 para serem retirados:
			for (int i {0}; i < k1; i++){
				
				// Número de nós da rota R1 (talvez fique mais rápido contabilizando isso fora do laço e subtraindo 2 a cada remoção);
				int n_nodes = S.Rotas.at(index_R1).size();
				
				// Inicializando pedido a ser removido:
				double pedido {9999};
				
				// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
				while (pedido > S.inst.n){
				
					// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
					double index_pedido_removido = 1 + rand()%(n_nodes - 1);
					
					pedido = S.Rotas.at(index_R1).at(index_pedido_removido);
					
				}
				
				// std::cout << "\nPedido escolhido: " << pedido << std::endl;
				
				// Removendo pedido de R1:
				
				S.remover_pedido(pedido, index_R1);
				pedidosRemovidos_R1.push_back(pedido);
				
			}
			
			// Escolhendo aleatoriamente k2 pedidos da rota R2 para serem retirados:
			for (int i {0}; i < k2; i++){
				
				// Número de nós da rota R2 (talvez fique mais rápido contabilizando isso fora do laço e subtraindo 2 a cada remoção);
				int n_nodes = S.Rotas.at(index_R2).size();
				
				// Inicializando pedido a ser removido:
				double pedido {9999};
				
				// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
				while (pedido > S.inst.n){
				
					// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
					double index_pedido_removido = 1 + rand()%(n_nodes - 1);
					
					pedido = S.Rotas.at(index_R2).at(index_pedido_removido);
					
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
				
				S = melhor_insercao(S, pedido, index_R2);
				
			}
			
			// Inserindo os "k2" pedidos na rota R2, na melhor posição factível:
			
			// Obs -> inserir na melhor posição considerando todos os pedidos ou apenas a ordem deles na lista?
			
			for (auto &pedido: pedidosRemovidos_R2){
				
				S = melhor_insercao(S, pedido, index_R1);
				
			}
			
			// Teste: inserindo em quaisquer posições os pedidos após tentar trocá-los:
			
			for (auto &pedido: S.L){
				
				S = melhor_insercao(S, pedido);
				
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
					
					S.remover_pedido(node, index_R1);
					
				}
				
			}
			
			// Questão: tentar inserir todos pedidos de L ou apenas os recém-removidos pela heurística?
			
			for (auto &pedido: S.L){
				
				S = melhor_insercao(S, pedido, index_R2);
				
			}
			
			// Caso não tenha sido possível realizar a inserção
			
			for (auto &pedido: S.L){
				
				S = melhor_insercao(S, pedido);
				
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
			
			// Removendo pedidos:
			
			// Removendo "or_opt" pedidos da rota
			for (auto i {0}; i < or_opt; i++){
				
				// Inicializando pedido a ser removido:
				double pedido {9999};
				
				// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
				while (pedido > S.inst.n){
				
					// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
					double index_pedido_removido = 1 + rand()%(n_nodes - 2);
					
					pedido = S.Rotas.at(index_rota).at(index_pedido_removido);
					
				}
				
				S.remover_pedido(pedido, index_rota);
				
				// Atualizando variável com tamanho da rota, após remoção do pedido
				n_nodes -= 2;
				
			}
			
			// Reinserindo pedidos:
			
			// Obs -> inserir na melhor posição considerando todos os pedidos ou apenas a ordem deles na lista?
			
			// Questão: tentar inserir todos pedidos de L ou apenas os recém-removidos pela heurística?
			
			for (auto &pedido: S.L){
				
				S = melhor_insercao(S, pedido, index_rota);
				
			}
			
			// Caso não tenha sido possível realizar a inserção
			
			for (auto &pedido: S.L){
				
				S = melhor_insercao(S, pedido);
				
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
			
			// O pedido deve ser de delivery!
			while (no_D1 <= S.inst.n){
				
				// std::cout << "\nPrimeiro while" << std::endl;
				
				index_D1 = 1 + rand()%(index_ultimo_no_pickup - 1);
				
				no_D1 = S.Rotas.at(index_rota).at(index_D1);
				
			}
			
			// Escolhendo pedido P2, com ordinalidade necessariamente maior que D1
			int index_P2 {};
			
			int no_P2 {9999};
			
			// O pedido deve ser de pickup!
			while (no_P2 > S.inst.n){
				
				// std::cout << "\nSegundo while" << std::endl;
				
				index_P2 = index_D1 + rand()%(n_nodes - index_D1 - 2);
				
				no_P2 = S.Rotas.at(index_rota).at(index_P2);
				
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
			
			for (auto &pedido: S.L){
				
				S = melhor_insercao(S, pedido);
				
			}
			
			
			//} else {
				
				// Inicia-se por 1, pois o pedido de índice 0 é o pedido que já está em L!
				
			//	for (int index {1}; index < D.size() ; index++){
					
			//		S.remover_pedido(D.at(index));
					
			//	}
				
			//}
			
			// S.print_sol();
			
			// Inserindo pedidos
			
			// Cada pedido tentará ser inserido em cada uma das posições 
			
			// Buscando rota, posição de pickup e posição de delivery de cada pedido na solução
			
			/*
			
			// Vetor para guardar informações de cada pedido
			std::vector<std::vector<double>> posicoes_pedidos {};
			
			for (auto pedido: D){
				
				// Índice do nó de pickup correspondente ao request
				int no_pickup {pedido};
				
				// Índice do nó de delivery correspondente ao request
				int no_delivery {pedido + S.inst.n};
				
				// "Procurando" pedido (no_pickup) na solução:
				
				for (auto index_rota {0}; index_rota < S.Rotas.size(); index_rota++){
					
					// Caso o nó esteja contido na rota
					if (count(S.Rotas.at(index_rota).begin(), S.Rotas.at(index_rota).end(), no_pickup)){
						
						// Índice (posição) do nó de pickup na rota
						double pos_no_pickup = std::find(S.Rotas.at(index_rota).begin(),S.Rotas.at(index_rota).end(), no_pickup) - S.Rotas.at(index_rota).begin();
						
						// Índice (posição) do nó de delivery na rota
						double pos_no_delivery = std::find(S.Rotas.at(index_rota).begin(),S.Rotas.at(index_rota).end(), no_delivery) - S.Rotas.at(index_rota).begin();
						
						posicoes_pedidos.push_back({index_rota, pos_no_pickup, pos_no_delivery});
						
						break;
						
					}
				}
				
			}
			
			//for (auto i {0}; i < k_shaw; i++){
				
			//	std::cout << "\nPedido: " << D.at(i) << std::endl;
				
			//	std::cout << "\n index_rota: " << posicoes_pedidos.at(i).at(0) << std::endl;
				
			//	std::cout << "\n pos_no_pickup: " << posicoes_pedidos.at(i).at(1) << std::endl;
				
			//	std::cout << "\n pos_no_delivery: " << posicoes_pedidos.at(i).at(2) << std::endl;
				
			//}
			
			// Testando valor de cada posição de troca:
			
			// Dados do pedido "i" cuja troca resulta no custo mínimo
			
			/*
			
			std::vector<double> dados_pedido_i_min = {};
			
			// Dados do pedido "j" cuja troca resulta no custo mínimo
			std::vector<double> dados_pedido_j_min = {};
			
			// Delta mínimo de função objetivo encontrado
			double delta_min {0};
			
			// Quantidade de trocas factíveis entre pedidos encontradas
			// int trocas_factiveis {0};
			
			// Realizando troca entre as posições dos nós do pedido "i" com o pedido "j"
			
			for (int index_pedido_i {0}; index_pedido_i < k_shaw; index_pedido_i ++){
				
				for (int index_pedido_j {0}; index_pedido_j < k_shaw; index_pedido_j ++){
					
					// Para testar todas as combinações possíveis:
					if (index_pedido_j > index_pedido_i){
						
						// Rota, posição de pickup e delivery para cada pedido:
						
						std::vector<double> dados_pedido_i = posicoes_pedidos.at(index_pedido_i);
						
						std::vector<double> dados_pedido_j = posicoes_pedidos.at(index_pedido_j);
						
						// Criando uma cópia do objeto solução
						S_teste = S;
						
						// Trocando nós:
						
						// *obs: possivelmente transformar em um método/função "trocar pedidos" possa deixar o código mais legível!
						
						S_teste.Rotas.at(dados_pedido_i.at(0)).at(dados_pedido_i.at(1)) = dados_pedido_j.at(1);
						S_teste.Rotas.at(dados_pedido_i.at(0)).at(dados_pedido_i.at(2)) = dados_pedido_j.at(2);
						
						S_teste.Rotas.at(dados_pedido_j.at(0)).at(dados_pedido_j.at(1)) = dados_pedido_i.at(1);
						S_teste.Rotas.at(dados_pedido_j.at(0)).at(dados_pedido_j.at(2)) = dados_pedido_i.at(2);
						
						// Se a troca for factível
						if (S_teste.isFeasible()){
							
							// trocas_factiveis += 1;
							
							// Calculando variação na função objetivo (uma vez que a troca afeta possivelmente mais de uma rota)
							double delta = S_teste.FO() - S.FO();
							
							// Caso a troca tenha resultado em uma melhoria na função objetivo, os dados da troca são guardados
							if (delta < delta_min){
								
								dados_pedido_i_min = dados_pedido_i;
								dados_pedido_j_min = dados_pedido_j;
								
								delta_min = delta;
								
							}
							
							
							
						}
						
					}
					
				}
					
			}
			
			// Caso tenha sido encontrada alguma troca factível com decremento na função objetivo:
			
			
			// if (dados_pedido_i_min.size() > 0){
				
				S.Rotas.at(dados_pedido_i_min.at(0)).at(dados_pedido_i_min.at(1)) = dados_pedido_j_min.at(1);
				S.Rotas.at(dados_pedido_i_min.at(0)).at(dados_pedido_i_min.at(2)) = dados_pedido_j_min.at(2);
				
				S.Rotas.at(dados_pedido_j_min.at(0)).at(dados_pedido_j_min.at(1)) = dados_pedido_i_min.at(1);
				S.Rotas.at(dados_pedido_j_min.at(0)).at(dados_pedido_j_min.at(2)) = dados_pedido_i_min.at(2);
				
			}
			
			
			// Trocando nó de pickup do pedido "i" pelo nó de pickup do pedido "j"
			S.Rotas.at( posicoes_pedidos.at(0).at(0) ).at( posicoes_pedidos.at(0).at(1) ) =  D.at(1);//posicoes_pedidos.at(1).at(1);
			
			// Trocando nó de delivery do pedido "i" pelo nó de delivery do pedido "j"
			S.Rotas.at( posicoes_pedidos.at(0).at(0) ).at( posicoes_pedidos.at(0).at(2) ) =  D.at(1) + S.inst.n;//posicoes_pedidos.at(1).at(2);
			
			// Trocando nó de pickup do pedido "j" pelo nó de pickup do pedido "i"
			S.Rotas.at( posicoes_pedidos.at(1).at(0) ).at( posicoes_pedidos.at(1).at(1) ) =  D.at(0) ;//posicoes_pedidos.at(0).at(1);
			
			// Trocando nó de delivery do pedido "j" pelo nó de delivery do pedido "i"
			S.Rotas.at( posicoes_pedidos.at(1).at(0) ).at( posicoes_pedidos.at(1).at(2) ) =  D.at(0) + S.inst.n;//posicoes_pedidos.at(0).at(2);
			*/
			
			// Fim da heurística Shaw's removal
			
			
			break;
			
		}
			
			
	//}
		
		default:
			std::cout << "Invalido" << std::endl;
			
	}
	
	// Dúvida pertinente: if S.isfeasible // se a mudança melhorou a função objetivo ?
	
	return S;
	
	// Caso a aplicação do operador tenha resultado uma função objetivo menor:
	
	/*
	
	if (name == 'C'){
		
		return S;
		
	}
	
	if (S.FO() < FO_S){ // && S.isfeasible && S.L.size() < L_S
		
		std::cout << "Solucao melhorada: de " << FO_S << " para " << S.FO() << std::endl;
		
		return S;
	} else {
		
		std::cout << "Solucao piorada! De " << FO_S << " para " << S.FO() << std::endl;
		return S_ins;
	}
	
	*/
	
	
}
