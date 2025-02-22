import math

NUM_SERVER_C0_1 = 1
NUM_SERVER_C1_1 = 5
NUM_SERVER_C2_1 = 13
NUM_SERVER_C3_1 = 1
NUM_SERVER_C4_1 = 6
NUM_SERVER_C5_1 = 9
NUM_SERVER_C6_1 = 1

NUM_SERVER_C0_2 = 1
NUM_SERVER_C1_2 = 11
NUM_SERVER_C2_2 = 26
NUM_SERVER_C3_2 = 1
NUM_SERVER_C4_2 = 13
NUM_SERVER_C5_2 = 19
NUM_SERVER_C6_2 = 1

NUM_SERVER_C0_3 = 1
NUM_SERVER_C1_3 = 2
NUM_SERVER_C2_3 = 3
NUM_SERVER_C3_3 = 1
NUM_SERVER_C4_3 = 2
NUM_SERVER_C5_3 = 2
NUM_SERVER_C6_3 = 1

LAMBDA1 = 0.5875
LAMBDA2 = 1.273056
LAMBDA3 = 0.09778

AVG_SERVICE_C0 = 30.0
AVG_SERVICE_C1 = 90.0
AVG_SERVICE_C2 = 20.0
AVG_SERVICE_C3 = 20.0
AVG_SERVICE_C4 = 40.0
AVG_SERVICE_C5 = 15.0
AVG_SERVICE_C6 = 15.0

P_C0  = 0.02
P_C1  = 0.08
P_A2  = 0.9
P_C2  = 0.99
P_C3  = 0.01
P_SB0 = 0.01
P_SB1 = 0.15
P_SB2 = 0.05
P_SB3 = 0.05
P_PD0 = 0.65
P_CP4 = 0.05
P_PC  = 0.25
P_SB5 = 0.0025

p0_0 = LAMBDA1*P_C0*AVG_SERVICE_C0
p1_0 = LAMBDA1*P_C1*AVG_SERVICE_C1/NUM_SERVER_C1_1
p2_0 = ((LAMBDA1*P_A2) + (p1_0*(1-P_SB1)*(NUM_SERVER_C1_1/AVG_SERVICE_C1)))*P_C2*AVG_SERVICE_C2/NUM_SERVER_C2_1
p3_0 = ((LAMBDA1*P_A2) + (p1_0*(1-P_SB1)*(NUM_SERVER_C1_1/AVG_SERVICE_C1)))*P_C3*AVG_SERVICE_C3
lambda4_0 = ((p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)) + (p0_0*(1/AVG_SERVICE_C0)*(1-P_SB0))*(1-P_PD0)
p5_0 = ((lambda4_0*P_PC*(1-P_CP4)) + (lambda4_0*(1-P_PC)))*(AVG_SERVICE_C5/NUM_SERVER_C5_1)
p6_0 = (((p3_0/AVG_SERVICE_C3)*(1-P_SB3)) + ((p0_0*(1/AVG_SERVICE_C0))*(1-P_SB0)*(P_PD0)))*AVG_SERVICE_C6

p0_1 = LAMBDA2*P_C0*AVG_SERVICE_C0
p1_1 = LAMBDA2*P_C1*AVG_SERVICE_C1/NUM_SERVER_C1_2
p2_1 = ((LAMBDA2*P_A2) + (p1_1*(1-P_SB1)*(NUM_SERVER_C1_2/AVG_SERVICE_C1)))*P_C2*AVG_SERVICE_C2/NUM_SERVER_C2_2
p3_1 = ((LAMBDA2*P_A2) + (p1_1*(1-P_SB1)*(NUM_SERVER_C1_2/AVG_SERVICE_C1)))*P_C3*AVG_SERVICE_C3
lambda4_1 = ((p2_1*(NUM_SERVER_C2_2/AVG_SERVICE_C2))*(1-P_SB2)) + (p0_1*(1/AVG_SERVICE_C0)*(1-P_SB0))*(1-P_PD0)
p5_1 = ((lambda4_1*P_PC*(1-P_CP4)) + (lambda4_1*(1-P_PC)))*(AVG_SERVICE_C5/NUM_SERVER_C5_2)
p6_1 = (((p3_1/AVG_SERVICE_C3)*(1-P_SB3)) + ((p0_1*(1/AVG_SERVICE_C0))*(1-P_SB0)*(P_PD0)))*AVG_SERVICE_C6

p0_2 = LAMBDA3*P_C0*AVG_SERVICE_C0
p1_2 = LAMBDA3*P_C1*AVG_SERVICE_C1/NUM_SERVER_C1_3
p2_2 = ((LAMBDA3*P_A2) + (p1_2*(1-P_SB1)*(NUM_SERVER_C1_3/AVG_SERVICE_C1)))*P_C2*AVG_SERVICE_C2/NUM_SERVER_C2_3
p3_2 = ((LAMBDA3*P_A2) + (p1_2*(1-P_SB1)*(NUM_SERVER_C1_3/AVG_SERVICE_C1)))*P_C3*AVG_SERVICE_C3
lambda4_2 = ((p2_2*(NUM_SERVER_C2_3/AVG_SERVICE_C2))*(1-P_SB2)) + (p0_2*(1/AVG_SERVICE_C0)*(1-P_SB0))*(1-P_PD0)
p5_2 = ((lambda4_2*P_PC*(1-P_CP4)) + (lambda4_2*(1-P_PC)))*(AVG_SERVICE_C5/NUM_SERVER_C5_3)
p6_2 = (((p3_2/AVG_SERVICE_C3)*(1-P_SB3)) + ((p0_2*(1/AVG_SERVICE_C0))*(1-P_SB0)*(P_PD0)))*AVG_SERVICE_C6


def markov(lambda4, m):
	den = 0
	for j in range (0,m+1):
		pot = math.pow((lambda4*AVG_SERVICE_C4),j)
		inv_fac = 1/math.factorial(j)
		den = den + (pot*inv_fac)
	p_loss = (math.pow((lambda4*AVG_SERVICE_C4), m)/(math.factorial(m)))/(den)
	print(" • p_loss =", p_loss)

print("\n I valori riportati fanno riferimento al centro 4 (Police Control):")

print("\n CONFIGURAZIONE 1\n • lambda =", lambda4_0)
markov(lambda4_0, NUM_SERVER_C4_1)

print("\n CONFIGURAZIONE 2\n • lambda =", lambda4_1)
markov(lambda4_1,NUM_SERVER_C4_2)

print("\n CONFIGURAZIONE 3\n • lambda =", lambda4_2)
markov(lambda4_2,NUM_SERVER_C4_3)

print("")
