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

	extern NSFWNET_API PVOID classifier_create(const char* protoTxtPath, const uint32_t protoTxtLen, const char* caffeModelPath, const uint32_t caffeModelPathLen);

	extern NSFWNET_API bool classifier_classify(PVOID classifier, unsigned char* imageData, const uint32_t imageDataLength);

	extern NSFWNET_API bool classifier_get_is_empty(PVOID classifier);

	extern NSFWNET_API double classifier_get_cutoff(PVOID classifier);

	extern NSFWNET_API void classifier_set_cutoff(PVOID classifier, const double cutoff);

	extern NSFWNET_API void classifier_destroy(PVOID classifier);

#ifdef __cplusplus
};
#endif // __cplusplus
