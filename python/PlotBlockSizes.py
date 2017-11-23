import matplotlib.pylab as plt
import numpy as np

oneThread       = np.loadtxt("./data/BlockSizes1Threads.dat").T
twoThread       = np.loadtxt("./data/BlockSizes2Threads.dat").T
fourThread      = np.loadtxt("./data/BlockSizes4Threads.dat").T
eightThread     = np.loadtxt("./data/BlockSizes8Threads.dat").T
tenThread       = np.loadtxt("./data/BlockSizes10Threads.dat").T
twelveThread    = np.loadtxt("./data/BlockSizes12Threads.dat").T
fourteenThread  = np.loadtxt("./data/BlockSizes14Threads.dat").T



one,          = plt.plot(oneThread[0], oneThread[1], lw=2)
two,          = plt.plot(twoThread[0], twoThread[1]/2, lw=2)
four,         = plt.plot(fourThread[0], fourThread[1]/4, lw=2)
eight,        = plt.plot(eightThread[0], eightThread[1]/8, lw=2)
ten,          = plt.plot(tenThread[0], tenThread[1]/10, lw=2)
twelve,       = plt.plot(twelveThread[0], twelveThread[1]/12, lw=2)
fourteen,     = plt.plot(fourteenThread[0], fourteenThread[1]/14, lw=2)
plt.legend([one,two,four,eight,ten,twelve,fourteen],\
           [r'$p=1$',r'$p=2$',r'$p=4$',r'$p=8$',r'$p=10$',r'$p=12$',r'$p=14$'],loc=4)

ax = plt.gca()
ax.set_xscale('log')
ax.set_yscale('log')

plt.xlabel(r'Matrix Dimension ($N$)',fontsize=16)
plt.ylabel(r'CPU Time / $p$ (seconds)',fontsize=16)
plt.xticks(fontsize=16)
plt.yticks(fontsize=16)
plt.show()
