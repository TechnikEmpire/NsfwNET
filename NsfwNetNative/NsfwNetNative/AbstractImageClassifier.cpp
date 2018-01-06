/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "AbstractImageClassifier.hpp"

#ifndef NSFW_USE_EXPERIMENTAL
#include <fstream>
#else
#include <experimental/filesystem>
#endif

const cv::Size AbstractImageClassifier::s_zeroSize {};

const cv::Scalar AbstractImageClassifier::s_zeroScalar {};

AbstractImageClassifier::AbstractImageClassifier(const std::string & prototxtPath, const std::string & mdlPath) noexcept
{
	try
	{
#ifndef NSFW_USE_EXPERIMENTAL
		std::ifstream ifp(prototxtPath);
		std::ifstream ifm(mdlPath);

		if (ifp.good() && ifm.good())
#else
		if (std::experimental::filesystem::exists(prototxtPath) && std::experimental::filesystem::exists(mdlPath))
#endif
		{
#ifndef NSFW_USE_EXPERIMENTAL
			ifp.close();
			ifm.close();
#endif

			m_network = cv::dnn::readNetFromCaffe(prototxtPath, mdlPath);
			m_network.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
			m_network.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);

			m_network.enableFusion(true);
		}
	}
	catch (std::exception&)
	{

	}
}

#ifdef NSFW_HAVE_NONBROKEN_CV_3_4
AbstractImageClassifier::AbstractImageClassifier(const std::vector<char>& prototxt, const std::vector<char>& mdl) noexcept
{
	if (prototxt.size() > 0 && mdl.size() > 0)
	{
		try
		{
			m_network = cv::dnn::readNetFromCaffe(prototxt.data(), prototxt.size(), mdl.data(), mdl.size());
			m_network.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
			m_network.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);

			m_network.enableFusion(true);
		}
		catch (std::exception&)
		{

		}
	}
}
#endif

AbstractImageClassifier::~AbstractImageClassifier()
{

}

const void AbstractImageClassifier::SetCutoff(const double cutoff) noexcept
{
	m_cutoff = cutoff;
}

const double AbstractImageClassifier::GetCutoff() const noexcept
{
	return m_cutoff;
}

const bool AbstractImageClassifier::GetIsEmpty() const noexcept
{
	return m_network.empty();
}

cv::Mat AbstractImageClassifier::RemoveBlackBars(cv::Mat & mat) const noexcept
{
	cv::Mat borders;

	double min, max;
	cv::minMaxLoc(mat, &min, &max);	
	mat.convertTo(borders, CV_8U, 255.0 / (max - min), -255.0*min / (max - min));
	cv::cvtColor(borders, borders, cv::COLOR_BGR2GRAY);

	cv::threshold(borders, borders, 2, 255, CV_THRESH_BINARY);

	std::vector<std::vector<cv::Point>> ctrs;

	cv::findContours(borders, ctrs, cv::RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (auto& ctr : ctrs)
	{
		auto r = cv::boundingRect(ctr);

		if (r.width >= (mat.cols - (mat.cols % 10)) || r.height >= (mat.rows - (mat.rows % 10)))
		{
			mat = mat(r);
			return mat;
		}
	}

	return mat;
}
