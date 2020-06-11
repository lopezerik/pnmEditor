# pnmEditor  

This is a pnm image editor built using C/C++. This is an object oriented program and is run using  
the command line. There are a number of modifications you can make to an image such as mirroring  
it, rotating it, or joining it together with another image.  

# How To  

The recommended way to use this program is in the command line. You can run the program right  
away without any modifications.  
### Example  
1. `make` - Run the makefile in order to compile the program.  
2. `./pnmEditor input.pnm output.pnm` - If `output.pnm` does not exist when the program is run  
the program will create it. The program will overwrite ‘output.pnm’ if the file already exists.  
If you forget to add `.pnm` to the end of the output file, rename the file with `.pnm` at the end  
and the file will work as intended.  
3. Done.  

### Create your own  
In order to modify what filters are applied to the input image, you will have to open  
`mainEditor.C` and create your own filter hierarchies. This file already contains an example of a  
valid filter setup which you can use as a template. It is recommended to create your filter  
objects in the order you intend to use them to avoid confusion and errors. Next, pass the  
`inputImage` to your first filter using the `SetInput()` method. Use the `GetOutput` method in  
order to use the result of a filter as the input of another filter. Once you have lined up all of  
your filters, call the `Update()` method on the final image from the final filter.  
```C++
finalFilter.GetOutput()->Update()
```
Update the pointer `finalImage` to point to the result of  
the final filter. 
``` C++
Image* finalImage = finalFilter.GetOutput()
```

# Filters  
This is an explanation of the various filters and examples of their usage.  

* <b>Shrink</b>  
This filter shrinks your image in half. Final image has half the width and half the height of the  
original image.    
    ```C++  
    Shrink shrink;                    // Create Shrink object
    shrink.setInput(Image* a);        // Give the shrink object an image to shrink
    shrink.GetOutput()->Update();     // Execute the update
    ```  

* <b>joinLR</b>
This filter attaches two images side by side. The two images must be of equal height in order to  
join them together. The two images are allowed to have different widths.  
    ```C++
    joinLR lr;                   // Create joinLR object
    lr.setInput(Image* a);       // Give the object the left image
	lr.setInput2(Image* b);      // Give the object the right image
	lr.GetOutput()->Update();    // Execute the update
    ```  

* <b>joinTB</b>
This filter attaches one image on top of another. The two images must be of equal width in order  
to join them together. The two images are allowed to have different heights.  
    ```C++
    joinTB tb;                    // Create joinTB object
    tb.setInput(Image* a);        // Give the object the top image
    tb.setInput2(Image* b);       // Give the object the bottom image
    tb.GetOutput()->Update();     // Execute the update
    ```  

* <b>Blend</b>
This filter takes two images and blends them together. You can decide the intensity of the filter  
by setting the `factor` to different values. For example, a factor of 0.7 would result in a final  
image that is 70% of Input 1 and 30% of Input 2. Factor values must be above 0.5 for the filter  
to work properly. Images should have the same width and height in order for this filter to work  
properly.
    ```C++
    Blend bl;                     // Create Blend object
    bl.SetFactor(float f);        // Set the blend factor
    bl.SetInput(Image* a);        // Give the object the left image
    bl.SetInput2(Image* b);       // Give the object the right image
    bl.GetOutput()->Update();     // Execute the update
    ```  

* <b>Mirror</b>
This filter takes an image and flips it over the y-axis. In other words, it takes the image and  
flips it horizontally. 
    ```C++
    Mirror mr;                    // Create the Mirror object
    mr.setInput(Image* a);        // Give the Mirror object the image
    mr.GetOutput()->Update();     // Execute the update
    ```  

* <b>Rotate</b>
This filter takes an image and rotates it 90 degrees clockwise. Keep in mind that the width and  
height swap when a rotation is performed. This can cause issues if you are trying to use joinLR  
or joinTB on rotated images. If the original image had a height H and a width W, the result has a  
height W and a width H.  
    ```C++
    Rotate rt;                   // Create Rotate object
	rt.SetInput(Image a);        // Pass the image to the object
	rt.GetOutput()->Update();    // Execute the update
    ```  

* <b>Subtract</b>
This filter takes the difference between two images (Result = Input1 - Input2). This is  
accomplished by subtracting the difference between color channels in each pixel. Both images must  
have the same exact dimensions for this filter to work.  
    ```C++
    Subtract sb;                 // Create Subtract object
    sb.SetInput(Image a);        // Pass in Input 1
    sb.SetInput2(Image b);       // Pass in Input 2
    sb.GetOutput()->Update();    // Execute the update
    ```  

* <b>Grayscale</b>
This filter essentially removes the color from an image by reducing the values in the color  
channels.  
The calculations for the color channel values are:  
    - Red / 5
    - Green / 2
    - Blue / 4  
The values are calculated using the C/C++ division operator `/` and not the modulus operator `%`.  
    ```C++
    Grayscale gs;              // Create Grayscale object
    gs.SetInput(Image a);      // Pass in image
    gs.GetOutput()->Update;    // Execute update
    ```  

* <b>Blur</b>
This filter blurs an image by setting the color value of each pixel to the average value of the  
pixels that neighbor it. Pixels with less than 8 neighbors (meaning pixels along the edge of an  
image) are not modified.  
    ```C++
    Blur br;                     // Create Blur object
    br.SetInput(Image a);        // Pass in image
    br.GetOutput()->Update();    // Execute update
    ```  


