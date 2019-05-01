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

Reference
-----------------
[1] https://github.com/meijieru/crnn.pytorch
[2] https://github.com/bgshih/crnn
[3]
