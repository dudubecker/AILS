#include "Sol.hpp"
#include "Instance.hpp"
 

// Inicialização - heurística construtiva
Sol::Sol(Instance &dados_instancia){
	
	// Populando dados do atributo com instância
	inst = dados_instancia;
	
	// Populando atributos da solução:
	
	// Inserindo no conjunto L os pedidos não atendidos
	for(double value = 1; value < inst.n + 1; value++){
		
		L.push_back(value);
	}
	
	// Dando valor inicial para o número de pedidos não atendidos
	L_size = inst.n;
	
	// Populando o vetor de posições
	
	for (int i = 1; i <= inst.n; ++i) {
		
		posicoes_pedidos[i] = std::vector<int>();
		
	}
	
	// Criando uma rota inicial vazia para a solução:
	rotas.push_back({0, 2*(inst.n) + 1});
	
	// Adicionando vetor de cargas vazio
	cargas.push_back({0,0});
	
	// Adicionando vetor de tempos de visita - (0,0), já que os nós finais e inicial estão no mesmo lugar!
	tempos_de_visita.push_back({0,0});
	
	// Criando posição de tamanho igual a 2 no atributo com tamanhos de Rotas
	rotas_size.push_back(2);
	
	// Quantidade de requests atendidos (inicia-se em 0)
	int qtd_atendidos {0};
	
	// Início do algoritmo de inserção:
	while (qtd_atendidos < inst.n){
		
		// Variável que abrigará o request a ser inserido na iteração
		double pedido {L.at(0)};
		
		// Objeto solução, que tentará fazer melhor inserção
		
		std::vector<double> dados_melhor_insercao = calcularDadosMelhorInsercao(pedido);
		
		// Caso haja inserções factíveis
		
		if (dados_melhor_insercao.at(0) != 99999){
			
			inserirPedido(pedido, dados_melhor_insercao.at(1),dados_melhor_insercao.at(2),dados_melhor_insercao.at(3));
			
		
		} else {
			
			// Rota vazia com depósito central
			
			std::vector <int> nova_rota {0, 2*(inst.n) + 1};
			
			// Adicionando informações nos vetores de rotas, cargas e tempos de visita
			
			rotas.push_back(nova_rota);
			
			cargas.push_back({0,0});
			
			tempos_de_visita.push_back({0,0});
			
			// Adicionando valor igual a 2 (nós do depósito central) no vetor de tamanhos de rota
			
			rotas_size.push_back(2);
			
			inserirPedido(pedido, rotas.size() - 1, 1, 2);
			
			
		}
		
		// Atualizando quantidade de pedidos atendidos
		qtd_atendidos += 1;
		
		
	}
	
	// Fim da heurística construtiva
	
}

// Atribuição direta
Sol::Sol()
{
}

// Destructor
Sol::~Sol()
{
}

