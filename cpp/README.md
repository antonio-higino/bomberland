--------------------------Competição Bomberland--------------------------

-> INDRODUÇÂO
O intuito desde projeto é usar o que foi aprendido em aula sobre as IA's
nos jogos, tais como algoritmos de busca  para implementar no jogo 
Bomberland uma inteligência artificial que consiga minimamente fazer algo 
lógico e funcional diante do estado atual do jogo.

-> METODOLOGIA
Tendo como base o que foi visto em aula, preferimos adotar o algoritmo 
de busca A*(A Estrela), que está implementado no arquivo "a_estrela.cpp". 
Com isso, o algoritmo é chamado em "pathFinder()" em "agent.cpp", que é 
responsável por fazer com que ele navegue pelo mapa em busca do objetivo
(inimigo). Além disso, aliado ao algoritmo de busca
implementamos também uma função chamada "escolherOrdem()" em "agent.cpp"
para fazer com que o agente decida se coloca uma bomba ou mover (chamar a 
função "pathFinder()"). 
Mediante testes e dúvidas comentadas em aula, vimos que ocorria um
problema de loop infinito, onde para solucionar criamos uma 
variável chamada "timeout" para forçar com que o programa saísse do loop 
quando necessário, como uma espécie de condição de parada.

-> BIBLIOTECAS
Quanto as bibliotecas ultilizadas, usamos apenas as bibliotecas próprias 
do C++ como a stdlib. Nada externo ou de terceiros foi ultilizado.
Para ajudar na implementação do algoritmo de busca, as bibliotecas 
"unordered_map" e "unordered_set" foram usadas para auxiliar na 
construção do A*. Vale ressaltar também que as bibliotecas como "queue" e 
de "vector", por exemplo, ajudaram na hora de implementar estruturas como 
filas e vetores. 

-> PARTICIPANTES
Denys Rafael Nobre de Oliveira 
Antonio Higino Bisneto Leite Medeiros