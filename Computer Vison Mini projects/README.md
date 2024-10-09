# Computer Vision Mini Projects

This repository contains several mini-projects centered on computer vision and image processing. Each project was provided as part of the lecture to explore various topics covered and apply them to practical tasks.

## 1. Image Compression 
Developed a software solution to compress noisy images captured from a gaming console during a tic-tac-toe championship for efficient transmission over the internet. The goal was to compress the images using a codebook (e.g., via PCA) and reconstruct them on the client side. The performance is evaluated based on codebook size, compression ratio, and reconstruction quality.

Key Features:
- Compression of noisy 96x96 RGB images.
- Codebook generation using PCA.
- Reconstruction of images with minimal reconstruction error.

---

## 2. Interactive Segmentation 
Created a segmentation algorithm to extract the foreground (a cat) from an image based on user-provided scribbles marking the foreground and background. This algorithm enables users to easily segment and paste their cat onto different backgrounds to create custom memes.

Key Features:
- K-means clustering for foreground and background separation.
- Patch extraction to enhance segmentation accuracy.
- Evaluation using the Intersection-over-Union (IoU) metric.

---

## 3. Stereo 3D Reconstruction
Solved the stereo vision problem by reconstructing 3D world coordinates from two images taken from different points of view. The final output is a point cloud that visualizes the 3D scene.

Key Features:
- Automatic camera calibration.
- Solving the correspondence problem using Normalized Cross-Correlation (NCC).
- 3D triangulation to obtain a dense distance map and certainty scores.

---

## 4. Image Classification
Trained a convolutional neural network (CNN) to classify images into one of six categories: buildings, forests, mountains, glaciers, seas, and streets. The focus of this project was to design and optimize a neural network for accurate classification using PyTorch.

Key Features:
- Implementation of a basic CNN architecture.
- Data augmentation and mini-batch training.
- Evaluation using classification accuracy on the test set.

---

## 5. Transfer Learning for Image Classification
Extended an existing pre-trained ResNet model to classify images into ten categories, using transfer learning techniques. With a limited dataset, transfer learning enabled the network to generalize better from pre-existing knowledge.

Key Features:
- Application of transfer learning with a ResNet model.
- Adaptation of the model to classify additional unseen classes.
- Optimization for higher accuracy using data augmentation and other techniques.

---
