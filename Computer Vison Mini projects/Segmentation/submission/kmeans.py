import numpy as np


def kmeans_fit(data, k, n_iter=500, tol=1.e-4):
    """
    Fit kmeans
    
    Args:
        data ... Array of shape n_samples x n_features
        k    ... Number of clusters
        
    Returns:
        centers   ... Cluster centers. Array of shape k x n_features
    """
    N, P = data.shape
    

    rng = np.random.default_rng(6174)

    centroids = data[rng.choice(N, k, replace=False)]
    
    prev_centroids = np.zeros_like(centroids)
    
    for _ in range(n_iter):
        distances = compute_distance(data, centroids)
        labels = np.argmin(distances, axis=1)
        new_centroids = np.array([data[labels == i].mean(axis=0) for i in range(k)])
        if np.linalg.norm(new_centroids - centroids) < tol:
            break
            
        centroids = new_centroids
        
    return centroids


def compute_distance(data, clusters):
    """
    Compute all distances of every sample in data, to every center in clusters.
    
    Args:
        data     ... n_samples x n_features
        clusters ... n_clusters x n_features
        
    Returns:
        distances ... n_samples x n_clusters
    """
    
    # TO IMPLEMENT
    minus_clus = data[:, np.newaxis] - clusters
    distances = np.linalg.norm(minus_clus, axis=2)
    return distances


def kmeans_predict_idx(data, clusters):
    """
    Predict index of closest cluster for every sample
    
    Args:
        data     ... n_samples x n_features
        clusters ... n_clusters x n_features
    """
    # TO IMPLEMENT
    distances = compute_distance(data, clusters)
    idx = np.argmin(distances, axis=1)
    #print(idx)
    return idx