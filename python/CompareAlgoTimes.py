import matplotlib.pylab as plt
import numpy as np

n3OneThreads          = np.loadtxt("./data/n3_matrix_mult_1_threads.dat").T
n3SixThreads          = np.loadtxt("./data/n3_matrix_mult_6_threads.dat").T
n3TwelveThreads       = np.loadtxt("./data/n3_matrix_mult_12_threads.dat").T
strassenOneThreads    = np.loadtxt("./data/strassen_matrix_mult_1_threads.dat").T
strassenSixThreads    = np.loadtxt("./data/strassen_matrix_mult_6_threads.dat").T
strassenTwelveThreads = np.loadtxt("./data/strassen_matrix_mult_12_threads.dat").T




n3One,                = plt.plot(n3OneThreads[0],n3OneThreads[1], color='blue', ls='--',lw=2)
n3Six,                = plt.plot(n3SixThreads[0],n3SixThreads[1]/6, color='purple', ls='--',lw=2)
n3Twelve,             = plt.plot(n3TwelveThreads[0],n3TwelveThreads[1]/12, color='red', ls='--',lw=2)
sOne,                 = plt.plot(strassenOneThreads[0],strassenOneThreads[1], color='blue', ls='-',lw=2)
sSix,                 = plt.plot(strassenSixThreads[0],strassenSixThreads[1]/6, color='purple', ls='-',lw=2)
sTwelve,              = plt.plot(strassenTwelveThreads[0],strassenTwelveThreads[1]/12, color='red', ls='-',lw=2)


plt.legend([n3One, n3Six,n3Twelve,sOne,sSix,sTwelve],\
           [r'Naive $p=1$', r'Naive $p=6$',r'Naive $p=12$',r'Strassen $p=1$',r'Strassen $p=6$',r'Strassen $p=12$'],loc=4)

plt.plot([128,128],[1e-5,5e4],color='black',lw=2)

plt.ylim(1e-5,5e3)
plt.xlim(100,1e4)#4096)
ax = plt.gca()
ax.set_xscale('log')
ax.set_yscale('log')

plt.xlabel(r'Matrix Dimension ($N$)',fontsize=16)
plt.ylabel(r'CPU Time / $p$ (seconds)',fontsize=16)
plt.xticks(fontsize=16)
plt.yticks(fontsize=16)
plt.savefig("data/algocompare.eps")
#plt.show()
