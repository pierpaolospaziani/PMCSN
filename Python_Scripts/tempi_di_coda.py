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

def tempi_coda(centro,m,p,e_s):
	e_s = e_s/m
	p_0 = math.pow((m*p),m)/(math.factorial(m)*(1-p))
	p_q = p_0
	for i in range (0,m):
		pot = math.pow((m*p),i)
		fac = math.factorial(i)
		p_0 = p_0 + (pot/fac)
	p_q = p_q/p_0
	e_t_q = (p_q*e_s)/(1-p)
	print(" CENTRO",centro,": E(T_Q)  =",e_t_q)

def r_tempi_coda(centro,m,p,e_s):
	e_s = e_s/m
	p_0 = math.pow((m*p),m)/(math.factorial(m)*(1-p))
	p_q = p_0
	for i in range (0,m):
		pot = math.pow((m*p),i)
		fac = math.factorial(i)
		p_0 = p_0 + (pot/fac)
	p_q = p_q/p_0
	e_t_q = (p_q*e_s)/(1-p)
	return(e_t_q)

def multi_tempi_coda(centro,m,p,e_s):
	e_s = e_s/m
	p_0 = math.pow((m*p),m)/(math.factorial(m)*(1-p))
	p_q = p_0
	for i in range (0,m):
		pot = math.pow((m*p),i)
		fac = math.factorial(i)
		p_0 = p_0 + (pot/fac)
	p_q = p_q/p_0
	e_t_q1 = (p_q*e_s)/(1-(p*P_C3))
	e_t_q2 = (p_q*e_s)/((1-p)*(1-(p*P_C3)))
	print(" CENTRO",centro,": E(T_Q)1 =",e_t_q1)
	print("            E(T_Q)2 =",e_t_q2)

def r_multi_tempi_coda(centro,m,p,e_s):
	e_s = e_s/m
	p_0 = math.pow((m*p),m)/(math.factorial(m)*(1-p))
	p_q = p_0
	for i in range (0,m):
		pot = math.pow((m*p),i)
		fac = math.factorial(i)
		p_0 = p_0 + (pot/fac)
	p_q = p_q/p_0
	e_t_q1 = (p_q*e_s)/(1-(p*P_C3))
	e_t_q2 = (p_q*e_s)/((1-p)*(1-(p*P_C3)))
	return e_t_q1,e_t_q2

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

avgs = AVG_SERVICE_C0+AVG_SERVICE_C1+AVG_SERVICE_C2+AVG_SERVICE_C3+AVG_SERVICE_C4+AVG_SERVICE_C5+AVG_SERVICE_C6

print("\n CONFIGURAZIONE 1:")
tempi_coda(0, NUM_SERVER_C0_1, p0_0, AVG_SERVICE_C0)
tempi_coda(1, NUM_SERVER_C1_1, p1_0, AVG_SERVICE_C1)
tempi_coda(2, NUM_SERVER_C2_1, p2_0, AVG_SERVICE_C2)
tempi_coda(3, NUM_SERVER_C3_1, p3_0, AVG_SERVICE_C3)
print(" CENTRO 4 : E(T_Q)  = NO CODA")
tempi_coda(5, NUM_SERVER_C5_1, p5_0, AVG_SERVICE_C5)
tempi_coda(6, NUM_SERVER_C6_1, p6_0, AVG_SERVICE_C6)
a = r_tempi_coda(0, NUM_SERVER_C0_1, p0_0, AVG_SERVICE_C0)
b = r_tempi_coda(1, NUM_SERVER_C1_1, p1_0, AVG_SERVICE_C1)
c = r_tempi_coda(2, NUM_SERVER_C2_1, p2_0, AVG_SERVICE_C2)
d = r_tempi_coda(3, NUM_SERVER_C3_1, p3_0, AVG_SERVICE_C3)
f = r_tempi_coda(5, NUM_SERVER_C5_1, p5_0, AVG_SERVICE_C5)
g = r_tempi_coda(6, NUM_SERVER_C6_1, p6_0, AVG_SERVICE_C6)
print("\n   TOTALE : T(T_S)  =",avgs + a+b+c+d+f+g)

