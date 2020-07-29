import matplotlib.pyplot as plt
import csv

T = 146             # total time

t = range(0, T)
S = [10000]
I = [1]
R = [0]
D = [0]

N = S[0] + I[0] + R[0] + D[0]

beta = 0.45
gamma = 0.19
epsilon = 0.015

for j in range(0, T-1):
    S.append(S[j] - beta*S[j]*I[j]/N)
    I.append(I[j] + beta*S[j]*I[j]/N - gamma*I[j] -epsilon*I[j])
    R.append(R[j] + gamma*I[j])
    D.append(D[j] +epsilon*I[j])

f = open("data.csv", "wt")

with f:
    writer = csv.writer(f)
    for j in range(0, T-1):
        writer.writerow([I[j] + R[j], D[j]])
    
plt.xlabel('Tempo')
plt.ylabel('Número de indivíduos')
plt.title('SIRD')
plt.scatter(t, S, marker='o', s=2)
plt.scatter(t, I, marker='o', s=2)
plt.scatter(t, R, marker='o', s=2)
plt.scatter(t, D, marker='o', s=2)
plt.legend(['S', 'I', 'R', 'D'])
plt.savefig('FiguraSIRD.png')
plt.show()