#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

static const char *TAG = "demo";

BaseType_t xReturned1, xReturned2, xReturned3;
TaskHandle_t Task1 = NULL, Task2 = NULL, Task3 = NULL;
SemaphoreHandle_t xSemaphore;
int gi_semaphoreTakeTask = 0; 

void task_1(void){
    ESP_LOGI(TAG, "TASK 1");

    vTaskDelay(2000 / portTICK_PERIOD_MS);
    
    vTaskDelete(Task1);
}

void task_2(void){
    ESP_LOGI(TAG, "TASK 2");

    vTaskDelay(5000 / portTICK_PERIOD_MS);
    
    vTaskDelete(Task2);
}

void task_3(void){
    ESP_LOGI(TAG, "TASK 3");

    vTaskDelay(10000 / portTICK_PERIOD_MS);

    vTaskDelete(Task3);
}

void task_1_create(void){

    if( xSemaphoreTake( xSemaphore, 50 / portTICK_PERIOD_MS ) == pdTRUE )
    {

        gi_semaphoreTakeTask = 1;

        xReturned1 = xTaskCreate(task_1, "Task1", 4096, NULL, 5, &Task1);

        if (xReturned1 == pdPASS)
        { 
            ESP_LOGI(TAG, "Task 1 criada com sucesso.");
        }else{
            ESP_LOGW(TAG, "Task 1 não pode ser criada.");
        }
    }
    if(gi_semaphoreTakeTask == 1){
        if (eTaskGetState( Task1 ) == 4){
            xSemaphoreGive(xSemaphore);
        }
    }
}

void task_2_create(void){

    if( xSemaphoreTake( xSemaphore, 50 / portTICK_PERIOD_MS ) == pdTRUE )
    {

        gi_semaphoreTakeTask = 2;

        xReturned2 = xTaskCreate(task_2, "Task1", 4096, NULL, 5, &Task2);

        if (xReturned2 == pdPASS)
        { 
            ESP_LOGI(TAG, "Task 2 criada com sucesso.");
        }else{
            ESP_LOGW(TAG, "Task 2 não pode ser criada.");
        }
    }
    if(gi_semaphoreTakeTask == 2){
        if (eTaskGetState( Task2 ) == 4){
            xSemaphoreGive(xSemaphore);
        }
    }
}

void task_3_create(void){

    if( xSemaphoreTake( xSemaphore, 50 / portTICK_PERIOD_MS ) == pdTRUE )
    {

        gi_semaphoreTakeTask = 3;

        xReturned3 = xTaskCreate(task_3, "Task1", 4096, NULL, 5, &Task3);

        if (xReturned3 == pdPASS)
        { 
            ESP_LOGI(TAG, "Task 3 criada com sucesso.");
        }else{
            ESP_LOGW(TAG, "Task 3 não pode ser criada.");
        }
    }
    if(gi_semaphoreTakeTask == 3){
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
    ESP_LOGI(TAG, "Início do Exemplo");

    xSemaphore = xSemaphoreCreateBinary();
    if( xSemaphore != NULL ){
        ESP_LOGI(TAG, "Semáforo criado com sucesso.");
        xSemaphoreGive(xSemaphore);
    }

    loop();
       
}
