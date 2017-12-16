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

using System;
using System.Collections.Generic;
using System.Text;

namespace NsfwNET
{
    public class ImageClassifier
    {
        private IntPtr m_nativeObj = IntPtr.Zero;

        // Because we doubt sincerely the thread safety of openCV's dnn library.
        private object m_classLock = new object();

        /// <summary>
        /// Gets whether or not the classifier is empty. 
        /// </summary>
        /// <remarks>
        /// If this is true, then in most cases this is surely an indication that the model was not
        /// initialized correctly.
        /// </remarks>
        public bool IsEmpty
        {
            get
            {
                return ImageClassifierPInvoke.classifier_get_is_empty(m_nativeObj);
            }
        }

        /// <summary>
        /// Gets or sets the cutoff. Images who's score is above the set cutoff will be classified as
        /// pornographic. The default value is set based on experimentation.
        /// </summary>
        public double Cutoff
        {
            get
            {
                return ImageClassifierPInvoke.classifier_get_cutoff(m_nativeObj);
            }

            set
            {
                ImageClassifierPInvoke.classifier_set_cutoff(m_nativeObj, value);
            }
        }

        /// <summary>
        /// Constructs a new image classifier instance from the nsfw caffe model. 
        /// </summary>
        /// <param name="protoTextPath">
        /// Path to the prototxt file.
        /// </param>
        /// <param name="caffeModelPath">
        /// Path to the model bin file.
        /// </param>
        public ImageClassifier(string protoTextPath, string caffeModelPath)
        {
            m_nativeObj = ImageClassifierPInvoke.classifier_create(protoTextPath, protoTextPath.Length, caffeModelPath, caffeModelPath.Length);

            if(m_nativeObj == IntPtr.Zero)
            {
                throw new Exception("Failed to allocate native classifier.");
            }
        }

        /// <summary>
        /// Classifies the image input. Note that you need to ensure that you are supplying an image
        /// in JPEG encoding.
        /// </summary>
        /// <param name="imageData">
        /// The image data to classify. 
        /// </param>
        /// <returns>
        /// True of the supplied image was classified as pornography, false otherwise. 
        /// </returns>
        public bool ClassifyImage(byte[] imageData)
        {
            bool result = false;

            lock(m_classLock)
            {
                result = ImageClassifierPInvoke.classifier_classify(m_nativeObj, imageData, imageData.Length);
            }

            return result;
        }
    }
}
