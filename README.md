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
+ Diâmetro
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

| Doença   | Modelo          | Diametro | taxa_transmissao    | taxa_recuperacao    | taxa_mortalidade     | num_vertices | num_repeticoes | total_infectados | total_contaminantes | total_mortos | pico_infectados | tempo_ate_pico | media_infeccao       | desvio_padrao_infeccao | variancia_infeccao   |
|----------|-----------------|----------|---------------------|---------------------|----------------------|--------------|----------------|------------------|---------------------|--------------|-----------------|----------------|----------------------|------------------------|----------------------|
| Doença A | Pequenos Mundos | Grande   | 0.80000000000000004 | 0.40000000000000002 | 0.029999999999999999 | 10000        | 100            | 9700             | 9700                | 407          | 3366            | 15             | 193.99000000000001   | 646.55999999999995     | 418035.66999999998   |
| Doença A | Pequenos Mundos | Pequeno  | 0.80000000000000004 | 0.40000000000000002 | 0.029999999999999999 | 10000        | 100            | 9959             | 9959                | 422          | 3662            | 10             | 199.16999999999999   | 698.60000000000002     | 488046.37            |
| Doença A | Aleatórias      | Grande   | 0.80000000000000004 | 0.40000000000000002 | 0.029999999999999999 | 10000        | 100            | 10000            | 10000               | 442          | 7167            | 2              | 199.99000000000001   | 1071.51                | 1148126.23           |
| Doença A | Aleatórias      | Pequeno  | 0.80000000000000004 | 0.40000000000000002 | 0.029999999999999999 | 10000        | 100            | 10000            | 10000               | 455          | 8376            | 3              | 199.99000000000001   | 1172.8399999999999     | 1375552.23           |
| Doença A | Livre de Escala | Médio    | 0.80000000000000004 | 0.40000000000000002 | 0.029999999999999999 | 10000        | 100            | 10000            | 10000               | 388          | 5575            | 8              | 199.99000000000001   | 888                    | 788541.77000000002   |
| Doença B | Pequenos Mundos | Grande   | 0.29999999999999999 | 0.75                | 0.01                 | 10000        | 100            | 5378             | 5378                | 20           | 457             | 37             | 107.55               | 144.53                 | 20888.040000000001   |
| Doença B | Pequenos Mundos | Pequeno  | 0.29999999999999999 | 0.75                | 0.01                 | 10000        | 100            | 3                | 3                   | 0            | 1               | 4              | 0.050000000000000003 | 0.22                   | 0.050000000000000003 |
| Doença B | Aleatórias      | Grande   | 0.29999999999999999 | 0.75                | 0.01                 | 10000        | 100            | 10000            | 10000               | 38           | 7294            | 4              | 199.99000000000001   | 1082.03                | 1170793.1899999999   |
| Doença B | Aleatórias      | Pequeno  | 0.29999999999999999 | 0.75                | 0.01                 | 10000        | 100            | 10000            | 10000               | 37           | 9389            | 4              | 199.99000000000001   | 1315.6400000000001     | 1730912.3899999999   |
| Doença B | Livre de Escala | Médio    | 0.29999999999999999 | 0.75                | 0.01                 | 10000        | 100            | 9142             | 9142                | 23           | 2793            | 12             | 182.83000000000001   | 555.65999999999997     | 308757.83000000002   |
| Doença C | Pequenos Mundos | Grande   | 0.5                 | 0.5                 | 0.20000000000000001  | 10000        | 100            | 8983             | 8983                | 1500         | 2117            | 21             | 179.65000000000001   | 473.54000000000002     | 224242.79000000001   |
| Doença C | Pequenos Mundos | Pequeno  | 0.5                 | 0.5                 | 0.20000000000000001  | 10000        | 100            | 6                | 6                   | 0            | 1               | 11             | 0.11                 | 0.31                   | 0.10000000000000001  |
| Doença C | Aleatórias      | Grande   | 0.5                 | 0.5                 | 0.20000000000000001  | 10000        | 100            | 10000            | 10000               | 1691         | 5558            | 4              | 199.99000000000001   | 986.25                 | 972680.87            |
| Doença C | Aleatórias      | Pequeno  | 0.5                 | 0.5                 | 0.20000000000000001  | 10000        | 100            | 10000            | 10000               | 1725         | 8940            | 4              | 199.99000000000001   | 1257.5                 | 1581305.5900000001   |
| Doença C | Livre de Escala | Médio    | 0.5                 | 0.5                 | 0.20000000000000001  | 10000        | 100            | 9947             | 9947                | 1646         | 4082            | 9              | 198.93000000000001   | 762.12                 | 580829.12            |
|          |                 |          |                     |                     |                      |              |                |                  |                     |              |                 |                |                      |                        |                      |

