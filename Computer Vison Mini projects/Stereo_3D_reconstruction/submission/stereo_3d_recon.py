import numpy as np

from calibration import compute_mx_my, estimate_f_b
from extract_patches import extract_patches


def triangulate(u_left, u_right, v, calib_dict):
    """
    Triangulate (determine 3D world coordinates) a set of points given their projected coordinates in two images.
    These equations are according to the simple setup, where C' = (b, 0, 0)

    Args:
        u_left  (np.array of shape (num_points,))   ... Projected u-coordinates of the 3D-points in the left image
        u_right (np.array of shape (num_points,))   ... Projected u-coordinates of the 3D-points in the right image
        v       (np.array of shape (num_points,))   ... Projected v-coordinates of the 3D-points (same for both images)
        calib_dict (dict)                           ... Dict containing camera parameters
    
    Returns:
        points (np.array of shape (num_points, 3)   ... Triangulated 3D coordinates of the input - in units of [mm]
    """
    #
    # TO IMPLEMENT
    #
    mx,my = compute_mx_my(calib_dict)
    epsilon = 1e-5
    z = calib_dict['b'] * calib_dict['f'] * mx / (u_left - u_right + epsilon)
    x = z * (u_left - calib_dict['o_x']) / (calib_dict['f'] * mx)
    y = z * (v - calib_dict['o_y']) / (calib_dict['f'] * my)

    return np.array([x, y, z])


def compute_ncc(img_l, img_r, p):
    """
    Calculate normalized cross-correlation (NCC) between patches at the same row in two images.
    
    The regions near the boundary of the image, where the patches go out of image, are ignored.
    That is, for an input image, "p" number of rows and columns will be ignored on each side.

    For input images of size (H, W, C), the output will be an array of size (H - 2*p, W - 2*p, W - 2*p)

    Args:
        img_l (np.array of shape (H, W, C)) ... Left image
        img_r (np.array of shape (H, W, C)) ... Right image
        p (int):                            ... Defines square neighborhood. Patch-size is (2*p+1, 2*p+1).
                              
    Returns:
        corr    ... (np.array of shape (H - 2*p, W - 2*p, W - 2*p))
                    The value output[r, c_l, c_r] denotes the NCC between the patch centered at (r + p, c_l + p) 
                    in the left image and the patch centered at  (r + p, c_r + p) at the right image.
    """

    # Add dummy channel dimension
    if img_l.ndim == 2:
        img_l = img_l[:, :, None]
        img_r = img_r[:, :, None]
    
    assert img_l.ndim == 3, f"Expected 3 dimensional input. Got {img_l.shape}"
    assert img_l.shape == img_r.shape, "Shape mismatch."
    
    H, W, C = img_l.shape

    # Extract patches - patches_l/r are NumPy arrays of shape H, W, C * (2*p+1)**2
    patches_l = extract_patches(img_l, 2*p+1)
    patches_r = extract_patches(img_r, 2*p+1)
    
    # Standardize each patch
    #
    mean_l = patches_l.mean(axis=2, keepdims=True)
    std_l = patches_l.std(axis=2, keepdims=True)
    patches_l = (patches_l - mean_l) / (std_l + 1e-8)

    mean_r = patches_r.mean(axis=2, keepdims=True)
    std_r = patches_r.std(axis=2, keepdims=True)
    patches_r = (patches_r - mean_r) / (std_r + 1e-8)

    # TO IMPLEMENT
    #
    
    # Compute correlation (using matrix multiplication) - corr will be of shape H, W, W
    #
    # TO IMPLEMENT
    #
    corr = np.zeros((H, W, W))
    for i in range(H):
        corr[i] = np.matmul(patches_l[i], patches_r[i].T) / (C * (2*p+1)**2)
    print(corr.shape)
    # Ignore boundaries
    return corr[p:H-p, p:W-p, p:W-p]


class Stereo3dReconstructor:
    def __init__(self, p=6, w_mode='none'):
        """
        Feel free to add hyper parameters here, but be sure to set defaults
        
        Args:
            p       ... Patch size for NCC computation
            w_mode  ... Weighting mode. I.e. method to compute certainty scores
        """
        self.p = p
        self.w_mode = w_mode

    def fill_calib_dict(self, calib_dict, calib_points):
        """ Fill missing entries in calib dict - nothing to do here """
        calib_dict['mx'], calib_dict['my'] = compute_mx_my(calib_dict)
        calib_dict['f'], calib_dict['b'] = estimate_f_b(calib_dict, calib_points)
        
        return calib_dict


    def recon_scene_3d(self, img_l, img_r, calib_dict):
        """
        Compute point correspondences for two images and perform 3D reconstruction.
        Args:
            img_l (np.array of shape (H, W, C)) ... Left image
            img_r (np.array of shape (H, W, C)) ... Right image
            calib_dict (dict)                   ... Dict containing camera parameters
        Returns:
            points3d (np.array of shape (H, W, 4))
                Array containing the re-constructed 3D world coordinates for each pixel in the left image.
                Boundary points - which are not well defined for NCC might be padded with 0s.
                4th dimension holds the certainties.
        """
        
        # Add dummy channel dimension if grayscale images
        if img_l.ndim == 2:
            img_l = img_l[:, :, None]
            img_r = img_r[:, :, None]
        
        assert img_l.ndim == 3, f"Expected 3 dimensional input. Got {img_l.shape}"
        assert img_l.shape == img_r.shape, "Shape mismatch."
        
        H, W, C = img_l.shape
        
        # Adjust the size considering the patch size
        H_small, W_small = H - 2*self.p, W - 2*self.p
        
        # Adjust calibration parameters for the reduced image size
        calib_small = calib_dict.copy()
        calib_small['height'], calib_small['width'] = H_small, W_small
        calib_small['o_x'] = calib_dict['o_x'] - self.p
        calib_small['o_y'] = calib_dict['o_y'] - self.p
        
        # Create (u, v) pixel grid
        y, u_left = np.meshgrid(
            np.arange(H_small, dtype=float),
            np.arange(W_small, dtype=float),
            indexing='ij'
        )
        corr = compute_ncc(img_l, img_r, self.p)
        #corr = np.tril(corr)
        u_right = np.argmax(corr, axis=2)
        disparity = (u_left - u_right)
        # # median_disp = np.median(disparity)
        mean_disp = disparity.mean()
        disparity[disparity < 0] = mean_disp

        u_right = u_left - disparity
        
        if self.w_mode == 'none':
            certainty_score = np.ones((H_small, W_small), dtype=float)
        elif self.w_mode == 'ncc':
            certainty_score = np.max(corr, axis=2)
        else:
            raise NotImplementedError("Implement your own certainty estimation")
        
        disparity_std = np.std(disparity)
        disparity_mean = np.mean(disparity)
        certainty_score *= np.exp(-((disparity - disparity_mean) ** 2) / (2 * (disparity_std ** 2)))
        
        points = triangulate(
            u_left.flatten(), u_right.flatten(), y.flatten(), calib_small
        )

        points = points.T.reshape(H_small, W_small, 3)

        points = np.pad(points, ((self.p, self.p), (self.p, self.p), (0, 0)))
        certainty_score = np.pad(certainty_score, self.p)[:, :, None]
        return np.concatenate([points, certainty_score], axis=2)
        