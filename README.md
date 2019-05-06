# EE599 final project
This project implements the Convolutional Recurrent Neural Network (CRNN) in pytorch.

Run demo
--------
Before running the demo, downolad and install pytorch and lmdb first.
Run the demo by
`python demo.py`
Demo program can load the CRNN model and recoginze the text content of an image.

Train a new model
-----------------
Use create_dataset.py to create your own training set.
Execute ``python train.py --adadelta --trainRoot {train path} --valRoot {val path} '--displayInterval {Interval to be displayed} --expr_dir {output path} --cuda --random_sample``

Website reference
-----------------
The code to upload image in HTML code is referenced from the blog[3], and HTML code uses image from the website[4]. The code to show image in Javascript code is referenced from the blog[3]

The code to get the file is referenced from the website[5]. The code to download file is referenced from the website[6]. The code to run the python code is referenced from the website[7]. The method to return the result to the website is referenced from the website[8].

Also when run the website, please install portch and put all python files in the server root dictionary.

Reference
-----------------
[1] https://github.com/meijieru/crnn.pytorch

[2] https://github.com/bgshih/crnn

[3] https://blog.csdn.net/weixin_38023551/article/details/78318532  

[4] https://fontawesome.com/icons/user?style=solid 

[5] https://stackoverflow.com/questions/2422468/how-to-upload-files-to-server-using-jsp-servlet

[6] https://blog.csdn.net/sabic/article/details/6564169

[7] https://blog.csdn.net/IT_xiao_bai/article/details/79074988  

[8] http://www.runoob.com/servlet/servlet-file-uploading.html


Image Preprocessing with OpenCV
-----------------
There are two version of image-preprocessing, one is run in Visual Studio, another one is run in linux and can be called by the Web. To run the code, you should have set up your system with OpenCV4.0(Not sure if it is compatible with lower version of OpenCV. There are some const values that have different name in different version of OpenCV.)

This program is able to rotate a tilted image to horizontal position (works to most of the bw/color images). For image with multiple lines, the program is able to segment the image to images with single lines(In order to have a good performance, test this part of program with bw images). For word segmentation, the program can only do vertical projections which represents positions of characters. As for the segmentation part, not working yet, therefore it is not included in the submitted program.


To run the visual studio version, just change the input file name at line 287 then run it. 

To run the linux version, compile it then run it with following command./{compiled file name} {image name}

This program was expected work with the Web code. However, for unknow reasons, it somrtimes returns error message when called form the Web code.  We recommand to run this part of code with visual studio to check its function.
