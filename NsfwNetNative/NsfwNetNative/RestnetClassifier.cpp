/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "ResnetClassifier.hpp"

const cv::Scalar ResnetClassifier::s_mean { 104, 117, 123, 0 };

const cv::Rect ResnetClassifier::s_roi { 16, 16, 224, 224 };

const cv::Size ResnetClassifier::s_inDim { 256, 256 };

ResnetClassifier::ResnetClassifier(const std::string & prototxtPath, const std::string & mdlPath, cv::Scalar mean) noexcept : m_mean(mean), AbstractImageClassifier(prototxtPath, mdlPath)
{
	m_cutoff = 0.2;
}

#ifdef NSFW_HAVE_NONBROKEN_CV_3_4
ResnetClassifier::ResnetClassifier(const std::vector<char>& prototxt, const std::vector<char>& mdl, cv::Scalar mean) noexcept : m_mean(mean), AbstractImageClassifier(prototxt, mdl)
{
	m_cutoff = 0.2;
}
#endif

ResnetClassifier::~ResnetClassifier()
{

}

const bool ResnetClassifier::Classify(cv::Mat& mat) noexcept
{
	auto result = GetPositiveProbability(mat);

	return result > m_cutoff;
}

const double ResnetClassifier::GetPositiveProbability(cv::Mat& mat) noexcept
{
	try
	{
		if (!GetIsEmpty())
		{
			mat = Preprocess(mat);

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

cv::Mat ResnetClassifier::Preprocess(cv::Mat& mat) const noexcept
{
	mat.convertTo(mat, CV_32F);

	resize(mat, mat, s_inDim);	
	mat = mat(s_roi);

	cv::subtract(mat, m_mean, mat);

	return mat;
}
