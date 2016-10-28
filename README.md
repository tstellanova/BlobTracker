# BlobTracker

Demonstrates how to track a colored blob using the OpenCV C++ API.

The method is:

- Open a VideoCapture. In this example we use an input video file, though you can open a camera as well.
- Capture a frame from the VideoCapture
- Blur the image a little to eliminate noise divisions between parts of the blob
- Threshold the image (use the HSV color provided to mask out non-relevant colors)
- Find contours that outline the blobs that remain in the thresholded image
- Find the convex hull of one contour
- Find the moments of the convex hull, from which we can derive the center of the blob

