/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_hal.h"
#include "i2c.h"
#include "spi.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "lcd5110.h"

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* TESTING BLOCK BEGIN 0
 *
 * Block of testing methods for logical elements.
 * Correspondence between STM32F3Discovery pins and labels.
 * PC1:PC12 = O1:O12 -> GPIO Outputs
 * PD1:PD6 = I1:I6 -> GPIO I1s
 */

/*
 * Tester method for SN74HC157N multiplexer.
 * Checks whether the multiplexer operates correctly.
 *
 * Relation between STM32F3Discovery | Labels | Multiplexer pins:
 * O1 = Strobe; O2 = Select; O3 = A; O4 = B; I1 = Output
 *
 * Return value:
 * true - test is passed;
 * false - test is not passed.
 */
bool test_SN74HC157N(){

	bool test_passed = false;
	int test_count = 0;

	/* 1)
	 * Strobe = H
	 * Select = x
	 * A = x
	 * B = x
	 * Result = L
	 */
	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET); //Set Strobe = H
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0) {     // Check if Result = L
		test_count += 1; // test passed
	}

	/* 2)
	 * Strobe = L
	 * Select = L
	 * A = L
	 * B = x
	 * Result = L
	 */
	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET); // Set Strobe = L
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET); // Set Select = L
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET); // Set A      = L
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0) {   // Check if Result = L
			test_count += 1; // test passed
		}

	/* 3)
	 * Strobe = L
	 * Select = L
	 * A = H
	 * B = x
	 * Result = H
	 */
	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET); // Set Strobe = L
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET); // Set Select = L
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);   // Set A      = H
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1) {   // Check if Result = H
			test_count += 1; // test passed
		}

	/* 4)
	 * Strobe = L
	 * Select = H
	 * A = x
	 * B = L
	 * Result = L
	 */
	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET); // Set Strobe = L
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);   // Set Select = H
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET); // Set B      = L
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0) {   // Check if Result = L
			test_count += 1; // test passed
		}

	/* 5)
	 * Strobe = L
	 * Select = H
	 * A = x
	 * B = H
	 * Result = H
	 */
	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET); // Set Strobe = L
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);   // Set Select = H
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);	 // Set B      = H
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1) {	 // Check if Result = H
			test_count += 1; // test passed
		}

	if (test_count == 5) { // check if all tests passed
		test_passed = true;
	}

	return test_passed;
}

/*
 * Tester method for 74LS138.
 * Datasheet hhttp://www.electroschematics.com/wp-content/uploads/2013/07/74LS138-datasheet.pdf,
 * https://www.allsyllabus.com/aj/note/EEE/8086%20Microprocessor%20&%20Peripherals/unit%205/Interfacing%20Devices.php#.WSmM72iGOUk
 * Checks whether the element operates correctly.
 *
 * Relation between STM32F3Discovery labels | Element pins:
 * O1 = E1, O2 = E2, O3 = E3; O4 = A0, O5 = A1, O6 = A2;
 * I1:I8 = O0:O7
 *
 * Return value:
 * true - test is passed;
 * false - test is not passed.
 */
