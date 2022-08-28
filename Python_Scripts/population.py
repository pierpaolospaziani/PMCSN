LAMBDA1 = 0.5875
LAMBDA2 = 1.273056
LAMBDA3 = 0.09778

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

l0_1 = LAMBDA1*3600*P_C0
l1_1 = LAMBDA1*3600*P_C1
l2_1 = ((LAMBDA1*3600*P_A2) + (l1_1*(1-P_SB1))) *P_C2
l3_1 = ((LAMBDA1*3600*P_A2) + (l1_1*(1-P_SB1)))*P_C3
l4_1 = ((l2_1*(1-P_SB2)) + (l0_1*(1-P_SB0))*(1-P_PD0))*P_PC
l5_1 = (l4_1*(1-P_CP4)) + (l4_1*((1/P_PC)-1))
l6_1 = (l3_1*(1-P_SB3)) + (l0_1*(1-P_SB0)*(P_PD0))

l0_2 = LAMBDA2*3600*P_C0
l1_2 = LAMBDA2*3600*P_C1
l2_2 = ((LAMBDA2*3600*P_A2) + (l1_2*(1-P_SB1))) *P_C2
l3_2 = ((LAMBDA2*3600*P_A2) + (l1_2*(1-P_SB1)))*P_C3
l4_2 = ((l2_2*(1-P_SB2)) + (l0_2*(1-P_SB0))*(1-P_PD0))*P_PC
l5_2 = (l4_2*(1-P_CP4)) + (l4_2*((1/P_PC)-1))
l6_2 = (l3_2*(1-P_SB3)) + (l0_2*(1-P_SB0)*(P_PD0))

l0_3 = LAMBDA3*3600*P_C0
l1_3 = LAMBDA3*3600*P_C1
l2_3 = ((LAMBDA3*3600*P_A2) + (l1_3*(1-P_SB1))) *P_C2
l3_3 = ((LAMBDA3*3600*P_A2) + (l1_3*(1-P_SB1)))*P_C3
l4_3 = ((l2_3*(1-P_SB2)) + (l0_3*(1-P_SB0))*(1-P_PD0))*P_PC
l5_3 = (l4_3*(1-P_CP4)) + (l4_3*((1/P_PC)-1))
l6_3 = (l3_3*(1-P_SB3)) + (l0_3*(1-P_SB0)*(P_PD0))

d0_1 = l0_1*P_SB0
d1_1 = l1_1*P_SB1
d2_1 = l2_1*P_SB2
d3_1 = l3_1*P_SB3
d4_1 = l4_1*P_CP4
d5_1 = l5_1*P_SB5
d6_1 = l6_1*P_SB5

d0_2 = l0_2*P_SB0
d1_2 = l1_2*P_SB1
d2_2 = l2_2*P_SB2
d3_2 = l3_2*P_SB3
d4_2 = l4_2*P_CP4
d5_2 = l5_2*P_SB5
d6_2 = l6_2*P_SB5

d0_3 = l0_3*P_SB0
d1_3 = l1_3*P_SB1
d2_3 = l2_3*P_SB2
d3_3 = l3_3*P_SB3
d4_3 = l4_3*P_CP4
d5_3 = l5_3*P_SB5
d6_3 = l6_3*P_SB5

print("\n *** MODELLO BASE ***\n")
print(" POPOLAZIONI 1° FASCIA\n")
print(" ARRIVI CENTRO 0        :",  l0_1)
print(" ARRIVI CENTRO 1        :",  l1_1)
print(" ARRIVI CENTRO 2        :",  l2_1)
print(" ARRIVI CENTRO 3        :",  l3_1)
print(" ARRIVI MINIMI CENTRO 4 :",  l4_1)
print(" ARRIVI CENTRO 5        :",  l5_1)
print(" ARRIVI CENTRO 6        :",  l6_1)
print("")
print(" DROPPED CENTRO 0 :", d0_1)
print(" DROPPED CENTRO 1 :", d1_1)
print(" DROPPED CENTRO 2 :", d2_1)
print(" DROPPED CENTRO 3 :", d3_1)
print(" DROPPED CENTRO 4 :", d4_1)
print(" DROPPED CENTRO 5 :", d5_1)
print(" DROPPED CENTRO 6 :", d6_1)
print("")
print(" ARRIVI TOTALI        =", l0_1+l2_1+l3_1+d1_1)
print(" DROPPED TOTALI       =", d0_1+d2_1+d3_1+d1_1+d4_1+d5_1+d6_1)
print(" COMPLETAMENTI TOTALI =", l5_1+l6_1-d5_1-d6_1)

print("\n --------------------------------------")