print("\n CONFIGURAZIONE 2:")
tempi_coda(0, NUM_SERVER_C0_2, p0_1, AVG_SERVICE_C0)
tempi_coda(1, NUM_SERVER_C1_2, p1_1, AVG_SERVICE_C1)
tempi_coda(2, NUM_SERVER_C2_2, p2_1, AVG_SERVICE_C2)
tempi_coda(3, NUM_SERVER_C3_2, p3_1, AVG_SERVICE_C3)
print(" CENTRO 4 : E(T_Q)  = NO CODA")
tempi_coda(5, NUM_SERVER_C5_2, p5_1, AVG_SERVICE_C5)
tempi_coda(6, NUM_SERVER_C6_2, p6_1, AVG_SERVICE_C6)
a = r_tempi_coda(0, NUM_SERVER_C0_2, p0_1, AVG_SERVICE_C0)
b = r_tempi_coda(1, NUM_SERVER_C1_2, p1_1, AVG_SERVICE_C1)
c = r_tempi_coda(2, NUM_SERVER_C2_2, p2_1, AVG_SERVICE_C2)
d = r_tempi_coda(3, NUM_SERVER_C3_2, p3_1, AVG_SERVICE_C3)
f = r_tempi_coda(5, NUM_SERVER_C5_2, p5_1, AVG_SERVICE_C5)
g = r_tempi_coda(6, NUM_SERVER_C6_2, p6_1, AVG_SERVICE_C6)
print("\n   TOTALE : T(T_S)  =",avgs + a+b+c+d+f+g)


print("\n CONFIGURAZIONE 3:")
tempi_coda(0, NUM_SERVER_C0_3, p0_2, AVG_SERVICE_C0)
tempi_coda(1, NUM_SERVER_C1_3, p1_2, AVG_SERVICE_C1)
tempi_coda(2, NUM_SERVER_C2_3, p2_2, AVG_SERVICE_C2)
tempi_coda(3, NUM_SERVER_C3_3, p3_2, AVG_SERVICE_C3)
print(" CENTRO 4 : E(T_Q)  = NO CODA")
tempi_coda(5, NUM_SERVER_C5_3, p5_2, AVG_SERVICE_C5)
tempi_coda(6, NUM_SERVER_C6_3, p6_2, AVG_SERVICE_C6)
a = r_tempi_coda(0, NUM_SERVER_C0_3, p0_2, AVG_SERVICE_C0)
b = r_tempi_coda(1, NUM_SERVER_C1_3, p1_2, AVG_SERVICE_C1)
c = r_tempi_coda(2, NUM_SERVER_C2_3, p2_2, AVG_SERVICE_C2)
d = r_tempi_coda(3, NUM_SERVER_C3_3, p3_2, AVG_SERVICE_C3)
f = r_tempi_coda(5, NUM_SERVER_C5_3, p5_2, AVG_SERVICE_C5)
g = r_tempi_coda(6, NUM_SERVER_C6_3, p6_2, AVG_SERVICE_C6)
print("\n   TOTALE : T(T_S)  =",avgs + a+b+c+d+f+g)

print("\n --------------------------------------")

NUM_SERVER_C0_1 = 1
NUM_SERVER_C1_1 = 5
NUM_SERVER_C2_1 = 14
NUM_SERVER_C3_1 = 6
NUM_SERVER_C4_1 = 10

NUM_SERVER_C0_2 = 1
NUM_SERVER_C1_2 = 11
NUM_SERVER_C2_2 = 27
NUM_SERVER_C3_2 = 13
NUM_SERVER_C4_2 = 20

NUM_SERVER_C0_3 = 1
NUM_SERVER_C1_3 = 2
NUM_SERVER_C2_3 = 4
NUM_SERVER_C3_3 = 2
NUM_SERVER_C4_3 = 3

