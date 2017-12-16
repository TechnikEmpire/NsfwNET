# NsfwNET
.NET Standard library for classifying pornographic images, ported from Yahoo's open NSFW.

This project is available as a .NET Standard 2.0 project on [Nuget](https://www.nuget.org/packages/NsfwNET). The Nuget package bundles the necessary model data and is configured to automatically stage those files, along with native, architecture specific dll's inside your parent project output directory.

NsfwNET uses OpenCV's DNN module to load and use Yahoo's NSFW pre-trained caffe model. A simple, convenient .NET library with a single exported class wraps this functionality nicely. The .NET classifier class has a single modifiable property, `Cutoff`, which allows you to set the value at which all classifications results exceeding this value are determined to be pornography. This value defaults to `0.2`, because in my manual testing, this is the most accurate setting.

The NSFW model emits a single output, which a probability of pornographic content within the image, ranging from 0.0 to 1.0. The cutoff should be set within this range, although the library will not stop you from setting values outside this range.

Please note that, while .NET project is .NET Standard 2.0, only Windows native binaries have been built into the package. As such, the package will not run on other platforms presently. However, it's not complicated to build OpenCV with the DNN module for your platform and modify the PInvoke code and nuget package to emit platform specific binaries as well. I'll most likely be doing this sometime down the future, but that's not guaranteed.

Lastly it should be noted that this package bundles OpenCV 3.3.1 compiled as a single, world DLL, WITHOUT the non-free addons.
