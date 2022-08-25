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

with open("CSV/resp_time.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    for row in reader:
        results.append(row[0])

with open("CSV/bypass.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    for row in reader:
        bypass.append(row[0])

with open("CSV/phase_file_0.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_0.append(1)
        else:
            phase_0.append(a)

with open("CSV/phase_file_1.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_1.append(1)
        else:
            phase_1.append(a)

with open("CSV/phase_file_2.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_2.append(1)
        else:
            phase_2.append(a)

with open("CSV/phase_file_3.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_3.append(1)
        else:
            phase_3.append(a)

with open("CSV/phase_file_4.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_4.append(1)
        else:
            phase_4.append(a)

with open("CSV/phase_file_5.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_5.append(1)
        else:
            phase_5.append(a)

with open("CSV/phase_file_6.csv") as csvfile:
    reader = csv.reader(csvfile, quoting=csv.QUOTE_NONNUMERIC)
    for row in reader:
        a = row[0]
        if (a >= 1.0):
            phase_6.append(1)
        else:
            phase_6.append(a)

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
    print(" Usage: python3 batch_graph <0/1/2>\n 0: System Response Time\n 1: Percentage Police Control\n 2: Utilization")
elif (int(sys.argv[1]) == 0):
    plt.axhline(y = 390,  color = 'red', label = 'QoS 6\'30\'\'', linestyle = 'dashdot')

    plt.plot(x, color="royalblue", label = 'System Response Time')
    plt.axhline(y = np.mean(x),  color = 'royalblue', label = 'Mean', linestyle = 'dotted')

    plt.legend(loc="upper left")
    plt.xlabel('Batch')
    plt.show()
elif (int(sys.argv[1]) == 1):
    plt.plot(y, color="b", label = 'Percentage Police Control')
    plt.axhline(y = np.mean(y),  color = 'b', label = 'Mean', linestyle = 'dotted')
    plt.axhline(y = 0.25,  color = 'red', label = 'QoS 25%', linestyle = 'dashdot')
    plt.legend(loc="lower right")
    plt.xlabel('Batch')
    plt.show()
elif (int(sys.argv[1]) == 2):
    plt.ylabel('Utilization')
    plt.axhline(y = np.mean(x_0),  color = 'b', linestyle = 'dotted')
    plt.axhline(y = np.mean(x_1),  color = 'violet', linestyle = 'dotted')
    plt.axhline(y = np.mean(x_2),  color = 'black', linestyle = 'dotted')
    plt.axhline(y = np.mean(x_3),  color = 'green', linestyle = 'dotted')
    plt.axhline(y = np.mean(x_4),  color = 'orange', linestyle = 'dotted')
    plt.axhline(y = np.mean(x_5),  color = 'purple', linestyle = 'dotted')
    plt.axhline(y = np.mean(x_6),  color = 'salmon', linestyle = 'dotted')
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
    plt.legend(loc="lower right")
    plt.xlabel('Batch')
    plt.show()
else:
    print(" Usage: python3 batch_graph <0/1/2>\n 0: System Response Time\n 1: Percentage Police Control\n 2: Utilization")