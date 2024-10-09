
import numpy as np

class ImageCompressor:
    """
      This class is responsible to
          1. Learn the codebook given the training images
          2. Compress an input image using the learnt codebook
    """
    def __init__(self):
        """
        Feel free to add any number of parameters here.
        But be sure to set default values. Those will be used on the evaluation server
        """
        
        # Here you can set some parameters of your algorithm, e.g.
        self.dtype = np.float16
        self.compressed_U = None
        self.z = None
        self.mean = None

    def get_codebook(self):
        """ Codebook contains all information needed for compression/reconstruction """
        
        # TODO ...
        #codebook = np.array([])
        #print(self.mean.shape)
        print("mean shape:")
        print(self.mean.shape)

        codebook = np.concatenate((self.compressed_U, self.mean), axis=-1)
        #codebook = np.concatenate((self.mean.reshape(-1), self.compressed_U.reshape(-1)))
        print("codebook shape", codebook.shape)
        return codebook.astype(self.dtype)
        

    def train(self, train_images):
        """
        Training phase of your algorithm - e.g. here you can perform PCA on training data
        
        Args:
            train_images  ... A list of NumPy arrays.
                              Each array is an image of shape H x W x C, i.e. 96 x 96 x 3
        """
        N = len(train_images)

        # TODO ...
        # train the codebook with the data you have
        # you can use the function np.linalg.svd to perform PCA 
        stacked_images = np.stack(train_images, axis=-1)
        print("stacked_images shape:", stacked_images.shape)
        stacked_images = (stacked_images.astype(np.float64) / 255.0)

        #print(stacked_images.shape)
        stacked_images = stacked_images.reshape(-1,N)
        print("stacked_images shape:", stacked_images.shape)
        self.mean = np.mean(stacked_images, axis=-1, keepdims=True)
        print("mean shape:", self.mean.shape)
        demeaned_images = stacked_images - self.mean
        print("demeaned:", demeaned_images.shape)
        ###### first try ######

        print(demeaned_images.shape)
        U, S, V = np.linalg.svd(demeaned_images, full_matrices=False)

        num_components = 25
        print("U shape:", U.shape)
        self.compressed_U = U[:, :num_components]
        #print("U shape:" ,self.compressed_U.shape)
        print("U compressed:", self.compressed_U.shape)
        

    def compress(self, test_image):
        """ Given an array of shape H x W x C return compressed code """

        # TODO
        # compress the test image using the codebook
        U_transpose = np.transpose(self.compressed_U)
        # test_image_mean = np.mean(test_image, axis=-1, keepdims=True)
        print("test image", test_image.shape)  
        print("test image reshape", test_image.astype(np.float64).reshape(-1,1).shape)
        test_image= np.expand_dims(test_image,-1)
        print("test image expand", test_image.shape)
        test_image = test_image.reshape(-1,1) / 255.0 - self.mean
        z = np.dot(U_transpose, test_image.reshape(-1,1))
        print("utrans", U_transpose.shape)
        print("z size", z.shape)
        test_code = z #np.array([])
        return test_code.astype(self.dtype)


class ImageReconstructor:
    """ This class is used on the server to reconstruct images """
    def __init__(self, codebook):
        """ The only information this class may receive is the codebook """
        self.codebook = codebook
        
    def reconstruct(self, test_code):
        """ Given a compressed code of shape K, reconstruct the original image """
        full_codebook = self.codebook
        codebook = full_codebook[:, :-1]
        mean = full_codebook[:, -1:] 
        print(mean)
        recunstructed_image = (np.dot(codebook, test_code) + mean)
        print(recunstructed_image.shape)
        # recunstructed_image = np.clip(recunstructed_image, 0, 255)
        recunstructed_image = recunstructed_image.reshape(96,96,3)* 255.0
        # TODO
        return recunstructed_image.astype(np.uint8) 