bool test_74LS138(){

	bool test_passed = false;
	int test_count = 0;

	/* 1)
	 * O1 = E1 = H
	 * I1:I8 = O0:O7 = H
	 */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 1 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 1 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 1 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 1) {
		test_count += 1;
	}

	/* 2)
	 * O2 = E2 = H
	 * I1:I8 = O0:O7 = H
	 */

	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 1 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 1 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 1 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 1) {
		test_count += 1;
	}

	/* 3)
	 * O3 = E3 = L
	 * I1:I8 = O0:O7 = H
	 */

	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 1 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 1 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 1 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 1) {
		test_count += 1;
	}

	/* 4)
	 * O1 = E1 = L
	 * O2 = E2 = L
	 * O3 = E3 = H
	 * O4 = A0 = L
	 * O5 = A1 = L
	 * O6 = A1 = L
	 *
	 * I1 = O0 = L
	 * I2 = O1 = H
	 * I3 = O2 = H
	 * I4 = O3 = H
	 * I5 = O4 = H
	 * I6 = O5 = H
	 * I7 = O6 = H
	 * I8 = O7 = H
	 */

	HAL_GPIO_WritePin(O1_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 1 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 1 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 1 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 1) {
		test_count += 1;
	}

	/* 5)
	 * O1 = E1 = L
	 * O2 = E2 = L
	 * O3 = E3 = H
	 * O4 = A0 = H
	 * O5 = A1 = L
	 * O6 = A1 = L
	 *
	 * I1 = O0 = H
	 * I2 = O1 = L
	 * I3 = O2 = H
	 * I4 = O3 = H
	 * I5 = O4 = H
	 * I6 = O5 = H
	 * I7 = O6 = H
	 * I8 = O7 = H
	 */

	HAL_GPIO_WritePin(O1_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 1 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 1 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 1 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 1) {
		test_count += 1;
	}

	/* 6)
	 * O1 = E1 = L
	 * O2 = E2 = L
	 * O3 = E3 = H
	 * O4 = A0 = L
	 * O5 = A1 = H
	 * O6 = A1 = L
	 *
	 * I1 = O0 = H
	 * I2 = O1 = H
	 * I3 = O2 = L
	 * I4 = O3 = H
	 * I5 = O4 = H
	 * I6 = O5 = H
	 * I7 = O6 = H
	 * I8 = O7 = H
	 */

	HAL_GPIO_WritePin(O1_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 1 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 1 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 1 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 1) {
		test_count += 1;
	}

	/* 7)
	 * O1 = E1 = L
	 * O2 = E2 = L
	 * O3 = E3 = H
	 * O4 = A0 = H
	 * O5 = A1 = H
	 * O6 = A1 = L
	 *
	 * I1 = O0 = H
	 * I2 = O1 = H
	 * I3 = O2 = H
	 * I4 = O3 = L
	 * I5 = O4 = H
	 * I6 = O5 = H
	 * I7 = O6 = H
	 * I8 = O7 = H
	 */

	HAL_GPIO_WritePin(O1_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 0 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 1 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 1 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 1 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 1) {
		test_count += 1;
	}

	/* 8)
	 * O1 = E1 = L
	 * O2 = E2 = L
	 * O3 = E3 = H
	 * O4 = A0 = L
	 * O5 = A1 = L
	 * O6 = A1 = H
	 *
	 * I1 = O0 = H
	 * I2 = O1 = H
	 * I3 = O2 = H
	 * I4 = O3 = H
	 * I5 = O4 = L
	 * I6 = O5 = H
	 * I7 = O6 = H
	 * I8 = O7 = H
	 */

	HAL_GPIO_WritePin(O1_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 0 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 1 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 1 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 1) {
		test_count += 1;
	}

	/* 9)
	 * O1 = E1 = L
	 * O2 = E2 = L
	 * O3 = E3 = H
	 * O4 = A0 = H
	 * O5 = A1 = L
	 * O6 = A1 = H
	 *
	 * I1 = O0 = H
	 * I2 = O1 = H
	 * I3 = O2 = H
	 * I4 = O3 = H
	 * I5 = O4 = H
	 * I6 = O5 = L
	 * I7 = O6 = H
	 * I8 = O7 = H
	 */

	HAL_GPIO_WritePin(O1_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 1 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 0 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 1 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 1) {
		test_count += 1;
	}

	/* 10)
	 * O1 = E1 = L
	 * O2 = E2 = L
	 * O3 = E3 = H
	 * O4 = A0 = L
	 * O5 = A1 = H
	 * O6 = A1 = H
	 *
	 * I1 = O0 = H
	 * I2 = O1 = H
	 * I3 = O2 = H
	 * I4 = O3 = H
	 * I5 = O4 = H
	 * I6 = O5 = H
	 * I7 = O6 = L
	 * I8 = O7 = H
	 */

	HAL_GPIO_WritePin(O1_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 1 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 1 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 0 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 1) {
		test_count += 1;
	}

	/* 11)
	 * O1 = E1 = L
	 * O2 = E2 = L
	 * O3 = E3 = H
	 * O4 = A0 = H
	 * O5 = A1 = H
	 * O6 = A1 = H
	 *
	 * I1 = O0 = H
	 * I2 = O1 = H
	 * I3 = O2 = H
	 * I4 = O3 = H
	 * I5 = O4 = H
	 * I6 = O5 = H
	 * I7 = O6 = H
	 * I8 = O7 = L
	 */

	HAL_GPIO_WritePin(O1_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_SET);
	if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
			HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
			HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
			HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1 &&
			HAL_GPIO_ReadPin(I5_GPIO_Port, I5_Pin) == 1 &&
			HAL_GPIO_ReadPin(I6_GPIO_Port, I6_Pin) == 1 &&
			HAL_GPIO_ReadPin(I7_GPIO_Port, I7_Pin) == 1 &&
			HAL_GPIO_ReadPin(I8_GPIO_Port, I8_Pin) == 0) {
		test_count += 1;
	}

	if (test_count == 11) { // check if all tests passed
		test_passed = true;
	}

	return test_passed;

}

