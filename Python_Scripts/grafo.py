import csv
import numpy as np
import matplotlib.pyplot as plt
import math
import sys

results = []
bypass  = []
phase_0 = []
phase_1 = []
phase_2 = []
phase_3 = []
phase_4 = []
phase_5 = []
phase_6 = []
for i in range(0,1080):
    results.append(0.0)
    bypass.append(0.0)
    phase_0.append(0.0)
    phase_1.append(0.0)
    phase_2.append(0.0)
    phase_3.append(0.0)
    phase_4.append(0.0)
    phase_5.append(0.0)
    phase_6.append(0.0)
k=0
with open("CSV/resp_time.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    j = 0
    for row in reader:
        a = row[0]
        results[j] = results[j] + a
        j = j + 1
        if (j == 1080):
            j = 0
            k = k + 1
k=0
with open("CSV/bypass.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    j = 0
    for row in reader:
        a = row[0]
        bypass[j] = bypass[j] + a
        j = j + 1
        if (j == 1080):
            j = 0
            k = k + 1
k=0
with open("CSV/phase_file_0.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    j = 0
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_0[j] = phase_0[j] + 1.0
        else:
            phase_0[j] = phase_0[j] + a
        j = j + 1
        if (j == 1080):
            j = 0
            k = k + 1
k=0
with open("CSV/phase_file_1.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    j = 0
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_1[j] = phase_1[j] + 1.0
        else:
            phase_1[j] = phase_1[j] + a
        j = j + 1
        if (j == 1080):
            j = 0
            k = k + 1
k=0
with open("CSV/phase_file_2.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    j = 0
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_2[j] = phase_2[j] + 1.0
        else:
            phase_2[j] = phase_2[j] + a
        j = j + 1
        if (j == 1080):
            j = 0
            k = k + 1
k=0
with open("CSV/phase_file_3.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    j = 0
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_3[j] = phase_3[j] + 1.0
        else:
            phase_3[j] = phase_3[j] + a
        j = j + 1
        if (j == 1080):
            j = 0
            k = k + 1
k=0
with open("CSV/phase_file_4.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    j = 0
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_4[j] = phase_4[j] + 1.0
        else:
            phase_4[j] = phase_4[j] + a
        j = j + 1
        if (j == 1080):
            j = 0
            k = k + 1
k=0
with open("CSV/phase_file_5.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    j = 0
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_5[j] = phase_5[j] + 1.0
        else:
            phase_5[j] = phase_5[j] + a
        j = j + 1
        if (j == 1080):
            j = 0
            k = k + 1
k=0
with open("CSV/phase_file_6.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    j = 0
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_6[j] = phase_6[j] + 1.0
        else:
            phase_6[j] = phase_6[j] + a
        j = j + 1
        if (j == 1080):
            j = 0
            k = k + 1

for i in range(0,1080):
    results[i] = results[i]/k
    bypass[i]  = bypass[i]/k
    phase_0[i] = phase_0[i]/k
    phase_1[i] = phase_1[i]/k
    phase_2[i] = phase_2[i]/k
    phase_3[i] = phase_3[i]/k
    phase_4[i] = phase_4[i]/k
    phase_5[i] = phase_5[i]/k
    phase_6[i] = phase_6[i]/k

x = np.array(results)
y = np.array(bypass)
x_0 = np.array(phase_0)
x_1 = np.array(phase_1)
x_2 = np.array(phase_2)
x_3 = np.array(phase_3)
x_4 = np.array(phase_4)
x_5 = np.array(phase_5)
x_6 = np.array(phase_6)

if (len(sys.argv) < 2):
    print(" Usage: python3 grafo.py <0/1/2>\n 0: System Response Time\n 1: Percentage Police Control\n 2: Utilization")
elif (int(sys.argv[1]) == 0):

    plt.plot(x, color="royalblue", label = 'Response Time')

    plt.ylabel('Response Time')

    plt.axhline(y = 390,  color = 'red', label = 'QoS 6\'30\'\'', linestyle = 'dashdot')
    plt.axvline(x = 360,  color = 'cornflowerblue', label = 'Switch phase', linestyle = 'dotted')
    plt.axvline(x = 720,  color = 'cornflowerblue', linestyle = 'dotted')
    plt.axvline(x = 1080, color = 'cornflowerblue', linestyle = 'dotted')
    plt.legend(loc="lower right")
    plt.xlabel('Simulation Time/10')
    plt.show()
elif (int(sys.argv[1]) == 1):
    plt.ylabel('Percentage Police Control')
    plt.plot(y, color="b", label = 'Percentage Police Control')
    plt.axhline(y = np.mean(y),  color = 'b', label = 'Mean', linestyle = 'dotted')
    plt.axhline(y = 0.25,  color = 'red', label = 'QoS 25%', linestyle = 'dashdot')
    plt.axvline(x = 360,  color = 'cornflowerblue', label = 'Switch phase', linestyle = 'dotted')
    plt.axvline(x = 720,  color = 'cornflowerblue', linestyle = 'dotted')
    plt.axvline(x = 1080, color = 'cornflowerblue', linestyle = 'dotted')
    plt.legend(loc="lower right")
    plt.xlabel('Simulation Time/10')
    plt.show()
elif (int(sys.argv[1]) == 2):
    plt.ylabel('Utilization')
    plt.plot(x_0, color="b", label = 'Park Entry')
    plt.plot(x_1, color="violet", label = 'Ticket Buy')
    if (len(sys.argv) >= 3):
        plt.plot(x_5, color="purple", label = 'Doc & Pat Prio')
        plt.plot(x_2, color="black",  label = 'Doc & Pat No-Prio')
        plt.plot(x_3, color="green",  label = 'Police Control')
        plt.plot(x_6, color="salmon", label = 'Turnstiles Prio')
        plt.plot(x_4, color="orange", label = 'Turnstiles No-Prio')
    else:
        plt.plot(x_2, color="black", label = 'Doc & Pat')
        plt.plot(x_3, color="green",  label = 'Doc & Pat D.')
        plt.plot(x_4, color="orange", label = 'Police Control')
        plt.plot(x_5, color="purple", label = 'Turnstiles')
        plt.plot(x_6, color="salmon", label = 'Turnstiles D.')
    plt.axvline(x = 360,  color = 'cornflowerblue', label = 'Switch phase', linestyle = 'dotted')
    plt.axvline(x = 720,  color = 'cornflowerblue', linestyle = 'dotted')
    plt.axvline(x = 1080, color = 'cornflowerblue', linestyle = 'dotted')
    plt.legend(loc="lower right")
    plt.xlabel('Simulation Time/10')
    plt.show()
else:
    print(" Usage: python3 grafo.py <0/1/2>\n 0: System Response Time\n 1: Percentage Police Control\n 2: Utilization")