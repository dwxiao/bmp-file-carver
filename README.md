# bmp-file-carver

A basic file carver that detects if a block of bytes is related to a .bmp file or not, and extracts and outputs them to a specified directory.

## Example input

The original .bmp file:

![original](https://raw.githubusercontent.com/davidshower/bmp-file-carver/master/mystery.bmp?token=AB3kBC75tNIDM-qaUSW4R83kO2IZ9yBhks5YxJF3wA%3D%3D)

## Example output

The first block of bytes (the whole image) which is the original input .bmp file:

![output1](https://raw.githubusercontent.com/davidshower/bmp-file-carver/master/mystery_Output/0.bmp?token=AB3kBJ5I1RbaD5TCbFBJdgxZA9STrtekks5YxJGSwA%3D%3D)

The second block of bytes within the file, which is a misspelling of 'surprise':

![output2](https://raw.githubusercontent.com/davidshower/bmp-file-carver/master/mystery_Output/130448.bmp?token=AB3kBGkD_ETznkGfXVGKJ-2d2ZyneIXNks5YxJGxwA%3D%3D)
---
#### Additional notes
*Not demonstrated above is the ability to extract and output blocks of bytes that are not related to .bmp files.*