// Método para inserção de um pedido
void Sol::inserirPedido(int pedido, int index_rota, int pos_no_pickup, int pos_no_delivery){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
	// Inserindo nós na rota, nas determinadas posições
	
	rotas.at(index_rota).insert(rotas.at(index_rota).begin() + pos_no_pickup, no_pickup);
	rotas.at(index_rota).insert(rotas.at(index_rota).begin() + pos_no_delivery, no_delivery);
	
	// Aumentando o número de nós da rota correspondente
	rotas_size.at(index_rota) += 2;
	
	// Retirando pedido da lista de pedidos não atendidos
	
	L.erase(std::remove_if(L.begin(), L.end(), [&pedido](double value) -> bool { return value == pedido; }), L.end());
	
	// Diminuindo o número de pedidos não atendidos
	L_size -= 1;
	
	// Adicionando pedido na lista de pedidos atendidos
	
	A.push_back(pedido);
	
	// Aumentando o número de pedidos atendidos
	A_size += 1;
	
	// Atualizando vetor de posições para o pedido inserido
	
	posicoes_pedidos[pedido] = {index_rota, pos_no_pickup, pos_no_delivery};
	
	// Atualizando vetor de posições para demais pedidos da rota 
	
	// Para pedidos anteriores a P - as posições não se alteram!
	
	// Para pedidos entre P e D - Acréscimo de 1 unidade na posição
	
	// * OBS: no caso em que pos_no_pickup = pos_no_delivery - 1 (coleta e entrega consecutivas), esse laço não é executado!
	for (int index_node {pos_no_pickup + 1}; index_node < pos_no_delivery; index_node++){
		
		// Nó de referência
		int node = rotas.at(index_rota).at(index_node);
		
		// Se o nó é de pickup
		if (node <= inst.n){
			
			int pedido = node;
			
			// Atualizando posição no atributo de posições (pickup)
			int posicao = posicoes_pedidos.at(pedido).at(1) + 1;
			posicoes_pedidos.at(pedido).at(1) = posicao;
			
			
		// Se o nó é de delivery
		} else {
			
			// Índice do pedido correspondente
			int pedido = node - inst.n;
			
			// Atualizando posição no atributo de posições (delivery)
			int posicao = posicoes_pedidos.at(pedido).at(2) + 1;
			posicoes_pedidos.at(pedido).at(2) = posicao;
			
		}
		
	}
	
	// Para pedidos após D - Acréscimo de 2 unidades na posição
	
	for (int index_node {pos_no_delivery + 1}; index_node < rotas_size.at(index_rota) - 1; index_node++){
		
		// Nó de referência
		int node = rotas.at(index_rota).at(index_node);
		
		// Se o nó é de pickup
		if (node <= inst.n){
			
			int pedido = node;
			
			// Atualizando posição no atributo de posições (pickup)
			int posicao = posicoes_pedidos.at(pedido).at(1) + 2;
			posicoes_pedidos.at(pedido).at(1) = posicao;
			
			
		// Se o nó é de delivery
		} else {
			
			// Índice do pedido correspondente
			int pedido = node - inst.n;
			
			// Atualizando posição no atributo de posições (delivery)
			int posicao = posicoes_pedidos.at(pedido).at(2) + 2;
			posicoes_pedidos.at(pedido).at(2) = posicao;
			
		}
		
		
	}
	
	// Incrementando vetor de Cargas
	
	// Se as posições de inserção são consecutivas
	if (pos_no_pickup + 1 == pos_no_delivery){
		
		// Valor da carga na posição anterior à posição de pickup
		double carga_anterior = cargas.at(index_rota).at(pos_no_pickup - 1);
		
		// [0, ..., Q, +P, -D, ..., ] -> [0, ..., Q, Q + d, Q, ..., ]
		
		// Adicionando carga na posição de pickup
		cargas.at(index_rota).insert(cargas.at(index_rota).begin() + pos_no_pickup, carga_anterior + inst.q.at(pedido));
		
		// Adicionando carga na posição de delivery
		cargas.at(index_rota).insert(cargas.at(index_rota).begin() + pos_no_delivery, carga_anterior);
		
	
	// Se as posições de inserção não são consecutivas
	} else {
		
		// Valor da carga na posição anterior à posição de pickup
		double carga_anterior_pickup = cargas.at(index_rota).at(pos_no_pickup - 1);
		
		// Valor da carga na posição anterior à posição de delivery
		double carga_anterior_delivery = cargas.at(index_rota).at(pos_no_delivery - 2);
		
		// Adicionando carga na posição de pickup
		cargas.at(index_rota).insert(cargas.at(index_rota).begin() + pos_no_pickup, carga_anterior_pickup + inst.q.at(pedido));
		
		// Adicionando carga na posição de delivery
		cargas.at(index_rota).insert(cargas.at(index_rota).begin() + pos_no_delivery, carga_anterior_delivery);
		
		// Incrementando cargas intermediárias - entre nós de pickup e delivery
		for (auto index_node {pos_no_pickup + 1}; index_node < pos_no_delivery; index_node++){
			
			cargas.at(index_rota).at(index_node) += inst.q.at(pedido);
			
		}
		
		
	}
	
	
	// Incrementando vetor com tempos de visita
	
	// A alteração nos tempos de visita só se dá a partir do vetor de pickup
	
	// Inserindo "zeros" no vetor de tempo de visitas:
	
	tempos_de_visita.at(index_rota).insert(tempos_de_visita.at(index_rota).begin() + pos_no_pickup, 0);
	
	tempos_de_visita.at(index_rota).insert(tempos_de_visita.at(index_rota).begin() + pos_no_delivery, 0);
	
	// Tempo inicial - a partir do nó anterior ao nó de pickup - os tempos de visitas dos nós anteriores são conservados!
	
	double t_atual = tempos_de_visita.at(index_rota).at(pos_no_pickup - 1);
	
	// Atualizando tempos de visita
	for (int index_node {pos_no_pickup - 1}; index_node < rotas_size.at(index_rota) - 1; index_node++){
		
		int no_atual = rotas.at(index_rota).at(index_node);
		
		int no_seguinte = rotas.at(index_rota).at(index_node + 1);
		
		double delta_t = inst.t.at(no_atual).at(no_seguinte);
		
		// Será atribuída à variável "t_atual" o maior valor entre o delta e o tempo de abertura de TW, no caso de adiantamentos!
		t_atual = std::max(t_atual + delta_t, inst.e.at(no_seguinte));
		
		tempos_de_visita.at(index_rota).at(index_node + 1) = t_atual;
		
		
	}
	
	
	// Adição -> para remover rotas vazias ao fim de uma inserção (apenas se não há pedidos não atendidos)
	
	if (L_size == 0){
		
		for (size_t index_rota {0}; index_rota < rotas.size(); index_rota++){
			
			if (rotas_size.at(index_rota) == 2){
				
				removerRota(index_rota);
				
			}
			
			
		}
		
	}
	
}

