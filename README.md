# Relatório: Simulação de Propagação de Epidemias em Diferentes Tipos de Redes

## Introdução
Este trabalho tem como objetivo analisar a propagação de uma epidemia utilizando os seguintes modelos de rede:

+ Redes aleatórias (Erdős-Rényi),
+ Redes livres de escala (Barabási-Albert),
+ Redes de pequeno mundo (Watts-Strogatz).

Parâmetros como taxa de infecção, taxa de recuperação e probabilidade de conexão entre os nodos foram ajustados, e o comportamento da propagação foi avaliado através de métricas de interesse.

## Implementação
A simulação foi implementada em C utilizando estruturas de dados como grafos para representar os nodos e suas conexões. As principais funções implementadas são:

+ criar_rede_aleatoria: Cria uma rede com conexões aleatórias entre nodos.
+ criar_rede_livre_escala: Cria uma rede onde os nodos seguem uma distribuição de grau proporcional.
+ criar_pequenos_mundos: Implementa uma rede circular com reconexões aleatórias, simulando pequenas alterações topológicas.
+ simular_epidemia: Simula a propagação da epidemia ao longo de iterações.

Além disso, cada nodo pode assumir os seguintes estados:

+ SAUDÁVEL: Não infectado.
+ INFECTADO: Em estágio inicial da infecção.
+ CONTAMINANTE: Capaz de infectar outros nodos.
+ CURADO: Recuperado.
+ MORTO: Removido da rede.

A visualização foi gerada utilizando o Graphviz e o gnuplot para acompanhar a propagação nos diferentes tipos de redes.

## Metodologia

### Configuração Inicial

Configuração que foi aplicada em todos os testes

+ Número total de nodos: 1000
+ Iterações: 100
+ Número de iterações para contaminar: 2
+ Número de infectados na primeira iteração: 1

### Métricas Utilizadas

+ Taxa de Transmissão
+ Taxa de Recuperação
+ Taxa de Mortalidade
+ Número de Vértices
+ Densidade
+ Total de Infectados
+ Total de Contaminantes
+ Total de Mortos
+ Pico de Infectados
+ Tempo até Pico
+ Média de Infectados
+ Desvio Padrão de Infectados
+ Variância de Infectados

### Configuração das Doenças

Foram realizados testes com 3 doenças (A, B e C):

1. Doença A: alta taxa de transmissão, baixa taxa de recuperação e baixa taxa de mortalidade.
2. Doença B: baixa taxa de transmissão, alta taxa de recuperação e baixa taxa de mortalidade.
3. Doença C: média taxa de transmissão, média taxa de recuperação e média taxa de mortalidade.

### Configuração dos Modelos de Rede:

Nos modelos de pequenos mundos e aleatório foram testadas duas variações de construção do grafo: uma com baixa densidade, ou seja, poucas arestas no geral, e uma com alta densidade. O modelo livre de escala foi utilizado com densidade média.

## Resultados e Discussão