p0_0 = LAMBDA1*P_C0*AVG_SERVICE_C0
p1_0 = LAMBDA1*P_C1*AVG_SERVICE_C1/NUM_SERVER_C1_1
p2_0 = ((LAMBDA1*P_A2) + (p1_0*(1-P_SB1)*(NUM_SERVER_C1_1/AVG_SERVICE_C1)))*AVG_SERVICE_C2/NUM_SERVER_C2_1
lambda3_0 = ((p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)*P_C2) + (p0_0*(1/AVG_SERVICE_C0)*(1-P_SB0))*(1-P_PD0)
p4_0 = ((lambda3_0*P_PC*(1-P_CP4)) + (lambda3_0*(1-P_PC)) + (p0_0*(1/AVG_SERVICE_C0)*(1-P_SB0))*P_PD0 + (p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)*P_C3)*(AVG_SERVICE_C5/NUM_SERVER_C4_1)

print(" CONFIGURAZIONE 1 MULTI-QUEUE:")
tempi_coda(0, NUM_SERVER_C0_1, p0_0, AVG_SERVICE_C0)
tempi_coda(1, NUM_SERVER_C1_1, p1_0, AVG_SERVICE_C1)
multi_tempi_coda(2, NUM_SERVER_C2_1, p2_0, AVG_SERVICE_C2)
print(" CENTRO 3 : E(T_Q)  = NO CODA")
multi_tempi_coda(4, NUM_SERVER_C4_1, p4_0, AVG_SERVICE_C5)
a = r_tempi_coda(0, NUM_SERVER_C0_1, p0_0, AVG_SERVICE_C0)
b = r_tempi_coda(1, NUM_SERVER_C1_1, p1_0, AVG_SERVICE_C1)
c = r_multi_tempi_coda(2, NUM_SERVER_C2_1, p2_0, AVG_SERVICE_C2)
f = r_multi_tempi_coda(4, NUM_SERVER_C4_1, p4_0, AVG_SERVICE_C5)

lambda_disabili_usciti_da_parcheggio = LAMBDA1*P_C0*(1-P_SB0)*P_PD0
lambda_disabili_usciti_da_doc_pat = ((p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)*(1-P_C2))
P_disabili = (lambda_disabili_usciti_da_parcheggio + lambda_disabili_usciti_da_doc_pat)/(p4_0/(AVG_SERVICE_C5/NUM_SERVER_C4_1))
avgs = AVG_SERVICE_C0+AVG_SERVICE_C1+AVG_SERVICE_C2+AVG_SERVICE_C4+AVG_SERVICE_C5

print("\n   TOTALE : T(T_S)  =",avgs + a+b+(c[0]*P_C3)+(c[1]*P_C2)+(f[0]*P_disabili)+(f[1]*(1-P_disabili)))

print("\n --------------------------------------")


p0_1 = LAMBDA2*P_C0*AVG_SERVICE_C0
p1_1 = LAMBDA2*P_C1*AVG_SERVICE_C1/NUM_SERVER_C1_2
p2_1 = ((LAMBDA2*P_A2) + (p1_1*(1-P_SB1)*(NUM_SERVER_C1_2/AVG_SERVICE_C1)))*AVG_SERVICE_C2/NUM_SERVER_C2_2
lambda3_1 = ((p2_1*(NUM_SERVER_C2_2/AVG_SERVICE_C2))*(1-P_SB2)*P_C2) + (p0_1*(1/AVG_SERVICE_C0)*(1-P_SB0))*(1-P_PD0)
p4_1 = ((lambda3_1*P_PC*(1-P_CP4)) + (lambda3_1*(1-P_PC)) + (p0_1*(1/AVG_SERVICE_C0)*(1-P_SB0))*P_PD0 + (p2_1*(NUM_SERVER_C2_2/AVG_SERVICE_C2))*(1-P_SB2)*P_C3)*(AVG_SERVICE_C5/NUM_SERVER_C4_2)