// Método para remoção de um pedido
void Sol::removerPedido(double &pedido){
	
	// Índice da rota onde o pedido está:
	int index_rota = posicoes_pedidos[pedido].at(0);
	
	// Posição original de pickup do pedido
	int pos_no_pickup = posicoes_pedidos[pedido].at(1);
	
	// Posição original de delivery do pedido
	int pos_no_delivery = posicoes_pedidos[pedido].at(2);
	
	// Atualizando posições antes de realizar a remoção (mais simples)
	
	// Atualizando vetor de posições para o pedido removido
	// A chave 9999 indicará que o pedido não está mais na solução!
	posicoes_pedidos[pedido] = {9999, 9999, 9999};
	
	// Para pedidos anteriores a P - as posições não se alteram!
	
	// Para pedidos entre P e D - Decréscimo de 1 unidade na posição
	
	
	// * OBS: no caso em que pos_no_pickup = pos_no_delivery - 1 (coleta e entrega consecutivas), esse laço não é executado!
	for (int index_node {pos_no_pickup + 1}; index_node < pos_no_delivery; index_node++){
		
		// Nó de referência
		int node = rotas.at(index_rota).at(index_node);
		
		// Se o nó é de pickup
		if (node <= inst.n){
			
			int pedido = node;
			
			// Atualizando posição no atributo de posições (pickup)
			int posicao = posicoes_pedidos.at(pedido).at(1) - 1;
			posicoes_pedidos.at(pedido).at(1) = posicao;
			
			
		// Se o nó é de delivery
		} else {
			
			// Índice do pedido correspondente
			int pedido = node - inst.n;
			
			// Atualizando posição no atributo de posições (delivery)
			int posicao = posicoes_pedidos.at(pedido).at(2) - 1;
			posicoes_pedidos.at(pedido).at(2) = posicao;
			
		}
		
	}
	
	// Para pedidos após D - Decréscimo de 2 unidades na posição
	
	for (int index_node {pos_no_delivery + 1}; index_node < rotas_size.at(index_rota) - 1; index_node++){
		
		// Nó de referência
		int node = rotas.at(index_rota).at(index_node);
		
		// Se o nó é de pickup
		if (node <= inst.n){
			
			int pedido = node;
			
			// Atualizando posição no atributo de posições (pickup)
			int posicao = posicoes_pedidos.at(pedido).at(1) - 2;
			posicoes_pedidos.at(pedido).at(1) = posicao;
			
			
		// Se o nó é de delivery
		} else {
			
			// Índice do pedido correspondente
			int pedido = node - inst.n;
			
			// Atualizando posição no atributo de posições (delivery)
			int posicao = posicoes_pedidos.at(pedido).at(2) - 2;
			posicoes_pedidos.at(pedido).at(2) = posicao;
			
		}
		
		
	}
	
	
	// Realizando remoções no vetor de rotas
	
	// Removendo nó de pickup
	rotas.at(index_rota).erase(rotas.at(index_rota).begin() + pos_no_pickup);
	
	// Removendo nó de delivery (um índice a menos)
	rotas.at(index_rota).erase(rotas.at(index_rota).begin() + pos_no_delivery - 1);
	
	// Diminuindo número de nós na rota correspondente
	rotas_size.at(index_rota) -= 2;
	
	// Adicionando pedido ao conjunto L
	L.push_back(pedido);
	
	// Aumentando o número de pedidos não atendidos
	L_size += 1;
	
	// Removendo pedido do conjunto A
	A.erase(std::remove_if(A.begin(), A.end(), [&pedido](double value) -> bool { return value == pedido; }), A.end());
	
	// Diminuindo o número de pedidos atendidos
	A_size -= 1;
	
	// Vetor de Cargas
	
	// Decrementando posições entre P e D (contempla também casos onde pos_pickup = pos_delivery - 1)
	for (auto index_node {pos_no_pickup + 1}; index_node < pos_no_delivery; index_node++){
		
		cargas.at(index_rota).at(index_node) -= inst.q.at(pedido);
		
	}
	
	// Retirando posições no vetor de Cargas
	
	// Posição de Pickup
	cargas.at(index_rota).erase(cargas.at(index_rota).begin() + pos_no_pickup);
	
	// Posição de delivery (um índice a menos)
	cargas.at(index_rota).erase(cargas.at(index_rota).begin() + pos_no_delivery - 1);
	
	// Atualizando valores do vetor de tempos de visita
	
	// Posição de Pickup
	tempos_de_visita.at(index_rota).erase(tempos_de_visita.at(index_rota).begin() + pos_no_pickup);
	
	// Posição de delivery (um índice a menos)
	tempos_de_visita.at(index_rota).erase(tempos_de_visita.at(index_rota).begin() + pos_no_delivery - 1);
	
	// Atualizando valores a partir de pos_pickup - 1
	
	// Tempo inicial - a partir do nó anterior ao nó de pickup - os tempos de visitas dos nós anteriores são conservados!
	
	double t_atual = tempos_de_visita.at(index_rota).at(pos_no_pickup - 1);
	
	// Atualizando tempos de visita
	for (int index_node {pos_no_pickup - 1}; index_node < rotas_size.at(index_rota) - 1; index_node++){
		
		int no_atual = rotas.at(index_rota).at(index_node);
		
		int no_seguinte = rotas.at(index_rota).at(index_node + 1);
		
		double delta_t = inst.t.at(no_atual).at(no_seguinte);
		
		// Será atribuída à variável "t_atual" o maior valor entre o delta e o tempo de abertura de TW, no caso de adiantamentos!
		t_atual = std::max(t_atual + delta_t, inst.e.at(no_seguinte));
		
		tempos_de_visita.at(index_rota).at(index_node + 1) = t_atual;
		
	}
	
}

