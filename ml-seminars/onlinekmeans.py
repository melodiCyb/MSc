


# Online k-means algorithm using SGD
def onlinekmeans(X, k):
    """ Online k-means using stochastic gradient descent.
    X : data matrix
    C : initial centers
    eta: 1 / n_k gradually decreasing learning rate
    """
    C = initialize_centroids(X,k)
    cluster_etas = np.zeros(C.shape[0])
    labels = []
    for x in X:
        j = np.argmin(((C - x)**2).sum(1))
        cluster_etas [j] += 1
        eta = 1.0 / cluster_etas [j]
        # update only the centroid with minimum distance
        C[j] = (1.0 - eta) * C[j] + eta * x
        labels.append(j)
  
    return C,np.array(labels),total_time




# Fully online k-means algorithm
def fully_online(X, k):
    """
    X : data matrix
    C : initial centers
    f:  facility cost
    """
    
    C = initialize_centroids(X, k+1)
    n = k+1
    dists = []
    for x in C:
        d = ((C - x)**2).sum(1)
        d = [i for i in d if np.abs(i) > 1e-5]
        dists.append(d)
    j = np.min(dists)/2
    f = j / k
    r=1
    q = 0
    for x in X:
        n += 1
        D2 = np.min((C - x).sum(1))**2
        p = np.min((D2/f, 1))
        if np.random.random() < p:
            C = np.append(C,np.array([x]), axis=0)
            q += 1
        if q >= 3*k*(1+np.log(n)):
            r += 1
            q = 0
            f = 2*f
       
    return C, total_time




def modified_fully_online(X, k_target):
    """
    Modified algorithm in (Liberty et al.) for experimental design.
    X : data matrix
    k_target :  the number of clusters we would like the algorithm to output
    """
    k = (k_target - 15) / 5
    C = initialize_centroids(X, 10)
    n = k+1
    dists = []
    for x in C:
        d = ((C - x)**2).sum(1)
        d = [i for i in d if np.abs(i) > 1e-5]
        dists.append(d)
    j = np.sum(sorted(dists)[:10])/2
    f = j
    r = 1
    q = 0
    for x in X:
        n += 1
        D2 = np.min((C - x).sum(1))**2
        p = np.min((D2/f, 1))
        if np.random.random() < p:
            C = np.append(C, np.array([x]), axis=0)
            q += 1
        if q >= k:
            r += 1
            q = 0
            f = 10*f
        
    return C, total_time