print(" CONFIGURAZIONE 2 MULTI-QUEUE:")
tempi_coda(0, NUM_SERVER_C0_2, p0_1, AVG_SERVICE_C0)
tempi_coda(1, NUM_SERVER_C1_2, p1_1, AVG_SERVICE_C1)
multi_tempi_coda(2, NUM_SERVER_C2_2, p2_1, AVG_SERVICE_C2)
print(" CENTRO 3 : E(T_Q)  = NO CODA")
multi_tempi_coda(4, NUM_SERVER_C4_2, p4_1, AVG_SERVICE_C5)
a = r_tempi_coda(0, NUM_SERVER_C0_2, p0_1, AVG_SERVICE_C0)
b = r_tempi_coda(1, NUM_SERVER_C1_2, p1_1, AVG_SERVICE_C1)
c = r_multi_tempi_coda(2, NUM_SERVER_C2_2, p2_1, AVG_SERVICE_C2)
f = r_multi_tempi_coda(4, NUM_SERVER_C4_2, p4_1, AVG_SERVICE_C5)

lambda_disabili_usciti_da_parcheggio = LAMBDA1*P_C0*(1-P_SB0)*P_PD0
lambda_disabili_usciti_da_doc_pat = ((p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)*(1-P_C2))
P_disabili = (lambda_disabili_usciti_da_parcheggio + lambda_disabili_usciti_da_doc_pat)/(p4_0/(AVG_SERVICE_C5/NUM_SERVER_C4_1))
avgs = AVG_SERVICE_C0+AVG_SERVICE_C1+AVG_SERVICE_C2+AVG_SERVICE_C4+AVG_SERVICE_C5

print("\n   TOTALE : T(T_S)  =",avgs + a+b+(c[0]*P_C3)+(c[1]*P_C2)+(f[0]*P_disabili)+(f[1]*(1-P_disabili)))

print("\n --------------------------------------")


p0_2 = LAMBDA3*P_C0*AVG_SERVICE_C0
p1_2 = LAMBDA3*P_C1*AVG_SERVICE_C1/NUM_SERVER_C1_3
p2_2 = ((LAMBDA3*P_A2) + (p1_2*(1-P_SB1)*(NUM_SERVER_C1_3/AVG_SERVICE_C1)))*AVG_SERVICE_C2/NUM_SERVER_C2_3
lambda3_2 = ((p2_2*(NUM_SERVER_C2_3/AVG_SERVICE_C2))*(1-P_SB2)*P_C2) + (p0_2*(1/AVG_SERVICE_C0)*(1-P_SB0))*(1-P_PD0)
p4_2 = ((lambda3_2*P_PC*(1-P_CP4)) + (lambda3_2*(1-P_PC)) + (p0_2*(1/AVG_SERVICE_C0)*(1-P_SB0))*P_PD0 + (p2_2*(NUM_SERVER_C2_3/AVG_SERVICE_C2))*(1-P_SB2)*P_C3)*(AVG_SERVICE_C5/NUM_SERVER_C4_3)

print(" CONFIGURAZIONE 3 MULTI-QUEUE:")
tempi_coda(0, NUM_SERVER_C0_3, p0_2, AVG_SERVICE_C0)
tempi_coda(1, NUM_SERVER_C1_3, p1_2, AVG_SERVICE_C1)
multi_tempi_coda(2, NUM_SERVER_C2_3, p2_2, AVG_SERVICE_C2)
print(" CENTRO 3 : E(T_Q)  = NO CODA")
multi_tempi_coda(4, NUM_SERVER_C4_3, p4_2, AVG_SERVICE_C5)
a = r_tempi_coda(0, NUM_SERVER_C0_3, p0_2, AVG_SERVICE_C0)
b = r_tempi_coda(1, NUM_SERVER_C1_3, p1_2, AVG_SERVICE_C1)
c = r_multi_tempi_coda(2, NUM_SERVER_C2_3, p2_2, AVG_SERVICE_C2)
f = r_multi_tempi_coda(4, NUM_SERVER_C4_3, p4_2, AVG_SERVICE_C5)

