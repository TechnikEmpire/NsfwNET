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

#include "ImageClassifier.hpp"

ImageClassifier::ImageClassifier(const std::string & prototxtPath, const std::string & caffeModelPath)
{
	m_network = cv::dnn::readNetFromCaffe(prototxtPath, caffeModelPath);
}

ImageClassifier::~ImageClassifier()
{

}

const bool ImageClassifier::IsEmpty() const
{
	return m_network.empty();
}

const bool ImageClassifier::Classify(cv::Mat& image)
{
	image.convertTo(image, CV_32F);

	resize(image, image, cv::Size(256, 256));
	auto roi = cv::Rect(16, 16, 224, 224);
	image = image(roi);

	auto inputBlob = cv::dnn::blobFromImage(image, 1., cv::Size(), cv::Scalar(104, 117, 123), false, false);

	m_network.setInput(inputBlob);

	auto fw = m_network.forward();

	float probability = fw.ptr<float>(0)[1];

	return probability > m_cutoff;
}

const double ImageClassifier::GetCutoff() const
{
	return m_cutoff;
}

const void ImageClassifier::SetCutoff(const double value)
{
	m_cutoff = value;
}
