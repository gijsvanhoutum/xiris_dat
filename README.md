# XIRIS_DAT: Binary file reader for Xiris camera's

**XIRIS_DAT** is a small C library for reading binary files created by Xiris
camera's. 

<p align="center">
  <img alt="Light" src="https://github.com/gijsvanhoutum/xiris_dat/blob/master/icons/xiris_camera.jpg?raw=true" width="45%">
&nbsp; &nbsp; &nbsp; &nbsp;
  <img alt="Dark" src="https://github.com/gijsvanhoutum/xiris_dat/blob/master/icons/DAT_image_format_XVC1000.png?raw=true" width="45%">
</p>

## Main Features / Comments
Major information:

  - Dynamic allocation allows multiple resolution images to be read
  - Directory scraping for DAT files. 
  - Utility functions for information display about the binary file i.e header

## Dependencies
GCC compiler needed. To check run in terminal:
```sh
gcc --version
```
If the package is not installed, install it with:
```sh
sudo apt install build-essential
```

## How to get it

Git has to be installed to clone: 
```sh
sudo apt install git
```
Clone the repository to current working directory
```sh
git clone https://github.com/gijsvanhoutum/xiris_dat.git
```

## How to run it
Compile test.c in tests/ using:
```sh
gcc tests/test.c src/image.c src/dir.c -o test
```
To run execute the following from the current working directory:
```sh
./test
```