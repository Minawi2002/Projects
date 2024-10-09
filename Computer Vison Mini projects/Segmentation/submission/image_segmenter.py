import numpy as np

from kmeans import (
    compute_distance,
    kmeans_fit,
    kmeans_predict_idx
)

from extract_patches import extract_patches
from advanced_methods import perform_min_cut


class ImageSegmenter:
    def __init__(self, k_fg=2, k_bg=5, mode='kmeans'):
        """ Feel free to add any hyper-parameters to the ImageSegmenter.
            
            But note:
            For the final submission the default hyper-parameteres will be used.
            In particular the segmetation will likely crash, if no defaults are set.
        """
        
        # Number of clusters in FG/BG
        self.k_fg = k_fg
        self.k_bg = k_bg
        
        self.mode= mode
        
    def extract_features_(self, sample_dd):
        """ Extract features, e.g. p x p neighborhood of pixel, from the RGB image """
        
        img = sample_dd['img']
        H, W, C = img.shape
        P = 3
        #
        # TO IMPLEMENT
        
        # Extract patches of size p x p
        patches = extract_patches(img, P)
        ############# Correct ############
        # print("patches size:", patches.shape)
        # return patches
        ###################################

        ######## BETTER RESULTS ##########
        x = np.linspace(-1, 1, W).reshape(1, W, 1)
        y = np.linspace(-1, 1, H).reshape(H, 1, 1)
        xy = np.concatenate([y.repeat(W, axis=1), x.repeat(H, axis=0)], axis=2)
        xy_tiled = np.tile(xy, (1, 1, patches.shape[2]//C))
        features = np.concatenate([patches, xy_tiled], axis=-1)
        
        return features
        ###################################



    def segment_image_dummy(self, sample_dd):
        return sample_dd['scribble_fg']

    # def segment_image_kmeans(self, sample_dd):
    #     """ Segment images using k means """
    #     H, W, C = sample_dd['img'].shape
    #     # # TO IMPLEMENT

    #     features = self.extract_features_(sample_dd)
    #     img = sample_dd['img']
    #     fg_1 = np.argwhere(sample_dd['scribble_fg'])
    #     bg_1 = np.argwhere(sample_dd['scribble_bg'])
    #     print(fg_1.shape)
    #     fg_patches = features[fg_1[:, 0], fg_1[:, 1]]
    #     bg_patches = features[bg_1[:, 0], bg_1[:, 1]]
    #     print(fg_patches.shape)
    #     fg_clusters = kmeans_fit(fg_patches, self.k_fg)
    #     bg_clusters = kmeans_fit(bg_patches, self.k_bg)
    #     print(fg_clusters.shape)
    #     all_clusters = np.vstack([fg_clusters, bg_clusters])
    #     all_features_2d = features.reshape(-1, features.shape[-1])
    #     labels = kmeans_predict_idx(all_features_2d, all_clusters)
    #     fg_mask = labels < self.k_fg

    #     return fg_mask.reshape(H, W)
    
    def segment_image_kmeans(self, sample_dd):
        """ Segment images using k means with spatial coordinates """
        img = sample_dd['img']
        H, W, C = img.shape
        features = self.extract_features_(sample_dd)
        x = np.linspace(0, 1, W) 
        y = np.linspace(0, 1, H)  
        x, y = np.meshgrid(x, y)
        xy_coords = np.stack((x, y), axis=-1)
        augmented_features = np.concatenate([features, xy_coords], axis=-1)
        fg_coords = np.argwhere(sample_dd['scribble_fg'])
        bg_coords = np.argwhere(sample_dd['scribble_bg'])
        fg_samples = augmented_features[fg_coords[:, 0], fg_coords[:, 1]]
        bg_samples = augmented_features[bg_coords[:, 0], bg_coords[:, 1]]
        
        # fg_clusters = kmeans_fit(fg_samples, self.k_fg)
        # bg_clusters = kmeans_fit(bg_samples, self.k_bg)
        # all_clusters = np.vstack([fg_clusters, bg_clusters])
        # all_features_2d = augmented_features.reshape(-1, augmented_features.shape[-1])
        # labels = kmeans_predict_idx(all_features_2d, all_clusters)

        labels = self.get_labels(fg_samples,bg_samples,augmented_features)
        fg_mask = labels < self.k_fg
        return fg_mask.reshape(H, W)

    def get_labels(self,fg_samples,bg_samples,augmented_features):
        fg_clusters = kmeans_fit(fg_samples, self.k_fg)
        bg_clusters = kmeans_fit(bg_samples, self.k_bg)
        all_clusters = np.vstack([fg_clusters, bg_clusters])
        all_features_2d = augmented_features.reshape(-1, augmented_features.shape[-1])
        labels = kmeans_predict_idx(all_features_2d, all_clusters)
        return labels



    def segment_image_grabcut(self, sample_dd):
        """ Segment via an energy minimisation """

        # Foreground potential set to 1 inside box, 0 otherwise
        unary_fg = sample_dd['scribble_fg'].astype(np.float32) / 255

        # Background potential set to 0 inside box, 1 everywhere else
        unary_bg = 1 - unary_fg

        # Pairwise potential set to 1 everywhere
        pairwise = np.ones_like(unary_fg)

        # Perfirm min cut to get segmentation mask
        im_mask = perform_min_cut(unary_fg, unary_bg, pairwise)
        
        return im_mask

    def segment_image(self, sample_dd):
        """ Feel free to add other methods """
        if self.mode == 'dummy':
            return self.segment_image_dummy(sample_dd)
        
        elif self.mode == 'kmeans':
            return self.segment_image_kmeans(sample_dd)
        
        elif self.mode == 'grabcut':
            return self.segment_image_grabcut(sample_dd)
        
        else:
            raise ValueError(f"Unknown mode: {self.mode}")