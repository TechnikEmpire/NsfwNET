/*
* Copyright © 2018 Jesse Nicholson
* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this
* file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#pragma once

#include <cstdint>

#ifndef PVOID
typedef void* PVOID;
#endif

#ifdef NSFWNET_EXPORT
	#ifdef _MSC_VER
		#define NSFWNET_API __declspec(dllexport)
	#else
		#define NSFWNET_API __attribute__((visibility("default")))
	#endif // #ifdef _MSC_VER
#else
	#ifdef _MSC_VER
		#define NSFWNET_API __declspec(dllimport)
	#else
		#define NSFWNET_API
	#endif
#endif // #ifdef HTTP_FILTERING_ENGINE_EXPORT

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

	extern NSFWNET_API PVOID classifier_create_from_fs(const char* protoTxtPath, const uint32_t protoTxtLen, const char* caffeModelPath, const uint32_t caffeModelPathLen, const uint8_t classifierType, double* mean = nullptr);

#ifdef NSFW_HAVE_NONBROKEN_CV_3_4
	extern NSFWNET_API PVOID classifier_create_from_memory(const char* protoTxtData, const uint32_t protoTxtDataLen, const char* caffeModelData, const uint32_t caffeModelDataLen, const uint8_t classifierType, double* mean = nullptr);
#endif

	extern NSFWNET_API bool classifier_classify(PVOID classifier, unsigned char* imageData, const uint32_t imageDataLength);

	extern NSFWNET_API double classifier_get_positive_probability(PVOID classifier, unsigned char* imageData, const uint32_t imageDataLength);

	extern NSFWNET_API bool classifier_classify_cvmat(PVOID classifier, PVOID cvMatPtr);

	extern NSFWNET_API bool classifier_get_is_empty(PVOID classifier);

	extern NSFWNET_API double classifier_get_cutoff(PVOID classifier);

	extern NSFWNET_API void classifier_set_cutoff(PVOID classifier, const double cutoff);

	extern NSFWNET_API void classifier_destroy(PVOID classifier);

#ifdef __cplusplus
};
#endif // __cplusplus
