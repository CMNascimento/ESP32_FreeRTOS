#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_heap_caps.h"
#include "freertos/queue.h"


BaseType_t xReturned1, xReturned2, xReturned3;
TaskHandle_t Task1 = NULL, Task2 = NULL, Task3 = NULL;
SemaphoreHandle_t xSemaphore;
QueueHandle_t xQueue;
static const char *TAG = "demo";
uint8_t g_semaphoreTakeTask = 0; 
uint32_t g_currentramLeft = 0;
uint32_t gp_previousramLeft = 0;


void task_1(void){
    ESP_LOGI(TAG, "TASK 1");
    
    if( xQueueReceive(xQueue, &gp_previousramLeft, ( TickType_t ) 0)){
        ESP_LOGW(TAG, "Memória RAM livre com a tarefa anterior: %d bytes", gp_previousramLeft);
    }else{
        ESP_LOGE(TAG, "Elemento não pode ser lido na fila.");
    }
    
    g_currentramLeft = esp_get_free_heap_size();
    ESP_LOGI(TAG, "RAM restante com atual tarefa: %d bytes", g_currentramLeft);

    if(xQueueSend(xQueue, &g_currentramLeft, ( TickType_t ) 0)){
        ESP_LOGI(TAG, "Elemento salvo na fila com sucesso.");
    }else{
        ESP_LOGE(TAG, "Elemento não pode ser salvo na fila.");
    }

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    
    vTaskDelete(Task1);
}

void task_2(void){
    ESP_LOGI(TAG, "TASK 2");
    
    if( xQueueReceive(xQueue, &gp_previousramLeft, ( TickType_t ) 0)){
        ESP_LOGW(TAG, "Memória RAM livre com a tarefa anterior: %d bytes", gp_previousramLeft);
    }else{
        ESP_LOGE(TAG, "Elemento não pode ser lido na fila.");
    }
    
    g_currentramLeft = esp_get_free_heap_size();
    ESP_LOGI(TAG, "RAM restante com atual tarefa: %d bytes", g_currentramLeft);

    if(xQueueSend(xQueue, &g_currentramLeft, ( TickType_t ) 0)){
        ESP_LOGI(TAG, "Elemento salvo na fila com sucesso.");
    }else{
        ESP_LOGE(TAG, "Elemento não pode ser salvo na fila.");
    }

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    
    vTaskDelete(Task2);
}

void task_3(void){
    ESP_LOGI(TAG, "TASK 3");
    
    if( xQueueReceive(xQueue, &gp_previousramLeft, ( TickType_t ) 0)){
        ESP_LOGW(TAG, "Memória RAM livre com a tarefa anterior: %d bytes", gp_previousramLeft);
    }else{
        ESP_LOGE(TAG, "Elemento não pode ser lido na fila.");
    }
    
    g_currentramLeft = esp_get_free_heap_size();
    ESP_LOGI(TAG, "RAM restante com atual tarefa: %d bytes", g_currentramLeft);

    if(xQueueSend(xQueue, &g_currentramLeft, ( TickType_t ) 0)){
        ESP_LOGI(TAG, "Elemento salvo na fila com sucesso.");
    }else{
        ESP_LOGE(TAG, "Elemento não pode ser salvo na fila.");
    }

    vTaskDelay(10000 / portTICK_PERIOD_MS);

    vTaskDelete(Task3);
}


void task_1_create(void){

    if( xSemaphoreTake( xSemaphore, 50 / portTICK_PERIOD_MS ) == pdTRUE )
    {

        g_semaphoreTakeTask = 1;

        xReturned1 = xTaskCreate(task_1, "Task1", 32768, NULL, 5, &Task1);

        if (xReturned1 == pdPASS)
        { 
            ESP_LOGI(TAG, "Task 1 criada com sucesso.");
        }else{
            ESP_LOGW(TAG, "Task 1 não pode ser criada.");
        }
    }
    if(g_semaphoreTakeTask == 1){
        if (eTaskGetState( Task1 ) == 4){
            xSemaphoreGive(xSemaphore);
        }
    }
}

void task_2_create(void){

    if( xSemaphoreTake( xSemaphore, 50 / portTICK_PERIOD_MS ) == pdTRUE )
    {

        g_semaphoreTakeTask = 2;

        xReturned2 = xTaskCreate(task_2, "Task1", 65536, NULL, 5, &Task2);

        if (xReturned2 == pdPASS)
        { 
            ESP_LOGI(TAG, "Task 2 criada com sucesso.");
        }else{
            ESP_LOGW(TAG, "Task 2 não pode ser criada.");
        }
    }
    if(g_semaphoreTakeTask == 2){
        if (eTaskGetState( Task2 ) == 4){
            xSemaphoreGive(xSemaphore);
        }
    }
}

void task_3_create(void){

    if( xSemaphoreTake( xSemaphore, 50 / portTICK_PERIOD_MS ) == pdTRUE )
    {

        g_semaphoreTakeTask = 3;

        xReturned3 = xTaskCreate(task_3, "Task1", 16384, NULL, 5, &Task3);

        if (xReturned3 == pdPASS)
        { 
            ESP_LOGI(TAG, "Task 3 criada com sucesso.");
        }else{
            ESP_LOGW(TAG, "Task 3 não pode ser criada.");
        }
    }
    if(g_semaphoreTakeTask == 3){
        if (eTaskGetState( Task3 ) == 4){
            xSemaphoreGive(xSemaphore);
        }
    }
}


void loop(void){
    for(;;){
        task_1_create();
        task_2_create();
        task_3_create();
    }
}


void app_main(void)
{
    ESP_LOGI(TAG, "Início do Exemplo.");
    g_currentramLeft = esp_get_free_heap_size();
    ESP_LOGI(TAG, "RAM restante: %d bytes", g_currentramLeft);

    xQueue = xQueueCreate(1, sizeof( uint32_t ));

    if(xQueue != NULL){
        ESP_LOGI(TAG, "Fila criada com sucesso.");
        if(xQueueSend(xQueue, &g_currentramLeft, ( TickType_t ) 0)){
            ESP_LOGI(TAG, "Elemento salvo na fila com sucesso.");
        }else{
            ESP_LOGE(TAG, "Elemento não pode ser salvo na fila.");
        }
    }

    xSemaphore = xSemaphoreCreateBinary();
    if( xSemaphore != NULL ){
        ESP_LOGI(TAG, "Semáforo criado com sucesso.");
        xSemaphoreGive(xSemaphore);
    }

    loop();
       
}
