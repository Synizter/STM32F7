'''
@Author : Goragod
@Version : 1.1 revision
@Nrief : This file generate a initial value
		 for struct in c for PLL configuration.
		 A result of generation will be save on
		 'map.h' for further usage
To start using this file, please declare an object 'processor_clk_gen'
with your crystal speed (eq, processor_clk_gen(16) for HSI with 16MHz)
'''
#range 16 - 216MHz
from __future__ import division

class processor_clk_gen:
	__fclk = 0
	__pll_m = 0
	__pll_n = 0
	__pll_p = 0

	__pll_state = 'DISABLE'

	def __init__(self, f):
		self.__fclk = f * 1e6
		self.__pll_m = [val for val in range(2, 64)]
		self.__pll_n = [val for val in range(50, 433)]
		self.__pll_p = [val for val in range(2, 10, 2)]

	
	def config_pll_output(self, target_freq):
		clk_param = { 'PLL M' : 0,
			 		  'PLL N' : 0,
			 		  'PLL P' : 0,
			 		  'Frequency' : 0 }
		if self.__pll_state == 'ENABLE':
			for m in self.__pll_m:
				if self.__fclk / m >= 0.95e6 and self.__fclk / m <= 2.1e6: 
					
					for n in self.__pll_n:
						if n * self.__fclk / m >= 192e6 and n * self.__fclk / m <= 432e6:
							
							for p in self.__pll_p:
								if n * self.__fclk / (m * p) >= 24e6 and n * self.__fclk / (m * p) <= 216e6:
									if self.__fclk * (n / (m * p)) == target_freq:
										clk_param['Frequency'] = self.__fclk * (n / (m * p))
										clk_param['PLL M'] = m
										clk_param['PLL N'] = n
										clk_param['PLL P'] = p
										return clk_param
		else:
			raise ValueError('Phase lock loop must me enable!')	
	
	def hse_lowest_clk(self):
		clk_param = { 'PLL M' : 'disble',
			 		  	  'PLL N' : 'disble',
			 		      'PLL P' : 'disble',
			 		      'Frequency' : 16 }
		return clk_param
	def pll_state(self, state):
		self.__pll_state = state

	def get_pll_state(self):
		return self.__pll_state

					

if __name__ == '__main__':

	#write constant function
	map_file = open("map.h", "wr+")
	var_type = "PLLParam_TypeDef "
	var_name = "ClockRateScale_Low[]"
	clk_src = "LL_RCC_PLLSOURCE_HSE"
	div_pllm = "LL_RCC_PLLM_DIV_"
	div_pllp = "LL_RCC_PLLP_DIV_"



	#Create Array of Mid Frequency Range
	arg_ = ""
	map_file.write("const " + var_type + var_name + " = { \r\n")
	
	for i in range(60, 145):
		stm32f7_clk = processor_clk_gen(25)
		stm32f7_clk.pll_state('ENABLE')
		pll_param = stm32f7_clk.config_pll_output(i * 1e6)
		clk_src = "LL_RCC_PLLSOURCE_HSE"

		if(pll_param == None):
			stm32f7_clk = processor_clk_gen(16)
			stm32f7_clk.pll_state('ENABLE')
			pll_param = stm32f7_clk.config_pll_output(i * 1e6)
			clk_src = "LL_RCC_PLLSOURCE_HSI"
			

		print "writing f parameter at : {}".format(i)
		print pll_param

		arg_ = ("{" + clk_src + ", " 
			+ (div_pllm + str(pll_param['PLL M'])) + ", " 
			+ str(pll_param['PLL N']) + ", " 
			+ (div_pllp + str(pll_param['PLL P']))+ ("},"))


		if i == 144:
			arg_ = ("{" + clk_src + ", " 
			+ (div_pllm + str(pll_param['PLL M'])) + ", " 
			+ str(pll_param['PLL N']) + ", " 
			+ (div_pllp + str(pll_param['PLL P']))+ ("}"))
		map_file.write(arg_ + "\r\n")
	map_file.write("};\r\n")

	#Create Array of HIGH Frequency Range
	var_name = "ClockRateScale_High[]"	
	arg_ = ""
	map_file.write("const " + var_type + var_name + " = { \r\n")
	
	for i in range(180, 217):
		stm32f7_clk = processor_clk_gen(25)
		stm32f7_clk.pll_state('ENABLE')
		pll_param = stm32f7_clk.config_pll_output(i * 1e6)
		clk_src = "LL_RCC_PLLSOURCE_HSE"

		if(pll_param == None):
			stm32f7_clk = processor_clk_gen(16)
			stm32f7_clk.pll_state('ENABLE')
			pll_param = stm32f7_clk.config_pll_output(i * 1e6)
			clk_src = "LL_RCC_PLLSOURCE_HSI"
			
		print "writing f parameter at : {}".format(i)
		print pll_param

		arg_ = ("{" + clk_src + ", " 
			+ (div_pllm + str(pll_param['PLL M'])) + ", " 
			+ str(pll_param['PLL N']) + ", " 
			+ (div_pllp + str(pll_param['PLL P']))+ ("},"))

		if i == 216:
			arg_ = ("{" + clk_src + ", " 
			+ (div_pllm + str(pll_param['PLL M'])) + ", " 
			+ str(pll_param['PLL N']) + ", " 
			+ (div_pllp + str(pll_param['PLL P']))+ ("}"))
		map_file.write(arg_ + "\r\n")
	map_file.write("};\r\n")

	#Create Array of LOW Frequency Range
	var_name = "ClockRateScale_Medium[]"
	arg_ = ""
	map_file.write("const " + var_type + var_name + " = { \r\n")
	pll_param = stm32f7_clk.config_pll_output(168 * 1e6)
	arg_ = ("{" + clk_src + ", " 
			+ (div_pllm + str(pll_param['PLL M'])) + ", " 
			+ str(pll_param['PLL N']) + ", " 
			+ (div_pllp + str(pll_param['PLL P']))+ ("},"))
	map_file.write(arg_ + "\r\n")
	map_file.write("};\r\n")	

	map_file.close()
