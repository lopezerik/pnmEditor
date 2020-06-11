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

