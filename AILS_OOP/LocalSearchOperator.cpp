#include "LocalSearchOperator.hpp"
#include "Sol.hpp"
#include "Instance.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>

// Método principal de todos os operadores
Sol LocalSearchOperator::aplicar(Sol &S) {
	
	// Inicialização do método - determinando se será aplicado o ruído aleatório
	
	double random_value = static_cast<double>(std::rand()) / RAND_MAX;
	
	bool aplicar_ruido = false;
	
	if (random_value < alpha){
		
		aplicar_ruido = true;
		
	}
	
	// Aplicando método específico de cada operador
	aplicarMetodoEspecifico(S, aplicar_ruido);
	
	return S;
}


// Swap: trocando "k1" pedidos de uma rota "r1" e por "k2" pedidos de uma rota "r2"
Sol Swap::aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido){
	
	// std::cout << "Aplicando operador Swap" << std::endl;
	
	// Swap só deve funcionar caso o número de rotas seja maior ou igual a 2!
	
	// Quantidade "m" de rotas na solução:
	int m = S.rotas.size();
	
	// Swap só deve funcionar caso o número de rotas seja maior ou igual a 2!
	if (m >= 2){
		
		// Escolhendo índice da rota R1:
		double index_r1 = rand()%(m);
		
		// Escolhendo índice da rota R2, necessariamente diferente de R1:
		double index_r2 = rand()%(m);
		
		while (index_r1 == index_r2){
			
			index_r2 = rand()%(m);
			
		}
		
		// Tratando exceção: quando a rota possui menos pedidos do que "k1" ou "k2"
		
		// k1: número de pedidos a serem retirados de R1 e inseridos em R2
		int k1_value = std::min(k1, (S.rotas_size.at(index_r1) - 2)/2);
		
		// k2: número de pedidos a serem retirados de R2 e inseridos em R1
		int k2_value = std::min(k2, (S.rotas_size.at(index_r2) - 2)/2);
		
		// Vetor com pedidos retirados da rota R1:
		std::vector<double> pedidos_removidos_r1 {};
		
		// Vetor com pedidos retirados da rota R2:
		std::vector<double> pedidos_removidos_r2 {};
		
		// Contabilizando número de pedidos atendidos pela rota R1
		
		// Removendo pedidos:
		
		// Escolhendo aleatoriamente k1 pedidos da rota R1 para serem retirados:
		for (int i {0}; i < k1_value; i++){
			
			// Número de nós da rota R1 (talvez fique mais rápido contabilizando isso fora do laço e subtraindo 2 a cada remoção);
			int n_nodes = S.rotas_size.at(index_r1);
			
			// Inicializando pedido a ser removido:
			double pedido {9999};
			
			// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
			while (pedido > S.inst.n){
			
				// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
				double index_pedido_removido = 1 + rand()%(n_nodes - 2);
				
				pedido = S.rotas.at(index_r1).at(index_pedido_removido);
				
			}
			
			// Removendo pedido de R1:
			
			S.removerPedido(pedido);
			pedidos_removidos_r1.push_back(pedido);
			
		}
		
		// Escolhendo aleatoriamente k2 pedidos da rota R2 para serem retirados:
		for (int i {0}; i < k2_value; i++){
			
			int n_nodes = S.rotas_size.at(index_r2);
			
			// Inicializando pedido a ser removido:
			double pedido {9999};
			
			// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
			while (pedido > S.inst.n){
			
				// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
				double index_pedido_removido = 1 + rand()%(n_nodes - 2);
				
				pedido = S.rotas.at(index_r2).at(index_pedido_removido);
				
			}
			
			// Removendo pedido de R2:
			
			S.removerPedido(pedido);
			pedidos_removidos_r2.push_back(pedido);
			
		}
		
		
		// Inserindo os "k1" pedidos na rota R2, na melhor posição factível:
		S.executarMelhoresInsercoes(pedidos_removidos_r1, index_r2, aplicar_ruido);
		
		// Inserindo os "k2" pedidos na rota R1, na melhor posição factível:
		S.executarMelhoresInsercoes(pedidos_removidos_r2, index_r1, aplicar_ruido);
		
		// Inserindo em quaisquer posições os pedidos após tentar trocá-los:
		
		std::vector<double> pedidos_nao_inseridos = S.L;
		
		S.executarMelhoresInsercoes(pedidos_nao_inseridos, aplicar_ruido);
		
	}
	
	return S;
};


