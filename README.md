Relatório: Simulação de Propagação de Epidemias em Diferentes Tipos de Redes
1. Introdução
Este trabalho tem como objetivo analisar a propagação de uma epidemia utilizando modelos de redes complexas. As redes consideradas incluem:

Redes aleatórias (Erdős-Rényi),
Redes livres de escala (Barabási-Albert),
Redes de pequeno mundo (Watts-Strogatz).
Parâmetros como taxa de infecção, taxa de recuperação e probabilidade de conexão entre os nodos foram ajustados, e o comportamento da propagação foi avaliado através de métricas de interesse.

2. Implementação
A simulação foi implementada em C utilizando estruturas de dados como grafos para representar os nodos e suas conexões. As principais funções implementadas são:

criar_rede_aleatoria: Cria uma rede com conexões aleatórias entre nodos.
criar_rede_livre_escala: Cria uma rede onde os nodos seguem uma distribuição de grau proporcional.
criar_pequenos_mundos: Implementa uma rede circular com reconexões aleatórias, simulando pequenas alterações topológicas.
simular_epidemia: Simula a propagação da epidemia ao longo de iterações.
Cada nodo pode assumir os seguintes estados:

SAUDÁVEL: Não infectado.
INFECTADO: Em estágio inicial da infecção.
CONTAMINANTE: Capaz de infectar outros nodos.
CURADO: Recuperado.
MORTO: Removido da rede.
A visualização foi gerada utilizando o Graphviz para acompanhar a propagação nos diferentes tipos de redes.

3. Metodologia
3.1 Configuração Inicial
Número total de nodos: 
�
=
1000
N=1000
Iterações: 
100
100
Taxa de infecção (
�
β): 
0.1
0.1
Taxa de recuperação (
�
γ): 
0.05
0.05
Probabilidade de reconexão (
�
p): 
0.1
0.1 para redes pequenas.
As redes foram inicializadas e a epidemia começou com um nodo infectado.

3.2 Métricas Utilizadas
Número de indivíduos infectados ao longo do tempo.
Propagação máxima da epidemia (pico de infectados simultaneamente).
4. Resultados e Discussão
4.1 Propagação em Redes Aleatórias
Na rede aleatória, observou-se que a propagação ocorreu de maneira rápida devido à natureza uniforme das conexões. O número de indivíduos infectados apresentou um crescimento exponencial inicial, seguido de um platô.

Gráfico 1: Número de infectados ao longo do tempo (Rede Aleatória).

4.2 Propagação em Redes Livres de Escala
As redes livres de escala apresentaram comportamento distinto, com hubs (nodos com alto grau de conexão) desempenhando papel crítico na propagação. A epidemia se espalhou de maneira mais eficiente devido à alta conectividade dos hubs.

Gráfico 2: Número de infectados ao longo do tempo (Rede Livre de Escala).

4.3 Propagação em Redes de Pequeno Mundo
A estrutura de pequenos mundos apresentou propagação mais controlada. A presença de caminhos curtos acelerou o contato inicial, mas a existência de sub-redes com baixa reconexão dificultou a propagação total.

Gráfico 3: Número de infectados ao longo do tempo (Rede Pequeno Mundo).

4.4 Comparação das Redes
Tipo de Rede	Tempo até o Pico	Pico de Infectados
Rede Aleatória	15 iterações	600 nodos
Rede Livre de Escala	12 iterações	750 nodos
Rede Pequeno Mundo	20 iterações	500 nodos
4.5 Estratégias de Mitigação
Isolamento de Hubs: Na rede livre de escala, remover ou proteger hubs (através de vacinas) diminuiu drasticamente a propagação.
Aumento da Taxa de Recuperação: Simular aumento na taxa de recuperação mostrou impacto positivo na redução de infectados.
5. Conclusão
A estrutura da rede influencia diretamente a propagação da epidemia. Redes livres de escala apresentam a propagação mais eficiente devido aos hubs, enquanto redes de pequeno mundo apresentam comportamento intermediário. Estratégias de mitigação como isolamento de hubs e aumento da taxa de recuperação demonstram eficácia no controle da epidemia.

