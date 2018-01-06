/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "SqueezenetClassifier.hpp"

const cv::Scalar SqueezenetClassifier::s_mean { 112.005, 120.294, 138.682 };

const cv::Rect SqueezenetClassifier::s_roi { 16, 24, 227, 227 };

const cv::Size SqueezenetClassifier::s_inDim { 259, 275 };

const cv::Size SqueezenetClassifier::s_barDim{ 299, 299 };

SqueezenetClassifier::SqueezenetClassifier(const std::string & prototxtPath, const std::string & mdlPath, cv::Scalar mean) noexcept : m_mean(mean), AbstractImageClassifier(prototxtPath, mdlPath)
{
	m_cutoff = 0.8;
}

#ifdef NSFW_HAVE_NONBROKEN_CV_3_4
SqueezenetClassifier::SqueezenetClassifier(const std::vector<char>& prototxt, const std::vector<char>& mdl, cv::Scalar mean) noexcept : m_mean(mean), AbstractImageClassifier(prototxt, mdl)
{
	m_cutoff = 0.9;
}
#endif

SqueezenetClassifier::~SqueezenetClassifier() noexcept
{

}

const bool SqueezenetClassifier::Classify(cv::Mat& mat) noexcept
{
	auto result = GetPositiveProbability(mat);

	return result > m_cutoff;
}

const double SqueezenetClassifier::GetPositiveProbability(cv::Mat& mat) noexcept
{
	try
	{
		if (!m_network.empty())
		{
			mat = Preprocess(mat);

			//cv::flip(mat, mat, 1);

			auto inputBlob = cv::dnn::blobFromImage(mat, 1., s_zeroSize, s_zeroScalar, false, false);

			m_network.setInput(inputBlob);

			auto fw = m_network.forward();

			return fw.ptr<float>(0)[1];
		}
	}
	catch (std::exception&)
	{

	}

	return 0.;
}

cv::Mat SqueezenetClassifier::Preprocess(cv::Mat& mat) const noexcept
{
	cv::Mat sample = mat;

	if (mat.channels() == 4)
	{
		cv::cvtColor(mat, sample, cv::COLOR_BGRA2BGR);
	}
	else if (mat.channels() == 1)
	{
		cv::cvtColor(mat, sample, cv::COLOR_GRAY2BGR);
	}
	else
	{
		sample = mat;
	}

	if (sample.size() != s_barDim)
	{
		cv::resize(sample, sample, s_inDim);		
	}
	
	sample = RemoveBlackBars(sample);

	cv::Mat sample_resized;
	if (sample.size() != s_inDim)
	{
		cv::resize(sample, sample_resized, s_inDim);
	}
	else
	{
		sample_resized = sample;
	}

	sample_resized = sample_resized(s_roi);

	cv::flip(sample_resized, sample_resized, 1);

	cv::Mat sample_float;

	sample_resized.convertTo(sample_float, CV_32FC3);

	cv::Mat sample_normalized;
	cv::subtract(sample_float, m_mean, sample_normalized);

	return sample_normalized;
}