// Método para remoção de uma rota (usado na heurística de remoção de rotas)
void Sol::removerRota(int index_rota){
	
	// Removendo pedidos contidos na rota
	
	std::vector<double> pedidos_a_remover {};
	
	for (auto &node: rotas.at(index_rota)){
		
		if ((node > 0) && (node <= inst.n)){
			
			pedidos_a_remover.push_back(node);
			
		}
		
	}
	
	for (auto &node: pedidos_a_remover){
		
		removerPedido(node);
		
	}
	
	
	// Atualizando índices de rota na estrutura de posições de pedidos, para rotas com ordinalidade maior do que "index_rota"
	for (int pedido = 1; pedido <= inst.n; pedido++){
		
		if ((posicoes_pedidos[pedido].at(0) > index_rota) && (posicoes_pedidos[pedido].at(0) != 9999)){
			
			posicoes_pedidos[pedido].at(0) -= 1;
			
		}
		
	}
	
	// Removendo rotas vazias dos vetores de rotas, cargas e tempos de visita
	
	// Removendo rota vazia da solução
	rotas.erase(rotas.begin() + index_rota);
	
	// Removendo vetor de cargas
	cargas.erase(cargas.begin() + index_rota);
	
	// Removendo vetor de tempos de visita
	tempos_de_visita.erase(tempos_de_visita.begin() + index_rota);
	
	// Removendo posição no vetor de tamanhos
	rotas_size.erase(rotas_size.begin() + index_rota);
	
}

// Método de cálculo da função objetivo
double Sol::calcularFO(){
	
	// Calculando custos da solução: ainda segundo método convencional, não utilizando FO do artigo!
	
	// Custo de cada veículo da instância:
	int custo_veiculo = 10000;
	
	// Custos totais
	double custos_totais = 0;
	
	for (size_t index_rota {0}; index_rota < rotas.size(); index_rota++){
		
		custos_totais += custo_veiculo;
		
		for (int index_no {0}; index_no < rotas_size.at(index_rota) - 1; index_no++){
			
			custos_totais += inst.t[rotas.at(index_rota)[index_no]][rotas.at(index_rota)[index_no + 1]];
			
		}
	}
	
	// Penalizando número de pedidos não atendidos:
	
	custos_totais += 10000*(L_size);
	
	return (double) custos_totais;
}

