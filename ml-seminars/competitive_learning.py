

import numpy as np
import pandas as pd
from sklearn.preprocessing import normalize

def CompetitiveNetwork(X, k, epochs, eta=0.01):
    '''
    X:Input data points
    epochs: number of epochs
    eta: learning rate
    '''

    # number of data points
    N = X.shape[0]
    
    # it's convenient to choose
    # initial k weights randomly from data points 
    W = X[np.random.choice(np.arange(len(X)),k ), :]
    
    # store weights to plot weights trajectories
    trj = [W] 
    X2 = (X**2).sum(axis=1)[:,np.newaxis]
    

    for epoch in range(epochs):
        assigned_labels = []
        for i in range(N):
            distance = X2[i:i+1].T-2*np.dot(W,X[i:i+1,:].T)
            +(W**2).sum(axis=1)[:,np.newaxis]
            output = (distance==distance.min(axis=0)[np.newaxis,:]).T 
            output = output.astype("int")
            assigned_labels.append([X[i:i+1,:],output])
            #output=[1,0,0] if first class is winner and so on.
            #So multiplication with "output" in below, 
            #provides update for the winner weight only.
            
            #weight update
            W += eta*(np.dot(output.T,X[i:i+1,:])
                     -output.sum(axis=0)[:,np.newaxis]*W)
            
            #normalize the weights
            W = normalize(W, norm='l2', axis=1)
                  
        trj.append(W)
        #if weights doesn't change then break
        if (trj[epoch] == trj[epoch-1]).all():
            break
            
    return W,trj,assigned_labels


# Adaptive Resonance Theory algorithm
def art(X, rho, eta):
    """
    X : data matrix
    C : initial centers
    rho: vigilance parameter
    """
    C = initialize_centroids(X,1)
    labels = []
    for x in X:
        d = np.min(((C - x)**2).sum(1))
        if d < rho:
            j = np.argmin(((C - x)**2).sum(1))
            C[j] = (1.0 - eta) * C[j] + eta * x
            labels.append(j)
        else:
            C = np.append(C,np.array([x]),axis=0)
            j = np.argmin(((C - x)**2).sum(1))
            labels.append(j)
    
    return C, total_time, np.array(labels)

