# Update - This project doesn't is deprecated and you shouldn't use it.


# NsfwNET
.NET Standard library for classifying pornographic images.

This project bundles two different trained network models for binary classification of pornographic and non-pornographic imagery. One is a Resnet model by Yahoo, known as the "open NSFW" model. The other is a Squeezenet 1.1 type model, custom trained by me. Pay attention to the licenses on these models as they are different.

## Straight To The Point
Here's the output of the test program as of version 1.1.1:

```
Squeezenet classifier has cutoff of 0.8.
Testing against a total of 1164 images, 582 from each class.
Classifier with type Squeezenet classified pornographic images with an accuracy of 98.1099656357388%.
Classifier with type Squeezenet classified non-pornographic images with an accuracy of 98.4536082474227%.
Classifier with type Squeezenet took an average of 11.9312714776632 msec per image to classify.

Resnet classifier has cutoff of 0.2.
Testing against a total of 1164 images, 582 from each class.
Classifier with type Resnet classified pornographic images with an accuracy of 99.1408934707904%.
Classifier with type Resnet classified non-pornographic images with an accuracy of 97.2508591065292%.
Classifier with type Resnet took an average of 33.9493127147766 msec per image to classify.

SqueezeNet classifier was 2.84392314888105 times faster.
```


## Everything Else

This project is available as a .NET Standard 2.0 project on [Nuget](https://www.nuget.org/packages/NsfwNET). The Nuget package bundles the necessary model data and is configured to automatically stage those files, along with native, architecture specific dll's inside your parent project output directory.

NsfwNET uses OpenCV's DNN module to load and use either Yahoo's NSFW pre-trained caffe model, or my bundled SqueezNet 1.1 caffe model. A simple, convenient .NET library with a single exported class wraps this functionality nicely. The .NET classifier class has a single modifiable property, `Cutoff`, which allows you to set the value at which all classifications results exceeding this value are determined to be pornography. This value defaults to `0.2` for Yahoo's model, because in my manual testing, this is the most accurate setting. For my SqueeNet 1.1 model, this value defaults to `0.8` for the same reason.

The NSFW model emits a single output, which is a probability of pornographic content within the image, ranging from 0.0 to 1.0. The cutoff should be set within this range, although the library will not stop you from setting values outside this range.

Please note that, while the project is .NET Standard 2.0, only Windows native binaries have been built into the package. As such, the package will not run on other platforms presently. However, it's not complicated to build OpenCV with the DNN module for your platform and modify the PInvoke code and nuget package to emit platform specific binaries as well. I'll most likely be doing this sometime down the road, but that's not guaranteed.

Lastly it should be noted that this package bundles OpenCV 3.3.1 compiled as a single, world DLL, WITHOUT the non-free addons.
