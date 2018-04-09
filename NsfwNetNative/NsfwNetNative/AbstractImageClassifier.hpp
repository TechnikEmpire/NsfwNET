/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <vector>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/dnn.hpp>

#pragma comment(lib, "opencv_core341.lib")
#pragma comment(lib, "opencv_dnn341.lib")
#pragma comment(lib, "opencv_imgproc341.lib")
#pragma comment(lib, "opencv_imgcodecs341.lib")

/// <summary>
/// The AbstractImageClassifier services as the base abstract type for all classifiers within the library. 
/// </summary>
class AbstractImageClassifier
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
	AbstractImageClassifier(const std::string& prototxtPath, const std::string& mdlPath) noexcept;

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
	AbstractImageClassifier(const std::vector<char>& prototxt, const std::vector<char>& mdl) noexcept;

	/// <summary>
	/// Default dtor.
	/// </summary>
	virtual ~AbstractImageClassifier() noexcept;

	/// <summary>
	/// Classifies the supplied image mat as either pornographic or non-pornographic. 
	/// </summary>
	/// <param name="mat">
	/// The image material to classify.
	/// </param>
	/// <returns>
	/// True if the supplied image was determined to be pornographic in nature, false otherwise.
	/// </returns>
	virtual const bool Classify(cv::Mat& mat) noexcept = 0;

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
	virtual const double GetPositiveProbability(cv::Mat& mat) noexcept = 0;

	/// <summary>
	/// Sets the cutoff value. Classification result probabilities returned from the network that are
	/// beyond this value will be interpreted as indicative of pornographic content being present
	/// within the classification target.
	/// </summary>
	/// <param name="cutoff">
	/// The probability cutoff.
	/// </param>
	/// <remarks>
	/// The cutoff should be within the range of 0.0 to 1.0.
	/// </remarks>
	const void SetCutoff(const double cutoff) noexcept;

	/// <summary>
	/// Sets the cutoff score. Classification result probabilities returned from the network that are
	/// beyond this value will be interpreted as indicative of pornographic content being present
	/// within the classification target.
	/// </summary>
	/// <returns>
	/// The current cutoff value. 
	/// </returns>
	const double GetCutoff() const noexcept;

	/// <summary>
	/// Gets whether or not the underlying DNN is empty. If this returns true, a serious issue has
	/// occurred. Either something is wrong with the model you used to construct this instance, or
	/// the model was not supplied correctly so initialization failed.
	/// </summary>
	/// <returns>
	/// True if the underlying DNN is empty, false otherwise.
	/// </returns>
	const bool GetIsEmpty() const noexcept;

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
	virtual cv::Mat Preprocess(cv::Mat& mat) const noexcept = 0;

	/// <summary>
	/// Convenience function for trimming black space around images off. This method uses a
	/// thresholding technique to approximate a region of interest that does not include black bars
	/// around any edge of the image.
	/// </summary>
	/// <param name="mat">
	/// The input image material. 
	/// </param>
	/// <returns>
	/// An image equal to the input image, but with any black space around the edges trimmed away. 
	/// </returns>
	cv::Mat RemoveBlackBars(cv::Mat& mat) const noexcept;

	/// <summary>
	/// The classification cutoff.
	/// </summary>
	double m_cutoff;

	/// <summary>
	/// The network.
	/// </summary>
	cv::dnn::Net m_network;

	static const cv::Size s_zeroSize;

	static const cv::Scalar s_zeroScalar;

	private:

		/// <summary>
		/// Initializes static members in an organized and ordered fashion.
		/// </summary>
		static class StaticMemberInitializer
		{
		public:

			StaticMemberInitializer()
			{
				InitOcl();
			}
		} s_staticMemberInitializer;

		static void InitOcl()
		{
			// Explicitly dispable OpenCL
			if (cv::ocl::haveOpenCL())
			{
				cv::ocl::setUseOpenCL(false);
			}
		}
};

