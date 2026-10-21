# main(Golf)
 
Jogo de mini golfe para terminal, feito em C com a cli-lib. A cada tacada o jogador escolhe o ângulo e a força pelo teclado e tenta levar a bola até o buraco no menor número de jogadas possível.
 
Projeto da disciplina de Programação Imperativa e Funcional.
 
## Como o jogo funciona
 
O terreno influencia o movimento da bola: na grama ela rola mais, na areia perde velocidade rápido. As paredes fazem a bola quicar, a água devolve a bola ao ponto anterior com penalidade na pontuação, e alguns obstáculos se movem pelo mapa, então às vezes vale esperar o momento certo para tacar.
 
### Pontuação
 
Cada fase tem um par, e o resultado do jogador é comparado a ele. Jogadas limpas, sem cair na água, dão pontos de bônus, e uma sequência de bons resultados seguidos aumenta um multiplicador aplicado à pontuação. Quando a partida termina, o jogador entra no ranking se tiver feito pontos suficientes. O ranking fica salvo em arquivo e continua disponível na próxima vez que o jogo for aberto.
 
### Fases
 
Cada fase é um arquivo de texto, lido e guardado em uma matriz alocada dinamicamente. Para criar um mapa novo basta escrever outro arquivo, sem mexer no código.
 
## Estruturas de dados
 
- Structs representam a bola, a fase, os obstáculos e os jogadores.
- A matriz do mapa é alocada dinamicamente conforme o tamanho da fase.
- Os obstáculos móveis e o ranking são mantidos em listas encadeadas.
- Ponteiros são usados em todo o código para passar e modificar essas estruturas.
- O ranking é gravado e lido de um arquivo.
## Compilação e execução
 
Ajuste os caminhos abaixo para a estrutura do repositório.
 
```bash
git clone https://github.com/HeitorVillachan/mainGolf.git
cd mainGolf
gcc src/*.c -Iinclude -o maingolf
./maingolf
```
 
## Controles

- Setas esquerda e direita: ajustam o ângulo
- Setas para cima e para baixo: ajustam a força
- Enter: dá a tacada