Na maioria dos testes observou-se um pico de indivíduos infectados com um crescimento exponencial inicial, seguido de um platô. Outras métricas como curados e mortos tendem a subir e se estabilizar dependendo da gravidade da doença.

### Propagação em Redes Aleatórias

Propagação Total:

Todas as doenças atingiram 100% de propagação (total de infectados), independentemente da densidade ou transmissibilidade.
Mesmo em baixa densidade, o modelo aleatório não restringe a propagação. Isso é evidente nas Doenças B e C, onde o total de infectados é sempre 10.000.
Tempo até o Pico:

O tempo até o pico de infecção é extremamente curto. Para Doença A, o pico ocorre em apenas 2 a 3 unidades de tempo, mostrando uma explosão inicial.
Variância e Desvio Padrão:

Valores de variância são muito altos (exemplo: 1.3 milhão para Doença B em baixa densidade), indicando instabilidade e uma propagação caótica.

### Propagação em Redes Livres de Escala

Embora a propagação seja ampla (quase 100% em todos os casos), o tempo até o pico é intermediário em comparação ao modelo aleatório. Por exemplo, para Doença A, o pico é atingido em 8 a 12 unidades de tempo.
Isso ocorre porque os hubs aceleram a disseminação, mas a estrutura da rede introduz alguma resiliência.
Impacto de Doenças com Baixa Transmissão:

Para doenças menos transmissíveis (Doença B), a propagação é significativa, mas ligeiramente limitada em comparação ao modelo aleatório. 91% dos indivíduos foram infectados, o que demonstra certa contenção.
Desvio Padrão e Variância:

Menor do que no modelo aleatório, indicando uma propagação mais estável e menos caótica.

### Propagação em Redes de Pequeno Mundo

Diametro Grande:

Permite uma boa disseminação para doenças com alta transmissibilidade (Doença A), mas o tempo até o pico é intermediário (15 a 21 unidades de tempo).
Para doenças menos transmissíveis (Doença B e C), a propagação é muito limitada. Por exemplo, na Doença B com baixa densidade, apenas 3 indivíduos foram infectados, praticamente impedindo a epidemia.
Baixa Densidade:

Diametro Pequeno:

Funciona como barreira para propagação. Mesmo doenças com taxa de transmissão média (Doença C) não conseguem se espalhar (apenas 6 infectados).
Essa estrutura mantém o tempo até o pico mais longo ou inexistente, reduzindo drasticamente a disseminação.

### Estratégias de Mitigação

Nas doenças B e C observou-se qua as doenças quase não se propagaram no modelo livre de escala com diametro pequeno. Isso também ocorreu porque a densidade do grafo foi diminuida. O que corrobora para a eficiência do isolamento social como estratégia de combate a epidemias e pandemias.

## Conclusão

A estrutura da rede influencia diretamente a propagação da epidemia. Portanto, para contenção de epidemias, o modelo Pequenos Mundos em baixa densidade se mostra mais eficaz. Em contrapartida, redes aleatórias são as mais vulneráveis e devem ser evitadas em cenários onde a conectividade pode facilitar surtos