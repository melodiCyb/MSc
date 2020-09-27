# Misra-Gries

def misra_gries(stream,n):
    '''
    stream: Input data
    n: window size
    returns --> All items in a sequence whose frequency exceeds a 1/n fraction of the total count.
    '''
    A = dict()
    for i in stream:
        if i in A.keys():
            A[i] += 1
        elif  len(A.keys()) < n:
            A[i] = 1
        else:
            for j in list(A.keys()):
                A[j] -= 1
                if A[j] == 0:
                    A.pop(j)
    return A



# Lossy Counting

import numpy as np

def LossyCounting(stream,w,s):
    '''
    stream:input data
    w:size of a bucket
    s: support threshold
    returns-->  Items with frequency exceeding (s-epsilon)*N   
    
    '''
    D = np.zeros((0,3))
    epsilon = 1.0 / w  # error threshold
    bcurrent = 1
    for N in range(len(stream)):
        if  np.any(stream[N] == D[:,0]):
            D[stream[N] == D[:,0],1] = D[stream[N] == D[:,0],1]+1
        else:
            D = np.append(D,[[stream[N],1,bcurrent-1]],axis = 0)
        if N % w == N-1:
            D[D[:,1] + D[:,2] <= bcurrent,:] = []
            bcurrent = bcurrent + 1
    return D[D[:,1] > (s-epsilon)*N, 0:2]
      



# Sticky Sampling

import numpy as np
from itertools import chain


def StickySampling(stream,f,e,sigma):
    '''
    stream:input data
    f:support threshold
    e:error threshold
    sigma:probability of failure
    returns-->Items with frequency exceeding (f-e)*N
    '''
    print "lenght of the sequence:", len(stream)
    D = np.zeros((0,2))
    
    t = np.ceil((1/e)*np.log(1/(f*sigma))) #sampling rate
    r = 1 
    for N in range(len(stream)):
        if np.any(stream[N] == D[:,0]):
            D[stream[N] == D[:,0],1] = D[stream[N] == D[:,0],1] + 1
        elif np.random.uniform(0,1) <= (1/r):
            D=np.append(D,[[stream[N],1]], axis=0)
        if N % t == N-1:
            for j in D.shape[0]:
                while np.random.randint(0,2):
                    if D[j,1] == 0:
                        D[j,:] = []
                        break
                    else:
                        D[j,1] = D[j,1] - 1
            r = r + 1
    return D
            


