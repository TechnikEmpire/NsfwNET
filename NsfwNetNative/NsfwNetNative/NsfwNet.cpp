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

#include "NsfwNet.hpp"
#include "ImageClassifier.hpp"
#include <stdexcept>

NSFWNET_API PVOID classifier_create(const char* protoTxtPath, const uint32_t protoTxtLen, const char* caffeModelPath, const uint32_t caffeModelPathLen)
{
	if (protoTxtPath != nullptr && caffeModelPath != nullptr)
	{
		std::string proto(protoTxtPath, protoTxtLen);
		std::string mdl(caffeModelPath, caffeModelPathLen);

		return static_cast<PVOID>(new ImageClassifier(proto, mdl));
	}

	return nullptr;
}

NSFWNET_API bool classifier_classify(PVOID classifier, unsigned char * imageData, const uint32_t imageDataLength)
{
	try
	{
		auto cast = static_cast<ImageClassifier*>(classifier);

		if (cast != nullptr)
		{
			cv::Mat rawData(1, imageDataLength, CV_8UC1, static_cast<void*>(imageData));
			auto decoded = cv::imdecode(rawData, CV_LOAD_IMAGE_UNCHANGED);

			return cast->Classify(decoded);
		}
	}
	catch (std::exception& e)
	{
		// WHAT CAN I DO FAM? TELL ME DO. WHAT. CAN. I. DO.
	}

	return false;
}

NSFWNET_API bool classifier_get_is_empty(PVOID classifier)
{
	try
	{
		auto cast = static_cast<ImageClassifier*>(classifier);

		if (cast != nullptr)
		{
			return cast->IsEmpty();
		}
	}
	catch (std::exception& e)
	{
		// WHAT CAN I DO FAM? TELL ME DO. WHAT. CAN. I. DO.
	}	

	return true;
}

NSFWNET_API double classifier_get_cutoff(PVOID classifier)
{
	try
	{
		auto cast = static_cast<ImageClassifier*>(classifier);

		if (cast != nullptr)
		{
			return cast->GetCutoff();
		}
	}
	catch (std::exception& e)
	{
		// WHAT CAN I DO FAM? TELL ME DO. WHAT. CAN. I. DO.
	}	

	return 0.;
}

NSFWNET_API void classifier_set_cutoff(PVOID classifier, const double cutoff)
{
	try
	{
		auto cast = static_cast<ImageClassifier*>(classifier);

		if (cast != nullptr)
		{
			cast->SetCutoff(cutoff);
		}
	}
	catch (std::exception& e)
	{
		// WHAT CAN I DO FAM? TELL ME DO. WHAT. CAN. I. DO.
	}
}

NSFWNET_API void classifier_destroy(PVOID classifier)
{
	try
	{
		auto cast = static_cast<ImageClassifier*>(classifier);

		if (cast != nullptr)
		{
			delete cast;
		}
	}
	catch (std::exception& e)
	{
		// WHAT CAN I DO FAM? TELL ME DO. WHAT. CAN. I. DO.
	}	
}