lambda_disabili_usciti_da_parcheggio = LAMBDA1*P_C0*(1-P_SB0)*P_PD0
lambda_disabili_usciti_da_doc_pat = ((p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)*(1-P_C2))
P_disabili = (lambda_disabili_usciti_da_parcheggio + lambda_disabili_usciti_da_doc_pat)/(p4_0/(AVG_SERVICE_C5/NUM_SERVER_C4_1))
avgs = AVG_SERVICE_C0+AVG_SERVICE_C1+AVG_SERVICE_C2+AVG_SERVICE_C4+AVG_SERVICE_C5

print("\n   TOTALE : T(T_S)  =",avgs + a+b+(c[0]*P_C3)+(c[1]*P_C2)+(f[0]*P_disabili)+(f[1]*(1-P_disabili)))

print("\n --------------------------------------")

NUM_SERVER_C0_1 = 1
NUM_SERVER_C1_1 = 5
NUM_SERVER_C2_1 = 12
NUM_SERVER_C3_1 = 6
NUM_SERVER_C4_1 = 9

NUM_SERVER_C0_2 = 1
NUM_SERVER_C1_2 = 11
NUM_SERVER_C2_2 = 26
NUM_SERVER_C3_2 = 13
NUM_SERVER_C4_2 = 18

NUM_SERVER_C0_3 = 1
NUM_SERVER_C1_3 = 2
NUM_SERVER_C2_3 = 3
NUM_SERVER_C3_3 = 2
NUM_SERVER_C4_3 = 2

p0_0 = LAMBDA1*P_C0*AVG_SERVICE_C0
p1_0 = LAMBDA1*P_C1*AVG_SERVICE_C1/NUM_SERVER_C1_1
p2_0 = ((LAMBDA1*P_A2) + (p1_0*(1-P_SB1)*(NUM_SERVER_C1_1/AVG_SERVICE_C1)))*AVG_SERVICE_C2/NUM_SERVER_C2_1
lambda3_0 = ((p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)*P_C2) + (p0_0*(1/AVG_SERVICE_C0)*(1-P_SB0))*(1-P_PD0)
p4_0 = ((lambda3_0*P_PC*(1-P_CP4)) + (lambda3_0*(1-P_PC)) + (p0_0*(1/AVG_SERVICE_C0)*(1-P_SB0))*P_PD0 + (p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)*P_C3)*(AVG_SERVICE_C5/NUM_SERVER_C4_1)

print(" CONFIGURAZIONE 1 MULTI-QUEUE:")
tempi_coda(0, NUM_SERVER_C0_1, p0_0, AVG_SERVICE_C0)
tempi_coda(1, NUM_SERVER_C1_1, p1_0, AVG_SERVICE_C1)
multi_tempi_coda(2, NUM_SERVER_C2_1, p2_0, AVG_SERVICE_C2)
print(" CENTRO 3 : E(T_Q)  = NO CODA")
multi_tempi_coda(4, NUM_SERVER_C4_1, p4_0, AVG_SERVICE_C5)
a = r_tempi_coda(0, NUM_SERVER_C0_1, p0_0, AVG_SERVICE_C0)
b = r_tempi_coda(1, NUM_SERVER_C1_1, p1_0, AVG_SERVICE_C1)
c = r_multi_tempi_coda(2, NUM_SERVER_C2_1, p2_0, AVG_SERVICE_C2)
f = r_multi_tempi_coda(4, NUM_SERVER_C4_1, p4_0, AVG_SERVICE_C5)

lambda_disabili_usciti_da_parcheggio = LAMBDA1*P_C0*(1-P_SB0)*P_PD0
lambda_disabili_usciti_da_doc_pat = ((p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)*(1-P_C2))
P_disabili = (lambda_disabili_usciti_da_parcheggio + lambda_disabili_usciti_da_doc_pat)/(p4_0/(AVG_SERVICE_C5/NUM_SERVER_C4_1))
avgs = AVG_SERVICE_C0+AVG_SERVICE_C1+AVG_SERVICE_C2+AVG_SERVICE_C4+AVG_SERVICE_C5

