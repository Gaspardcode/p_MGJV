/* install tutorial */
Welcome. \n
To start the application, first clone the repository.
Then execute the following command:

cd src/graphics && make -B && ./graphics

You are now ready to use our OCR.

/* Navigation tutorial */

Our application features 11 buttons.
1. BLACK AND WHITE : can be used to convert an image in black and white.
2. HOUGH : performes a canny edge followed by a hough transform.
            displays the lines found.
3. ROTATION : opens a menu to rotate the image.
4. GRID DISPLAY : performs a flood fill to find the grid of the sudoku.
                    displays the result.
5. CROP : displays the image sent to the neural network to be analysed.
6. FULL SOLUTION : displays the entire solution of the sudoku.
7. STEP BY STEP : displays the hidden digits, one by one.
                    each press display another digit solved.
8. LOAD IMAGE : can be used to upload your own image.
                any format : png, jpg, jpeg.
9. BACK : when pressed, comes back to the last image displayed.
10. DRAW : enters a menu to test the IA.
            The user can draw a number, which the IA shall try to find out.
11. QUIT : allows to quit the application.

/* NB */
The project was made possible by SDL and GTK.
Credits to my workmates :
Medhi Azouz : https://gitmehdii.github.io/
Victor Agahi : https://victoragahi.github.io/
Jules Magnan : https://magnanjules.github.io/
Feel free to explore. The subfolder digit_recognition contains a self-generated database.
