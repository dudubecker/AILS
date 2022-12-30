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
			
			std::cout << "\nindex_R1: " << index_R1 << "\nindex_R2: " << index_R2 << std::endl;
			
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
				
				std::cout << "\nPedido escolhido: " << pedido << std::endl;
				
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
				
				std::cout << "\nPedido escolhido: " << pedido << std::endl;
				
				// Removendo pedido de R2:
				
				S.remover_pedido(pedido);
				pedidosRemovidos_R2.push_back(pedido);
				
			}
			
			
			std::cout << "\nSolucao apos remocoes: \n" << std::endl;
			
			S.print_sol();
			
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
			
			//for (auto &pedido: S.L){
				
			//	S = melhor_insercao(S, pedido);
				
			//}
			
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
			int index_no_inicial = 1 + rand()%(n_nodes - 1);
			
			// Escolhendo índice do nó final para remoção: mínimo entre o último nó visitado e "no_inicial + k" (não considerando o depósito central) 
			int index_no_final = std::min(n_nodes - 2, index_no_inicial + k);
			
			// Contabilizando todos os pedidos contidos na seção de "index_no_inicial" a "index_no_final":
			
			// Obtendo seção:
			std::vector<double> k_nodes = {S.Rotas.at(index_R1).begin() + index_no_inicial, S.Rotas.at(index_R1).begin() + index_no_final};
			
			std::cout << "\n";
			
			for (auto &node: k_nodes){
				
				std::cout << node << " ";
				
			}
			
			std::cout << "\n";
			
			for (auto &node: k_nodes){
				
				// Se o nó referido for de pickup e o nó de delivery correspondente também estiver na subseção:
				if ((node <= S.inst.n) && (count(k_nodes.begin(), k_nodes.end(), node + S.inst.n))){
					
					std::cout << node << std::endl;
					
					S.remover_pedido(node, index_R1);
					
				}
				
			}
			
			// Questão: tentar inserir todos pedidos de L ou apenas os recém-removidos pela heurística?
			
			//for (auto &pedido: S.L){
				
			//	S = melhor_insercao(S, pedido, index_R2);
				
			//}
			
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
					double index_pedido_removido = 1 + rand()%(n_nodes - 1);
					
					pedido = S.Rotas.at(index_rota).at(index_pedido_removido);
					
				}
				
				S.remover_pedido(pedido, index_rota);
				
				// Atualizando variável com tamanho da rota, após remoção do pedido
				n_nodes -= 2;
				
			}
			
			// Reinserindo pedidos:
			
			// Obs -> inserir na melhor posição considerando todos os pedidos ou apenas a ordem deles na lista?
			
			// Questão: tentar inserir todos pedidos de L ou apenas os recém-removidos pela heurística?
			
			//for (auto &pedido: S.L){
				
			//	S = melhor_insercao(S, pedido, index_R2);
				
			//}
			
			
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
			
			std::cout << "\nRota: " << index_rota << std::endl;
			
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
				
				std::cout << "\nPrimeiro while" << std::endl;
				
				index_D1 = 1 + rand()%(index_ultimo_no_pickup - 1);
				
				no_D1 = S.Rotas.at(index_rota).at(index_D1);
				
			}
			
			// Escolhendo pedido P2, com ordinalidade necessariamente maior que D1
			int index_P2 {};
			
			int no_P2 {9999};
			
			// O pedido deve ser de pickup!
			while (no_P2 > S.inst.n){
				
				std::cout << "\nSegundo while" << std::endl;
				
				index_P2 = index_D1 + rand()%(n_nodes - index_D1 - 2);
				
				no_P2 = S.Rotas.at(index_rota).at(index_P2);
				
			}
			
			std::cout << "D1: " << no_D1 << "\nP2: " << no_P2 << std::endl;
			
			// Trocando nós de posição:
			
			S.Rotas.at(index_rota).at(index_D1) = no_P2;
			
			S.Rotas.at(index_rota).at(index_P2) = no_D1;
			
			
			
			
			break;
		}
		
		
		
		default:
			std::cout << "Invalido" << std::endl;
			
	}
	
	// if S.isfeasible // se a mudança melhorou a função objetivo
	
	return S;
	
	
}