print("\n   TOTALE : T(T_S)  =",avgs + a+b+(c[0]*P_C3)+(c[1]*P_C2)+(f[0]*P_disabili)+(f[1]*(1-P_disabili)))

print("\n --------------------------------------")


p0_1 = LAMBDA2*P_C0*AVG_SERVICE_C0
p1_1 = LAMBDA2*P_C1*AVG_SERVICE_C1/NUM_SERVER_C1_2
p2_1 = ((LAMBDA2*P_A2) + (p1_1*(1-P_SB1)*(NUM_SERVER_C1_2/AVG_SERVICE_C1)))*AVG_SERVICE_C2/NUM_SERVER_C2_2
lambda3_1 = ((p2_1*(NUM_SERVER_C2_2/AVG_SERVICE_C2))*(1-P_SB2)*P_C2) + (p0_1*(1/AVG_SERVICE_C0)*(1-P_SB0))*(1-P_PD0)
p4_1 = ((lambda3_1*P_PC*(1-P_CP4)) + (lambda3_1*(1-P_PC)) + (p0_1*(1/AVG_SERVICE_C0)*(1-P_SB0))*P_PD0 + (p2_1*(NUM_SERVER_C2_2/AVG_SERVICE_C2))*(1-P_SB2)*P_C3)*(AVG_SERVICE_C5/NUM_SERVER_C4_2)

print(" CONFIGURAZIONE 2 MULTI-QUEUE:")
tempi_coda(0, NUM_SERVER_C0_2, p0_1, AVG_SERVICE_C0)
tempi_coda(1, NUM_SERVER_C1_2, p1_1, AVG_SERVICE_C1)
multi_tempi_coda(2, NUM_SERVER_C2_2, p2_1, AVG_SERVICE_C2)
print(" CENTRO 3 : E(T_Q)  = NO CODA")
multi_tempi_coda(4, NUM_SERVER_C4_2, p4_1, AVG_SERVICE_C5)
a = r_tempi_coda(0, NUM_SERVER_C0_2, p0_1, AVG_SERVICE_C0)
b = r_tempi_coda(1, NUM_SERVER_C1_2, p1_1, AVG_SERVICE_C1)
c = r_multi_tempi_coda(2, NUM_SERVER_C2_2, p2_1, AVG_SERVICE_C2)
f = r_multi_tempi_coda(4, NUM_SERVER_C4_2, p4_1, AVG_SERVICE_C5)

lambda_disabili_usciti_da_parcheggio = LAMBDA1*P_C0*(1-P_SB0)*P_PD0
lambda_disabili_usciti_da_doc_pat = ((p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)*(1-P_C2))
P_disabili = (lambda_disabili_usciti_da_parcheggio + lambda_disabili_usciti_da_doc_pat)/(p4_0/(AVG_SERVICE_C5/NUM_SERVER_C4_1))
avgs = AVG_SERVICE_C0+AVG_SERVICE_C1+AVG_SERVICE_C2+AVG_SERVICE_C4+AVG_SERVICE_C5

print("\n   TOTALE : T(T_S)  =",avgs + a+b+(c[0]*P_C3)+(c[1]*P_C2)+(f[0]*P_disabili)+(f[1]*(1-P_disabili)))

print("\n --------------------------------------")


p0_2 = LAMBDA3*P_C0*AVG_SERVICE_C0
p1_2 = LAMBDA3*P_C1*AVG_SERVICE_C1/NUM_SERVER_C1_3
p2_2 = ((LAMBDA3*P_A2) + (p1_2*(1-P_SB1)*(NUM_SERVER_C1_3/AVG_SERVICE_C1)))*AVG_SERVICE_C2/NUM_SERVER_C2_3
lambda3_2 = ((p2_2*(NUM_SERVER_C2_3/AVG_SERVICE_C2))*(1-P_SB2)*P_C2) + (p0_2*(1/AVG_SERVICE_C0)*(1-P_SB0))*(1-P_PD0)
p4_2 = ((lambda3_2*P_PC*(1-P_CP4)) + (lambda3_2*(1-P_PC)) + (p0_2*(1/AVG_SERVICE_C0)*(1-P_SB0))*P_PD0 + (p2_2*(NUM_SERVER_C2_3/AVG_SERVICE_C2))*(1-P_SB2)*P_C3)*(AVG_SERVICE_C5/NUM_SERVER_C4_3)