/*
 * Tester method for 74HC147.
 * Datasheet http://pdf1.alldatasheet.com/datasheet-pdf/view/114731/PHILIPS/74HC147.html
 * Tests according to the function table. See the datasheet.
 * Checks whether the element operates correctly.
 *
 * Relation between STM32F3Discovery labels | Element pins:
 * O1 = A0; O2 = A1; O3 = A2; O4 = A3; 05 = A4; O6 = A5; O7 = A6; O8 = A7; O9 = A8;
 * I1 = Y0; I2 = Y1; I3 = Y2; I4 = Y3
 *
 * Return value:
 * true - test is passed;
 * false - test is not passed.
 */
bool test_74HC147(){

	bool test_passed = false;
	int test_count = 0;

	/* 1) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
				HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1) {
			test_count += 1;
		}

	/* 2) */

	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
				HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 0) {
			test_count += 1;
		}

	/* 3) */

	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1 &&
				HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 0) {
			test_count += 1;
		}

	/* 4) */

	HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0 &&
				HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1) {
			test_count += 1;
		}

	/* 5) */

	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0 &&
				HAL_GPIO_ReadPin(I4_GPIO_Port, I4_Pin) == 1) {
			test_count += 1;
		}

	/* 6) */

	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 7) */

	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 8) */

	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}


	/* 9) */
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 10) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	if (test_count == 10) { // check if all tests passed
		test_passed = true;
	}

	return test_passed;

}

/*
 * Tester method for SN74HC151.
 * Datasheet http://www.ti.com/lit/ds/symlink/sn74hc151.pdf
 * Tests according to the function table. See the datasheet.
 * Checks whether the element operates correctly.
 *
 * Relation between STM32F3Discovery labels | Multiplexer pins:
 * O1 = Strobe !G; O2 = A; O3 = B; O4 = C; O5:012 = D0:D7
 * I1 = Y; I2 = W
 *
 * Return value:
 * true - test is passed;
 * false - test is not passed.
 */
