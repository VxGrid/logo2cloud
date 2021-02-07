# logo2cloud
Transform a logo (image) into a point cloud

The GUI was created using Qt: https://www.qt.io/

If you like to contribute by adding translations, proposing ideas, or code, feel free to contact me.


Dependencies to the project:

-C++ 17  
-Qt (must)  
-LASzip (optional; https://laszip.org/ - necessary for LAS and LAZ export)  

1. Open an image (png, jpg, svg, ...)
2. "Choose disabled color", will set the background to that color, so you can see which pixel in the image have an alpha value of zero and will not get exported
3. Select pixel in the image with the mouse button, then all pixel with the same color get selected
4. To remove the selected color from export click "Remove color"
5. The checkbox "P2P" stands for pixel to point, meaning exactly one pixel gets converted in one 3d point
6. "Height", "Width" and "Depth" is for defining the resulting size of the point cloud
7. "Spacing" shows how far apart two neighbouring points are (the space between them)
8. With "Noise", for every coordinate in every point, a random offset between 0.0 and the choosen value will be applied to the complete cloud. This adds Noise
9. "~points" shows how many points will get exported, given no pixel were removed. From the export removed pixel are not considered, therefore the approx sign
10. "Export" will open a dialog to choose the export file name and format. Currently there is LAS, LAZ and XYZ
