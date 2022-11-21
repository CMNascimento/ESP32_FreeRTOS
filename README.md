# ESP32_FreeRTOS

Data: terceira semana de Novembro/2022

Descrição: Código desenvolvido para o estudo do FreeRTOS com o ESP-IDF utilizando a ESP32. Nele foi utilizado semáforo, fila, tarefas e atrasos.

O porgrama comportasse da seguinte maneira: 

I) Inicialmente verifica-se o quanto de memória RAM está disponível, cria a fila e salva o valor obtido nela. Cria o semáforo e entra na função loop();

II) Na função loop() estão outras três funções que ficam alternando na criação das tarefas;

III) O semáforo é utilizado para gerenciar a criação das tarefas, não permitindo que exista duas tarefas no mesmo instante de tempo;

IV) Quando a função da criação da tarefa obtém a vez no semáforo, cria a tarefa e só libera a vez quando a tarefa criada é finalizada, alternado assim entras as criações das tarefas;

V) As tarefas criadas possuem tamanhos diferentes, atrasos diferentes, verificam o último valor de memória RAM na pilha e salvam na pilha o atual valor.