bool test_SN74HC151(){

	bool test_passed = false;
	int test_count = 0;

	/*
	* Pre configuration.
	* Set O5:O12 (D0:D7 on chip) to 0.
	*/
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O6_GPIO_Port, O6_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O7_GPIO_Port, O7_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O8_GPIO_Port, O8_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O9_GPIO_Port, O9_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O10_GPIO_Port, O10_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O11_GPIO_Port, O11_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O12_GPIO_Port, O12_Pin, GPIO_PIN_RESET);

	/* 1) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET); //Strobe = H
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 2) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET); //Strobe = L
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET); //A = L
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET); //B = L
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET); //C = L
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 3) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 4) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 5) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 6) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 7) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 8) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	/* 9) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1) {
			test_count += 1;
		}

	if (test_count == 9) { // check if all tests passed
		test_passed = true;
	}

	return test_passed;

}

/*
 * Tester method for DM74LS281.
 * Datasheet http://pdf.datasheetcatalog.com/datasheets/70/248191_DS.pdf,
 * Tests according to the function table. See the datasheet.
 * Checks whether the element operates correctly.
 *
 * Relation between STM32F3Discovery labels | Element pins:
 * O1 = A1; O2 = B1; O3 = A2; O4 = B2; O5 = C0.
 * I1 = sum1; I2 = sum2; I3 = C2.
 *
 * Return value:
 * true - test is passed;
 * false - test is not passed.
 */
bool test_DM74LS281(){

	bool test_passed = false;
	int test_count = 0;

	/* 1) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET); //A1 = L
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET); //B1 = L
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET); //A2 = L
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET); //B2 = L
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET); // C0 = L
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 && //sum1 = 0
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 && //sum 2 = 0
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0) { //C2 = 0
			test_count += 1;
		}

	/* 2) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET); //A1 = H
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET); //B1 = L
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET); //A2 = L
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET); //B2 = L
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET); //C0 = L
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 && //sum1 = H
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 && //sum2 = L
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0) { //C2 = L
			test_count += 1;
			}

	/* 3) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0) {
			test_count += 1;
			}

	/* 4) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0) {
			test_count += 1;
			}

	/* 5) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0) {
			test_count += 1;
			}

	/* 6) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0) {
			test_count += 1;
			}

	/* 7) */
	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0) {
			test_count += 1;
			}

	/* 8) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1) {
			test_count += 1;
			}

	/* 9) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0) {
			test_count += 1;
			}

	/* 10) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0) {
			test_count += 1;
			}

	/* 11) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 0) {
			test_count += 1;
			}

	/* 12) */
	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1) {
			test_count += 1;
			}

	/* 13) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1) {
			test_count += 1;
			}

	/* 14) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1) {
			test_count += 1;
			}

	/* 15) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 1 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 0 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1) {
			test_count += 1;
			}

	/* 16) */

	HAL_GPIO_WritePin(O1_GPIO_Port, O1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O2_GPIO_Port, O2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O3_GPIO_Port, O3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O4_GPIO_Port, O4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(O5_GPIO_Port, O5_Pin, GPIO_PIN_RESET);
		if (HAL_GPIO_ReadPin(I1_GPIO_Port, I1_Pin) == 0 &&
				HAL_GPIO_ReadPin(I2_GPIO_Port, I2_Pin) == 1 &&
				HAL_GPIO_ReadPin(I3_GPIO_Port, I3_Pin) == 1) {
			test_count += 1;
			}

	if (test_count == 16) { // check if all tests passed
		test_passed = true;
	}

	return test_passed;

}

bool draft_test() {
	bool test_passed = false;
	int test_count = 5;

	if (test_count == 5) {
		test_passed = true;
	}
	return test_passed;
}

/* TESTING BLOCK END 0 */

void wait_for_button1_pressed()
{
	while( HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_RESET ){}
	HAL_Delay(50);
	while( HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_SET ){}
	HAL_Delay(50);
}


bool pressed_and_held() {
	bool press_and_hold = false;
    while(!press_and_hold) {
	  if(HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_SET) {
		 HAL_Delay(3000);
		  if(HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_SET) {
			  press_and_hold = true;
			  }
		  }
	  }
    return press_and_hold;
}

bool button_pressed() {
	bool pressed = false;
	while(!pressed) {
		if(HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_SET) {
			pressed = true;
		}
	}
	return pressed;
}

