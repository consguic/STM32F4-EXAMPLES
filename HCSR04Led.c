/**10cm icerisinde herhangi bir cisim algiladiginda led yakan kucuk bir sensor uygulamasidir.
  *STM32F407DISC karti kullanilarak gerceklestirilmistir.
  *Pinler su sekilde baglanmistir.
  *PD8: ECHO
  *PD9: TRIG
  *PD10: Led
**/

#include "main.h"

//Uzaklik icin float tipinde bir degisken tanimlandi.
float distance= 0;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);

//Burada mikrosaniye cinsinden ayarlanabilen bir delay fonksiyonu tanimlandi.
void delayuS(uint16_t timed){
	uint32_t newTime = timed*24;
	while(newTime--);
}

//Bir deger dondurmesini istedigimiz icin float tipinde sensorden gelen veriyi okumak icin bir fonksiyon olusturduk.
float HCSRread(){
	//Sensorumuz gonderdigi sinyalin geri donusundeki sureye bagli olarak olcum yapıyor. Bu yuzden time ve tempDistance olarak bunları tutacak iki degisken tanimladik.
	uint32_t time = 0;
	float tempDistance = 0;
	//Ilk olarak Trig pinimizi resetledik
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, RESET);
	delayuS(10);
	//Trig pinini 10 mikro saniye aktif ettik.
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, SET);
	delayuS(10);
	//Trig pinimizi tekrar resetledik.
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, RESET);
	delayuS(10);

	//Echo pininden deger okumak istiyoruz eger deger donmuyorsa while dongusunde kalacak.
	while(!(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8)));
	//Echodan deger gelmeye baslayınca gelen degerleri okuyoruz.
	while(HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8)){
		time++;
		delayuS(1);
	}
	tempDistance=(float)time/58;
	if(tempDistance>400) tempDistance=400;
	if(tempDistance<2) tempDistance=2;
	delayuS(1000);
	return tempDistance;
}

int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  while (1)
  {
	//Sensorden gelen uzaklık verisini distance degiskenimize atadik.
	distance = HCSRread();

	//Eger 10 cm den yakın bir cisim algılandıysa ledimi
	if(distance<10){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, SET);
		delayuS(1000);
	}
	else {
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, RESET);
		delayuS(1000);
	}
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }


  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}


static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9|GPIO_PIN_10|LD4_Pin|LD3_Pin
                          |LD5_Pin|LD6_Pin, GPIO_PIN_RESET);


  /*Configure GPIO pin : PD8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|LD4_Pin|LD3_Pin
                          |LD5_Pin|LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  while (1)
  {
  }
}

