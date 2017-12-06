from __future__ import division
import numpy as np

#scale 3 OVerdrive
I_216_200 = [92,92.52,91.25,90.87,90.5,90.12,89.75,89.37,89,88.62,88.25,87.87,87.5,87.12,86.75,86.37,86]
I_199_190 = [85.65,85.3,84.95,84.6,84.25,83.9,83.55,83.2,82.85,82.5]
I_189_181 = [82.15,81.8,81.45,81.1,80.75,80.4,80.05,79.7,79.35]
#scale 3
I_180 = 79
# #scale 2
I_168 = 71
# #scale 1
I_144_140 = [60,59.67,59.33,59,58.67]
I_139_130 = [58.33,58,57.67,57.33,57,56.67,56.33,56,55.67,55.33]
I_129_120 = [55,54.67,54.33,54,53.67,53.33,53,52.67,52.33,52]
I_119_110 = [51.67,51.33,51,50.67,50.33,50,49.67,49.33,49,48.67]
I_109_100 = [48.33,48,47.67,47.33,47,46.67,46.33,46,45.67,45.33]
I_99_90 = [45,44.67,44.33,44,43.67,43.33,43,42.67,42.33,42]
I_89_80 = [41.67,41.33,41,40.67,40.33,40,39.67,39.33,39,38.67]
I_79_70 = [38.33,38,37.67,37.33,37,36.67,36.33,36,35.67,35.33]
I_69_60 = [35,34.67,34.33,34,33.67,33.33,33,32.67,32.33,32]
# #HSI no PLL
I_25 = 16

#scale 3 OD 
v = 3.6;
I = I_216_200 + I_199_190 + I_189_181
F = [x for x in range(216,180, -1)]
C_scale3_OD = [i/(f * v) for i,f in zip(I,F)]
print "Switching Capacitance for Scale 3 OD Mode : " + str(np.mean(C_scale3_OD))

#scale 3  
v = 1.4;
I = I_180
F = 180
C_scale3 = I/(F * v)
print "Switching Capacitance for Scale 3 Mode : " + str(np.mean(C_scale3))

#scale 2  
v = 1.32;
I = I_168
F = 168
C_scale2 = I/(F * v)
print "Switching Capacitance for Scale 2 Mode : " + str(np.mean(C_scale2))

#scale 1 
v = 1.14;
I = I_144_140 + I_139_130 + I_129_120 + I_119_110 + I_109_100 + I_99_90 + I_89_80 + I_79_70 + I_69_60
F = [x for x in range(144,59, -1)]
C_scale1 = [i/(f * v) for i,f in zip(I,F)]
print "Switching Capacitance for Scale 1 Mode : " + str(np.mean(C_scale1))

#scale 1 NO PLL  
v = 1.14;
I = I_25
F = 25
C_scale1_NOPLL = I/(F * v)
print "Switching Capacitance for Scale 1 NO PLL Mode : " + str(np.mean(C_scale1_NOPLL))

print "Avg of C : " + str((np.mean(C_scale3_OD) + C_scale3 + C_scale2 + np.mean(C_scale1) + C_scale1_NOPLL)/5)