LCD5110_display lcd1;

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_I2C2_Init();

  /* USER CODE BEGIN 2 */
  lcd1.hw_conf.spi_handle = &hspi2;
  lcd1.hw_conf.spi_cs_pin =  LCD1_CS_Pin;
  lcd1.hw_conf.spi_cs_port = LCD1_CS_GPIO_Port;
  lcd1.hw_conf.rst_pin =  LCD1_RST_Pin;
  lcd1.hw_conf.rst_port = LCD1_RST_GPIO_Port;
  lcd1.hw_conf.dc_pin =  LCD1_DC_Pin;
  lcd1.hw_conf.dc_port = LCD1_DC_GPIO_Port;
  lcd1.def_scr = lcd5110_def_scr;
  LCD5110_init(&lcd1.hw_conf, LCD5110_NORMAL_MODE, 0x40, 2, 3);

  /* INTERFACE CODE BEGIN 1 */

  LCD5110_clear_scr(&lcd1);

  LCD5110_set_cursor(20, 20, &lcd1);
  LCD5110_print("Welcome!\n", BLACK, &lcd1);
  HAL_Delay(3000);
  LCD5110_clear_scr(&lcd1);

  LCD5110_set_cursor(19, 5, &lcd1);
  LCD5110_print("Hold the\n", BLACK, &lcd1);
  LCD5110_set_cursor(0, 15, &lcd1);
  LCD5110_print("button pressed\n", BLACK, &lcd1);
  LCD5110_set_cursor(21, 25, &lcd1);
  LCD5110_print("for 3s\n", BLACK, &lcd1);
  LCD5110_set_cursor(18, 35, &lcd1);
  LCD5110_print("to choose\n", BLACK, &lcd1);
  HAL_Delay(3500);
  LCD5110_clear_scr(&lcd1);

  int count = 0;
  /*
   * Display logical elements on the display
   * and let the user choose which one to test.
   */
  while(1) {
	  LCD5110_set_cursor(13, 20, &lcd1);
	  LCD5110_print("SN74HC157N\n", BLACK, &lcd1);
	  HAL_Delay(2500);
	  LCD5110_clear_scr(&lcd1);
	  count += 1;

	  if(HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_SET) {
		  LCD5110_set_cursor(20, 15, &lcd1);
		  LCD5110_print("Chosen:\n", BLACK, &lcd1);
		  LCD5110_set_cursor(13, 25, &lcd1);
		  LCD5110_print("SN74HC157N\n", BLACK, &lcd1);
		  HAL_Delay(2500);
		  break;
	  }

	  LCD5110_set_cursor(20, 20, &lcd1);
	  LCD5110_print("74LS138\n", BLACK, &lcd1);
	  HAL_Delay(2500);
	  LCD5110_clear_scr(&lcd1);
	  count += 1;

	  if(HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_SET) {
		  LCD5110_set_cursor(20, 15, &lcd1);
		  LCD5110_print("Chosen:\n", BLACK, &lcd1);
		  LCD5110_set_cursor(20, 25, &lcd1);
		  LCD5110_print("74LS138\n", BLACK, &lcd1);
		  HAL_Delay(2500);
		  break;
	  }

	  LCD5110_set_cursor(20, 20, &lcd1);
	  LCD5110_print("74HC147\n", BLACK, &lcd1);
	  HAL_Delay(2500);
	  LCD5110_clear_scr(&lcd1);
	  count += 1;

	  if(HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_SET) {
		  LCD5110_set_cursor(20, 15, &lcd1);
		  LCD5110_print("Chosen:\n", BLACK, &lcd1);
		  LCD5110_set_cursor(20, 25, &lcd1);
		  LCD5110_print("74HC147\n", BLACK, &lcd1);
		  HAL_Delay(2500);
		  break;
	  }

	  LCD5110_set_cursor(16, 20, &lcd1);
	  LCD5110_print("SN74HC151\n", BLACK, &lcd1);
	  HAL_Delay(2500);
	  LCD5110_clear_scr(&lcd1);
	  count += 1;

	  if(HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_SET) {
		  LCD5110_set_cursor(20, 15, &lcd1);
		  LCD5110_print("Chosen:\n", BLACK, &lcd1);
		  LCD5110_set_cursor(16, 25, &lcd1);
		  LCD5110_print("SN74HC151\n", BLACK, &lcd1);
		  HAL_Delay(2500);
		  break;
	  }

	  LCD5110_set_cursor(17, 20, &lcd1);
	  LCD5110_print("DM74LS281\n", BLACK, &lcd1);
	  HAL_Delay(2500);
	  LCD5110_clear_scr(&lcd1);
	  count += 1;

	  if(HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_SET) {
		  LCD5110_set_cursor(20, 15, &lcd1);
		  LCD5110_print("Chosen:\n", BLACK, &lcd1);
		  LCD5110_set_cursor(17, 25, &lcd1);
		  LCD5110_print("DM74LS281\n", BLACK, &lcd1);
		  HAL_Delay(2500);
		  break;
	  }

	  LCD5110_set_cursor(22, 20, &lcd1);
	  LCD5110_print("sample\n", BLACK, &lcd1);
	  HAL_Delay(2500);
	  LCD5110_clear_scr(&lcd1);
	  count += 1;

	  if(HAL_GPIO_ReadPin(UButton_GPIO_Port, UButton_Pin) == GPIO_PIN_SET) {
		  LCD5110_set_cursor(20, 15, &lcd1);
		  LCD5110_print("Chosen:\n", BLACK, &lcd1);
		  LCD5110_set_cursor(22, 25, &lcd1);
		  LCD5110_print("sample\n", BLACK, &lcd1);
		  HAL_Delay(2500);
		  break;
	  }
	  count = 0;
  }

  /* INTERFACE CODE END 1 */

  LCD5110_clear_scr(&lcd1);
  LCD5110_set_cursor(0, 0, &lcd1);
  LCD5110_print("Testing...\n", BLACK, &lcd1);
  HAL_Delay(1000);

  // Check which logical element was chosen and perform a test.
  if(count == 1) {
	  if(test_SN74HC157N()) {
		  LCD5110_print("Passed\n", BLACK, &lcd1);
	  }
	  else {
		  LCD5110_print("NOT passed\n", BLACK, &lcd1);
	  }
  }
  if(count == 2) {
	  if(test_74LS138()) {
		  LCD5110_print("Passed\n", BLACK, &lcd1);
	  }
	  else {
		  LCD5110_print("NOT passed\n", BLACK, &lcd1);
	  }
  }
  if(count == 3) {
	  if(test_74HC147()) {
		  LCD5110_print("Passed\n", BLACK, &lcd1);
	  }
	  else {
		  LCD5110_print("NOT passed\n", BLACK, &lcd1);
	  }

  }
  if(count == 4) {
	  if(test_74LS138()) {
		  LCD5110_print("Passed\n", BLACK, &lcd1);
	  }
	  else {
		  LCD5110_print("NOT passed\n", BLACK, &lcd1);
	  }

  }
  if(count == 5) {
	  if(test_DM74LS281()) {
		  LCD5110_print("Passed\n", BLACK, &lcd1);
	  }
	  else {
		  LCD5110_print("NOT passed\n", BLACK, &lcd1);
	  }
  }
  if(count == 6) {
	  if(draft_test()) {
		  LCD5110_print("Passed\n", BLACK, &lcd1);
	  }
	  else {
		  LCD5110_print("NOT passed\n", BLACK, &lcd1);
	  }
  }
  LCD5110_print("Author:\n Karabyn\n", BLACK, &lcd1);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
  PeriphClkInit.I2c2ClockSelection = RCC_I2C2CLKSOURCE_HSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
