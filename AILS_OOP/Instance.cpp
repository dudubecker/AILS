#include "Instance.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

Instance::Instance()
{
}


// Inicializando

Instance::Instance(std::string &file_name){
	
	// A string "instancia" armazenará os dados linha a linha
	std::string instancia;
	std::fstream my_file;
	
	my_file.open(file_name, std::ios::in);
	if (my_file.is_open()){
		std::string line;
		
		while (getline(my_file, line)){
			instancia.append(line + ' ');
			
		}
		my_file.close();
		
	}
	
	std::string delim = " ";
	size_t pos = 0;  
	std::string token;
	
	// Contador, atualizado a cada iteração cujo resultado é um número
	int i {0};
	
	while ((pos = instancia.find(delim)) != std::string::npos){
		
		token = instancia.substr(0, pos);
		
		// Checando se o número é float:
		
		std::istringstream iss(token);
		double f;
		iss >> std::noskipws >> f;
		
		bool isFloat = (iss.eof() && !iss.fail());
		
		if (isFloat){
			
			i += 1;
			
			// Guardando dado para número de requests (segundo dado na instância)
			if (i == 2){
				
				//cout << "n =" << token << endl;
				n = stoi(token);
			}
			// Guardando dado para capacidade dos veículos (quarto dado na instância)
			else if (i == 4){
				
				//cout << "Cap =" << token << endl;
				Cap = stoi(token);
			}
			// Caso o valor do contador não corresponda aos dados da primeira linha, será verificada a divisão inteira pelos índices
			else if (i > 5){
				
				// Caso o valor "i" seja divisível por 7, ele corresponde a um valor de coordenada x, e assim por diante
				if (i%7==0){
					x.push_back(stof(token));
				}
				else if ((i-1)%7==0){
					y.push_back(stof(token));
				}
				else if ((i-2)%7==0){
					d.push_back(stof(token));
				}
				else if ((i-3)%7==0){
					q.push_back(stof(token));
				}
				else if ((i-4)%7==0){
					e.push_back(stof(token));
				}
				else if ((i-5)%7==0){
					l.push_back(stof(token));
				}
			}
		}
	instancia.erase(0, pos + delim.length());
	}
	
	// Atribuindo ao vetor de tempos de viagem t_ij valores iguais a 0:
	t =  std::vector<std::vector<double>> (2*n + 2, std::vector<double> (2*n + 2, 0));
	
	// Atribuindo ao vetor de tempos de viagem t_ij normalizados valores iguais a 0:
	t_norm = t;
	
	// Atribuindo ao vetor de demandas normalizadas valores iguais a 0:
	q_norm = std::vector<double> (2*n + 2, 0);
	
	// Valor mínimo para tempos de viagem
	double min_t = 99999;
	// Valor máximo para tempos de viagem
	double max_t = 0;
	
	// Atribuindo distâncias t_ij para cada elemento
	for(int i = 0; i < 2*n + 2; i++)
	{
		for(int j = 0; j < 2*n + 2; j++)
		{
			// Calculando distância e atribuindo à variável
			
			// Alteração em relação à ALNS: colocando 3 casas decimais e não duas!
			
			t[i][j] = floor(sqrt(pow(x[i]-x[j], 2) + pow(y[i]-y[j],2))*1000.0)/1000.00;
			
			// Adição -> tempo de serviço (instâncias li-lim)
			t[i][j] += d[i];
			
			// Determinando maiores e menores tempos de viagem (útil para normalização dos dados)
			if (t[i][j] > max_t){
				max_t = t[i][j];
			}
			if (t[i][j] < min_t){
				min_t = t[i][j];
			}
			
		}
		
		
	}
	
	// Atribuindo valor máximo de t para o atributo "max_dist" (utilizado na aplicação de ruído no cálculo da FO)
	max_dist = max_t;
	
	// Dados normalizados para Shaw's removal: demandas (q) e tempos de viagem (t)
	
	// Vetor com demandas normalizadas
	std::vector <double> q_norm (2*n + 2, 0);
	
	// Valor mínimo para demanda
	double min_q = *min_element(q.begin(), q.end());
	// Valor máximo para demanda
	double max_q = *max_element(q.begin(), q.end());
	
	// Fazendo uma iteração geral para normalização
	
	for (int i {0}; i < 2*n + 2; i++){
		
		// Normalizando valores de demanda
		
		q_norm[i] = (q[i] - min_q)/(max_q - min_q);
		
		for (int j {0}; j < 2*n + 2; j++){
			
			// Normalizando valores de tempos de viagem
			
			t_norm[i][j] = (t[i][j] - min_t)/(max_t - min_t);
		}
	}
	
	// Populando vetores para concentric removal
	
	x_med = std::vector<double> (n + 1, 0);
	
	y_med = std::vector<double> (n + 1, 0);
	
	t_med = std::vector<std::vector<double>> (n + 1, std::vector<double> (n + 1, 0));
	
	// Determinando valores médios entre os pontos correspondentes a pickup e delivery
	
	for (int index_pedido {1}; index_pedido < n + 1; index_pedido++){
		
		x_med.at(index_pedido) = (x.at(index_pedido) + x.at(index_pedido + n))/2;
		
		y_med.at(index_pedido) = (y.at(index_pedido) + y.at(index_pedido + n))/2;
		
	}
	
	// Determinando distâncias entre pedidos
	
	for (int i {1}; i < n + 1; i++){
		
		for (int j {1}; j < n + 1; j++){
			
			t_med[i][j] = floor(sqrt(pow(x_med[i]-x_med[j], 2) + pow(y_med[i]-y_med[j],2))*1000.0)/1000.00;
			
		}
	}
	
	
}

Instance::~Instance()
{
}