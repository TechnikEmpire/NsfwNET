/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "NsfwNet.hpp"
#include "ResnetClassifier.hpp"
#include "SqueezenetClassifier.hpp"
#include <stdexcept>

enum ClassifierType
{
	Resnet = 0,
	Squeezenet = 1,
	NumEntries
};

int handleError(int status, const char* func_name,
	const char* err_msg, const char* file_name,
	int line, void* userdata)
{
	//Do nothing -- will suppress console output
	return 0;   //Return value is not used
}

NSFWNET_API PVOID classifier_create_from_fs(const char* protoTxtPath, const uint32_t protoTxtLen, const char* caffeModelPath, const uint32_t caffeModelPathLen, const uint8_t classifierType, double* mean)
{
	cv::redirectError(handleError);

	if (protoTxtPath != nullptr && caffeModelPath != nullptr)
	{
		std::string proto(protoTxtPath, protoTxtLen);
		std::string mdl(caffeModelPath, caffeModelPathLen);

		ClassifierType type = ClassifierType::Squeezenet;

		if (classifierType < 0 && classifierType > ClassifierType::NumEntries)
		{
			return nullptr;
			// throw std::exception("Unknown classifier type.");
		}

		type = static_cast<ClassifierType>(classifierType);

		cv::Scalar cvtMean;

		if (mean != nullptr)
		{
			auto len = sizeof(mean) / sizeof(mean[0]);

			switch (len)
			{
				case 3:
				{
					cvtMean = cv::Scalar{ mean[0], mean[1], mean[2] };
				}
				break;

				case 4:
				{
					cvtMean = cv::Scalar{ mean[0], mean[1], mean[2], mean[3] };
				}
				break;

				default:
				{
					// throw std::exception("Invalid number of arguments supplied as the mean. Must be 3 or 4 values. RGB and optionally A.");
				}
				break;
			}			
		}

		switch (type)
		{
			case ClassifierType::Resnet:
			{
				switch (mean != nullptr)
				{
					case true:
					{
						return static_cast<PVOID>(new ResnetClassifier(proto, mdl, cvtMean));
					}
					break;

					case false:
					{
						return static_cast<PVOID>(new ResnetClassifier(proto, mdl));
					}
					break;
				}
			}
			break;

			case ClassifierType::Squeezenet:
			{
				switch (mean != nullptr)
				{
					case true:
					{
						return static_cast<PVOID>(new SqueezenetClassifier(proto, mdl, cvtMean));
					}
					break;

					case false:
					{
						return static_cast<PVOID>(new SqueezenetClassifier(proto, mdl));
					}
					break;
				}
			}
			break;
		}
	}

	return nullptr;
}


NSFWNET_API PVOID classifier_create_from_memory(const char* protoTxtData, const uint32_t protoTxtDataLen, const char* caffeModelData, const uint32_t caffeModelDataLen, const uint8_t classifierType, double* mean)
{
	if (protoTxtData != nullptr && caffeModelData != nullptr)
	{
		std::vector<char> proto(protoTxtData, protoTxtData + protoTxtDataLen);
		std::vector<char> mdl(caffeModelData, caffeModelData + caffeModelDataLen);

		ClassifierType type = ClassifierType::Squeezenet;

		if (classifierType < 0 && classifierType > ClassifierType::NumEntries)
		{
			throw std::exception("Unknown classifier type.");
		}

		type = static_cast<ClassifierType>(classifierType);

		cv::Scalar cvtMean;

		if (mean != nullptr)
		{
			auto len = sizeof(mean) / sizeof(mean[0]);

			switch (len)
			{
				case 3:
				{
					cvtMean = cv::Scalar{ mean[0], mean[1], mean[2] };
				}
				break;

				case 4:
				{
					cvtMean = cv::Scalar{ mean[0], mean[1], mean[2], mean[3] };
				}
				break;

				default:
				{
					throw std::exception("Invalid number of arguments supplied as the mean. Must be 3 or 4 values. RGB and optionally A.");
				}
				break;
			}
		}

		switch (type)
		{
			case ClassifierType::Resnet:
			{
				switch (mean != nullptr)
				{
					case true:
					{
						return static_cast<PVOID>(new ResnetClassifier(proto, mdl, cvtMean));
					}
					break;

					case false:
					{
						return static_cast<PVOID>(new ResnetClassifier(proto, mdl));
					}
					break;
				}
			}
			break;

			case ClassifierType::Squeezenet:
			{
				switch (mean != nullptr)
				{
					case true:
					{
						return static_cast<PVOID>(new SqueezenetClassifier(proto, mdl, cvtMean));
					}
					break;

					case false:
					{
						return static_cast<PVOID>(new SqueezenetClassifier(proto, mdl));
					}
					break;
				}
			}
			break;
		}
	}

	return nullptr;
}

NSFWNET_API bool classifier_classify(PVOID classifier, unsigned char * imageData, const uint32_t imageDataLength)
{	
	try
	{
		auto cast = static_cast<AbstractImageClassifier*>(classifier);

		if (cast != nullptr)
		{
			cv::Mat rawData(1, imageDataLength, CV_8UC1, static_cast<void*>(imageData), false);
			auto decoded = cv::imdecode(rawData, CV_LOAD_IMAGE_UNCHANGED);

			if (decoded.data == nullptr || decoded.empty())
			{
				return false;
			}

			//std::vector<unsigned char> vec(imageData, imageData + imageDataLength);
			//auto decoded = cv::imdecode(vec, CV_LOAD_IMAGE_UNCHANGED);
			
			return cast->Classify(decoded);
		}
	}
	catch (std::exception& e)
	{
		// WHAT CAN I DO FAM? TELL ME DO. WHAT. CAN. I. DO.
	}

	return false;
}

NSFWNET_API double classifier_get_positive_probability(PVOID classifier, unsigned char * imageData, const uint32_t imageDataLength)
{
	try
	{
		auto cast = static_cast<AbstractImageClassifier*>(classifier);

		if (cast != nullptr)
		{
			cv::Mat rawData(1, imageDataLength, CV_8UC1, static_cast<void*>(imageData), false);
			auto decoded = cv::imdecode(rawData, CV_LOAD_IMAGE_UNCHANGED);

			//std::vector<unsigned char> vec(imageData, imageData + imageDataLength);
			//auto decoded = cv::imdecode(vec, CV_LOAD_IMAGE_UNCHANGED);

			if (decoded.data == nullptr || decoded.empty())
			{
				return 0.;
			}

			return cast->GetPositiveProbability(decoded);
		}
	}
	catch (std::exception& e)
	{
		// WHAT CAN I DO FAM? TELL ME DO. WHAT. CAN. I. DO.
	}

	return 0.;
}

NSFWNET_API bool classifier_classify_cvmat(PVOID classifier, PVOID cvMatPtr)
{
	try
	{
		auto cast = static_cast<AbstractImageClassifier*>(classifier);

		if (cast != nullptr)
		{
			auto imgCast = static_cast<cv::Mat*>(cvMatPtr);
			if (imgCast != nullptr)
			{
				return cast->Classify(*imgCast);
			}
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
		auto cast = static_cast<AbstractImageClassifier*>(classifier);

		if (cast != nullptr)
		{
			return cast->GetIsEmpty();
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
		auto cast = static_cast<AbstractImageClassifier*>(classifier);

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
		auto cast = static_cast<AbstractImageClassifier*>(classifier);

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
		auto cast = static_cast<AbstractImageClassifier*>(classifier);

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