// Incremento por inserção: calcula o custo de inserção de um pedido em posições pré-determinadas e sem checar factibilidade!
double Sol::calcularVariacaoFO(int pedido, int index_rota, int &pos_no_pickup, int &pos_no_delivery){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
	// Objeto para a rota que potencialmente sofrerá a inserção
	
	if (pos_no_delivery == pos_no_pickup + 1){
		
		// Variação total
		
		double delta = (inst.t[(rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (inst.t[(no_pickup)][(no_delivery)]) + (inst.t[(no_delivery)][(rotas[index_rota][pos_no_delivery - 1])]) - (inst.t[(rotas[index_rota][pos_no_pickup - 1])][(rotas[index_rota][pos_no_delivery - 1])]);
		
		return delta;
		
	}else{
		
		// Variação pela inserção do nó de pickup
		double delta_pickup = (inst.t[(rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (inst.t[(no_pickup)][(rotas[index_rota][pos_no_pickup])]) - (inst.t[(rotas[index_rota][pos_no_pickup - 1])][(rotas[index_rota][pos_no_pickup])]);
		
		// Variação pela inserção do nó de delivery
		double delta_delivery = (inst.t[(rotas[index_rota][pos_no_delivery - 1])][(no_delivery)]) + (inst.t[(no_delivery)][(rotas[index_rota][pos_no_delivery - 2])]) - (inst.t[(rotas[index_rota][pos_no_delivery - 1])][(rotas[index_rota][pos_no_delivery - 2])]);
		
		// Variação total
		double delta {delta_pickup + delta_delivery};
		
		return delta;
		
	}
	
}

// Decréscimo por remoção: calcula o decréscimo na FO pela remoção de um pedido
double Sol::calcularVariacaoFO(int pedido){
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
	// Obtendo "dados" do pedido
	
	int index_rota = posicoes_pedidos.at(pedido).at(0);
	
	int pos_no_pickup = posicoes_pedidos.at(pedido).at(1);
	
	int pos_no_delivery = posicoes_pedidos.at(pedido).at(2);
	
	
	// Calculando decréscimo nos custos
	
	if (pos_no_delivery == pos_no_pickup + 1){
		
		// Variação total
		
		double delta = (inst.t[(rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (inst.t[(no_pickup)][(no_delivery)]) + (inst.t[(no_delivery)][(rotas[index_rota][pos_no_delivery + 1])]) - (inst.t[(rotas[index_rota][pos_no_pickup - 1])][(rotas[index_rota][pos_no_delivery + 1])]);
		
		return delta;
		
	}else{
		
		
		//// Variação pela inserção do nó de pickup
		double delta_pickup = (inst.t[(rotas[index_rota][pos_no_pickup - 1])][(no_pickup)]) + (inst.t[(no_pickup)][(rotas[index_rota][pos_no_pickup + 1])]) - (inst.t[(rotas[index_rota][pos_no_pickup - 1])][(rotas[index_rota][pos_no_pickup + 1])]);
		
		//// Variação pela inserção do nó de delivery
		double delta_delivery = (inst.t[(rotas[index_rota][pos_no_delivery - 1])][(no_delivery)]) + (inst.t[(no_delivery)][(rotas[index_rota][pos_no_delivery + 1])]) - (inst.t[(rotas[index_rota][pos_no_delivery - 1])][(rotas[index_rota][pos_no_delivery + 1])]);
		
		//// Variação total
		double delta {delta_pickup + delta_delivery};
		
		return delta;
		
	}
	
}

// Método para checar factibilidade de uma solução
bool Sol::checarFactibilidadeSolucao(){
	
	// *** Checando factibilidade da rota ***
	// *OBS: não é necessário testar pairing e precedence, porque os pedidos são inseridos em pares nas rotas e as posições de inserção já asseguram precedência
	
	// Variável booleana, que controla a factibilidade
	bool factivel = true;
	
	// Para cada rota da solução:
	
	for (auto &rota: rotas){
		
		// Capacidade atual do veículo na rota (inicia como 0)
		double cap_atual {0};
		
		// Tempo atual da rota (inicia como 0)
		double t_atual {0};
		
		for (unsigned index_no {1}; index_no < rota.size(); index_no++){
		
			// Variável que guarda o nó atual considerado na checagem de factibilidade
			int no_atual {rota.at(index_no - 1)};
			
			// Variável que guarda o nó seguinte considerado na checagem de factibilidade
			int no_seguinte {rota.at(index_no)};
			
			// Checando se ir do no atual para o nó seguinte irá violar as restrições de capacidade e time window
			if ((cap_atual + inst.q.at(no_seguinte) > inst.Cap) || (inst.l.at(no_seguinte) < t_atual + inst.t.at(no_atual).at(no_seguinte))){
				
				// Atribuindo valor falso para a variável de factibilidade e quebrando o laço for
				factivel = false;
				break;
				
			// Caso seja possível, os valores são atualizados
			} else {
				// Atualizando valores
				
				// Capacidade
				cap_atual += inst.q.at(no_seguinte);
				
				// Tempo
				// Caso haja adiantamento (tempo de chegada menor que a janela de tempo de abertura)
				if (t_atual + inst.t.at(no_atual).at(no_seguinte) < inst.e.at(no_seguinte)){
					t_atual = inst.e.at(no_seguinte);
					
				} else {
					t_atual += inst.t.at(no_atual).at(no_seguinte);
					
				}
			}
		}
		
		// Caso alguma rota da solução tenha sido infactível, a busca termina
		
		if (!factivel){
			break;
		}
	}
	
	return factivel;
	
}

// Novo método de checagem de factibilidade, que usa vetor de cargas e tempos de visita
bool Sol::checarFactibilidadeInsercao(int pedido, int index_rota, int &pos_no_pickup, int &pos_no_delivery){
	
	// Variável booleana, que controla a factibilidade
	bool factivel = true;
	
	// Índice do nó de pickup correspondente ao request
	int no_pickup {pedido};
	
	// Índice do nó de delivery correspondente ao request
	int no_delivery {pedido + inst.n};
	
	// Carga atual - Anterior à posição de pickup
	double cap_atual = cargas.at(index_rota).at(pos_no_pickup - 1);
	
	// Tempo atual - Anterior à posição de pickup
	double t_atual = tempos_de_visita.at(index_rota).at(pos_no_pickup - 1);
	
	// Iterando a partir da posição de pickup
	
	for (auto index_no {pos_no_pickup - 1}; index_no < rotas_size.at(index_rota) + 1; index_no++){
		
		// Variável que guarda o nó atual considerado na checagem de factibilidade
		int no_atual {};
		
		// Variável que guarda o nó seguinte considerado na checagem de factibilidade
		int no_seguinte {};
		
		// Se as posições de inserção são consecutivas (pos_no_delivery = pos_no_pickup + 1)
		if (pos_no_delivery == pos_no_pickup + 1){
			
			if (index_no < pos_no_pickup - 1){
				
				no_atual = rotas.at(index_rota).at(index_no);
				
				no_seguinte = rotas.at(index_rota).at(index_no + 1);
				
			} else if (index_no == pos_no_pickup - 1){
				
				no_atual = rotas.at(index_rota).at(index_no);
				
				no_seguinte = no_pickup;
				
			} else if (index_no == pos_no_pickup){
				
				no_atual = no_pickup;
				
				no_seguinte = no_delivery;
				
			} else if (index_no == pos_no_pickup + 1){
				
				no_atual = no_delivery;
				
				no_seguinte = rotas.at(index_rota).at(index_no - 1);
				
				
			} else {
				
				no_atual = rotas.at(index_rota).at(index_no - 2);
				
				no_seguinte = rotas.at(index_rota).at(index_no - 1);
				
			}
			
		// Se as posições de inserção não são consecutivas (pos_no_delivery > pos_no_pickup + 1)
		} else if (pos_no_delivery > pos_no_pickup + 1){
			
			if (index_no < pos_no_pickup - 1){
				
				no_atual = rotas.at(index_rota).at(index_no);
				
				no_seguinte = rotas.at(index_rota).at(index_no + 1);
				
			} else if (index_no == pos_no_pickup - 1){
				
				no_atual = rotas.at(index_rota).at(index_no);
				
				no_seguinte = no_pickup;
				
			} else if (index_no == pos_no_pickup){
				
				no_atual = no_pickup;
				
				no_seguinte = rotas.at(index_rota).at(index_no);
				
			} else if ((index_no > pos_no_pickup) && (index_no < pos_no_delivery - 1)){
				
				no_atual = rotas.at(index_rota).at(index_no - 1);
				
				no_seguinte = rotas.at(index_rota).at(index_no);
				
			} else if (index_no == pos_no_delivery - 1){
				
				no_atual = rotas.at(index_rota).at(index_no - 1);
				
				no_seguinte = no_delivery;
				
			} else if (index_no == pos_no_delivery){
				
				no_atual = no_delivery;
				
				no_seguinte = rotas.at(index_rota).at(index_no - 1);
				
				
			} else {
				
				no_atual = rotas.at(index_rota).at(index_no - 2);
				
				no_seguinte = rotas.at(index_rota).at(index_no - 1);
				
			}
			
		}
		
		// Checando se ir do no atual para o nó seguinte irá violar as restrições de capacidade e time window
		if ((cap_atual + inst.q.at(no_seguinte) > inst.Cap) || (inst.l.at(no_seguinte) < t_atual + inst.t.at(no_atual).at(no_seguinte))){
			
			// Atribuindo valor falso para a variável de factibilidade e quebrando o laço for
			factivel = false;
			break;
			
		// Caso seja possível, os valores são atualizados
		} else {
			
			// Atualizando valores
			
			// Capacidade
			cap_atual += inst.q.at(no_seguinte);
			
			// Tempo
			// Caso haja adiantamento (tempo de chegada menor que a janela de tempo de abertura)
			if (t_atual + inst.t.at(no_atual).at(no_seguinte) < inst.e.at(no_seguinte)){
				t_atual = inst.e.at(no_seguinte);
				
			} else {
				t_atual += inst.t.at(no_atual).at(no_seguinte);
				
			}
		}
	}
	
	return factivel;
	
}

// Retorna um vetor com o delta de melhor inserção, rota, posição de nó pickup e delivery correspondentes a um dado pedido
std::vector<double> Sol::calcularDadosMelhorInsercao(double &pedido){
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Posição de inserção de nó de pickup com delta mínimo
	double pos_insercao_no_pickup_min {};
	
	// Posição de inserção de nó de delivery com delta mínimo
	double pos_insercao_no_delivery_min {};
	
	// Índice da rota com o menor valor de inserção
	double index_rota_min {};
	
	// Vetor a ser retornado pela função:
	std::vector<double> return_vector {};
	
	// Avaliando inserções
	
	for (size_t index_rota {0}; index_rota < rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < rotas_size.at(index_rota) + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < rotas_size.at(index_rota) + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
					
					// Checando factibilidade das posições de inserção
					if (checarFactibilidadeInsercao(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery)){
					
						num_rotas_factiveis += 1;
						
						double delta_S = calcularVariacaoFO(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
						
						// Caso essa seja a melhor posição encontrada
						if (delta_S < delta_min){
							
							delta_min = delta_S;
							
							pos_insercao_no_pickup_min = pos_insercao_no_pickup;
							
							pos_insercao_no_delivery_min = pos_insercao_no_delivery;
							
							index_rota_min = index_rota;
							
						}
					}
				}
			}
		}
	}
	
	// Retornando valores após avaliações
	
	if (num_rotas_factiveis > 0){
		
		return_vector = {delta_min, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min};
		
	} else {
		
		return_vector = {99999, 0, 0,0};
		
	}
	
	return return_vector;
	
}

// Delta melhor inserção considerando rota:
std::vector<double> Sol::calcularDadosMelhorInsercao(double &pedido, int &index_rota){
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Posição de inserção de nó de pickup com delta mínimo
	double pos_insercao_no_pickup_min {};
	
	// Posição de inserção de nó de delivery com delta mínimo
	double pos_insercao_no_delivery_min {};
	
	// Índice da rota com o menor valor de inserção
	double index_rota_min {};
	
	// Vetor a ser retornado pela função:
	std::vector<double> return_vector {};
	
	// Realizando inserções
	
	for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < rotas_size.at(index_rota) + 1; pos_insercao_no_pickup++){
		
		for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < rotas_size.at(index_rota) + 1; pos_insercao_no_delivery++){
			
			// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
			// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
			if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
				
				// Checando factibilidade das posições de inserção
				if (checarFactibilidadeInsercao(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery)){
					
					num_rotas_factiveis += 1;
					
					double delta_S = calcularVariacaoFO(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					// Caso essa seja a melhor posição encontrada
					if (delta_S < delta_min){
						
						delta_min = delta_S;
						
						pos_insercao_no_pickup_min = pos_insercao_no_pickup;
							
						pos_insercao_no_delivery_min = pos_insercao_no_delivery;
							
						index_rota_min = index_rota;
						
					}
				}
			}
		}
	}
	
	if (num_rotas_factiveis > 0){
		
		return_vector = {delta_min, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min};
		
		
	} else {
		
		// Possível problema em retornar um valor grande: se não for factível, como busca-se maximizar (regret), vai dar problema!
		
		//return_vector = {0, 0, 0,0};
		return_vector = {99999, 0, 0,0};
		
	}
	
	return return_vector;
	
}

// Executa a melhor inserção considerando todas as rotas:
void Sol::executarMelhorInsercao(double &pedido, bool aplicar_ruido){
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Posição de inserção de nó de pickup com delta mínimo
	int pos_insercao_no_pickup_min {};
	
	// Posição de inserção de nó de delivery com delta mínimo
	int pos_insercao_no_delivery_min {};
	
	// Índice da rota com o menor valor de inserção
	int index_rota_min {};
	
	// Avaliando inserções
	for (size_t index_rota {0}; index_rota < rotas.size(); index_rota++){
		
		for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < rotas_size.at(index_rota) + 1; pos_insercao_no_pickup++){
			
			for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < rotas_size.at(index_rota) + 1; pos_insercao_no_delivery++){
				
				// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
				// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
				if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
					
					if (checarFactibilidadeInsercao(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery)){
						
						num_rotas_factiveis += 1;
						
						double delta_S = calcularVariacaoFO(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
						
						// Caso o algoritmo opte por aplicar o random noise nesse cálculo
						if (aplicar_ruido){
							
							// Random noise, entre -eta*max_dist e +eta*max_dist
							int rand_value = std::rand();
							
							double noise = (-1.0 + (2.0 * rand_value / RAND_MAX))*(eta_noise)*(inst.max_dist);
							
							delta_S = std::max(0.0, delta_S + noise);
							
						}
						
						// Caso essa seja a melhor posição encontrada
						if (delta_S < delta_min){
							
							delta_min = delta_S;
							
							pos_insercao_no_pickup_min = pos_insercao_no_pickup;
							
							pos_insercao_no_delivery_min = pos_insercao_no_delivery;
							
							index_rota_min = index_rota;
							
						}
					}
				}
			}		
		}			
	}
	
	if (num_rotas_factiveis > 0){
		
		inserirPedido(pedido, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min);
		
	}
	
}

// Executa a melhor inserção de um pedido considerando uma rota específica:
void Sol::executarMelhorInsercao(double &pedido, int index_rota, bool aplicar_ruido){
	
	// Variável que controlará o número de rotas factíveis encontradas
	int num_rotas_factiveis {0};
	
	// Delta mínimo pela inserção do pedido da iteração
	double delta_min {9999};
	
	// Posição de inserção de nó de pickup com delta mínimo
	int pos_insercao_no_pickup_min {};
	
	// Posição de inserção de nó de delivery com delta mínimo
	int pos_insercao_no_delivery_min {};
	
	// Índice da rota com o menor valor de inserção
	int index_rota_min {};
	
	// Avaliando inserções
	for (auto pos_insercao_no_pickup {1}; pos_insercao_no_pickup < rotas_size.at(index_rota) + 1; pos_insercao_no_pickup++){
		
		for (auto pos_insercao_no_delivery {1}; pos_insercao_no_delivery < rotas_size.at(index_rota) + 1; pos_insercao_no_delivery++){
			
			// Testando apenas índices de inserção válidos: índice de delivery maior do que o de pickup (precedence) e diferente dele!
			// A iteração começa em 1 e termina no tamanho da rota porque não se considera a primeira e última posição da rota, que são o depósito
			if ((pos_insercao_no_pickup != pos_insercao_no_delivery) and (pos_insercao_no_pickup < pos_insercao_no_delivery)){
				
				if (checarFactibilidadeInsercao(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery)){
					
					num_rotas_factiveis += 1;
					
					double delta_S = calcularVariacaoFO(pedido, index_rota, pos_insercao_no_pickup, pos_insercao_no_delivery);
					
					// Caso o algoritmo opte por aplicar o random noise nesse cálculo
					if (aplicar_ruido){
						
						// Random noise, entre -eta*max_dist e +eta*max_dist
						int rand_value = std::rand();
						
						double noise = (-1.0 + (2.0 * rand_value / RAND_MAX))*(eta_noise)*(inst.max_dist);
						
						delta_S = std::max(0.0, delta_S + noise);
						
					}
					
					// Caso essa seja a melhor posição encontrada
					if (delta_S < delta_min){
						
						delta_min = delta_S;
						
						pos_insercao_no_pickup_min = pos_insercao_no_pickup;
						
						pos_insercao_no_delivery_min = pos_insercao_no_delivery;
						
						index_rota_min = index_rota;
						
					}
				}
			}
		}
	}
	
	if (num_rotas_factiveis > 0){
		
		inserirPedido(pedido, index_rota_min, pos_insercao_no_pickup_min, pos_insercao_no_delivery_min);
		
	}
	
}

// Executa, de forma gulosa, as melhores inserções considerando todas as rotas
void Sol::executarMelhoresInsercoes(std::vector<double> &pedidos, bool aplicar_ruido){
	
	// Variável que controlará o número de pedidos inseridos pelo algoritmo
	int qtd_inseridos {0};
	
	// Quantidade de pedidos não atendidos na solução 
	int qtd_pedidos_nao_atendidos {L_size};
	
	// Condição de parada do algoritmo: caso não tenham sido encontradas posições factíveis de inserção ou caso todos os pedidos tenham sido inseridos
	while (true){
		
		// Variável que controlará o número de inserções factíveis encontradas
		int qtd_insercoes_factiveis {0};
		
		// Menor delta de inserção encontrado
		double delta_min {9999};
		
		// Pedido correspondente ao menor delta de inserção
		double pedido_min {};
		
		// Dados de rota e posição dos nós para o pedido de menor delta:
		std::vector<double> dados_melhor_insercao_min {};
		
		for (auto &pedido: pedidos){
			
			// Obtendo dados para melhor insercao do pedido (chamando função com parâmetro char)
			std::vector<double> dados_melhor_insercao = calcularDadosMelhorInsercao(pedido);
			
			// Delta FO -> Primeiro dado do vetor retornado pela função
			double delta = dados_melhor_insercao.at(0);
			
			// Caso o algoritmo opte por aplicar o random noise nesse cálculo
			if (aplicar_ruido){
				
				// Random noise, entre -eta*max_dist e +eta*max_dist
				int rand_value = std::rand();
				
				double noise = (-1.0 + (2.0 * rand_value / RAND_MAX))*(eta_noise)*(inst.max_dist);
				
				delta = std::max(0.0, delta + noise);
			}
			
			// Caso essa seja a melhor posição encontrada
			if (delta < delta_min){
				
				delta_min = delta;
				pedido_min = pedido;
				
				dados_melhor_insercao_min = dados_melhor_insercao;
				
				// A função retornará um valor maior do que 9999 para delta! Por isso a contagem de inserções factíveis está aqui!
				qtd_insercoes_factiveis += 1;
				
			}
			
		}
		
		// Caso se tenha achado pelo menos uma posição de inserção factível (delta_min < 9999)
		if (delta_min < 9999){
			
			// Inserindo pedido de delta mínimo, com dados do vetor retornado pela função
			inserirPedido(pedido_min, dados_melhor_insercao_min.at(1), dados_melhor_insercao_min.at(2), dados_melhor_insercao_min.at(3));
			
			qtd_inseridos += 1;
			
			// Removendo pedido da lista de pedidos
			pedidos.erase(std::remove_if(pedidos.begin(), pedidos.end(), [&pedido_min](double value) -> bool { return value == pedido_min; }), pedidos.end());
			
		// Caso não tenha se achado pelo menos uma posição de inserção factível (delta_min == 99999, valor retornado pela função)
		} else {
			
			qtd_insercoes_factiveis = 0;
			
		}
		
		if ((qtd_inseridos == qtd_pedidos_nao_atendidos) || (qtd_insercoes_factiveis == 0)){
			
			break;
			
		}
	}
}

// Executa, de forma gulosa, as melhores inserções considerando uma rota específica
void Sol::executarMelhoresInsercoes(std::vector<double> &pedidos, int index_rota, bool aplicar_ruido){
	
	// Variável que controlará o número de pedidos inseridos pelo algoritmo
	int qtd_inseridos {0};
	
	// Quantidade de pedidos não atendidos na solução 
	int qtd_pedidos_nao_atendidos {L_size};
	
	// Condição de parada do algoritmo: caso não tenham sido encontradas posições factíveis de inserção ou caso todos os pedidos tenham sido inseridos
	while (true){
		
		// Variável que controlará o número de inserções factíveis encontradas
		int qtd_insercoes_factiveis {0};
		
		// Menor delta de inserção encontrado
		double delta_min {9999};
		
		// Pedido correspondente ao menor delta de inserção
		double pedido_min {};
		
		// Dados de rota e posição dos nós para o pedido de menor delta:
		std::vector<double> dados_melhor_insercao_min {};
		
		for (auto &pedido: pedidos){
			
			// Obtendo dados para melhor insercao do pedido (chamando função com parâmetro char)
			std::vector<double> dados_melhor_insercao = calcularDadosMelhorInsercao(pedido, index_rota);
			
			// Delta FO -> Primeiro dado do vetor retornado pela função
			double delta = dados_melhor_insercao.at(0);
			
			// Caso o algoritmo opte por aplicar o random noise nesse cálculo
			if (aplicar_ruido){
				
				// Random noise, entre -eta*max_dist e +eta*max_dist
				int rand_value = std::rand();
				
				double noise = (-1.0 + (2.0 * rand_value / RAND_MAX))*(eta_noise)*(inst.max_dist);
				
				delta = std::max(0.0, delta + noise);
				
			}
			
			
			if (delta < delta_min){
				
				delta_min = delta;
				pedido_min = pedido;
				
				dados_melhor_insercao_min = dados_melhor_insercao;
				
				// A função retornará um valor maior do que 9999 para delta! Por isso a contagem de inserções factíveis está aqui!
				qtd_insercoes_factiveis += 1;
				
			}
			
		}
		
		// Caso se tenha achado pelo menos uma posição de inserção factível (delta_min < 9999)
		if (delta_min < 9999){
			
			// Inserindo pedido de delta mínimo, com dados do vetor retornado pela função
			inserirPedido(pedido_min, dados_melhor_insercao_min.at(1), dados_melhor_insercao_min.at(2), dados_melhor_insercao_min.at(3));
			
			qtd_inseridos += 1;
			
			// Removendo pedido da lista de pedidos
			pedidos.erase(std::remove_if(pedidos.begin(), pedidos.end(), [&pedido_min](double value) -> bool { return value == pedido_min; }), pedidos.end());
			
		// Caso não tenha se achado pelo menos uma posição de inserção factível (delta_min == 99999, valor retornado pela função)
		} else {
			
			qtd_insercoes_factiveis = 0;
			
		}
		
		if ((qtd_inseridos == qtd_pedidos_nao_atendidos) || (qtd_insercoes_factiveis == 0)){
			
			break;
			
		}
		
	}
	
}

// Método para printar uma solução
void Sol::printSol(){
	
	std::cout << "S: " << std::endl;
	
	for (auto &rota: rotas){
		
		for (auto &node: rota){
			
			std::cout << node << " ";
		}
		
		std::cout << "\n";
		
	}
	
	std::cout << "\n";
	
	std::cout << "L: " << std::endl;
	
	for (auto pedido: L){
			
			std::cout << pedido << " ";
	}
	
	std::cout << "\n";
	
	std::cout << "A: " << std::endl;
	
	for (auto pedido: A){
			
			std::cout << pedido << " ";
	}
	
	std::cout << "\n";
	
}