print("\n POPOLAZIONI 2° FASCIA\n")
print(" ARRIVI CENTRO 0        :",  l0_2)
print(" ARRIVI CENTRO 1        :",  l1_2)
print(" ARRIVI CENTRO 2        :",  l2_2)
print(" ARRIVI CENTRO 3        :",  l3_2)
print(" ARRIVI MINIMI CENTRO 4 :",  l4_2)
print(" ARRIVI CENTRO 5        :",  l5_2)
print(" ARRIVI CENTRO 6        :",  l6_2)
print("")
print(" DROPPED CENTRO 0 :", d0_2)
print(" DROPPED CENTRO 1 :", d1_2)
print(" DROPPED CENTRO 2 :", d2_2)
print(" DROPPED CENTRO 3 :", d3_2)
print(" DROPPED CENTRO 4 :", d4_2)
print(" DROPPED CENTRO 5 :", d5_2)
print(" DROPPED CENTRO 6 :", d6_2)
print("")
print(" ARRIVI TOTALI        =", l0_2+l2_2+l3_2+d1_2)
print(" DROPPED TOTALI       =", d0_2+d2_2+d3_2+d1_2+d4_2+d5_2+d6_2)
print(" COMPLETAMENTI TOTALI =", l5_2+l6_2-d5_2-d6_2)

print("\n --------------------------------------")

print("\n POPOLAZIONI 3° FASCIA\n")
print(" ARRIVI CENTRO 0        :",  l0_3)
print(" ARRIVI CENTRO 1        :",  l1_3)
print(" ARRIVI CENTRO 2        :",  l2_3)
print(" ARRIVI CENTRO 3        :",  l3_3)
print(" ARRIVI MINIMI CENTRO 4 :",  l4_3)
print(" ARRIVI CENTRO 5        :",  l5_3)
print(" ARRIVI CENTRO 6        :",  l6_3)
print("")
print(" DROPPED CENTRO 0 :", d0_3)
print(" DROPPED CENTRO 1 :", d1_3)
print(" DROPPED CENTRO 2 :", d2_3)
print(" DROPPED CENTRO 3 :", d3_3)
print(" DROPPED CENTRO 4 :", d4_3)
print(" DROPPED CENTRO 5 :", d5_3)
print(" DROPPED CENTRO 6 :", d6_3)
print("")
print(" ARRIVI TOTALI        =", l0_3+l2_3+l3_3+d1_3)
print(" DROPPED TOTALI       =", d0_3+d2_3+d3_3+d1_3+d4_3+d5_3+d6_3)
print(" COMPLETAMENTI TOTALI =", l5_3+l6_3-d5_3-d6_3)

l0_0 = LAMBDA1*3600*P_C0
l1_0 = LAMBDA1*3600*P_C1
l2_0 = (LAMBDA1*3600*P_A2) + (l1_0*(1-P_SB1))
l2_0_d = l2_0 * P_C3
l2_0_nd = l2_0 * P_C2
l3_0 = (l2_0_nd*(1-P_SB2)) + (l0_0*(1-P_SB0))*(1-P_PD0)
l4_0 = (l3_0*P_PC*(1-P_CP4)) + (l3_0*(1-P_PC)) + (l0_0*(1-P_SB0))*P_PD0 + (l2_0*(1-P_SB2)*P_C3)

lambda_disabili_usciti_da_parcheggio = LAMBDA1*P_C0*(1-P_SB0)*P_PD0
lambda_disabili_usciti_da_doc_pat = l2_0*(1-P_SB2)*(1-P_C2)
P_disabili = (lambda_disabili_usciti_da_parcheggio + lambda_disabili_usciti_da_doc_pat)/l3_0

l4_0_d = l4_0 * P_disabili
l4_0_nd = l4_0 * (1-P_disabili)

l0_1 = LAMBDA2*3600*P_C0
l1_1 = LAMBDA2*3600*P_C1
l2_1 = (LAMBDA2*3600*P_A2) + (l1_1*(1-P_SB1))
l2_1_d = l2_1 * P_C3
l2_1_nd = l2_1 * P_C2
l3_1 = (l2_1*(1-P_SB2)*P_C2) + (l0_1*(1-P_SB0))*(1-P_PD0)
l4_1 = (l3_1*P_PC*(1-P_CP4)) + (l3_1*(1-P_PC)) + (l0_1*(1-P_SB0))*P_PD0 + (l2_1*(1-P_SB2)*P_C3)

lambda_disabili_usciti_da_parcheggio = LAMBDA1*P_C0*(1-P_SB0)*P_PD0
lambda_disabili_usciti_da_doc_pat = l2_1*(1-P_SB2)*(1-P_C2)
P_disabili = (lambda_disabili_usciti_da_parcheggio + lambda_disabili_usciti_da_doc_pat)/l3_1

l4_1_d = l4_1 * P_disabili
l4_1_nd = l4_1 * (1-P_disabili)

l0_2 = LAMBDA3*3600*P_C0
l1_2 = LAMBDA3*3600*P_C1
l2_2 = (LAMBDA3*3600*P_A2) + (l1_2*(1-P_SB1))
l2_2_d = l2_2 * P_C3
l2_2_nd = l2_2 * P_C2
l3_2 = (l2_2*(1-P_SB2)*P_C2) + (l0_2*(1-P_SB0))*(1-P_PD0)
l4_2 = (l3_2*P_PC*(1-P_CP4)) + (l3_2*(1-P_PC)) + (l0_2*(1-P_SB0))*P_PD0 + (l2_2*(1-P_SB2)*P_C3)

