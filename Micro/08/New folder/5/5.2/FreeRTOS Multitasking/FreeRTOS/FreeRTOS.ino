#define LED1 2
#define LED2 4

void blinkLED1(void *pvParam) {
  pinMode(LED1, OUTPUT);
  while (1) {
    digitalWrite(LED1, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(LED1, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}

void blinkLED2(void *pvParam) {
  pinMode(LED2, OUTPUT);
  while (1) {
    digitalWrite(LED2, HIGH);
    vTaskDelay(300 / portTICK_PERIOD_MS);
    digitalWrite(LED2, LOW);
    vTaskDelay(300 / portTICK_PERIOD_MS);
  }
}

void setup() {
  // pinToCore: 0 = Core0, 1 = Core1
  xTaskCreatePinnedToCore(blinkLED1, "LED1Task", 1000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(blinkLED2, "LED2Task", 1000, NULL, 1, NULL, 1);
}

void loop() {
  // FreeRTOS จัดการให้ทั้งหมด
}