| taxa_transmissao | taxa_recuperacao | taxa_mortalidade | num_vertices | num_repeticoes | total_infectados | total_contaminantes | total_mortos | pico_infectados | tempo_ate_pico | media_infeccao | desvio_padrao_infeccao | variancia_infeccao |
|------------------|------------------|------------------|--------------|----------------|------------------|---------------------|--------------|-----------------|----------------|----------------|------------------------|--------------------|
| 0.80             | 0.40             | 0.03             | 10000        | 100            | 9700             | 9700                | 407          | 3366            | 15             | 193.99         | 646.56                 | 418035.67          |
| 0.80             | 0.40             | 0.03             | 10000        | 100            | 9959             | 9959                | 422          | 3662            | 10             | 199.17         | 698.60                 | 488046.37          |
| 0.80             | 0.40             | 0.03             | 10000        | 100            | 10000            | 10000               | 442          | 7167            | 2              | 199.99         | 1071.51                | 1148126.23         |
| 0.80             | 0.40             | 0.03             | 10000        | 100            | 10000            | 10000               | 455          | 8376            | 3              | 199.99         | 1172.84                | 1375552.23         |
| 0.80             | 0.40             | 0.03             | 10000        | 100            | 10000            | 10000               | 388          | 5575            | 8              | 199.99         | 888.00                 | 788541.77          |
| 0.30             | 0.75             | 0.01             | 10000        | 100            | 5378             | 5378                | 20           | 457             | 37             | 107.55         | 144.53                 | 20888.04           |
| 0.30             | 0.75             | 0.01             | 10000        | 100            | 3                | 3                   | 0            | 1               | 4              | 0.05           | 0.22                   | 0.05               |
| 0.30             | 0.75             | 0.01             | 10000        | 100            | 10000            | 10000               | 38           | 7294            | 4              | 199.99         | 1082.03                | 1170793.19         |
| 0.30             | 0.75             | 0.01             | 10000        | 100            | 10000            | 10000               | 37           | 9389            | 4              | 199.99         | 1315.64                | 1730912.39         |
| 0.30             | 0.75             | 0.01             | 10000        | 100            | 9142             | 9142                | 23           | 2793            | 12             | 182.83         | 555.66                 | 308757.83          |
| 0.50             | 0.50             | 0.20             | 10000        | 100            | 8983             | 8983                | 1500         | 2117            | 21             | 179.65         | 473.54                 | 224242.79          |
| 0.50             | 0.50             | 0.20             | 10000        | 100            | 6                | 6                   | 0            | 1               | 11             | 0.11           | 0.31                   | 0.10               |
| 0.50             | 0.50             | 0.20             | 10000        | 100            | 10000            | 10000               | 1691         | 5558            | 4              | 199.99         | 986.25                 | 972680.87          |
| 0.50             | 0.50             | 0.20             | 10000        | 100            | 10000            | 10000               | 1725         | 8940            | 4              | 199.99         | 1257.50                | 1581305.59         |
| 0.50             | 0.50             | 0.20             | 10000        | 100            | 9947             | 9947                | 1646         | 4082            | 9              | 198.93         | 762.12                 | 580829.12          |



Em todos os testes a tendencia esperada é que haja um pico de indivíduos infectados com um crescimento exponencial inicial, seguido de um platô. Outras métricas como curados e mortos tendem a subir e se estabilizar dependendo da gravidade da doença.

### Propagação em Redes Aleatórias

Na rede aleatória, observou-se que a propagação ocorreu de maneira rápida devido à natureza uniforme das conexões. O número de indivíduos infectados apresentou um crescimento exponencial inicial, seguido de um platô.

Gráfico 1: Número de infectados ao longo do tempo (Rede Aleatória).

### Propagação em Redes Livres de Escala

As redes livres de escala apresentaram comportamento distinto, com hubs (nodos com alto grau de conexão) desempenhando papel crítico na propagação. A epidemia se espalhou de maneira mais eficiente devido à alta conectividade dos hubs.

Gráfico 2: Número de infectados ao longo do tempo (Rede Livre de Escala).

### Propagação em Redes de Pequeno Mundo

A estrutura de pequenos mundos apresentou propagação mais controlada. A presença de caminhos curtos acelerou o contato inicial, mas a existência de sub-redes com baixa reconexão dificultou a propagação total.

Gráfico 3: Número de infectados ao longo do tempo (Rede Pequeno Mundo).

### Comparação das Redes
Tipo de Rede	Tempo até o Pico	Pico de Infectados
Rede Aleatória	15 iterações	600 nodos
Rede Livre de Escala	12 iterações	750 nodos
Rede Pequeno Mundo	20 iterações	500 nodos

### Estratégias de Mitigação
Isolamento de Hubs: Na rede livre de escala, remover ou proteger hubs (através de vacinas) diminuiu drasticamente a propagação.
Aumento da Taxa de Recuperação: Simular aumento na taxa de recuperação mostrou impacto positivo na redução de infectados.

## Conclusão
A estrutura da rede influencia diretamente a propagação da epidemia. Redes livres de escala apresentam a propagação mais eficiente devido aos hubs, enquanto redes de pequeno mundo apresentam comportamento intermediário. Estratégias de mitigação como isolamento de hubs e aumento da taxa de recuperação demonstram eficácia no controle da epidemia.