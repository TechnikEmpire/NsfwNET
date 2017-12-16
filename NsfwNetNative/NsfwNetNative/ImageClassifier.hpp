/*
* Copyright © 2017 Jesse Nicholson
* Copyright © 2016, Yahoo Inc.
* 
* Redistribution and use in source and binary forms, with or without
* modification, are permitted
* provided that the following conditions are met:
* 
* 1. Redistributions of source code must retain the above copyright
* notice, this list of conditions
* and the following disclaimer.
* 
* 2. Redistributions in binary form must reproduce the above copyright
* notice, this list of
* conditions and the following disclaimer in the documentation and/or
* other materials provided with
* the distribution.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND
* FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
* THE COPYRIGHT HOLDER OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
* THEORY OF LIABILITY, WHETHER
* IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
* OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*/

#pragma once

#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <string>

#pragma comment(lib, "opencv_world331.lib")

class ImageClassifier
{
public:

	/// <summary>
	/// Creates a new instance of the image classifier by loading the given caffe model. 
	/// </summary>
	/// <param name="prototxtPath">
	/// Path to the model prototxt file.
	/// </param>
	/// <param name="caffeModelPath">
	/// Path to the model bin file.
	/// </param>
	ImageClassifier(const std::string& prototxtPath, const std::string& caffeModelPath);

	/// <summary>
	/// Default dtor.
	/// </summary>
	~ImageClassifier();

	/// <summary>
	/// Classifies the input image as pornographic or non-pornographic. 
	/// </summary>
	/// <param name="image">
	/// The image to classify.
	/// </param>
	/// <returns>
	/// True if the supplied image was classified as pornography. False otherwise.
	/// </returns>
	const bool Classify(cv::Mat& image);

	/// <summary>
	/// Gets the cutoff. Values above the cutoff will be classified as pornography. 
	/// </summary>
	/// <returns>
	/// The current set cutoff.
	/// </returns>
	const double GetCutoff() const;

	/// <summary>
	/// Sets the cutoff. Values above the cutoff will be classified as pornography. 
	/// </summary>
	/// <param name="value">
	/// The cutoff value to set.
	/// </param>
	const void SetCutoff(const double value);

	/// <summary>
	/// Gets whether or not the underlying network is empty. This is a convenience function so that
	/// users can verify that the model supplied at construction was correctly loaded.
	/// </summary>
	/// <returns>
	/// True if the network is not populated, false otherwise. A return value of true almost
	/// certainly means in the general case that something went wrong loading the model data.
	/// </returns>
	const bool IsEmpty() const;

private:

	/// <summary>
	/// Muh network.
	/// </summary>
	cv::dnn::Net m_network;

	/// <summary>
	/// Cutoff. Default is hard coded to a value I found to perform very well (98-ish percent accuracy).
	/// </summary>
	double m_cutoff = 0.2;
};

