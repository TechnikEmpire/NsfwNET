# NsfwNET
.NET Standard library for classifying pornographic images, ported from Yahoo's open NSFW.

## Accuracy
Probably the only thing that matters. In my testing on 2000 images, 1000 from each class, which were manually sanitized:
```
Classified pornographic images with an accuracy of 100%.
Classified non-pornographic images with an accuracy of 96.8%.
```
That's with the default `Cutoff` value of `0.2.`. Very nice work Yahoo.

Note that this isn't truly 100%. When expanding to many thousand of more diverse positive images, I've found that certain pornography does slip through, but very rarely. An example of this would be a pornographic scene where most clothing is present, and private parts are mostly occluded, and what is visible makes up a very small area of the actual frame. So, it's not perfect, but I think it's as good as it gets.

## Speed

On a 6th gen i7 at 3.4 ghz, running on randomly sized images ranging from 1080P to <=480P,  this processes at ~27msec per image, or ~37FPS. The OpenCV DNN module is rather new, so I expect performance to increase in later versions. Presently, with specific Intel hardware, OpenCL acceleration can be enabled, but since we want a "work everywhere" solution, that's not configured here.

## Everything Else

This project is available as a .NET Standard 2.0 project on [Nuget](https://www.nuget.org/packages/NsfwNET). The Nuget package bundles the necessary model data and is configured to automatically stage those files, along with native, architecture specific dll's inside your parent project output directory.

NsfwNET uses OpenCV's DNN module to load and use Yahoo's NSFW pre-trained caffe model. A simple, convenient .NET library with a single exported class wraps this functionality nicely. The .NET classifier class has a single modifiable property, `Cutoff`, which allows you to set the value at which all classifications results exceeding this value are determined to be pornography. This value defaults to `0.2`, because in my manual testing, this is the most accurate setting.

The NSFW model emits a single output, which is a probability of pornographic content within the image, ranging from 0.0 to 1.0. The cutoff should be set within this range, although the library will not stop you from setting values outside this range.

Please note that, while the project is .NET Standard 2.0, only Windows native binaries have been built into the package. As such, the package will not run on other platforms presently. However, it's not complicated to build OpenCV with the DNN module for your platform and modify the PInvoke code and nuget package to emit platform specific binaries as well. I'll most likely be doing this sometime down the road, but that's not guaranteed.

Lastly it should be noted that this package bundles OpenCV 3.3.1 compiled as a single, world DLL, WITHOUT the non-free addons.