// Shift: retirando-se "k" nós adjacentes de uma rota e inserindo-os em outra:
Sol Shift::aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido){
	
	// std::cout << "Aplicando operador Shift" << std::endl;
	
	// Quantidade "m" de rotas na solução:
	int m = S.rotas.size();
	
	// Shift só deve funcionar caso o número de rotas seja maior ou igual a 2!
	if (m >= 2){
		
		// Escolhendo índice da rota R1, que terá os nós removidos
		double index_r1 = rand()%(m);
		
		// Escolhendo índice da rota R2, necessariamente diferente de R1:
		double index_r2 = rand()%(m);
		
		while (index_r1 == index_r2){
			
			index_r2 = rand()%(m);
			
		}
		
		// Retirando subseção com "k" elementos da rota:
		
		// Número de nós da rota R1
		int n_nodes = S.rotas_size.at(index_r1);
		
		// Escolhendo índice do nó inicial para remoção (a partir do índice 1)
		int index_no_inicial = 1 + rand()%(n_nodes - 2);
		
		// Escolhendo índice do nó final para remoção: mínimo entre o último nó visitado e "no_inicial + k" (não considerando o depósito central) 
		int index_no_final = std::min(n_nodes - 2, index_no_inicial + k);
		
		// Contabilizando todos os nós contidos na seção de "index_no_inicial" a "index_no_final":
		
		std::vector<double> k_nodes = {};
		
		for (int index {index_no_inicial}; index < index_no_final; index++){
			
			k_nodes.push_back(S.rotas.at(index_r1).at(index));
			
		}
		
		
		for (auto &node: k_nodes){
			
			// Se o nó referido for de pickup e o nó de delivery correspondente também estiver na subseção:
			if ((node <= S.inst.n) && (count(k_nodes.begin(), k_nodes.end(), node + S.inst.n))){
				
				S.removerPedido(node);
				
			}
			
		}
		
		
		std::vector<double> pedidos_nao_atendidos = S.L;
		
		S.executarMelhoresInsercoes(pedidos_nao_atendidos, index_r2, aplicar_ruido);
		
		pedidos_nao_atendidos = S.L;
		
		S.executarMelhoresInsercoes(pedidos_nao_atendidos, aplicar_ruido);
		
	}
	
	return S;
};


// Or-opt: retirando-se "k" pedidos de uma rota e inserindo-os na melhor posição
Sol OrOpt::aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido){
	
	// std::cout << "Aplicando operador Or-opt" << std::endl;
	
	// Quantidade "m" de rotas na solução:
	int m = S.rotas.size();
	
	// Escolhendo índice da rota que terá os nós removidos
	double index_rota = rand()%(m);
	
	// Número de nós contidos na rota:
	int n_nodes = S.rotas_size.at(index_rota);
	
	// Corrigindo bug quanto "or_opt" é maior do que o número de pedidos na rota
	int n_requests = (S.rotas_size.at(index_rota) - 2)/2;
	
	// "or_opt" deve ser o mínimo entre o valor atribuído à heurística e o número de pedidos atendidos pela rota
	
	or_opt = std::min(or_opt, n_requests);
	
	// Removendo pedidos:
	
	// Removendo "or_opt" pedidos da rota
	for (auto i {0}; i < or_opt; i++){
		
		// Inicializando pedido a ser removido:
		double pedido {9999};
		
		// Escolhendo pedido: deve ser representado por um nó de pickup, menor ou igual a "n"!
		while (pedido > S.inst.n){
			
			// Índice do pedido a ser removido na rota R1 (a partir do índice 1)
			// double index_pedido_removido = 1 + rand()%(n_nodes - 3);
			double index_pedido_removido = 1 + rand()%(n_nodes - 2);
			
			pedido = S.rotas.at(index_rota).at(index_pedido_removido);
			
		}
		
		S.removerPedido(pedido);
		
		// Atualizando variável com tamanho da rota, após remoção do pedido
		n_nodes -= 2;
		
	}
	
	std::vector<double> pedidos_nao_atendidos = S.L;
	
	//S.executarMelhoresInsercoes(pedidos_nao_atendidos, index_rota, aplicar_ruido);
	
	pedidos_nao_atendidos = S.L;
	
	S.executarMelhoresInsercoes(pedidos_nao_atendidos, aplicar_ruido);
	
	return S;
};

// Shaw's Removal: parâmetros da própria heurística e quantidade de pedidos analisados
Sol Shaw::aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido){
	
	// std::cout << "Aplicando operador Shaw" << std::endl;
	
	// Parâmetro "delta" para controle da aleatoriedade 
	const int delta {2};
	
	// Obtendo tempos de visita (T_i) de cada nó "i" na solução S
	
	std::vector <double> T (2*(S.inst.n) + 2, 0);
	
	// Iterando para cada rota na solução 
	for (auto &rota: S.rotas){
		
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
	
	if (S.L_size == 0){
		
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
	
	
	// k_shaws - 1 porque o primeiro pedido de referência já foi removido ou está em L!
	
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
			
			S.removerPedido(pedido);
			
		}
		
	}
	
	std::vector<double> pedidos_nao_atendidos = S.L;
	
	S.executarMelhoresInsercoes(pedidos_nao_atendidos, aplicar_ruido);
	
	return S;
};
