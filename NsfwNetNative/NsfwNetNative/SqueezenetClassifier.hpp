/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include "AbstractImageClassifier.hpp"

class SqueezenetClassifier : public AbstractImageClassifier
{
public:
	
	/// <summary>
	/// Constructs a new image classifier from the caffe prototxt and corresponding model files
	/// indicated by the supplied file paths.
	/// </summary>
	/// <param name="prototxtPath">
	/// Path to the prototxt file. 
	/// </param>
	/// <param name="mdlPath">
	/// Path to the model file. 
	/// </param>
	/// <param name="mean">
	/// The mean to use during normalization. Defaults to the mean appropriate for the supplied
	/// Squeezenet model. Change this if you are using a different Resnet model.
	/// </param>
	SqueezenetClassifier(const std::string& prototxtPath, const std::string& mdlPath, cv::Scalar mean = s_mean) noexcept;

	/// <summary>
	/// Constructs a new image classifier from the caffe prototxt and corresponding model binary data
	/// loaded into the supplied vectors.
	/// </summary>
	/// <param name="prototxt">
	/// The prototxt data.
	/// </param>
	/// <param name="mdl">
	/// The model data.
	/// </param>
	/// <param name="mean">
	/// The mean to use during normalization. Defaults to the mean appropriate for the supplied
	/// Squeezenet model. Change this if you are using a different Resnet model.
	/// </param>
	SqueezenetClassifier(const std::vector<char>& prototxt, const std::vector<char>& mdl, cv::Scalar mean = s_mean) noexcept;

	/// <summary>
	/// Default dtor.
	/// </summary>
	virtual ~SqueezenetClassifier() noexcept;

	/// <summary>
	/// Classifies the supplied image mat as either pornographic or non-pornographic. 
	/// </summary>
	/// <param name="mat">
	/// The image material to classify.
	/// </param>
	/// <returns>
	/// True if the supplied image was determined to be pornographic in nature, false otherwise.
	/// </returns>
	virtual const bool Classify(cv::Mat& mat) noexcept override;

	/// <summary>
	/// This is functionality equivalent to the Classify method, but instead it simply returns the
	/// calculated positive probability, rather than a boolean value.
	/// </summary>
	/// <param name="mat">
	/// The image material to classify.
	/// </param>
	/// <returns>
	/// The calculated positive match probability.
	/// </returns>
	virtual const double GetPositiveProbability(cv::Mat& mat) noexcept override;

protected:

	/// <summary>
	/// Preprocess an input image into a valid input for the underlying network. 
	/// </summary>
	/// <param name="mat">
	/// The input image material. 
	/// </param>
	/// <returns>
	/// A processed version of the supplied mat that is now suitable to set as the input for the
	/// underlying network.
	/// </returns>
	/// <remarks>
	/// Many things must be modified to create valid input for a specific network. Resizing the
	/// image, subtracting a mean (normalization), and flipping the image are some examples of what
	/// must be done to input before using it within the network. These requirements vary between
	/// network types, so this method is abstract.
	/// </remarks>
	virtual cv::Mat Preprocess(cv::Mat& mat) const noexcept override;

private:

	/// <summary>
	/// The instance-specific mean for preprocessing.
	/// </summary>
	const cv::Scalar m_mean;

	/// <summary>
	/// The default mean. This value is meant to be used with the provided Squeezenet model.
	/// </summary>
	static const cv::Scalar s_mean;

	/// <summary>
	/// The region of interest to extract during preprocessing.
	/// </summary>
	static const cv::Rect s_roi;

	/// <summary>
	/// The input dimensions. This may not match the input dimensions of the network, but rather may
	/// be a value meant to resize to before extracting the ROI.
	/// </summary>
	static const cv::Size s_inDim;

	static const cv::Size s_barDim;
};