print(" CONFIGURAZIONE 3 MULTI-QUEUE:")
tempi_coda(0, NUM_SERVER_C0_3, p0_2, AVG_SERVICE_C0)
tempi_coda(1, NUM_SERVER_C1_3, p1_2, AVG_SERVICE_C1)
multi_tempi_coda(2, NUM_SERVER_C2_3, p2_2, AVG_SERVICE_C2)
print(" CENTRO 3 : E(T_Q)  = NO CODA")
multi_tempi_coda(4, NUM_SERVER_C4_3, p4_2, AVG_SERVICE_C5)
a = r_tempi_coda(0, NUM_SERVER_C0_3, p0_2, AVG_SERVICE_C0)
b = r_tempi_coda(1, NUM_SERVER_C1_3, p1_2, AVG_SERVICE_C1)
c = r_multi_tempi_coda(2, NUM_SERVER_C2_3, p2_2, AVG_SERVICE_C2)
f = r_multi_tempi_coda(4, NUM_SERVER_C4_3, p4_2, AVG_SERVICE_C5)

lambda_disabili_usciti_da_parcheggio = LAMBDA1*P_C0*(1-P_SB0)*P_PD0
lambda_disabili_usciti_da_doc_pat = ((p2_0*(NUM_SERVER_C2_1/AVG_SERVICE_C2))*(1-P_SB2)*(1-P_C2))
P_disabili = (lambda_disabili_usciti_da_parcheggio + lambda_disabili_usciti_da_doc_pat)/(p4_0/(AVG_SERVICE_C5/NUM_SERVER_C4_1))
avgs = AVG_SERVICE_C0+AVG_SERVICE_C1+AVG_SERVICE_C2+AVG_SERVICE_C4+AVG_SERVICE_C5

print("\n   TOTALE : T(T_S)  =",avgs + a+b+(c[0]*P_C3)+(c[1]*P_C2)+(f[0]*P_disabili)+(f[1]*(1-P_disabili)))

print("\n --------------------------------------")


l0 = 4583*P_C0
l1 = 4583*P_C1
l2 = ((4583*P_A2) + (l1*(1-P_SB1))) *P_C2
l3 = ((4583*P_A2) + (l1*(1-P_SB1)))*P_C3
l4 = ((l2*(1-P_SB2)) + (l0*(1-P_SB0))*(1-P_PD0))*P_PC
l5 = (l4*(1-P_CP4)) + (l4*((1/P_PC)-1))
l6 = (l3*(1-P_SB3)) + (l0*(1-P_SB0)*(P_PD0))

d0 = l0*P_SB0
d1 = l1*P_SB1
d2 = l2*P_SB2
d3 = l3*P_SB3
d4 = l4*P_CP4
d5 = l5*P_SB5
d6 = l6*P_SB5

print(" --------------------------------------\n")

print(" POPOLAZIONI:")
print(" CENTRO 0 :",  l0)
print(" CENTRO 1 :",  l1)
print(" CENTRO 2 :",  l2)
print(" CENTRO 3 :",  l3)
print(" CENTRO 4 :",  l4)
print(" CENTRO 5 :",  l5)
print(" CENTRO 6 :",  l6)
print("")
print(" DROPPED 0 :", d0)
print(" DROPPED 1 :", d1)
print(" DROPPED 2 :", d2)
print(" DROPPED 3 :", d3)
print(" DROPPED 4 :", d4)
print(" DROPPED 5 :", d5)
print(" DROPPED 6 :", d6)
print("")
print(" ARRIVALS =", l0+l2+l3+d1)
print(" DROPPED  =", d0+d2+d3+d1+d4+d5+d6)
print(" EXITS    =", l5+l6-d5-d6)

print("")