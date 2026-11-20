# ⛳ main(Golf)

Jogo de minigolfe 2D desenvolvido em **C** com a biblioteca gráfica **Raylib**, criado como projeto acadêmico para a disciplina de **PIF (Programação Imperativa e Funcional)**.

Com uma interface inspirada na própria sintaxe da linguagem C, o jogo conta com simulação física, diferentes tipos de terreno que alteram o atrito da bola, 5 fases progressivas e sistema de ranking persistente.

---

## ✨ Funcionalidades

- **Interface Temática em C:** Menu inicial e tela de ranking estilizados como blocos de código (`int main(void)`).
- **Física e Colisões:** Simulação com sub-passos de alta precisão, rebote em paredes com coeficiente de restituição e controle de mira/força com ajuste fino.
- **Diferentes Superfícies:** Cada tipo de piso altera a dinâmica da tacada:
  - **Grama (`0`):** Atrito padrão.
  - **Areia (`1`):** Atrito elevado, desacelerando a bola mais rapidamente.
  - **Gelo (`2`):** Atrito reduzido, fazendo a bola deslizar por longas distâncias.
  - **Barro (`3`):** Atrito muito alto, freando bruscamente o movimento.
  - **Parede (`4`):** Obstáculo sólido com rebote.
  - **Água (`5`):** Penalidade — retorna a bola à posição da última tacada e adiciona `+1` tacada.
  - **Buraco (`6`):** Objetivo da fase (exige velocidade controlada para encaçapar).
- **5 Fases em Arquivos Externos:** Mapas carregados dinamicamente a partir de matrizes em arquivos `.txt` (`assets/mapas/mapa1.txt` a `mapa5.txt`), cada um com seu próprio *par*.
- **Sistema de Pontuação e Combo:** Pontuação baseada no desempenho em relação ao *par* da fase, com bônus para *Hole-in-One* (tacada única), fases sem quedas na água e multiplicador de sequência de até **5x**.
- **Ranking Persistente:** Armazenamento ordenado em memória via lista encadeada e salvo localmente em `scores.txt`, exibindo o Top 5 jogadores.
- **Vídeo Adaptativo:** Suporte a janela redimensionável e modo tela cheia sem bordas (`F11`) com ajuste automático de escala e proporção.

---

## 🎮 Controles

| Tecla | Ação |
| :--- | :--- |
| `↑` / `↓` | Aumenta ou diminui a força da tacada (e navega no menu) |
| `←` / `→` | Ajusta o ângulo da mira |
| `Shift` (Esq/Dir) | Modo de precisão (ajuste fino de ângulo e força) |
| `Espaço` | Realiza a tacada |
| `R` | Reinicia a bola no início da fase (`+1` tacada de penalidade) |
| `Enter` | Confirma seleção no menu / Avança de fase / Salva pontuação |
| `Backspace` | Volta da tela de ranking / Apaga caractere ao digitar o nome |
| `F11` | Alterna entre modo janela e tela cheia |

---

## 🎥 Demonstração (Gameplay)

> 🔗 **[Assista ao vídeo da gameplay aqui]https://youtu.be/BwWm7Is-VZk?si=wdIjkEkfONfnVLDm**


## 📁 Estrutura do Projeto

```text
mainGolf-main/
├── assets/
│   └── mapas/              # Arquivos de configuração das fases (mapa1.txt a mapa5.txt)
├── include/                # Cabeçalhos (.h) dos módulos do jogo e da Raylib
├── lib/                    # Biblioteca estática da Raylib (libraylib.a)
├── src/                    # Código-fonte principal em C
│   ├── main.c              # Loop principal, gerenciamento de estados e telas
│   ├── bola.c              # Física, colisões, mira e entrada do jogador
│   ├── mapa.c              # Leitura de matrizes de mapas e renderização dos tiles
│   ├── menu.c              # Interface e navegação do menu e do ranking
│   ├── pontos.c            # Cálculo de pontuação, bônus e multiplicadores
│   ├── ranking.c           # Lista encadeada e persistência em arquivo
│   └── video.c             # Controle de resolução, escala e tela cheia
├── Makefile                # Automação de build com GCC
├── scores.txt              # Registro local das pontuações do ranking
└── README.md