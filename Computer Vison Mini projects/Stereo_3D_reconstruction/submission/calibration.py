import numpy as np


def compute_mx_my(calib_dict):
    """
    Given a calibration dictionary, compute mx and my (in units of [px/mm]).
    
    mx -> Number of pixels per millimeter in x direction (ie width)
    my -> Number of pixels per millimeter in y direction (ie height)
    """
    
    #
    # TO IMPLEMENT
    #
    mx, my = calib_dict['width']/calib_dict['aperture_w'], calib_dict['height']/calib_dict['aperture_h']

    return mx, my


def estimate_f_b(calib_dict, calib_points, n_points=None):
    """
    Estimate focal lenght f and baseline b from provided calibration points.

    Note:
    In real life multiple points are useful for calibration - in case there are erroneous points.
    Here, this is not the case. It's OK to use a single point to estimate f, b.
    
    Args:
        calib_dict (dict)           ... Incomplete calibaration dictionary
        calib_points (pd.DataFrame) ... Calibration points provided with data. (Units are given in [mm])
        n_points (int)              ... Number of points used for estimation
        
    Returns:
        f   ... Focal lenght [mm]
        b   ... Baseline [mm]
    """
    if n_points is not None:
        calib_points = calib_points.head(n_points)
    else: 
        n_points = len(calib_points)

    #
    # TO IMPLEMENT
    #
    f, b = -1, -1
    fx = ((calib_points['ul [px]'] - calib_dict['o_x']) * calib_points['Z [mm]'] / calib_points['X [mm]'])
    fx = fx.mean()
    mx, my = compute_mx_my(calib_dict)
    f = fx / mx
    num = (calib_points['ul [px]'] - calib_points['ur [px]']) * calib_points['Z [mm]']
    num_mean  = num.mean()
    b = num_mean / fx

    return f, b