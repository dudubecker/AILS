#include "Perturbation.hpp"
#include <ctime>
#include <algorithm>
#include <stdio.h>
#include <chrono>
#include <numeric>
#include <iomanip>

// Método principal de todos os operadores
Sol Perturbation::aplicar(Sol &S) {
	
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

Sol RandomRemoval::aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido){
	
	// std::cout << "Solucao apos random removal" << std::endl;
	
	// Corrigindo bug de quando há mais pedidos a serem retirados do que pedidos na solução
	int num_pedidos = std::min(w, S.A_size);
	
	// Variável com quantidade de pedidos removidos
	int n_pedidos_removidos {0};
	
	// Retirando pedidos da solução iterativamente
	while (n_pedidos_removidos < num_pedidos){
		
		double pedido_aleatorio = S.A.at(rand()%S.A.size());
		
		S.removerPedido(pedido_aleatorio);
		
		n_pedidos_removidos += 1;
		
	}
	
	
	// Pedidos não inseridos na solução (referenciar diretamente no for loop deu um bug)
	
	std::vector<double> pedidos_nao_inseridos = S.L;
	
	// Código guloso
	S.executarMelhoresInsercoes(pedidos_nao_inseridos, aplicar_ruido);
	
	// Código não guloso
	//for (auto pedido: pedidos_nao_inseridos){
		
	//	S.executarMelhorInsercao(pedido, aplicar_ruido);
		
	//}
	
	return S;
};

Sol WorstRemoval::aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido){
	
	// Corrigindo bug de quando há mais pedidos a serem retirados do que pedidos na solução
	int num_pedidos = std::min(w, S.A_size);
	
	// std::cout << "Solucao apos worst removal" << std::endl;
	
	// Pedidos contidos na solução: complemento de L
	std::vector<double> pedidos = S.A;
	
	// Variável com quantidade de pedidos removidos
	int n_pedidos_removidos {0};
	
	// Início do algoritmo
	while (n_pedidos_removidos < num_pedidos){
		
		// Custos incrementais de cada pedido (não-ordenados)
		std::vector<double> delta_custos {};
		
		for (auto pedido: pedidos){
			
			// Calculando variação no custo da rota com a remoção do pedido
			double delta = S.calcularVariacaoFO(pedido);
			
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
		
		// Posição na lista ordenada de pedidos do pedido a ser retirado da solução
		
		// Obs: "-1" é necessário porque, caso o valor p seja muito próximo de 1, a posição de retirada será igual ao tamanho do vetor e isso resultará em um erro de índice!
		
		double pos_retirada = round(pow(p, delta)*((pedidos_sorted).size()-1));
		
		// Pedido retirado
		
		double pedido_retirado = pedidos_sorted.at(pos_retirada);
		
		// Retirando pedido do vetor "pedidos"
		
		pedidos.erase(std::remove_if(pedidos.begin(), pedidos.end(), [&pedido_retirado](double value) -> bool { return value == pedido_retirado; }), pedidos.end());
		
		// Retirando pedido da solução
		
		S.removerPedido(pedido_retirado);
		
		n_pedidos_removidos += 1;
		
	}
	
	// Pedidos não inseridos na solução
	std::vector<double> pedidos_nao_inseridos = S.L;
	
	// Código guloso
	// S.executarMelhoresInsercoes(pedidos_nao_inseridos, aplicar_ruido);
	
	// Código não guloso
	for (auto pedido: pedidos_nao_inseridos){
		
		S.executarMelhorInsercao(pedido, aplicar_ruido);
		
	}
	
	return S;
};

Sol ConcentricRemoval::aplicarMetodoEspecifico(Sol &S, bool aplicar_ruido){
	
	// Corrigindo bug de quando há mais pedidos a serem retirados do que pedidos na solução
	int num_pedidos = std::min(w, S.A_size);
	
	// std::cout << "Solucao apos concentric removal" << std::endl;
	
	// Escolhendo um pedido aleatório
	double pedido_aleatorio = S.A.at(rand()%S.A.size());
	
	// Removendo pedido aleatório
	S.removerPedido(pedido_aleatorio);
	
	// Considerando "w - 1" pedidos mais próximos do pedido escolhido (que estão na solução!)
	
	// Vetor com distâncias ordenadas entre o pedido aleatório e os demais
	std::vector<double> distancias_sorted = S.inst.t_med.at(pedido_aleatorio);
	std::sort(distancias_sorted.begin(), distancias_sorted.end());
	
	// Vetor com pedidos ordenados de acordo com distância
	std::vector<double> pedidos_sorted {};
	
	// Iterando no vetor ordenado
	for (auto i: distancias_sorted){
		
		// Buscando índice do pedido do vetor ordenado no vetor não ordenado:
		auto index = std::find(S.inst.t_med.at(pedido_aleatorio).begin(), S.inst.t_med.at(pedido_aleatorio).end(), i);
		
		int pedido = std::distance(S.inst.t_med.at(pedido_aleatorio).begin(), index);
		
		// Caso o pedido não seja o depósito central ou o próprio pedido aleatório (distância igual a 0) e esteja na solução
		if ((pedido != 0) && (!count(S.L.begin(), S.L.end(), pedido))){
			
			pedidos_sorted.push_back(pedido);
			
		}
	}
	
	// Removendo pedidos
	
	int qtdPedidosRemovidos {1};
	
	for (auto pedido: pedidos_sorted){
		
		if (qtdPedidosRemovidos == (num_pedidos)){
			
			break;
			
		} else {
			
			if (!count(S.L.begin(), S.L.end(), pedido)){
				
				S.removerPedido(pedido);
				
				qtdPedidosRemovidos += 1;
				
			}
		}
	}
	// Pedidos não inseridos na solução
	
	std::vector<double> pedidos_nao_inseridos = S.L;
	
	// Código guloso
	// S.executarMelhoresInsercoes(pedidos_nao_inseridos, aplicar_ruido);
	
	// Código não guloso
	for (auto pedido: pedidos_nao_inseridos){
		
		S.executarMelhorInsercao(pedido, aplicar_ruido);
		
	}
	
	return S;
};
