# ORB_KProfiller

## Interface gráfica do usuário para simulação de escalonamento de tarefas
## Funcionamento: 
dê "make", e então execute o arquivo orbKprofiller.exe; 
## Versões
A classe "main" possue a interface gráfica base, mas não há link com Orca-Rt-Bench, enquanto que a classe "linkORCA" faz o link com o orca-rt-bench, mas ainda não há garantia de funcionamento 100%
Recomendando executar o arquivo "main", pois o .exe referente à "linkORCA.c" não pode ser executado se não referenciar o path do ORCA diretamente.
## Funcionabilidades
O aplicativo ORB: Kprofiller, simula a ordem de tarefas escalonadas, além de mostrar o Schedulability Test, o qual define se é possível escalonar tais tarefas com determinado algoritmo de forma eficiente
## Preview:  
https://user-images.githubusercontent.com/65430820/123334328-50b6fb80-d519-11eb-9b77-bad12e587e32.mp4

### Schedulability Test 
Utilizando a fórmula:
![image](https://user-images.githubusercontent.com/65430820/123334625-befbbe00-d519-11eb-8ffd-5b4fef83ab7b.png)
Nas tarefas: ![image](https://user-images.githubusercontent.com/65430820/123334724-e2bf0400-d519-11eb-8b99-099002e964b4.png)
Podemos verificar que o sistema é escalonável pois (1/4 + 2/5 + 5/20 = 0.90)



