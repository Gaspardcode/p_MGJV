
Welcome. 
<br /> 

- [Install](#install)
- [Navigation](#navigation)
- [Troubleshooting](#troubleshooting)
- [Credits](#credits)


## Install

To start the application, first clone the repository.  <br />
```git
git clone https://github.com/Gaspardcode/p_MGJV.git
```

Then execute the following command:  <br />

```bash
 cd src/graphics && make -B && ./graphics
```

You are now ready to use the OCR.
 <br />  <br />


## Navigation  
<br />

Our application features 11 buttons.  <br />
1. BLACK AND WHITE : can be used to convert an image in black and white.
2. HOUGH : performes a canny edge followed by a hough transform.
            displays the lines found. <br />
3. ROTATION : opens a menu to rotate the image. <br />
4. GRID DISPLAY : performs a flood fill to find the grid of the sudoku.
                    displays the result. <br />
5. CROP : displays the image sent to the neural network to be analysed. <br />
6. FULL SOLUTION : displays the entire solution of the sudoku. <br />
7. STEP BY STEP : displays the hidden digits, one by one.
                    each press display another digit solved. <br />
8. LOAD IMAGE : can be used to upload your own image.
                any format : png, jpg, jpeg. <br />
9. BACK : when pressed, comes back to the last image displayed. <br />
10. DRAW : enters a menu to test the IA.
            The user can draw a number, which the IA shall try to find out. <br />
11. QUIT : allows to quit the application. <br />

## Troubleshooting
<br/>

If the application crashs, which it certainly can, and you cannot restart it, try to run the following : <br/>
```bash
make clean && make -B && ./graphics
```

## Credits 
<br />
The project was made possible by SDL and GTK. <br />
Credits to my workmates : <br />
Medhi Azouz : https://gitmehdii.github.io/ <br />
Victor Agahi : https://victoragahi.github.io/ <br />
Jules Magnan : https://magnanjules.github.io/ <br />

Feel free to explore. The subfolder digit_recognition contains a self-generated dataset. <br />
