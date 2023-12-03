# IC_Project2

# Authors and Contribuitions
- Bernardo Marçal - 103236 - 33%
- Ricardo Machado - 102737 - 33%
- Rui Campos - 103709 - 33%
  
Install OpenCV library
https://www.geeksforgeeks.org/how-to-install-opencv-in-c-on-linux/

## TO BUILD:
Go to 'IC_Project2/programs' and run
```bash
make all
```
## TESTING:
Go to 'IC_Project2/opencv-bin' and run the respective exercise you want, as listed below
### Exercise 1 
### Color Extraction - Blue, Green and Red
To test and check the picture of blue pixels, run:
```bash
./program1 ../imagensPPM/arial.ppm arial_changedblue.ppm 1
```
To test and check the picture of green pixels, run:
```bash
./program1 ../imagensPPM/arial.ppm arial_changedgreen.ppm 2
```
To test and check the picture of red pixels, run:
```bash
./program1 ../imagensPPM/arial.ppm arial_changedred.ppm 3
```
### Exercise 2
### 2 a) Negative Image
To test and check the picture in negative values, run:
```bash
./program2a ../imagensPPM/arial.ppm arial_negative.ppm
```
### 2 b) Image Mirroring
To check the picture mirrored in the horizontal axis, run:
```bash
./program2b ../imagensPPM/arial.ppm arial_horizontal.ppm h
```

To check the picture mirrored in the vertical axis, run:
```bash
./program2b ../imagensPPM/arial.ppm arial_vertical.ppm v
```

### 2 c) Image Rotation
To check the picture rotated 90 degrees, run:
```bash
./program2c ../imagensPPM/arial.ppm arial_90.ppm 90
```

To check the picture rotated 180 degrees, run:
```bash
./program2c ../imagensPPM/arial.ppm arial_180.ppm 180
```

If you want to test values by yourself, make sure to substitute the 'x' by multiples of 90, run:
```bash
./program2c ../imagensPPM/arial.ppm arial_x.ppm x
```
### 2 d) Light Increase/Decrease Image
To test and check, run the code below, where you should choose a number between 0 and 2 to increase/decrease the light levels
```bash
./program2d ../imagensPPM/arial.ppm arial_x.ppm x 
```
### Exercise 3
class (em conjunto com Rafa e net)

### Exercise 4
para testar
```bash
./program4enc ../AudioFiles/sample01.wav sample01_enc.wav 256
./program4dec sample01_enc sample01_decoded
```