lambda_disabili_usciti_da_parcheggio = LAMBDA1*P_C0*(1-P_SB0)*P_PD0
lambda_disabili_usciti_da_doc_pat = l2_2*(1-P_SB2)*(1-P_C2)
P_disabili = (lambda_disabili_usciti_da_parcheggio + lambda_disabili_usciti_da_doc_pat)/l3_2

l4_2_d = l4_2 * P_disabili
l4_2_nd = l4_2 * (1-P_disabili)

d0_1 = l0_0*P_SB0
d1_1 = l1_0*P_SB1
d2_1 = l2_0*P_SB2
d3_1 = l3_0*P_CP4*P_PC
d4_1 = l4_0*P_SB5

d0_2 = l0_1*P_SB0
d1_2 = l1_1*P_SB1
d2_2 = l2_1*P_SB2
d3_2 = l3_1*P_CP4*P_PC
d4_2 = l4_1*P_SB5

d0_3 = l0_2*P_SB0
d1_3 = l1_2*P_SB1
d2_3 = l2_2*P_SB2
d3_3 = l3_2*P_CP4*P_PC
d4_3 = l4_2*P_SB5

print("\n\n\n *** MODELLO MIGLIORATIVO ***\n")
print(" POPOLAZIONI 1° FASCIA\n")
print(" ARRIVI CENTRO 0         :", l0_0)
print(" ARRIVI CENTRO 1         :", l1_0)
print(" ARRIVI CENTRO 2 PRIO    :", l2_0_d)
print(" ARRIVI CENTRO 2 NO-PRIO :", l2_0_nd)
print(" ARRIVI MINIMI CENTRO 3  :", l3_0*P_PC)
print(" ARRIVI CENTRO 4 PRIO    :", l4_0_d)
print(" ARRIVI CENTRO 4 NO-PRIO :", l4_0_nd)
print("")
print(" DROPPED CENTRO 0 :", d0_1)
print(" DROPPED CENTRO 1 :", d1_1)
print(" DROPPED CENTRO 2 :", d2_1)
print(" DROPPED CENTRO 3 :", d3_1)
print(" DROPPED CENTRO 4 :", d4_1)
print("")
print(" ARRIVI TOTALI        =", l0_0+l2_0+d1_1)
print(" DROPPED TOTALI       =", d0_1+d2_1+d3_1+d1_1+d4_1)
print(" COMPLETAMENTI TOTALI =", l4_0-d4_1)

print("\n --------------------------------------")

print("\n POPOLAZIONI 2° FASCIA\n")
print(" ARRIVI CENTRO 0         :", l0_1)
print(" ARRIVI CENTRO 1         :", l1_1)
print(" ARRIVI CENTRO 2 PRIO    :", l2_1_d)
print(" ARRIVI CENTRO 2 NO-PRIO :", l2_1_nd)
print(" ARRIVI MINIMI CENTRO 3  :", l3_1*P_PC)
print(" ARRIVI CENTRO 4 PRIO    :", l4_1_d)
print(" ARRIVI CENTRO 4 NO-PRIO :", l4_1_nd)
print("")
print(" DROPPED CENTRO 0 :", d0_2)
print(" DROPPED CENTRO 1 :", d1_2)
print(" DROPPED CENTRO 2 :", d2_2)
print(" DROPPED CENTRO 3 :", d3_2)
print(" DROPPED CENTRO 4 :", d4_2)
print("")
print(" ARRIVI TOTALI        =", l0_1+l2_1+d1_2)
print(" DROPPED TOTALI       =", d0_2+d2_2+d3_2+d1_2+d4_2)
print(" COMPLETAMENTI TOTALI =", l4_1-d4_2)

print("\n --------------------------------------")

print("\n POPOLAZIONI 3° FASCIA\n")
print(" ARRIVI CENTRO 0         :", l0_2)
print(" ARRIVI CENTRO 1         :", l1_2)
print(" ARRIVI CENTRO 2 PRIO    :", l2_2_d)
print(" ARRIVI CENTRO 2 NO-PRIO :", l2_2_nd)
print(" ARRIVI MINIMI CENTRO 3  :", l3_2*P_PC)
print(" ARRIVI CENTRO 4 PRIO    :", l4_2_d)
print(" ARRIVI CENTRO 4 NO-PRIO :", l4_2_nd)
print("")
print(" DROPPED CENTRO 0 :", d0_3)
print(" DROPPED CENTRO 1 :", d1_3)
print(" DROPPED CENTRO 2 :", d2_3)
print(" DROPPED CENTRO 3 :", d3_3)
print(" DROPPED CENTRO 4 :", d4_3)
print("")
print(" ARRIVI TOTALI        =", l0_2+l2_2+d1_3)
print(" DROPPED TOTALI       =", d0_3+d2_3+d3_3+d1_3+d4_3)
print(" COMPLETAMENTI TOTALI =", l4_2-d4_3)

print("")